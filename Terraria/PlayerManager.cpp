#include "PlayerManager.h"

void PlayerManager::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram){
	player = new Player();
	player->init();

	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(player->getState());

	tileMapDispl = tileMapPos;
}

void PlayerManager::update(int deltaTime)
{
	sprite->update(deltaTime);
	player->update(deltaTime);

	glm::ivec2 posPlayer = player->getPosition();
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
void PlayerManager::render()
{
	player->render();

	int state = player->getState();
	if (sprite->animation() != state)
		sprite->changeAnimation(state);

	sprite->render();
}

void PlayerManager::setTileMap(TileMap *tileMap)
{
	player->setTileMap(tileMap);
}

void PlayerManager::setPosition(const glm::vec2 &pos)
{
	player->setPosition(pos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void PlayerManager::receiveDamage(float damage)
{
	player->receiveDamage(damage);
}