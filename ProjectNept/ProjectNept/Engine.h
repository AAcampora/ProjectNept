#pragma once
#include<glad/glad.h>	
#include<GLFW/glfw3.h>
#include"stb_image.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Vertex.h"
#include "ShaderCompiler.h"

class NeptShark
{
public:
	NeptShark(GLFWwindow* window);
	~NeptShark();
	void processInput(GLFWwindow* window);
	void CompileShaders();
	void CreateShaderProgram();
	void BufferHandler();
	void textureHandler();

	void mouse_callBack(GLFWwindow* window, double xpos, double ypos);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

private:
	unsigned int VBO; //Vertex buffer Object ID
	unsigned int VAO; //Vertex array Object ID
	unsigned int EBO; //Element Buffer Object ID
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int texture;
	//camera params
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;


};
