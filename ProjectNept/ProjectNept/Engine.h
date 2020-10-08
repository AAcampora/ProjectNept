#pragma once
#include<glad/glad.h>	
#include<GLFW/glfw3.h>
#include"stb_image.h"
#include <iostream>

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

private:
	unsigned int VBO; //Vertex buffer Object ID
	unsigned int VAO; //Vertex array Object ID
	unsigned int EBO; //Element Buffer Object ID
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	unsigned int texture;

};
