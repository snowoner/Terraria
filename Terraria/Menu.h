#include "Sprite.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

#include "Text.h"
#include "Sprite.h"


#define MENU_TILESIZE 32
#define ARROW_TILESIZE 80
#define OFFSET 16

#define SCREEN_MIDX SCREEN_WIDTH / 2 - 2 * MENU_TILESIZE
#define SCREEN_MIDY SCREEN_HEIGHT / 2 - MENU_TILESIZE - Menu::MAXOPTIONS / 2.f * (MENU_TILESIZE + OFFSET)

#define MAX_TIMECAPTURE 1000.f/8.f

#pragma once
class Menu
{
public:
	Menu();
	~Menu();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	const enum MENU_OPTION { PLAY, OPTIONS, CREDITS, EXIT, MAXOPTIONS };
	MENU_OPTION getMenuOption();

private:
	ShaderProgram *program;
	Text *text;

	// Sprites
	Texture spritesheet;
	Sprite *sprite;
	float timeCapture;

	int pos;
};

