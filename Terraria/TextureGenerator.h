#ifndef _TEXT_GEN_INCLUDE
#define _TEXT_GEN_INCLUDE

#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"

class TextureGenerator
{
public:
	void init(ShaderProgram *program, const string &filename, glm::ivec2 tileSheetSize, int blockSize, int tileSize, int offset, bool alpha = false);
	void render() const;

	void addTiles(vector<int> tiles, const glm::vec2 &position);
	void prepareArrays(const glm::vec2 &minCoords);
private:
	int nTiles;
	ShaderProgram *shaderProgram;
	GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
	bool alphaTex;

	glm::ivec2 tilesheetSize;
	glm::vec2 tileTexSize;
	int blockSize, tileSize, offset;

	Texture tilesheet;

	vector<glm::vec2> positions;
	vector< vector<int> > tiles;
};

#endif // _TEXT_GEN_INCLUDE