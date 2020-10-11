#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> //we always include this as it includes all openGL type  Declaration

//include maths headers
#include "glm/glm.hpp" 
#include "glm/gtc/matrix_transform.hpp"

//aditional needed headers
#include "ShaderCompiler.h"
#include <string>
#include <vector>

using namespace std;

//using this struct as data storage for our Vertex Buffers
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
struct Texture
{
	unsigned int ID;
	string type;
	string path;
};

class Mesh
{
public:
	//mesh data
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	void Draw(ShaderComp& shader);
private:
	//render buffers
	unsigned int VAO, VBO, EBO;
	void SetupMesh();

};
#endif 
