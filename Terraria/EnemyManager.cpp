#include "EnemyManager.h"
#include "Zombie.h"

EnemyManager::EnemyManager(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	tileMapDispl = tileMapPos;
	enemyFactory = new EnemyFactory(tileMapPos);

	enemySprites = new vector<EnemySprite*>();
	enemySprite.spritesheet.loadFromFile(Zombie::getURL(), TEXTURE_PIXEL_FORMAT_RGBA);
	enemySprite.sprite = Zombie::getSprite(	(&enemySprite)->spritesheet, shaderProgram);
	enemySprites->push_back(&enemySprite);
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::addEnemy() {
	enemyFactory->createEnemy(0, *(enemySprites->at(0)->sprite));
}

void EnemyManager::setTileMap(TileMap *tileMap)
{
	map = tileMap;
	enemyFactory->setTileMap(map);
}

void EnemyManager::update(int deltaTime, const glm::vec2 &pos) {
	enemyFactory->update(deltaTime,pos);
}

void EnemyManager::render()
{
	enemyFactory->render();
}