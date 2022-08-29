#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

extern GLfloat deltaTime;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    RIGHT_VEER,
    LEFT_VEER
};

class Camera
{
	private:
	GLfloat CameraSpeedK = 0.5f;
	public:
	glm::vec3 pos;
	glm::vec3 upWorld;
	glm::vec3 up;
	glm::vec3 right;
	GLfloat radius;

	Camera(GLfloat r);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction);
};

#endif
