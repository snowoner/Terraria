#ifndef _TEXT_GEN_INCLUDE
#define _TEXT_GEN_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class TextureGenerator
{
public:
	TextureGenerator();
	~TextureGenerator();

	void init(const string &filename, glm::ivec2 tileSheetSize, int blockSize, int tileSize, int offset);
	void render() const;

	void setTileSheet(const string &filename);
	void addTiles(vector<int> tiles, const glm::vec2 &position);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
private:
	int nTiles;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;

	glm::ivec2 tilesheetSize;
	glm::vec2 tileTexSize;
	int blockSize, tileSize, offset;

	Texture tilesheet;

	vector<glm::vec2> positions;
	vector< vector<int> > tiles;
};

#endif // _TEXT_GEN_INCLUDE