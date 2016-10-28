#include "Text.h"

Text::textType textTypes[2] =
{
	{ 32, 32, int('A'), 9, 4, "images/fontMenu.png" },
	{ 8, 8, int('0'), 5, 2, "images/numbers.png" }
};

void Text::init(ShaderProgram &shaderProgram, const glm::vec2 &minCoords, int type)
{
	text = new SpriteArray();
	text->init(&shaderProgram, minCoords, textTypes[type].font,
		glm::vec2(textTypes[type].tileLengthX, textTypes[type].tileLengthY),
		glm::ivec2(textTypes[type].blockSize, textTypes[type].blockSize), glm::ivec2(textTypes[type].tileSize, 0), textTypes[type].offset);
}

void Text::render()
{
	text->render();
}

void Text::addText(string text, glm::vec2 &position)
{
	vector<int> tiles = vector<int>(text.length());
	int i = 0;
	for (char& c : text) {
		tiles[i] = int(c) + 1;
		i++;
	}
	this->text->addTiles(tiles, position);
}

void Text::removeTiles()
{
	text->removeTiles();
}

void Text::prepareText()
{
	text->prepareArrays();
}

void Text::setPosition(const glm::vec2 &minCoord)
{
	text->setPosition(minCoord);
}