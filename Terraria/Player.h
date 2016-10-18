#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "ElementFactory.h"


#define MAXDISTANCE_BUILD 4

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() const { return posPlayer; }
	int getDirection() const { return direction; }
	float getLife() const { return life; }

	void getElement(int type);
	Element* getElementSelected();
	void setElementSelected(int selected);
	void removeElement(Element *element);
	void craftElement(int type);

	void receiveDamage(float damage);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int direction;
	int jumpAngle, startY;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	
	ElementFactory elementFactory;

	float life;
};


#endif // _PLAYER_INCLUDE


