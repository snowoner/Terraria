#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "TileMap.h"

#define MONSERTYPES 1


static const char* getURL();
static Sprite* getSprite(Texture &spritesheet, ShaderProgram &shaderProgram);

class Enemy
{
protected:
	Enemy(const glm::ivec2 &tileMapPos);
	~Enemy();
public:
	void init(Sprite &sprite, TileMap &tileMap);
	void update(int deltaTime, const glm::vec2 &pos, bool playerSeen, bool playerCollision);
	void render();

	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() const { return position; }
	//	float getDamageDeal() const { return (attackDelay == 0) ? monster->damage : 0; }
	
	void setDamage(int damage);

	const enum STATE { MOVE_LEFT, MOVE_RIGHT, ATTACK, DEAD };

protected:
	float health, speed, attSpeed;
	int attackDelay;
	glm::vec2 position;

private:
	int getDecision(const glm::vec2 &pos);
	
private:
	float monsterTime;

	Sprite *sprite;
	TileMap *map;

	glm::vec2 tileMapDispl;
};

#endif // _ENEMY_INCLUDE