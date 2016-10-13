#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/Menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	menu = new Menu();
	menu->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	firstTime = true;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	switch (state)
	{
	case Scene::ST_MENU:
		if (Game::instance().getKey(int('\r')))
		{
			switch (menu->getMenuOption())
			{
			case Menu::PLAY:
				if (firstTime) {
					map->free();
					map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

					player = new Player();
					player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
					player->setTileMap(map);


					enemies.push_back(new Enemy(0));
					enemies[enemies.size() - 1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					enemies[enemies.size() - 1]->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 80, INIT_PLAYER_Y_TILES * map->getTileSize()));
					enemies[enemies.size() - 1]->setTileMap(map);
					firstTime = false;
				}
				state = ST_GAME;

				break;
			case Menu::OPTIONS:
				break;
			case Menu::CREDITS:
				break;
			case Menu::EXIT:
				exit(0);
				break;
			default:
				break;
			}
		}
		else menu->update(deltaTime);
		break;
	case Scene::ST_GAME:
		if (Game::instance().getKey(int('m')))
		{
			state = ST_MENU;
		}
		else {
			glm::vec2 posPlayer = player->getPosition();
			for (unsigned int i = 0; i < enemies.size(); ++i) {
				glm::vec2 posEnemy = enemies[i]->getPosition();
				bool collision = map->playerCollisionBy(posPlayer, posEnemy);
				enemies[i]->update(deltaTime, posPlayer, collision ? true : map->playerSeenBy(posPlayer, posEnemy), collision);
				if (collision) {
					player->receiveDamage(enemies[i]->getDamageDeal());
					if (player->getLife() <= 0.f) {
						state = ST_DEAD;
						text = new Text();
						text->init(texProgram, 0);
						text->addText("DEAD", glm::vec2(SCREEN_MIDX-4/2.f*32, SCREEN_MIDY));
						text->prepareText(glm::ivec2(SCREEN_X, SCREEN_Y));
					}
				}
			}
			player->update(deltaTime);
		}
		break;
	case Scene::ST_DEAD:
		// TODO: Show DEAD to screen and stop animations. Also play sound.
	
		break;
	case Scene::ST_CREDITS:
		break;
	default:
		break;
	}

}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	switch (state)
	{
	case Scene::ST_MENU:
		menu->render();
		break;
	case Scene::ST_GAME:
		player->render();
		for (unsigned int i = 0; i < enemies.size(); ++i)
			enemies[i]->render();
		break;
	case Scene::ST_DEAD:
		text->render();
		break;
	case Scene::ST_CREDITS:
		break;
	default:
		break;
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}