#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


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

	map = TileMap::createTileMap("levels/Menu.txt", SCREEN_VEC, texProgram);

	posTileMap = glm::vec2(SCREEN_X, SCREEN_Y);
	menu = new Menu();
	menu->init(posTileMap, texProgram);

	camera = new Camera();

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
					map = TileMap::createTileMap("levels/level01.txt", SCREEN_VEC, texProgram);

					player = new Player();
					player->init(SCREEN_VEC, texProgram);
					glm::vec2 posPlayer = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize());
					player->setPosition(posPlayer);
					player->setTileMap(map);
					camera->update(deltaTime, posPlayer, glm::vec2(1, 1));

					enemyManager = new EnemyManager(SCREEN_VEC, texProgram);
					enemyManager->setTileMap(map);
					enemyManager->addEnemy();

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
			menu->setPosition(camera->getPosition());
			state = ST_MENU;
		}
		else {
			glm::vec2 posPlayer = player->getPosition();
			// TODO: only update camera when players moves
			camera->update(deltaTime, posPlayer, glm::vec2(1, 1));
			enemyManager->update(deltaTime, posPlayer);

			glm::vec2 posCamera = camera->getPosition();
			
			player->update(deltaTime, posCamera);
			projection = glm::ortho(float(posCamera.x), float(posCamera.x + SCREEN_WIDTH - 1), float(posCamera.y + SCREEN_HEIGHT - 1), float(posCamera.y));


			/*
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
			}*/
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
		enemyManager->render();
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