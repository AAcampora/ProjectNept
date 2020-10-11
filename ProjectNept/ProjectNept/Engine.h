#pragma once
#include<glad/glad.h>	
#include<GLFW/glfw3.h>
#include"stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Vertex.h"
#include "flyingCamera.h"
#include "ShaderCompiler.h"
#include "Model.h"

#include <iostream>
#include <filesystem>

//settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

class NeptShark
{
public:
	NeptShark();
	~NeptShark();
	void MainLoop(GLFWwindow* window);
	void processInput(GLFWwindow* window);
	void BufferHandler();
	void TextureHandler();

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	Camera cam;
	float lastX;
	float lastY;
	bool firstMouse;
	
private:
	unsigned int VBO; //Vertex buffer Object ID
	unsigned int VAO; //Vertex array Object ID
	unsigned int EBO; //Element Buffer Object ID
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int texture;

};
