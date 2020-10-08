#include "Engine.h"

//The value of each point of the vertices
//Vertex vertices[] = {
//	{0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f}, //top right
//	{0.5f, -0.5f, 0.0f,	    0.0f, 1.0f, 0.0f,	1.0f, 0.0f}, //bottom right
//	{-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f}, //bottom left
//	{-0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f} //top left
//};

float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

//The indices are what we are going to use to connect each points in order to make a triangle
unsigned int indices[] = {
	0, 1, 3,// first triangle
	1, 2, 3 //second triangle
};

NeptShark::NeptShark(GLFWwindow* window)
{
	ShaderComp* sCompiler = new ShaderComp("vertexShader.txt", "fragmentShader.txt");
	//Render Loop
	while (!glfwWindowShouldClose(window)) //function glfwWindowShouldClose checks if GLFW has been instructed to close
	{
		//input section
		processInput(window);

		//rendering section
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//shader Compilation
		sCompiler->ActivateShader();

		//render a Rectangle
		//0:create the needed buffers and store them in a VAO for later use
		BufferHandler();

		textureHandler();
		//2: use the shader program when we want to render an object

		glUseProgram(shaderProgram);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		//3: now draw the object
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//check all events and swap the buffers
		glfwSwapBuffers(window); //Swap the color buffer
		glfwPollEvents(); //check if any events have been triggered, like mouse or keyboard inputs
	}
}

NeptShark::~NeptShark()
{
	//Delete everything when the program is closed
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}

void NeptShark::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void NeptShark::BufferHandler() 
{
	
	//first thing first, we need to generate the Vertex Array Object
	//the VAO is a container which holds 1 or multiple buffers with their current specification. Allowing us to save our bindings
	//which without we would have to re-instate the buffers every time we draw an object

	//Buffer initialization
	glGenVertexArrays(1, &VAO); //VAO container to hold our current buffer
	glGenBuffers(1, &VBO);	//currently our vertex buffer, holds the information of where the vertices are drawn
	glGenBuffers(1, &EBO); //The Element Buffer is used in order to tell OpenGl the order and what vertices to draw. 
	
	//Vertex Buffer
	//when creating a buffer, always follow this pattern:
	glBindVertexArray(VAO);//bind the buffer to it's VAO for later use
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //assign a buffer type

	//copy the data you want to transfer in the buffer
	glBufferData(GL_ARRAY_BUFFER, //the kind of buffer
		sizeof(vertices),//---------How much data we want to input
		vertices,//-----------------The Data we want to pass
		GL_STATIC_DRAW);//----------How much manipulation the data inside the buffer is allowed to have, in this case, set once and used indefinetly
		//Element Buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position Attribute
	//Give to OpenGL the interpetation of the data
	glVertexAttribPointer(0, //Which attribute we want to configure, atm, we are configuring layout(location = 0), which is 0 in this case
		3, //-----------------------The size of our vertex attribute, because in this case we are using Vec3, we going to use 3 
		GL_FLOAT, //----------------The kind of data we are using, in this case a vec* in GLSL is made up of 3 floats, so we use GL_FLOAT
		GL_FALSE, //----------------Normalization, in this case no, because it would turn our parameters we passed to 1 and 0 
		8 * sizeof(float), //-------The Stride, which is the space between each attribute
		(void*)0); //---------------The OffSet, which is where the data begins in the buffer
	//Enable the buffer as all buffer are by default made disabled
	glEnableVertexAttribArray(0);
	// colour Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);		
}

void NeptShark::textureHandler()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//linking the image I want to use 
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
