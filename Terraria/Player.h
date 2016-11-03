#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Element.h"


#define MAXDISTANCE_ATTACK 1.5f
#define MAXDISTANCE_BUILD 2.5f

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, 
	HAND_RIGHT, HAND_LEFT,
	LASTANIM
};

class Player
{

public:
	void init();
	void update(int deltaTime, int delay);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setDirection(int dir);

	glm::vec2 getPosition() const { return posPlayer; }
	int getState() const { return state; }
	int getDirection() const { return direction; }
	int getItem() const { return (item!=NULL) ? item->getType() : -1; }
	float getHealth() const { return health; }

	void receiveDamage(float damage, float defense);
	void setItem(Element *item);
private:
	bool bJumping;
	glm::ivec2 posPlayer;
	int jumpAngle, startY;
	int state, direction;
	TileMap *map;
	float health;

	Element *item;
};


#endif // _PLAYER_INCLUDE


