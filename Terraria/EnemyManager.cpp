#include "EnemyManager.h"
#include "Zombie.h"

#define RESPAWN_TIME 10000
#define MAX_ENEMIES 5

EnemyManager::EnemyManager(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	tileMapDispl = tileMapPos;
	enemyFactory = new EnemyFactory(tileMapPos);

	enemySprites = new vector<EnemySprite*>();
	enemySprite.spritesheet.loadFromFile(Zombie::getURL(), TEXTURE_PIXEL_FORMAT_RGBA);
	enemySprite.sprite = Zombie::getSprite(	(&enemySprite)->spritesheet, shaderProgram);
	enemySprites->push_back(&enemySprite);
	respawnTime = 0;
}

void EnemyManager::addEnemy() 
{
	enemyFactory->createEnemy(0, *(enemySprites->at(0)->sprite->clone()));
}

void EnemyManager::setTileMap(TileMap *tileMap)
{
	enemyFactory->setTileMap(tileMap);
}

void EnemyManager::update(int deltaTime, const glm::vec2 &posPlayer) 
{
	respawnTime += deltaTime;
	if (respawnTime >= RESPAWN_TIME && enemyFactory->size() < MAX_ENEMIES)
	{
		addEnemy();
		respawnTime -= RESPAWN_TIME;
	}
	enemyFactory->update(deltaTime, posPlayer);
}

void EnemyManager::render()
{
	enemyFactory->render();
}

void EnemyManager::setDamage(const glm::vec2 posPlayer, float damage, int direction)
{
	enemyFactory->setDamage(posPlayer, damage, direction);
}

vector<float> EnemyManager::getDamageEnemies()
{
	return enemyFactory->getDamageEnemies();
}