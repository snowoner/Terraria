#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 11

#define MOUSE_DELAY 6


Scene::Scene()
{
	map = NULL;
	playerManager = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (playerManager != NULL)
		delete playerManager;
}


void Scene::init()
{
	initShaders();

	menu = new Menu();
	menu->init(SCREEN_VEC, texProgram);

	camera = new Camera();
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
					map = TileMap::createTileMap("levels/level01.txt", SCREEN_VEC, texProgram);

					glm::vec2 posPlayer = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize());
					playerManager = new PlayerManager();
					playerManager->init(SCREEN_VEC, texProgram);
					playerManager->setPlayerPosition(posPlayer);
					playerManager->setTileMap(map);
					camera->update(deltaTime, posPlayer);

					enemyManager = new EnemyManager(SCREEN_VEC, texProgram);
					enemyManager->setTileMap(map);
					enemyManager->addEnemy();

					elementManager = new ElementManager(SCREEN_VEC, texProgram);
					elementManager->setTileMap(map);

					playerManager->setItem(elementManager->getElementSelected());

					firstTime = false;
					delayPressed = 0;
				}
				state = ST_GAME;

				break;
			case Menu::INSTRUCCIONS:
			{
									   text = new Text();
									   text->init(&texProgram, SCREEN_VEC, 3);
									   const int nLines = 3;
									   string textsShown[nLines] = {
										   "Puedes cambiar de element seleccionado con los numeros 1 a 5.",
										   "Para coger materiales pica a los bloques( dependiendo del ",
										   "material necesitaras picar m�s)."
									   };
									   for (int i = 0; i < nLines; ++i) {
										   text->addText(textsShown[i], glm::vec2(screenSize.x / 2 - textsShown[i].length() / 2.f * 16.f, screenSize.y / 2 - 16.f*(nLines - i)));
									   }
									   text->prepareText();
									   projection = glm::ortho(0.f, float(screenSize.x - 1), float(screenSize.y - 1), 0.f);

									   state = ST_INSTR;
			}
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
		else{
			menu->update(deltaTime);
			menu->setPosition(camera->getPosition());
		}
		break;
	case Scene::ST_GAME:
		if (Game::instance().getKey(int('m')))
		{
			// TODO: position of camera is 1/3. Menu is not shown at center of screen
			menu->setPosition(camera->getPosition());
			state = ST_MENU;
		}
		else {
			glm::vec2 posPlayer = playerManager->getPosition();
			// TODO: only update camera when players moves
			camera->update(deltaTime, posPlayer);
			glm::ivec2 posCamera = camera->getPosition();


			userActions(posPlayer, posCamera);

			playerManager->update(deltaTime);
			playerManager->setPosition(posCamera);

			elementCollecion(posPlayer);
			elementSelection();

			enemyManager->update(deltaTime, playerManager->getPosition());

			vector<float> damages = enemyManager->getDamageEnemies();
			float defense = elementManager->getTotalDefenseEquip();
			for (unsigned int i = 0; i < damages.size(); ++i) playerManager->receiveDamage(damages[i], defense);

			elementManager->update(deltaTime);
			elementManager->setPosition(posCamera);

			projection = glm::ortho(float(posCamera.x), float(posCamera.x + screenSize.x - 1), float(posCamera.y + screenSize.y - 1), float(posCamera.y));

			if (playerManager->getHealth() <= 0.f) {
				map->free();
				state = ST_DEAD;
				text = new Text();
				text->init(&texProgram, SCREEN_VEC, 2);
				string textShown = "YOU ARE DEAD";
				text->addText(textShown, glm::vec2(screenSize.x / 2 - textShown.length() / 2.f * 32.f, screenSize.y / 2 - 32.f));
				text->prepareText();
				projection = glm::ortho(0.f, float(screenSize.x - 1), float(screenSize.y - 1), 0.f);
			}
		}
		break;
	case Scene::ST_DEAD:
		if (Game::instance().getKey(27)){
			firstTime = true;
			state = ST_MENU;
		}
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

	switch (state)
	{
	case Scene::ST_MENU:
		menu->render();
		break;
	case Scene::ST_GAME:
		map->render();
		elementManager->render();
		enemyManager->render();
		playerManager->render();
		break;
	case Scene::ST_INSTR:
	case Scene::ST_DEAD:
		text->render();
		break;
	case Scene::ST_CREDITS:
		break;
	default:
		break;
	}
}

void Scene::changeWindowSize(glm::ivec2 size)
{
	screenSize = size;
	glm::vec2 posPlayer;
	if (state == ST_MENU && firstTime)
		posPlayer = glm::vec2(screenSize.x / 3, screenSize.y / 3);
	else posPlayer = playerManager->getPosition();

	glm::ivec2 posCamera = camera->getPosition();
	camera->update(0, posPlayer);

	projection = glm::ortho(float(posCamera.x), float(posCamera.x + screenSize.x - 1), float(posCamera.y + screenSize.y - 1), float(posCamera.y));
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

void Scene::userActions(const glm::ivec2 &posPlayer, const glm::ivec2 &posCamera)
{
	if (elPressed != NULL && playerManager->getState() >= PlayerAnims::HAND_RIGHT && playerManager->getState() < LASTANIM) {
		int delay = playerManager->getActualDelay();
		if (delay == ACTION_DELAY && elPressed != NULL){
			if (dynamic_cast<Weapon*>(elPressed->second) != 0) {
				float damage = elPressed->second->getDamage();
				enemyManager->setDamage(posPlayer, damage, playerManager->getDirection());
			}
			else {
				if (map->insideDistance(posPlayer, *elPressed->first, MAXDISTANCE_BUILD)) {
					glm::ivec2 posElement = *elPressed->first / (glm::ivec2(map->getTileSize(), map->getTileSize()));

					if (dynamic_cast<Pick*>(elPressed->second) != 0 && map->getElementType(posElement) != NULL)
					{
						int type = map->getElementType(posElement);
						if (map->hitElement(posElement)){
							if (map->getElementHitsLeft(posElement) == 0)
							{
								//map->buildElement(posElement, NULL);
								elementManager->addElementMaterial(type, *elPressed->first);
							}
							map->prepareArrays(SCREEN_VEC, false);
						}
					}
					else if (dynamic_cast<Material*>(elPressed->second) != 0) {
						if (map->getElementType(posElement) == NULL) {
							map->buildElement(posElement, elPressed->second->getTileIndex()-1);
							map->prepareArrays(SCREEN_VEC, false);

							elementManager->consumeElement(elPressed->second, 1);

							playerManager->setItem(elementManager->getElementSelected());
						}
					}
				}
			}
			elPressed = NULL;
		}
	}
	else if (Game::instance().isMousePressed(0)) {
		if (delayPressed != MOUSE_DELAY) delayPressed++;
		else {
			delayPressed = 0;

			glm::ivec2 *pos = new glm::ivec2(Game::instance().getMousePosition() + posCamera - SCREEN_VEC);
			int elementSelected = elementManager->getCraftingElement(Game::instance().getMousePosition());
			if (elementSelected != -1) 
				elementManager->craftElement(elementSelected);
			else {
				elementSelected = elementManager->getElement(Game::instance().getMousePosition());
				if (elementSelected != -1) elementManager->equipElement(elementSelected);
				else {
					elementSelected = elementManager->getEquipElement(Game::instance().getMousePosition());
					if (elementSelected != -1) elementManager->unequipElement(elementSelected);
					else if (elPressed == NULL)
						elPressed = new pair<glm::ivec2*, Element*>(pos, elementManager->getElementSelected());

				}
			}
		}
	}
	else delayPressed = 0;
}

void Scene::elementCollecion(const glm::ivec2 &posPlayer)
{
	vector<pair<glm::ivec2*, int>> materials = elementManager->getMapMaterials();
	int i, j;
	i = j = 0;
	for (pair<glm::ivec2*, int> material : materials){
		glm::ivec2* position = material.first;
		if (map->playerSeenBy(posPlayer, *position, 3)) {
			elementManager->collectElement(i - j);
			j++;
		}
		i++;
	}
}

void Scene::elementSelection()
{
	bool found = false;
	int keyPressed = 1;
	while (keyPressed <= MAX_ITEMS_SHOWN && !found)
	{
		if (Game::instance().getKey(keyPressed + '0')) found = true;
		else keyPressed++;
	}
	if (found) {
		elementManager->setElementIndexSelected(keyPressed - 1);
		playerManager->setItem(elementManager->getElementSelected());
	}
}