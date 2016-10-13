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
	void update(int deltaTime, const glm::vec2 &pos, bool playerSeen, bool playerCollision);
	void render();

	void Enemy::setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition() const { return position; }
	float getDamageDeal() const { return (attackDelay == 0) ? monster->damage : 0; }

	const enum STATE { MOVE_LEFT, MOVE_RIGHT, ATTACK, DEAD };

private:
	int getDecision(const glm::vec2 &pos);
public:
	struct monsterType {
		short health, speed, attSpeed;
		float damage;
		char *name;
	};
private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::vec2 tileMapDispl;
	glm::vec2 position;

	monsterType *monster;
	float monsterTime;
	float attackDelay;
};

#endif // _ENEMY_INCLUDE