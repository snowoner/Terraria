#include "PlayerManager.h"

enum Elements
{
	PICK_STAND_RIGHT, PICK_STAND_LEFT, PICK_RIGHT, PICK_LEFT,
	WEAPON_STAND_RIGHT, WEAPON_STAND_LEFT, WEAPON_RIGHT, WEAPON_LEFT,
	LASTELEMENT
};


void PlayerManager::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram){
	player = new Player();
	player->init();

	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	elementSpritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	elementSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.5f), &elementSpritesheet, &shaderProgram);

	elementSprite->setNumberAnimations(8);
	int i = 0;
	while (i < Elements::LASTELEMENT) {
		float tx = 0.25*(i / 4.f);
		elementSprite->setAnimation(i, 8, (i % 2) == 0 ? 1.f : -1.f);
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(-1.f, 0.f), glm::vec2(-16.f, -4.f));
		i++;
		elementSprite->setAnimation(i, 8, (i % 2) == 0 ? 1.f : -1.f);
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(1.f, 0.f), glm::vec2(0.f, -4.f));
		i++;
		elementSprite->setAnimation(i, 8, (i % 2) == 0 ? 1.f : -1.f);
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(-1.f, 0.f), glm::vec2(-16.f, -4.f));
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(-2.f, 0.f), glm::vec2(-24.f, -8.f));
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(-1.f, 0.f), glm::vec2(-16.f, -4.f));
		i++;
		elementSprite->setAnimation(i, 8, (i % 2) == 0 ? 1.f : -1.f);
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(1.f, 0.f), glm::vec2(0.f, -4.f));
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(2.f, 0.f), glm::vec2(8.f, -8.f));
		elementSprite->addKeyframe(i, glm::vec2(tx, 0.5f), glm::vec2(1.f, 0.f), glm::vec2(0.f, -4.f));
		i++;
	}

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f / 3.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimation(STAND_LEFT, 8, -1.f);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimation(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimation(MOVE_LEFT, 8, -1.f);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1.f / 3.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2.f / 3.f, 0.f));

	sprite->setAnimation(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f / 3.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2.f / 3.f, 0.f));

	sprite->setAnimation(CHOP_RIGHT, 8);
	sprite->addKeyframe(CHOP_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(CHOP_RIGHT, glm::vec2(1.f / 3.f, 0.5f));
	sprite->addKeyframe(CHOP_RIGHT, glm::vec2(2.f / 3.f, 0.5f));

	sprite->setAnimation(CHOP_LEFT, 8, -1.f);
	sprite->addKeyframe(CHOP_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(CHOP_LEFT, glm::vec2(1.f / 3.f, 0.5f));
	sprite->addKeyframe(CHOP_LEFT, glm::vec2(2.f / 3.f, 0.5f));

	sprite->changeAnimation(player->getState());
	elementSprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
}

void PlayerManager::update(int deltaTime)
{
	player->update(deltaTime, sprite->keyFrame());

	sprite->update(deltaTime);
	elementSprite->update(deltaTime);

	if (sprite->animation() >= CHOP_RIGHT) {
		if (elementSprite->animation() % 4 != PICK_RIGHT && elementSprite->animation() % 4 != PICK_LEFT)
		{
			int itemIndex = (player->getItem());
			if (itemIndex >= 0) 
				elementSprite->changeAnimation((itemIndex * 4) + 3 - getDirection());
		}
	}
	else {
		if (elementSprite->animation() != 1 - getDirection())
			elementSprite->changeAnimation(1 - getDirection());
	}


	setSpritesPositions(player->getPosition());

	int state = player->getState();
	if (sprite->animation() != state)
		sprite->changeAnimation(state);
}
void PlayerManager::render()
{
	player->render();
	sprite->render();
	if (player->getItem() != -1) elementSprite->render();
}

void PlayerManager::setTileMap(TileMap *tileMap)
{
	player->setTileMap(tileMap);
}

void PlayerManager::setPosition(const glm::vec2 &pos)
{
	player->setPosition(pos);
	setSpritesPositions(pos);
}

void PlayerManager::setItem(Element *item)
{
	player->setItem(item);
}

void PlayerManager::receiveDamage(float damage)
{
	player->receiveDamage(damage);
}

void PlayerManager::setSpritesPositions(const glm::vec2 &pos) {
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	elementSprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + 16.f + 16.f*((getDirection() == LEFT) ? -1 : 1)), float(tileMapDispl.y + pos.y)));
}

int PlayerManager::getActualDelay()
{
	return sprite->keyFrame();
}