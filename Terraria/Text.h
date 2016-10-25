#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include "TextureGenerator.h"

class Text
{
public:
	void init(ShaderProgram &shaderProgram, const glm::vec2 &minCoords, int type);
	void render();

	void setPosition(const glm::vec2 &minCoord);

	void addText(string text, glm::vec2 &position);
	void removeTiles();
	void prepareText();
public:
	struct textType {
		short tileSize, blockSize, offset;
		short tileLengthX, tileLengthY;
		char *font;
	};
private:
	TextureGenerator *textGenerator;
};

#endif // _TEXT_INCLUDE