#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "ElementManager.h"

#include "Menu.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void playerActions(const glm::ivec2 &posPlayer, const glm::ivec2 &posCamera);
	void elementCollecion(const glm::ivec2 &posPlayer);
private:
	enum State { ST_MENU, ST_GAME, ST_DEAD, ST_CREDITS };
	State state;
	bool firstTime;

	glm::vec2 posTileMap;

	TileMap *map;
	Camera *camera;
	PlayerManager *playerManager;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Menu *menu;
	Text *text;

	EnemyManager *enemyManager;
	ElementManager *elementManager;
};


#endif // _SCENE_INCLUDE

