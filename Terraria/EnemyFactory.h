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

	void update(int deltaTime, const glm::vec2 &posPlayer);
	void render();
	
	Enemy *createEnemy(int type, Sprite &sprite);
	void setTileMap(TileMap *tileMap);

	void setDamage(const glm::vec2 posPlayer, float damage, int direction);
	vector<float> getDamageEnemies();
private:
	vector<Enemy*> enemies;
	TileMap *map;
	glm::vec2 tileMapDispl;
	int size;

	vector<float> damages;
};

#endif //_ENEMYFACTORY_INCLUDE