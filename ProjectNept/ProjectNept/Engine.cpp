#include "Engine.h"

//float vertices[] = {
//	// positions          // colors           // texture coords
//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
//};

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//The indices are what we are going to use to connect each points in order to make a triangle
unsigned int indices[] = {
	0, 1, 3,// first triangle
	1, 2, 3 //second triangle
};

NeptShark::NeptShark()
{
	BufferHandler();
	textureHandler();

	cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	lastX = SCREEN_WIDTH / 2.0f;
	lastY = SCREEN_HEIGHT / 2.0f;
	firstMouse = true;

}

void NeptShark::MainLoop(GLFWwindow* window)
{
	ShaderComp* sCompiler = new ShaderComp("vertexShader.txt", "fragmentShader.txt");
	//Render Loop
	while (!glfwWindowShouldClose(window)) //function glfwWindowShouldClose checks if GLFW has been instructed to close
	{
		//DeltaTime update
		//we calculate our deltaTime because different computer will have different frame output, and by doing this
		//we prevent inconsistencies of speed in different systems.
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input section
		processInput(window);

		//rendering section
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture);


		//shader activation
		sCompiler->ActivateShader();

		//pass the camera projection to our shader
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		sCompiler->setMat4("projection", projection);

		//camera view
		glm::mat4 view = cam.GetViewMatrix();
		sCompiler->setMat4("view", view);

		//render Container
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			sCompiler->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	const float cameraSpeed = 2.5f * deltaTime; 
	//close Window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.InputHandler(FORWARD, deltaTime);
	//move back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.InputHandler(BACKWARD, deltaTime);
	//move left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.InputHandler(LEFT, deltaTime);
	//move Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.InputHandler(RIGHT, deltaTime);
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
		5 * sizeof(float), //-------The Stride, which is the space between each attribute
		(void*)0); //---------------The OffSet, which is where the data begins in the buffer
	//Enable the buffer as all buffer are by default made disabled
	glEnableVertexAttribArray(0);
	// colour Attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
