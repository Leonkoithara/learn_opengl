#include "camera.h"

Camera::Camera()
{
	eye = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Camera::set_cam_pos(float x, float y, float z)
{
	eye.x = x;
	eye.y = y;
	eye.z = z;
}

void Camera::set_cam_direction(float pitch, float yaw, float roll)
{
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction = glm::normalize(direction);
}

void Camera::move_cam_pos(float x, float y, float z)
{
	eye.x += x;
	eye.y += y;
	eye.z += z;
}

void Camera::change_cam_direction(float x, float y, float z)
{
	direction.x += x;
	direction.y += y;
	direction.z += z;
}

glm::mat4 Camera::get_projection_mat(bool is_ortho)
{
	if (!is_ortho)
	{
		return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	}

	return glm::mat4(1.0f);
}

