#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <math.h>
#include "vendor/glm/ext/matrix_clip_space.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/geometric.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/trigonometric.hpp"

class Camera
{
private:
	glm::vec3 eye;              // Camera position
	glm::vec3 up;               // Vector pointing perpendicularly up
	glm::vec3 direction;        // Camera direction
	float fov;

public:
	Camera();
	void set_cam_pos(float x, float y, float z);

	void set_cam_direction(float pitch, float yaw, float roll);

	void move_cam_pos(float x, float y, float z);

	void change_cam_direction(float x, float y, float z);

	void set_fov(float f)
	{
		fov = f;
	}
	
	glm::mat4 get_view_mat() { return glm::lookAt(eye, eye+direction, up);}

	glm::mat4 get_projection_mat(bool is_ortho);

	void get_cam_characterstics()
	{
		std::cout << "Camera at: ";
		std::cout << eye.x << " " << eye.y << " " << eye.z <<  std::endl;
		std::cout << "Camera looking in direction: ";
		std::cout << direction.x << " " << direction.y << " " << direction.z << std::endl;
	}
};

#endif
