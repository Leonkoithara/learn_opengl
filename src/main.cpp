#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "shader.h"

void print_all_errors()
{
	while(auto error = glGetError())
	{
		std::cout << "OpenGL error: " << error << std::endl;
	}
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

	std::cout << glGetString(GL_VERSION) << std::endl;

	unsigned int vao1 = 0;

	glCreateVertexArrays(1, &vao1);

	float vbo_data[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,		
	};
	unsigned int t1_buff;
	glGenBuffers(1, &t1_buff);

	glBindVertexArray(vao1);

	unsigned int i1[] = {
		0, 1, 2,
		1, 2, 3
	};

	unsigned int index1;
	glGenBuffers(1, &index1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), i1, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, t1_buff);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), vbo_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	Shader s1("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");
	s1.use();

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();	
	
	return 0;
}
