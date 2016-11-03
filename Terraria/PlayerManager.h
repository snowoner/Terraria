#ifndef _PLAYER_MANAGER_INCLUDE
#define _PLAYER_MANAGER_INCLUDE

#include "Player.h"
#include "Text.h"

#define ACTION_DELAY 2

class PlayerManager
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	int getState() const { return player->getState(); }
	glm::vec2 getPosition() const { return player->getPosition(); }
	int getDirection() const { return player->getDirection(); }
	float getHealth() const { return player->getHealth(); }

	void setTileMap(TileMap *tileMap);
	void setPlayerPosition(const glm::vec2 &pos);
	void setPosition(const glm::vec2 &pos);
	void setState(int state);

	void receiveDamage(float damage, float defense);

	void setItem(Element *item);
	void setPositionPressed(glm::ivec2 position);
	int PlayerManager::getActualDelay();
private:
	void setSpritesPositions(const glm::vec2 &pos);
	void addAnimations();

private:
	glm::ivec2 tileMapDispl;

	Texture spritesheet;
	Texture elementSpritesheet;
	Sprite *sprite;
	Sprite *elementSprite;
	Text healthSprite;

	Player *player;
};

#endif // _PLAYER_MANAGER_INCLUDE