#include <GL/glew.h>
#include <GL/glut.h>

#include "Menu.h"
#include "Game.h"

const string options[Menu::MAXOPTIONS] = { "Jugar", "Opciones", "Creditos", "Salir" };

Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::init(const glm::ivec2 &minCoords, ShaderProgram &shaderProgram) {
	pos = PLAY;

	float midY = SCREEN_MIDY;

	text = new Text();
	text->init(minCoords, shaderProgram);
	for (int i = 0; i < Menu::MAXOPTIONS; ++i) {
		int length = (options[i]).length();
		text->addText(options[i], glm::vec2(SCREEN_MIDX - length / 2.f  * MENU_TILESIZE, midY + (MENU_TILESIZE + OFFSET)*i));
	}
	text->prepareText(glm::vec2(minCoords.x, minCoords.y));
	timeCapture = 0.f;

	// Create arrow sprite for animation
	spritesheet.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(ARROW_TILESIZE, MENU_TILESIZE), glm::vec2(1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.75f));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(SCREEN_MIDX - (options[pos].length()) / 2.f  * MENU_TILESIZE - ARROW_TILESIZE / 4.f), midY));
}

void Menu::update(int deltaTime) {
	sprite->update(deltaTime);

	timeCapture += deltaTime;
	if (timeCapture >= MAX_TIMECAPTURE) {
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			// Moure la fletxa avall
			if (pos == MAXOPTIONS-1) pos = PLAY;
			else pos++;

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			// Moure la flexa amunt
			if (pos == 0) pos = MAXOPTIONS-1;
			else pos--;
		}

		sprite->setPosition(glm::vec2(float(SCREEN_MIDX - (options[pos].length()) / 2.f  * MENU_TILESIZE - ARROW_TILESIZE / 4.f), float(SCREEN_MIDY + MENU_TILESIZE / 2.f + (MENU_TILESIZE + OFFSET)*pos)));

		timeCapture -= MAX_TIMECAPTURE;
	}

}

void Menu::render() {
	text->render();
	sprite->render();
}

Menu::MENU_OPTION Menu::getMenuOption() {
	return (MENU_OPTION)pos;
}