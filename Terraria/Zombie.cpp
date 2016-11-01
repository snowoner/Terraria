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

	sprite->setNumberAnimations(5);

	sprite->setAnimation(Enemy::MOVE_LEFT, 4);
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(Enemy::MOVE_LEFT, glm::vec2(0.f, 2.f / 3.f));

	sprite->setAnimation(Enemy::MOVE_RIGHT, 4, -1.f);
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(Enemy::MOVE_RIGHT, glm::vec2(0.f, 2.f / 3.f));

	sprite->setAnimation(Enemy::ATTACK_LEFT, 4);
	sprite->addKeyframe(Enemy::ATTACK_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimation(Enemy::ATTACK_RIGHT, 4, -1.f);
	sprite->addKeyframe(Enemy::ATTACK_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimation(Enemy::DEAD,8);
	sprite->addKeyframe(Enemy::DEAD, glm::vec2(0.0, 0.f));
	sprite->addKeyframe(Enemy::DEAD, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(Enemy::DEAD, glm::vec2(0.0, 0.f));
	sprite->addKeyframe(Enemy::DEAD, glm::vec2(0.5f,0.f));

	sprite->changeAnimation(0);

	return sprite;
}
