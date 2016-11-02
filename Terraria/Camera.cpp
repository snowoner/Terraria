#include "Camera.h"
#include "Game.h"

#define FACTOR 1.2f;

Camera::Camera() 
{
	currentTime = 0;
	posCamera = glm::vec2(0.f);
}

void Camera::update(int deltaTime, const glm::vec2 &pos)
{
	currentTime += deltaTime;
	
	float loopTime = 1.f;

	glm::vec2 posPlayer, velPlayer;
	posPlayer = pos;
	velPlayer = glm::vec2(1.f);
	posPlayer.x += loopTime * velPlayer.x;
	posPlayer.y += loopTime * velPlayer.y;
	cameraWidth = Game::instance().getSize().x;
	cameraHeight = Game::instance().getSize().y;

	if (velPlayer.x > 0.f)
	{
		if ((posPlayer.x - posCamera.x) > (cameraWidth / 3))
		{
			cameraVx = velPlayer.x * FACTOR;
		}
		else cameraVx = 0;
	}
	else
	{
		if ((posPlayer.x - posCamera.x) < (2 * cameraWidth / 3))
		{
			cameraVx = velPlayer.x * FACTOR;
		}
		else cameraVx = 0;
	}
	posCamera.x += loopTime * cameraVx;
	posCamera.x = min(posCamera.x, posPlayer.x - 2 * cameraWidth / 3);
	posCamera.x = max(posCamera.x, posPlayer.x - cameraWidth / 3);

	if (velPlayer.y > 0.f)
	{
		if ((posPlayer.y - posCamera.y) > (cameraHeight / 3))
		{
			cameraVy = velPlayer.y * FACTOR;
		}
		else cameraVy = 0;
	}
	else
	{
		if ((posPlayer.y - posCamera.y) < (2 * cameraHeight / 3))
		{
			cameraVy = velPlayer.y * FACTOR;
		}
		else cameraVy = 0;
	}
	posCamera.y += loopTime * cameraVy;
	posCamera.y = min(posCamera.y, posPlayer.y - 2 * cameraHeight / 3);
	posCamera.y = max(posCamera.y, posPlayer.y - cameraHeight / 3);
}
