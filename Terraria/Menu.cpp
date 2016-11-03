#include <GL/glew.h>
#include <GL/glut.h>

#include "Menu.h"
#include "Game.h"

#define MENU_TILESIZE 32
#define ARROW_TILESIZE 80
#define OFFSET 16

#define SCREEN_MIDX Game::instance().getSize().x / 2 -  MENU_TILESIZE
#define SCREEN_MIDY Game::instance().getSize().y / 2 - MENU_TILESIZE - Menu::MAXOPTIONS / 2.f * (MENU_TILESIZE + OFFSET)

#define ARROW_POS glm::vec2(float(SCREEN_MIDX - (options[posOption].length() / 2.f)  * MENU_TILESIZE - ARROW_TILESIZE) + position.x, float(SCREEN_MIDY + (MENU_TILESIZE + OFFSET)*posOption) + position.y)

#define MAX_TIMECAPTURE 1000.f/8.f

const string options[Menu::MAXOPTIONS] = { "JUGAR", "INSTRUCCIONES", "CREDITOS", "SALIR" };

void Menu::init(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram) {
	posOption = PLAY;

	text = new Text();
	text->init(&shaderProgram, minCoords, 0);
	for (int i = 0; i < MAXOPTIONS; ++i) {
		int length = (options[i]).length();
		text->addText(options[i], glm::vec2(-length / 2.f  * MENU_TILESIZE, (MENU_TILESIZE + OFFSET)*i));
	}
	text->prepareText();
	timeCapture = 0.f;

	// Create arrow sprite for animation
	spritesheet.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(ARROW_TILESIZE, MENU_TILESIZE), glm::vec2(1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimation(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.75f));
	sprite->changeAnimation(0);

	setPosition(minCoords);
}

void Menu::update(int deltaTime) {
	sprite->update(deltaTime);

	timeCapture += deltaTime;
	if (timeCapture >= MAX_TIMECAPTURE) {
		bool changed = false;
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			// Moure la fletxa avall
			if (posOption == MAXOPTIONS - 1) posOption = PLAY;
			else posOption++;
			changed = true;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			// Moure la flexa amunt
			if (posOption == 0) posOption = MAXOPTIONS - 1;
			else posOption--;
			changed = true;
		}

		if (changed)
			sprite->setPosition(ARROW_POS);
		timeCapture -= MAX_TIMECAPTURE;
	}
}

void Menu::render() {
	text->render();
	sprite->render();
}

void Menu::setPosition(const glm::ivec2 &minCoords)
{
	position = minCoords;
	text->setPosition(minCoords + glm::ivec2(SCREEN_MIDX, SCREEN_MIDY));
	sprite->setPosition(ARROW_POS);
}

Menu::MENU_OPTION Menu::getMenuOption() {
	return (MENU_OPTION)posOption;
}