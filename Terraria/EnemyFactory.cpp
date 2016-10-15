#include "EnemyFactory.h"


Enemy *EnemyFactory::createEnemy(int type, Sprite &sprite)
{
	// TODO: Get type monster class by int type
	enemies.push_back(new Zombie(tileMapDispl));
	enemies[enemies.size()-1]->init(sprite,*map);
	enemies[enemies.size()-1]->setPosition(glm::vec2(64 + 80, 400));

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

void EnemyFactory::update(int deltaTime, const glm::vec2 &pos)
{
	// TODO: Only update enemies can see in the screen
	for (Enemy* enemy : enemies) {
		glm::vec2 posEnemy = enemy->getPosition();
		bool collision = map->playerCollisionBy(pos, posEnemy);
		enemy->update(deltaTime, pos, collision ? true : map->playerSeenBy(pos, posEnemy), collision);
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
