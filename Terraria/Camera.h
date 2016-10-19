#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>
#include "Player.h"
#include <algorithm>


class Camera
{
public:

	Camera();
	~Camera();
	void init();
	void update(int deltaTime, const glm::vec2 &pos, const glm::vec2 &vel);
	glm::vec2 getPosition() const { return posCamera; }


private:
	glm::vec2 posCamera;
	float currentTime;
	float cameraWidth, cameraHeight;
	float cameraVx, cameraVy;
	
};


#endif // _CAMERA_INCLUDE