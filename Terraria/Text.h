#include "Sprite.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

#pragma once


#define TILE_LENGTH_X 10
#define TILE_LENGTH_Y 10

class Text
{
public:
	Text();
	~Text();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update();
	void render();

	void addText(string text, const glm::vec2 &position);
	void prepareText(const glm::vec2 &minCoords);

	
private:

	int nTiles;
	vector<glm::vec2> positions;
	vector<string > texts;

	ShaderProgram *program;

	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
};

