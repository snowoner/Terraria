#include "Text.h"

Text::textType textTypes[1] =
{
	{ 32, 32, int('A'), 9, 4, "images/fontMenu.png" }
};

void Text::init(ShaderProgram &shaderProgram, const glm::vec2 &minCoords, int type)
{
	textGenerator = new TextureGenerator();
	textGenerator->init(&shaderProgram, minCoords, textTypes[type].font,
		glm::vec2(textTypes[type].tileLengthX, textTypes[type].tileLengthY),
		textTypes[type].blockSize, textTypes[type].tileSize, textTypes[type].offset);
}

void Text::render()
{
	textGenerator->render();
}

void Text::addText(string text, const glm::vec2 &position)
{
	vector<int> tiles = vector<int>(text.length());
	int i = 0;
	for (char& c : text) {
		tiles[i] = int(c) + 1;
		i++;
	}
	textGenerator->addTiles(tiles, position);
}

void Text::prepareText()
{
	textGenerator->prepareArrays();
}

void Text::setPosition(const glm::vec2 &minCoord)
{
	textGenerator->setPosition(minCoord);
}