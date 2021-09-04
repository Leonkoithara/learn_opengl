#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image/stb_image.h"

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

	stbi_set_flip_vertically_on_load(1);
	
	int width, height, nrChannels;
	unsigned char *image = stbi_load("../res/sprites/background.png", &width, &height, &nrChannels, 0);
	if(!image)
	{
		std::cout << "Error loading image" << std::endl;
		return -1;
	}
	int gwidth, gheight, gnrChannels;
	unsigned char *ghost = stbi_load("../res/sprites/blue.png", &gwidth, &gheight, &gnrChannels, 0);
	if(!ghost)
	{
		std::cout << "Error loading image" << std::endl;
		return -1;
	}

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gwidth, gheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ghost);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
	stbi_image_free(ghost);

	unsigned int vao1 = 0;
	glCreateVertexArrays(1, &vao1);

	float vbo_data[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
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
	glBufferData(GL_ARRAY_BUFFER, 4 * 8 * sizeof(float), vbo_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	Shader s1("../res/shaders/vertex.shader", "../res/shaders/fragment.shader");
	s1.use();
	s1.seti("u_btexture", 0);
	s1.seti("u_gtexture", 1);
	float trans = 0.2f, delta = 0.05f;

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		s1.setf("u_trans", trans);

		if (glfwGetKey(window, GLFW_KEY_UP))
		{
		    trans += delta;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
		    trans -= delta;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(vao1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();	
	
	return 0;
}
