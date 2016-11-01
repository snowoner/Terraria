#include "Enemy.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

Enemy::Enemy(const glm::ivec2 &tileMapPos) {
	tileMapDispl = tileMapPos;
}

void Enemy::init(Sprite &sprite, TileMap &tileMap)
{
	this->sprite = &sprite;
	this->map = &tileMap;
	monsterTime = 0.f;
	bJumping = false;
}

void Enemy::update(int deltaTime,  PlayerManager* player, bool playerSeen, bool playerCollision)
{
	sprite->update(deltaTime);
	// each monster have his time, so we only search for monsters that player can see
	monsterTime += deltaTime;
	if (sprite->animation() != DEAD) {
		if (health < 0.f){
			deadDelay = 0;
			sprite->changeAnimation(DEAD);
		}
		else{
			if (monsterTime >= 1000.f / speed) {
				if (!playerCollision) {
					if (playerSeen) {
						int decision = getDecision(player->getPosition());
						if (!bJumping && decision == JUMP)
						{
							bJumping = true;
							jumpAngle = 0;
							startY = position.y;
						}
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
					STATE dir = (player->getPosition().x > position.x) ? ATTACK_RIGHT : ATTACK_LEFT;
					if (sprite->animation() != dir) {
						sprite->changeAnimation(dir);
						attackDelay = 0;
					}

					attackDelay++;
					if (attackDelay == attSpeed) {
						player->receiveDamage(damage);
						attackDelay = 0;
					}
				}
				monsterTime -= 1000.f / speed;
			}

			if (bJumping)
			{
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180)
				{
					bJumping = false;
					position.y = startY;
				}
				else
				{
					position.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					if (!map->collisionMoveUp(position, glm::ivec2(32, 32), &position.y))
					{
						if (jumpAngle > 90)
							bJumping = !map->collisionMoveDown(position, glm::ivec2(32, 32), &position.y);
					}
					else bJumping = false;
				}
			}
			else
			{
				position.y += FALL_STEP;
				map->collisionMoveDown(position, glm::ivec2(32, 32), &position.y);
			}

			sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
		}
	}
	else if (monsterTime >= 1000.f / speed){
		deadDelay++;
		monsterTime -= 1000.f / speed;
	}
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setPosition(const glm::vec2 &pos) 
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

int Enemy::getState()
{
	return sprite->animation();
}

int Enemy::getDecision(const glm::vec2 &pos) 
{
	int decision = (pos.x > position.x);
	if (pos.y < position.y) decision = JUMP;
	return decision;
}

void Enemy::setDamage(int damage)
{
	health -= damage;
}

bool Enemy::isDead()
{
	return deadDelay == 5;
}