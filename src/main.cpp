#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/ext/matrix_clip_space.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/geometric.hpp"
#include "vendor/glm/trigonometric.hpp"
#include "vendor/stb_image/stb_image.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <cmath>
#include <iostream>
#include <fstream>
#include <math.h>

#include "shader.h"
#include "camera.h"

bool first = true;
float lastx = 400, lasty = 300;
float pitch, yaw = -90.0f, zoom = 45.0f;
Camera cam;
int process_input(GLFWwindow *);
void mouse_movement_callback(GLFWwindow *, double, double);
void mouse_scroll_callback(GLFWwindow *, double, double);

void print_all_errors()
{
	bool no_err = true;
	while(auto error = glGetError())
	{
		no_err = false;
		std::cout << "OpenGL error: " << error << std::endl;
	}
	if(no_err)
		std::cout << "No error" << std::endl;
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

 
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return -1;

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_movement_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	std::cout << glGetString(GL_VERSION) << std::endl;

	stbi_set_flip_vertically_on_load(1);
	
	int width, height, nrChannels;
	unsigned char *image = stbi_load("./res/sprites/crate.png", &width, &height, &nrChannels, 0);
	if(!image)
	{
		std::cout << "Error loading background image" << std::endl;
		return -1;
	}

	unsigned int texture1;
	glGenTextures(1, &texture1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);

	unsigned int vao1 = 0;
	glCreateVertexArrays(1, &vao1);

	float vbo_data[] =
	{
		-10.0f, -10.0f, -10.0f, 0.0f, 0.0f,
		 10.0f, -10.0f, -10.0f, 1.0f, 0.0f,
		-10.0f,  10.0f, -10.0f, 0.0f, 1.0f,
		 10.0f,  10.0f, -10.0f, 1.0f, 1.0f,

		-10.0f, -10.0f,  10.0f, 0.0f, 0.0f,
		 10.0f, -10.0f,  10.0f, 1.0f, 0.0f,
		-10.0f,  10.0f,  10.0f, 0.0f, 1.0f,
		 10.0f,  10.0f,  10.0f, 1.0f, 1.0f,

		-10.0f, -10.0f,  10.0f, 1.0f, 0.0f,
		-10.0f,  10.0f,  10.0f, 1.0f, 1.0f,

		10.0f, -10.0f, -10.0f, 0.0f, 0.0f,
		10.0f,  10.0f, -10.0f, 0.0f, 1.0f,

		-10.0f, -10.0f, -10.0f, 0.0f, 1.0f,
		10.0f, -10.0f, -10.0f, 1.0f, 1.0f,

		-10.0f,  10.0f,  10.0f, 0.0f, 0.0f,
		10.0f,  10.0f,  10.0f, 1.0f, 0.0f
	};
	unsigned int t1_buff;
	glGenBuffers(1, &t1_buff);

	glBindVertexArray(vao1);

	unsigned int i1[] = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		0, 8, 2,
		8, 2, 9,
		10, 5, 11,
		5, 11, 7,
		4, 5, 12,
		5, 12, 13,
		14, 15, 2,
		15, 2, 3
	};

	unsigned int index1;
	glGenBuffers(1, &index1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(unsigned int), i1, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, t1_buff);
	glBufferData(GL_ARRAY_BUFFER, 16 * 5 * sizeof(float), vbo_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	cam.set_fov(zoom);

	Shader s1("./res/shaders/vertex.shader", "./res/shaders/fragment.shader");

	s1.use();
	s1.seti("u_btexture", 0);
	s1.setmat4("u_model", glm::value_ptr(model));
	s1.setmat4("u_view", glm::value_ptr(view));
	s1.setmat4("u_projection", glm::value_ptr(projection));

	cam.set_cam_pos(0.0f, 0.0f, 80.0f);
	
	print_all_errors();

	while(!glfwWindowShouldClose(window))
	{
		int action = process_input(window);

		switch (action)
		{
		case -1:
			return 0;
			break;
		case 87:
			cam.move_cam_pos(0.0f, -0.1f, 0.0f);
			break;
		case 65:
			cam.move_cam_pos(0.1f, 0.0f, 0.0f);
			break;
		case 83:
			cam.move_cam_pos(0.0f, 0.1f, 0.0f);
			break;
		case 68:
			cam.move_cam_pos(-0.1f, 0.0f, 0.0f);
			break;
		case -2:
			std::cout << zoom << std::endl;
			break;
		}

		s1.setmat4("u_view", glm::value_ptr(cam.get_view_mat()));
		s1.setmat4("u_projection", glm::value_ptr(cam.get_projection_mat(false)));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(vao1);
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();	

	return 0;
}

int process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		std::cout << "Exiting program" << std::endl;
	    return -1;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
		return 87;
	if (glfwGetKey(window, GLFW_KEY_A))
		return 65;
	if (glfwGetKey(window, GLFW_KEY_S))
		return 83;
	if (glfwGetKey(window, GLFW_KEY_D))
		return 68;
	if(glfwGetKey(window, GLFW_KEY_T))
		return -2;

	return 0;
}

void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
	if(first)
	{
		lastx = xpos;
		lasty = ypos;
		first = false;
	}
	float xoff = xpos - lastx, yoff = ypos - lasty;
	lastx = xpos, lasty = ypos;

	const float sensitivity = 0.01f;
	xoff *= sensitivity;
	yoff *= sensitivity;

	yaw += xoff;
	pitch += yoff;

	cam.set_cam_direction(pitch, yaw, 0.0f);
}

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	zoom -= yoffset;
	if (zoom < 0.0f)
	    zoom = 0.0f;
	else if (zoom > 45.0f)
		zoom = 45.0f;

	cam.set_fov(zoom);
}
