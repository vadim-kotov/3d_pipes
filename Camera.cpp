#include "Camera.hpp"

Camera::Camera(GLfloat r)
{
	radius = r;
	upWorld = glm::vec3(0.0f, 1.0f, 0.0f);

	float parallel = RAND_FLOAT(glm::pi<float>() / 4);
	float meridian = RAND_FLOAT(glm::pi<float>() / 4);

	pos = radius * glm::vec3(glm::sin(meridian) * glm::cos(parallel), glm::sin(parallel), glm::cos(meridian) * glm::cos(parallel));
	right = glm::normalize(glm::cross(upWorld, -pos));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), upWorld);
}

void Camera::ProcessKeyboard(Camera_Movement direction)
{
	GLfloat CameraSpeed = CameraSpeedK * deltaTime;

	if(direction == FORWARD)
	{
		pos = glm::rotate(glm::mat4(1.0f), CameraSpeed, right) * glm::vec4(pos, 1.0f);
		upWorld = glm::normalize(glm::cross(-pos, right));
	}
	if(direction == BACKWARD)
	{
		pos = glm::rotate(glm::mat4(1.0f), CameraSpeed, -right) * glm::vec4(pos, 1.0f);
		upWorld = glm::normalize(glm::cross(-pos, right));
	}
	if(direction == LEFT)
	{
		pos = glm::rotate(glm::mat4(1.0f), CameraSpeed, -upWorld) * glm::vec4(pos, 1.0f);
		right = glm::normalize(glm::cross(upWorld, -pos));
	}
	if(direction == RIGHT)
	{
		pos = glm::rotate(glm::mat4(1.0f), CameraSpeed, upWorld) * glm::vec4(pos, 1.0f);
		right = glm::normalize(glm::cross(upWorld, -pos));
	}
	if(direction == RIGHT_VEER)
	{
		upWorld = glm::rotate(glm::mat4(1.0f), CameraSpeed, -pos) * glm::vec4(upWorld, 1.0f);
		right = glm::normalize(glm::cross(upWorld, -pos));
	}
	if(direction == LEFT_VEER)
	{
		upWorld = glm::rotate(glm::mat4(1.0f), CameraSpeed, pos) * glm::vec4(upWorld, 1.0f);
		right = glm::normalize(glm::cross(upWorld, -pos));
	}
}
