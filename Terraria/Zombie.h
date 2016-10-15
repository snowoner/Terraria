#ifndef _ZOMBIE_INCLUDE
#define _ZOMBIE_INCLUDE

#include "Enemy.h"

class Zombie :
	public Enemy
{
public:
	Zombie(const glm::vec2 &tileMapPos);

	static const char* getURL(){ return "images/zombie.png"; }
	static Sprite* getSprite(Texture &spritesheet, ShaderProgram &shaderProgram);
};

#endif // _ZOMBIE_INCLUDE