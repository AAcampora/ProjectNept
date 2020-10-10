#include "Engine.h"
#include <iostream>

//functions
//initialization functions
void InitGLFW();
int checkGLFW(GLFWwindow* window);
int gladInit();

//camera functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//pointer params
GLFWwindow* window;
NeptShark* engine;

int main()
{
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Initialisation 
	InitGLFW();
	//Window Handler
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	checkGLFW(window);
	//whe the mouse is moved, call back to this functions
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	gladInit();

	//initalise engine
	engine = new NeptShark();
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//ViewPort 

	//first we need to tell how big is our render window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Render Loop (reference yourself to Engine.cpp for all the drawing done by the engine)
	
	engine->MainLoop(window);
	
	//this function will handle the resizing of the window if the user resizes it
//-----------------------------------------------------------------------------------------------------------------------------------------//
	//Garbage Collection
	engine->~NeptShark();
	glfwTerminate(); //clear All GLFW resources
	return 0;
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
	// tell GLFW to capture our mouse
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
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

//this function handles user screen resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (engine->firstMouse)
	{
		engine->lastX = xpos;
		engine->lastY = ypos;
		engine->firstMouse = false;
	}

	float xoffset = xpos - engine->lastX;
	float yoffset = engine->lastY - ypos; // reversed since y-coordinates go from bottom to top

	engine->lastX = xpos;
	engine->lastY = ypos;

	engine->cam.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	engine->cam.ProcessMouseScroll(yoffset);
}

