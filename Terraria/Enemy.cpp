#include "Enemy.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

Enemy::Enemy(const glm::ivec2 &tileMapPos) {
	tileMapDispl = tileMapPos;
}

void Enemy::init(Sprite &sprite, TileMap &tileMap, glm::vec2 position)
{
	this->sprite = &sprite;
	this->map = &tileMap;
	monsterTime = 0.f;
	bJumping = false;
	roundLeft = false;
	roundPositions = pair<glm::vec2, glm::vec2>(getRoundPosition(position, LEFT), getRoundPosition(position, RIGHT));
	this->position = position;
	this->sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

glm::vec2 Enemy::getRoundPosition(glm::vec2 position, int dir)
{
	bool isEmpty = false;

	position /= (glm::ivec2(map->getTileSize(), map->getTileSize()));

	int i = 0;
	while (!isEmpty && i < 10)
	{
		isEmpty = map->getElementType(position + glm::vec2(10 - i, 0)*((dir == LEFT) ? -1.f : 1.f)) == 0;
		i++;
	}
	return glm::vec2(position + glm::vec2(10 - (i - 1), 0)*((dir == LEFT) ? -1.f : 1.f));
}

bool Enemy::update(int deltaTime, const glm::vec2 &posPlayer, bool playerSeen, bool playerCollision)
{
	sprite->update(deltaTime);
	// each monster have his time, so we only search for monsters that player can see
	monsterTime += deltaTime;
	bool dealDamage = false;
	if (sprite->animation() != DEAD) {
		if (health <= 0.f){
			deadDelay = 0;
			sprite->changeAnimation(DEAD);
		}
		else{
			if (monsterTime >= 1000.f / speed) {
				if (!playerCollision) {
					int decision;
					if (playerSeen) decision = getDecision(posPlayer);
					else {
						if (roundLeft && roundPositions.first == position)  roundLeft = false;
						else if (!roundLeft && roundPositions.second == position)  roundLeft = true;
						decision = getDecision((roundLeft) ? roundPositions.first : roundPositions.second);
					}
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
					// Attack caused by collision
					STATE dir = (posPlayer.x > position.x) ? ATTACK_RIGHT : ATTACK_LEFT;
					if (sprite->animation() != dir) {
						sprite->changeAnimation(dir);
						attackDelay = 0;
					}
					else {
						attackDelay++;
						if (attackDelay == attSpeed) {
							dealDamage = true;
							attackDelay = 0;
						}
					}
				}
				monsterTime -= 1000.f / speed;
			}
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
	else if (monsterTime >= 1000.f / speed){
		deadDelay++;
		monsterTime -= 1000.f / speed;
	}
	return dealDamage;
}

void Enemy::render()
{
	sprite->render();
}

int Enemy::getState()
{
	return sprite->animation();
}

int Enemy::getDecision(const glm::vec2 &pos)
{
	int decision = (pos.x > position.x);
	//if (pos.y < position.y ) decision = JUMP;
	return decision;
}

void Enemy::setDamage(float damage)
{
	health -= damage;
}

bool Enemy::isDead()
{
	return deadDelay == 5;
}