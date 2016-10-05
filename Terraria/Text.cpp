#include "Text.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


Text::Text()
{
}


Text::~Text()
{
}


void Text::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {

	tilesheet.loadFromFile("images/font.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	tilesheetSize = glm::vec2(TILE_LENGTH_X, TILE_LENGTH_Y);
	nTiles = 0;

	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	program = &shaderProgram;
}

void Text::update() {

}

void Text::render() {
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void Text::addText(string text, const glm::vec2 &position) {
	positions.push_back(position);
	texts.push_back(text);
}

void Text::prepareText(const glm::vec2 &minCoords) {
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	std::vector<glm::vec2>::iterator itPositions = positions.begin();

	tileSize = 32;
	blockSize = 32;


	for (std::vector<string>::iterator it = texts.begin(); it != texts.end(); ++it) {
		posTile = (glm::vec2)*itPositions;

		posTile = glm::vec2(minCoords.x + posTile.x, minCoords.y + posTile.y);

		for (char& c : *it) {

			// Non-empty tile
			nTiles++;
			tile = c - int(' ');
			posTile = glm::vec2( posTile.x + tileSize, posTile.y);

			texCoordTile[0] = glm::vec2(float((tile) % TILE_LENGTH_X) / tilesheetSize.x, float((tile) / TILE_LENGTH_Y) / tilesheetSize.y);
			texCoordTile[1] = texCoordTile[0] + tileTexSize;
			texCoordTile[1] -= halfTexel;
			// First triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			// Second triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
		}
		++itPositions;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}