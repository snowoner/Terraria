#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
	size = quadSize;
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
		scaleX = animations[currentAnimation].scaleX;
		rotation = animations[currentAnimation].keyframeRot[currentKeyframe];
		posDispl = animations[currentAnimation].keyframePosDisp[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.f), glm::vec3(position.x + posDispl.x, position.y + posDispl.y, 0.f));
	modelview = glm::translate(modelview, glm::vec3(size.x / 2.f, size.x / 2.f, 0.f));
	modelview = glm::rotate(modelview, rotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(scaleX, 1.f, 1.f));
	modelview = glm::translate(modelview, glm::vec3(-size.x / 2.f, -size.x / 2.f, 0.f));
	
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

Sprite* Sprite::clone()
{
	return new Sprite(*this);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimation(int animId, int keyframesPerSec, float scaleX)
{
	if (animId < int(animations.size())) {
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
		animations[animId].scaleX = scaleX;
	}
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement, const glm::vec2 &rotation, const glm::vec2 &posdisp)
{
	if (animId < int(animations.size())) {
		animations[animId].keyframeDispl.push_back(displacement);
		animations[animId].keyframeRot.push_back(rotation);
		animations[animId].keyframePosDisp.push_back(posdisp);
	}
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

int Sprite::keyFrame() const
{
	return currentKeyframe;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}