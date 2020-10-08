#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //including Glad to get all the openGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderComp
{
public:
	//the program ID
	unsigned int ID;
	
	ShaderComp(const char* vertexPath, const char* fragmentPath);
	void ActivateShader();

	//utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
};

#endif // SHADER_H
