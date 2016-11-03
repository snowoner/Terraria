#include "EnemyFactory.h"
#define MAX_UPDATE_TIME 1000/60

Enemy *EnemyFactory::createEnemy(int type, Sprite &sprite)
{
	// TODO: Get type monster class by int type
	enemies.push_back(new Zombie(tileMapDispl));
	enemies[enemies.size() - 1]->init(sprite, *map, respawn);

	return enemies.back();
}


EnemyFactory::EnemyFactory(const glm::vec2 &tileMapPos)
{
	respawn = glm::vec2(1600, 280);
	tileMapDispl = tileMapPos;
	updateTime = 0;
}


EnemyFactory::~EnemyFactory()
{
	for (Enemy* enemy : enemies)
		delete &enemy;
}

void EnemyFactory::update(int deltaTime, const glm::vec2 &posPlayer)
{
	updateTime += deltaTime;
	damages.clear();
	if (updateTime >= MAX_UPDATE_TIME) {
		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			Enemy* enemy = enemies.at(i);
			glm::vec2 posEnemy = enemy->getPosition();
			bool collision = map->playerCollisionBy(posPlayer, posEnemy);
			// TODO: 32 -> map->size
			if (enemy->update(deltaTime, posPlayer, collision ? true : map->playerSeenBy(posPlayer, posEnemy, 32), collision))
				damages.push_back(enemy->getDamage());
			if (enemy->getState() == Enemy::DEAD && enemy->isDead())
			{
				delete enemies.at(i);
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
		updateTime -= MAX_UPDATE_TIME;
	}
}

void EnemyFactory::render()
{
	for (Enemy* enemy : enemies)
		enemy->render();
}

void EnemyFactory::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void EnemyFactory::setDamage(const glm::vec2 posPlayer, float damage, int direction)
{
	for (Enemy* enemy : enemies)
	{
		glm::vec2 posEnemy = enemy->getPosition();
		if ((direction == RIGHT || posPlayer.x > posEnemy.x)
			&& map->insideDistance(posPlayer, posEnemy, MAXDISTANCE_ATTACK))
			enemy->setDamage(damage);
	}
}

vector<float> EnemyFactory::getDamageEnemies()
{
	return damages;
}