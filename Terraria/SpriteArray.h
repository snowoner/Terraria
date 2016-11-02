#ifndef _SPRITE_ARRAY_INCLUDE
#define _SPRITE_ARRAY_INCLUDE

#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"

#define NULLTILE -1

class SpriteArray
{
public:
	void init(ShaderProgram *program, const glm::vec2 &minCoords, const string &filename, glm::ivec2 tileSheetSize, glm::ivec2 blockSize, glm::ivec2 tileSize, int offset = 0, bool alpha = false);
	void render() const;

	void setPosition(const glm::vec2 &minCoord);
	void removeTiles();
	void addTiles(vector<int> tiles, glm::vec2 &position);
	void prepareArrays();
	void free();
private:
	int nTiles;
	ShaderProgram *shaderProgram;
	GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
	bool alphaTex;

	glm::ivec2 tilesheetSize, blocksize, tilesize;
	glm::vec2 tileTexSize, position;
	int offset;

	Texture tilesheet;

	vector<std::pair<glm::vec2, vector<int>>> tiles;
};

#endif // _SPRITE_ARRAY_INCLUDE