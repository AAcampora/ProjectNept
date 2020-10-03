#include<glad/glad.h>	
#include<GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//initialise Glfw
	glfwInit();
	//we make sure that we are using the right version of OpenGl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Window Handler
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Glad Handler
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//ViewPort 
	//first we need to tell how big is our render window
	glViewport(0, 0, 800, 600);
	//this function will handle the resizing of the window if the user resizes it

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Render Loop
	while (!glfwWindowShouldClose(window)) //function glfwWindowShouldClose checks if GLFW has been instructed to close
	{
		//input section
		processInput(window);

		//rendering section
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//check all events and swap the buffers
		glfwSwapBuffers(window); //Swap the color buffer
		glfwPollEvents(); //check if any events have been triggered, like mouse or keyboard inputs
	}
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Garbage Collection
	glfwTerminate(); //clear All GLFW resources
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
