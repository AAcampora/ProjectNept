#include "Engine.h"

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	   // positions   // texCoords
		-2.0f,  2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f,  1.0f, 0.0f,

		-2.0f,  2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  1.0f, 1.0f

};

NeptShark::NeptShark()
{
	WaterPlane();
	dudvMap = TextureHandler(dudvMap, "dudv_map.jpg");
	normalMap = TextureHandler(normalMap, "w_normalmap.jpg");
	cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	WatBuff = new WaterFrameBuffers();
	lastX = SCREEN_WIDTH / 2.0f;
	lastY = SCREEN_HEIGHT / 2.0f;
	firstMouse = true;

}

void NeptShark::MainLoop(GLFWwindow* window)
{
	stbi_set_flip_vertically_on_load(true);

	ShaderComp bpShader ("vertexShader.txt", "fragmentShader.txt");
	ShaderComp waterShader("screenVert.txt", "screenFrag.txt");
	Model ourModel("backpack/backpack.obj");
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

		//frameBuffer pass 1: 
		/*render the scene with the camera filped behind, 
		then store this for later use on the quad that we are going to use as a refelection
		*/
		//rendering section
		WatBuff->BindReflectionFBO();
		glEnable(GL_DEPTH_TEST);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader activation
		bpShader.ActivateShader();

		//pass the camera projection to our shader
		float distance = 2 * (cam.Position.y - 1.0f);
		cam.Position.y -= distance;
		cam.Pitch = -cam.Pitch; // rotate the camera's pitch 180 degrees around
		cam.ProcessMouseMovement(0, 0, false); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
		glm::mat4 view = cam.GetViewMatrix();
		cam.Pitch = -cam.Pitch;
		cam.Position.y += distance;
		cam.ProcessMouseMovement(0, 0, true);
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_WIDTH, 0.1f, 100.0f);
		
		bpShader.setMat4("view", view);
		bpShader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model);
		ourModel.Draw(bpShader);


		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model2);
		ourModel.Draw(bpShader);

		//end of the frameBuffer
		WatBuff->UnbindCurrentFBO();

		WatBuff->BindRefractionFBO();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//load our model
		model = glm::mat4(1.0f);
		view = cam.GetViewMatrix();
		bpShader.setMat4("view", view);

		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model);
		ourModel.Draw(bpShader);

		model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model2);
		ourModel.Draw(bpShader);

		//end of the frameBuffer
		WatBuff->UnbindCurrentFBO();

		//second pass: render everything as normal 
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		//load our model
		model = glm::mat4(1.0f);
		view = cam.GetViewMatrix();
		bpShader.setMat4("view", view);

		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model);
		ourModel.Draw(bpShader);

		model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		bpShader.setMat4("model", model2);
		ourModel.Draw(bpShader);

		//load our Framebuffer texture to our Quad

		waterShader.ActivateShader();
		waterShader.setMat4("view", view);
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("model", glm::mat4(1.0f));

		waterShader.SetInt("reflectionTexture", 0);
		waterShader.SetInt("refractionTexture", 1);
		waterShader.SetInt("dudvMap", 2);
		waterShader.SetInt("normalMap", 3);
		waterShader.setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
		waterShader.setVec3("lightPos", glm::vec3(10.0f, 10.0f, 5.0f));

		float WaveSpeed = 0.03f;
		float watMovement = 0.0f;
		watMovement += WaveSpeed * glfwGetTime();
		watMovement = fmod(watMovement, 1);
		waterShader.SetFloat("moveFactor", watMovement);
		waterShader.setVec3("cameraPos", cam.Position);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WatBuff->GetReflectionTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, WatBuff->GetRefractionTexture());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudvMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap);

		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//check all events and swap the buffers
		glfwSwapBuffers(window); //Swap the color buffer
		glfwPollEvents(); //check if any events have been triggered, like mouse or keyboard inputs
	}
}

NeptShark::~NeptShark()
{
	//Delete everything when the program is closed
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteProgram(shaderProgram);
	WatBuff->CleanUp();

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


int NeptShark::TextureHandler(unsigned int texture, const char* filename )
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
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

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
	return texture;
}

void NeptShark::WaterPlane()
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));

}
