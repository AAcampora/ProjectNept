#include "Engine.h"


NeptShark::NeptShark(GLFWwindow* window)
{
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
}


void NeptShark::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}