#ifndef _PLAYER_MANAGER_INCLUDE
#define _PLAYER_MANAGER_INCLUDE

#include "Player.h"

class PlayerManager
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() const { return player->getPosition(); }
	int getDirection() const { return player->getDirection(); }
	float getLife() const { return player->getLife(); }

	void receiveDamage(float damage);

private:
	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Sprite *sprite;

	Player *player;
};

#endif // _PLAYER_MANAGER_INCLUDE