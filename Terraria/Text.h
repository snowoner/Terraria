#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

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
	TextureGenerator textGenerator;
};

#endif // _TEXT_INCLUDE