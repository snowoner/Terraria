#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "ShaderProgram.h"

class Text
{
public:
	void init(ShaderProgram &shaderProgram, int type);
	void update();
	void render();

	void addText(string text, const glm::vec2 &position);
	void prepareText(const glm::vec2 &minCoords);
public:

	struct textType {
		short tileSize, blockSize, offset;
		short tileLengthX, tileLengthY;
		char *font;
	};
private:
	int nTiles;
	int typeText;
	vector<glm::vec2> positions;
	vector<string > texts;

	ShaderProgram *program;

	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 tilesheetSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;

};

#endif // _TEXT_INCLUDE