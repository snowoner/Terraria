#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Element.h"


#define MAXDISTANCE_BUILD 4

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

class Player
{

public:
	void init();
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setDirection(int dir);

	glm::vec2 getPosition() const { return posPlayer; }
	int getState() const { return state; }
	int getDirection() const { return direction; }
	float getLife() const { return life; }

	void receiveDamage(float damage);
private:
	bool bJumping;
	glm::ivec2 posPlayer;
	int jumpAngle, startY;
	int state, direction;
	TileMap *map;
	float life;
};


#endif // _PLAYER_INCLUDE


