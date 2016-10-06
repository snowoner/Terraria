#include "Enemy.h"


#define MOVE_LEFT 0
#define MOVE_RIGHT 1

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/zombie.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 1.f / 3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 1.f/3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 2.f / 3.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 1.f / 3.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 2.f / 3.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	monsterTime = 0.f;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Enemy::update(int deltaTime, const glm::vec2 &pos)
{
	sprite->update(deltaTime);
	// each monster have his time, so we only search for monsters that player can see
	monsterTime += deltaTime;

	if (monsterTime >= MAX_TIMEMONSTER) {
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
		else if (decision == 1)
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
		monsterTime -= MAX_TIMEMONSTER;

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
