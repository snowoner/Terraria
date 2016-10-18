#include "Enemy.h"

Enemy::Enemy(const glm::ivec2 &tileMapPos) {
	tileMapDispl = tileMapPos;
}

Enemy::~Enemy()
{

}


void Enemy::init(Sprite &sprite, TileMap &tileMap)
{
	this->sprite = &sprite;
	this->map = &tileMap;
	monsterTime = 0.f;
}

void Enemy::update(int deltaTime, const glm::vec2 &pos, bool playerSeen, bool playerCollision)
{
	sprite->update(deltaTime);
	// each monster have his time, so we only search for monsters that player can see
	monsterTime += deltaTime;

	if (monsterTime >= 1000.f / speed) {
		if (!playerCollision) {
			if (playerSeen) {
				int decision = getDecision(pos);
				if (decision == MOVE_LEFT)
				{
					if (sprite->animation() != MOVE_LEFT)
						sprite->changeAnimation(MOVE_LEFT);
					position.x -= 2;
					if (map->collisionMoveLeft(position, glm::ivec2(32, 32)))
					{
						position.x += 2;
						sprite->changeAnimation(MOVE_RIGHT);
					}
				}
				else if (decision == MOVE_RIGHT)
				{
					if (sprite->animation() != MOVE_RIGHT)
						sprite->changeAnimation(MOVE_RIGHT);
					position.x += 2;
					if (map->collisionMoveRight(position, glm::ivec2(32, 32)))
					{
						position.x -= 2;
						sprite->changeAnimation(MOVE_LEFT);
					}
				}
			}
			else {
				// TODO:
				//random movement: Go to  a random position (near him) and go there until he see player
			}
		}
		else {
			// Attack caused by collision
			if (sprite->animation() != ATTACK) {
				sprite->changeAnimation(ATTACK);
				attackDelay = 0;
			}

			attackDelay++;
			if (attackDelay == attSpeed) attackDelay = 0;
		}
		monsterTime -= 1000.f /speed;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setPosition(const glm::vec2 &pos) 
{
	position = pos;
	sprite->setPosition(glm::vec2(float(32 + position.x),float(16 + position.y)));
}

int Enemy::getDecision(const glm::vec2 &pos) 
{
	return (pos.x > position.x);
}

void Enemy::setDamage(int damage)
{
	health -= damage;
}