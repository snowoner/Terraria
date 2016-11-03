#include "SpriteArray.h"
#include <glm/gtc/matrix_transform.hpp>

void SpriteArray::init(ShaderProgram *program, const glm::vec2 &minCoords, const string &filename,
	glm::ivec2 tileSheetSize, glm::ivec2 blockSize, glm::ivec2 tileSize,
	int textoffset, bool alpha)
{
	tilesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	tilesheetSize = tileSheetSize;
	blocksize = glm::ivec2(blockSize.x, blockSize.y);
	tilesize = tileSize;
	offset = textoffset;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	alphaTex = alpha;
	shaderProgram = program;
	position = minCoords;

}

void SpriteArray::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	if (alphaTex) shaderProgram->setUniform4f("color", 1.f, 1.f, 1.f, 0.5f);
	else shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void SpriteArray::free()
{
	glDeleteBuffers(1, &vbo);
}

void SpriteArray::setPosition(const glm::vec2 &minCoord)
{
	position = minCoord;
}

void SpriteArray::removeTiles()
{
	tiles.clear();
}

void SpriteArray::addTiles(vector<int> tile, glm::vec2 &position) {
	tiles.push_back(std::pair<glm::vec2, vector<int>>(position, tile));
}

void SpriteArray::prepareArrays()
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());

	for (std::vector<pair<glm::vec2, vector<int>>>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
		posTile = it->first;

		for (int c : it->second) {
			if (c != NULLTILE) {
				nTiles++;
				tile = c - offset;
				posTile = glm::vec2(posTile.x, posTile.y);

				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blocksize.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blocksize.x); vertices.push_back(posTile.y + blocksize.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blocksize.x); vertices.push_back(posTile.y + blocksize.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blocksize.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			posTile.x += tilesize.x;
			posTile.y += tilesize.y;
		}
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = shaderProgram->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shaderProgram->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}