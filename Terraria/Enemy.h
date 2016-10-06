#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "TileMap.h"


#define MAX_TIMEMONSTER 1000.f/4.f

#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();


	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, const glm::vec2 &pos);
	void render();

	void Enemy::setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	int getDecision(const glm::vec2 &pos);
private:
	float monsterTime;
	glm::vec2 tileMapDispl, position;


	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	
	
};

