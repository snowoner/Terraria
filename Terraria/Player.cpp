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

void Player::init()
{
	life = 100.f;
	bJumping = false;
	direction = LEFT;
	state = STAND_LEFT;
}

void Player::update(int deltaTime)
{
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (state != MOVE_LEFT)
		{
			state = MOVE_LEFT;
			direction = LEFT;
		}
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			state = STAND_LEFT;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (state != MOVE_RIGHT)
		{
			state = MOVE_RIGHT;
			direction = RIGHT;
		}

		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			state = STAND_RIGHT;
		}
	}
	else
	{
		if (state == MOVE_LEFT)
			state = STAND_LEFT;
		else if (state == MOVE_RIGHT)
			state = STAND_RIGHT;
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

void Player::render()
{
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
}

void Player::setDirection(int dir)
{
	direction = dir;
}

void Player::receiveDamage(float damage) {
	life -= damage;
}