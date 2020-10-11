#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderCompiler.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);


//the class model will help us breakdown the model passed throug assimp and inject the data throug our mesh class
//so we can finally draw our image to the screen
class Model 
{
public:
	Model(const char* path)
	{
		LoadModel(path);
	}
	void Draw(ShaderComp& shader);


private:

	//textue params
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	

};

#endif