#include "Mesh.h"

//the constructor gets the data sent by us and set them ready for our mesh renderer
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::SetupMesh() 
{
	//Step 1: generate IDs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Step 2: Bind the Buffers and tell them what kind of data we passing 
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//step 3: tell the buffers how to use the Data

	//vertex Positon
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//normal Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//Texture Position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	//vertex Tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	//bind them to our VAO
	glBindVertexArray(0);
}

void Mesh::Draw(ShaderComp& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	//we need to retrive the multiple texture names we gonna get from the model file we going to upload
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);//activate texture before binding it
		//retrive texture number 
		string idNum;
		string idName = textures[i].type;

		if (idName == "texture_diffuse")
		{
			idNum = std::to_string(diffuseNr++);
		}
			
		else if (idName == "texture_specular")
		{
			idNum = std::to_string(specularNr++);
		}
		else if (idName == "texture_normal")
		{
			idNum = std::to_string(normalNr++); 
		}
			
		else if (idName == "texture_height")
		{
			idNum = std::to_string(heightNr++); 
		}
			
		//shader.SetFloat(("material." + idName + idNum).c_str(), i);
		glUniform1i(glGetUniformLocation(shader.ID, (idName + idNum).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	//draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}