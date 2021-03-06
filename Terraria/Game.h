#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"


#define SCREEN_X 32
#define SCREEN_Y 16
#define SCREEN_VEC glm::ivec2(SCREEN_X, SCREEN_Y)

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void changeSize(int w, int h);
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button, int x, int y);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	bool isMousePressed(int button) const;
	glm::ivec2 getMousePosition() const;
	glm::ivec2 Game::getSize();

private:
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool bMousePressed[2];
	glm::ivec2 mousePos;

	glm::ivec2 windowSize;


};


#endif // _GAME_INCLUDE


