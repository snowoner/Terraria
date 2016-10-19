#include "TextureGenerator.h"
#include <vector>


TextureGenerator::TextureGenerator()
{
}


TextureGenerator::~TextureGenerator()
{
}


void TextureGenerator::init(const string &filename, glm::ivec2 tileSheetSize, int blockSize, int tileSize, int offset)
{
	tilesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	this->tilesheetSize = tileSheetSize;
	this->blockSize = blockSize;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	this->tileSize = tileSize;
	this->offset = offset;
}

void TextureGenerator::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}


void TextureGenerator::addTiles(vector<int> tile, const glm::vec2 &position) {
	positions.push_back(position);
	tiles.push_back(tile);
}

void TextureGenerator::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	nTiles = 0;
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	std::vector<glm::vec2>::iterator itPositions = positions.begin();

	for (std::vector<vector<int>>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
		posTile = (glm::vec2)*itPositions;
		posTile = glm::vec2(minCoords.x + posTile.x, minCoords.y + posTile.y);

		for (int c : *it) {
			// Non-empty tile
			nTiles++;
			tile = c - offset;
			// Non-empty tile
			nTiles++;
			posTile = glm::vec2(posTile.x + tileSize, posTile.y);
			texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
			texCoordTile[1] = texCoordTile[0] + tileTexSize;
			//texCoordTile[0] += halfTexel;
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
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}



