#ifndef _ENEMYFACTORY_INCLUDE
#define _ENEMYFACTORY_INCLUDE

#include "Zombie.h"
#include "Enemy.h"

using namespace std;

#define ENEMIESTYPES 1

class EnemyFactory
{
public:
	EnemyFactory(const glm::vec2 &tileMapDispl);
	~EnemyFactory();

	void update(int deltaTime, const glm::vec2 &pos);
	void render();
	
	Enemy *createEnemy(int type, Sprite &sprite);
	void setTileMap(TileMap *tileMap);

	void setDamage(const glm::vec2 posPlayer, int damage, int direction);
private:
	vector<Enemy*> enemies;
	TileMap *map;
	glm::vec2 tileMapDispl;
	int size;
};

#endif //_ENEMYFACTORY_INCLUDE