#include "Zombie.h"
#include "Sprite.h"
#include "Enemy.h"

Zombie::Zombie(const glm::vec2 &tileMapPos) : Enemy(tileMapPos)
{
	health = 100;
	speed = 4;
	attSpeed = 2;
}

Sprite* Zombie::getSprite(Texture &spritesheet, ShaderProgram &shaderProgram) {
	Sprite *sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 1.f / 3.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(Enemy::MOVE_LEFT, 4);
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 2.f / 3.f));

	sprite->setAnimationSpeed(Enemy::MOVE_RIGHT, 4);
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.5, 1.f / 3.f));
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.5, 2.f / 3.f));

	// TODO: Change position of tiles from ATTACK sprites
	sprite->setAnimationSpeed(Enemy::ATTACK, 2);
	sprite->addKeyframe(Enemy::ATTACK, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(Enemy::ATTACK, glm::vec2(0.5, 1.f / 3.f));
	sprite->addKeyframe(Enemy::ATTACK, glm::vec2(0.5, 2.f / 3.f));

	sprite->changeAnimation(0);

	return sprite;
}
