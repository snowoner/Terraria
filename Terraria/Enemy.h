#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "TileMap.h"
#include "PlayerManager.h"

#define MONSERTYPES 1


static const char* getURL();
static Sprite* getSprite(Texture &spritesheet, ShaderProgram &shaderProgram);

class Enemy
{
protected:
	Enemy(const glm::ivec2 &tileMapPos);
public:
	void init(Sprite &sprite, TileMap &tileMap, glm::vec2 position);
	bool update(int deltaTime, const glm::vec2 &posPlayer, bool playerSeen, bool playerCollision);
	void render();

	glm::vec2 Enemy::getRoundPosition(glm::vec2 position, int dir);

	glm::vec2 getPosition() const { return position; }
	float getDamage() const { return damage; }
	//	float getDamageDeal() const { return (attackDelay == 0) ? monster->damage : 0; }

	int getState();
	void setDamage(float damage); 
	bool isDead();

	const enum STATE { MOVE_LEFT, MOVE_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, DEAD, JUMP };

protected:
	float health, speed, attSpeed;
	int attackDelay;
	glm::ivec2 position;
	float damage;
private:
	int getDecision(const glm::vec2 &pos);

private:
	float monsterTime;
	bool bJumping;
	int jumpAngle, startY;
	
	int deadDelay;

	Sprite *sprite;
	TileMap *map;

	glm::vec2 tileMapDispl;

	bool roundLeft;
	pair<glm::ivec2,glm::ivec2> roundPositions;
};

#endif // _ENEMY_INCLUDE