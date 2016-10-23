#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include <sstream>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	elementManager = new ElementManager(tileMapPos, shaderProgram);
	life = 100.f;
	bJumping = false;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(0);
	direction = LEFT;

	tileMapDispl = tileMapPos;
	program = shaderProgram;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime, const glm::ivec2 &posCamera)
{
	sprite->update(deltaTime);

	playerMovements();
	setElementsPosition(posCamera);

	playerActions(posCamera);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	elementManager->render();
	sprite->render();
}

void Player::playerMovements()
{
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
		{
			sprite->changeAnimation(MOVE_LEFT);
			direction = LEFT;
		}
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
		{
			sprite->changeAnimation(MOVE_RIGHT);
			direction = RIGHT;
		}

		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}

}

void Player::playerActions(const glm::ivec2 &posCamera)
{
	if (Game::instance().isMousePressed(0)) {
		Element* item = getElementSelected();
		if (dynamic_cast<Weapon*>(item) != 0) {
			float damage = item->getDamage();
			// TOOD: Set damage to all enemies within a size of weapon (distance from player)
			//enemyManager->setDamage(posPlayer, damage, player->getDirection());
		}
		else {
			glm::ivec2 posElement = (Game::instance().getMousePosition() + posCamera - SCREEN_VEC);
			if (map->insideDistance(posPlayer, posElement, MAXDISTANCE_BUILD)) {
				posElement /= (glm::ivec2(map->getTileSize(), map->getTileSize()));
				if (dynamic_cast<Pick*>(item) != 0 && map->getElement(posElement) != NULL) {
					map->buildElement(posElement, NULL);
					map->prepareArrays(SCREEN_VEC, program);
					removeElement(item);
				}
				else if (dynamic_cast<Material*>(item) != 0) {
					map->buildElement(posElement, item->getType());
					map->prepareArrays(SCREEN_VEC, program);
					removeElement(item);
				}
			}
		}
	}

	bool found = false;
	int keyPressed = 1;
	while (keyPressed <= MAX_ITEMS_SHOWN && !found)
	{
		if (Game::instance().getKey(keyPressed + '0')) found = true;
		else keyPressed++;
	}
	if (found) elementManager->setElementSelected(keyPressed - 1);
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setElementsPosition(const glm::vec2 &minCoords)
{
	elementManager->setPosition(minCoords);
}

void Player::getElement(int type)
{
	elementManager->addElement(type);
}

Element* Player::getElementSelected() {
	return elementManager->getElementSelected();
}

void Player::setElementSelected(int selected)
{
	// TODO: selected is a position where the box with the element is.
	elementManager->setElementSelected(selected);
}

void Player::removeElement(Element *element)
{
	elementManager->removeElement(element);
}

void Player::craftElement(int type)
{
	elementManager->craftElement(type);
}

void Player::receiveDamage(float damage) {
	life -= damage;
}