#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "TileMap.h"

#define MONSERTYPES 1

#pragma once
class Enemy
{
public:
	Enemy(int type);
	~Enemy();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, const glm::vec2 &pos, bool playerSeen);
	void render();

	void Enemy::setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() const { return position; }
private:
	int getDecision(const glm::vec2 &pos);
public:
	struct monsterType {
		short health, speed;
		char *name;
	};
private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::vec2 tileMapDispl;

	float monsterTime;
	glm::vec2 position;

	monsterType *monser;
};

#endif // _ENEMY_INCLUDE