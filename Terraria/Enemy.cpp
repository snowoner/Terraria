#include "Enemy.h"



Enemy::monsterType monsterTypes[MONSERTYPES] =
{
	{ 100, 4, 2, 2.f, "images/zombie.png" }

};

Enemy::Enemy(int type)
{
	monsterType *mt = &(monsterTypes[type]);
	monster = new monsterType({
		mt->health,
		mt->speed,
		mt->attSpeed,
		mt->damage,
		mt->name
	});
	monsterTime = 0.f;
}

Enemy::~Enemy()
{
	if (monster != NULL)
		delete monster;
}


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	tileMapDispl = tileMapPos;

	spritesheet.loadFromFile(monster->name, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 1.f / 3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(MOVE_LEFT, monster->speed);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 2.f / 3.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, monster->speed);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 1.f / 3.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 2.f / 3.f));

	// TODO: Change position of tiles from ATTACK sprites
	sprite->setAnimationSpeed(ATTACK, monster->attSpeed);
	sprite->addKeyframe(ATTACK, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(ATTACK, glm::vec2(0.5, 1.f / 3.f));
	sprite->addKeyframe(ATTACK, glm::vec2(0.5, 2.f / 3.f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Enemy::update(int deltaTime, const glm::vec2 &pos, bool playerSeen, bool playerCollision)
{
	sprite->update(deltaTime);
	// each monster have his time, so we only search for monsters that player can see
	monsterTime += deltaTime;

	if (monsterTime >= 1000.f / monster->speed) {
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
			if (attackDelay == monster->attSpeed) attackDelay = 0;
		}
		monsterTime -= 1000.f / monster->speed;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos) {
	position = pos;
}

int Enemy::getDecision(const glm::vec2 &pos) {
	return (pos.x > position.x);
}