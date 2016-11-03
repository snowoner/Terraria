#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "Sprite.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

#include "Text.h"
#include "Sprite.h"

class Menu
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::ivec2 &minCoords);

	const enum MENU_OPTION { PLAY, INSTRUCCIONS, CREDITS, EXIT, MAXOPTIONS };
	MENU_OPTION getMenuOption();

private:
	ShaderProgram *program;
	Text *text;

	glm::vec2 position;

	Texture spritesheet;
	Sprite *sprite;
	float timeCapture;

	int posOption;
};

#endif // _MENU_INCLUDE