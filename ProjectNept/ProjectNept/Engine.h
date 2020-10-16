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
#include "WaterBuffers.h"

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

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	Camera cam;
	WaterFrameBuffers * WatBuff;
	float lastX;
	float lastY;
	bool firstMouse;
	
private:
	unsigned int waterVAO, waterVBO;
	unsigned int dudvMap;
	unsigned int normalMap;
	unsigned int loc_reflection;
	unsigned int loc_refraction;

	float WaveSpeed = 0.03f;
	float watMovement = 0;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 view;
	glm::mat4 projection;

	void processInput(GLFWwindow* window);
	void WaterPlane();
	int TextureLoader(unsigned int texture, const char* filename);
	void RenderScene(ShaderComp shader, glm::mat4 model, glm::mat4 model2, Model object);
};
