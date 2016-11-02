#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::changeSize(int w, int h)
{
	windowSize = glm::ivec2(w, h);
	scene.changeWindowSize(windowSize);
}

glm::ivec2 Game::getSize()
{
	return windowSize;
}

void Game::keyPressed(int key)
{
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button, int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;
	bMousePressed[button] = true;
}

void Game::mouseRelease(int button)
{
	bMousePressed[button] = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::isMousePressed(int button) const
{
	return bMousePressed[button];
}

glm::ivec2 Game::getMousePosition() const
{
	return mousePos;
}