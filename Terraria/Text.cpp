#include "Text.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

Text::textType textTypes[1] =
{
	{ 32, 32, int('A'), 9, 4, "images/fontMenu.png" }
};


void Text::init(ShaderProgram &shaderProgram, int type) {
	program = &shaderProgram;
	textGenerator.init(textTypes[type].font, glm::vec2(textTypes[type].tileLengthX, textTypes[type].tileLengthY),
		textTypes[type].blockSize, textTypes[type].tileSize, textTypes[type].offset);
}

void Text::render() {
	textGenerator.render();
}

void Text::addText(string text, const glm::vec2 &position) {

	vector<int> tiles = vector<int>(text.length());
	int i = 0;
	for (char& c : text) {
		tiles[i] = int(c) + 1;
		i++;
	}
	textGenerator.addTiles(tiles, position);
}

void Text::prepareText(const glm::vec2 &minCoords) {
	textGenerator.prepareArrays(minCoords, *program);
}