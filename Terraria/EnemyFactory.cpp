#include "EnemyFactory.h"


Enemy *EnemyFactory::createEnemy(int type, Sprite &sprite)
{
	// TODO: Get type monster class by int type
	enemies.push_back(new Zombie(tileMapDispl));
	enemies[enemies.size() - 1]->init(sprite, *map, glm::vec2(64 + 80, 400));

	return enemies.back();
}


EnemyFactory::EnemyFactory(const glm::vec2 &tileMapPos)
{
	tileMapDispl = tileMapPos;
}


EnemyFactory::~EnemyFactory()
{
	for (Enemy* enemy : enemies)
		delete &enemy;
}

void EnemyFactory::update(int deltaTime, const glm::vec2 &posPlayer)
{
	damages.clear();
	// TODO: Only update enemies can see in the screen
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
			&& map->insideDistance(posPlayer, posEnemy, 3))
			enemy->setDamage(damage);
	}
}

vector<float> EnemyFactory::getDamageEnemies()
{
	return damages;
}