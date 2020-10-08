#include<glad/glad.h>	
#include<GLFW/glfw3.h>

#include "Engine.h"
#include <iostream>

//settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void InitGLFW();
void texSetup();
int checkGLFW(GLFWwindow* window);
int gladInit();

int main()
{
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Initialisation 
	InitGLFW();
	//Window Handler
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	checkGLFW(window);

	glfwMakeContextCurrent(window);

	gladInit();

	texSetup();

//-----------------------------------------------------------------------------------------------------------------------------------------//
	//ViewPort 

	//first we need to tell how big is our render window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//this function will handle the resizing of the window if the user resizes it

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Render Loop (reference yourself to Engine.cpp for all the drawing done by the engine)
	NeptShark* engine = new NeptShark(window);

//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Garbage Collection
	engine->~NeptShark();
	glfwTerminate(); //clear All GLFW resources
	return 0;
}

//this function handles user screen resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void InitGLFW()
{
	//initialise Glfw
	glfwInit();
	//we make sure that we are targeting the right version of OpenGl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

int checkGLFW(GLFWwindow* window)
{
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
}

int gladInit()
{
	//Glad Handler
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

void texSetup()
{
	//setup texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//border colours
	float borderColor[] = { 1.0f, 1.0f, 0.0f , 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//setup texel filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}