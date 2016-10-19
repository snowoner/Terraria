#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "TextureGenerator.h"

class Text
{
public:
	void init(ShaderProgram &shaderProgram, int type);
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
	ShaderProgram *program;
	TextureGenerator textGenerator;
};

#endif // _TEXT_INCLUDE