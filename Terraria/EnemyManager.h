#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "TileMap.h"

#include "EnemyFactory.h"

class EnemyManager
{
public:

	struct EnemySprite {
		Texture spritesheet;
		Sprite *sprite;
	} enemySprite;

	EnemyManager(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	~EnemyManager();
	
	void update(int deltaTime, const glm::vec2 &pos);
	void render();
	
	void addEnemy();
	void setTileMap(TileMap *tileMap);


private:
	glm::vec2 tileMapDispl;
	TileMap *map;
	
	EnemyFactory *enemyFactory;
	vector<EnemySprite*> *enemySprites;

};


#endif // _ENEMYMANAGER_INCLUDE