#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	void update(int deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimation(int animId, int keyframesPerSec, float scaleX = 1.f);
	void addKeyframe(int animId, const glm::vec2 &frame, const glm::vec2 &rotation = glm::vec2(0.f), const glm::vec2 &posdisp = glm::vec2(0.f));
	void changeAnimation(int animId);
	int animation() const; 
	int keyFrame() const;
	
	void setPosition(const glm::vec2 &pos);

	Sprite* clone();

private:
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;
	float scaleX;
	glm::vec2 rotation, posDispl;

	glm::ivec2 size;
};


#endif // _SPRITE_INCLUDE

