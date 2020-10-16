#include "Engine.h"

float waterVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
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
	//make Our Instances
	cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	WatBuff = new WaterFrameBuffers();

	//Get our textures
	dudvMap = TextureLoader(dudvMap, "dudv_map.jpg");
	normalMap = TextureLoader(normalMap, "w_normalmap.jpg");
	loc_reflection = WatBuff->GetReflectionTexture();
	loc_refraction = WatBuff->GetRefractionTexture();

	//mouse inputs initializations
	lastX = SCREEN_WIDTH / 2.0f;
	lastY = SCREEN_HEIGHT / 2.0f;
	firstMouse = true;
	//additional functions that need to be initialized as well
	WaterPlane();
}

void NeptShark::MainLoop(GLFWwindow* window)
{
	//tell stb_image to flip the texture y axis, allowing the textures to be inserted from the right side
	stbi_set_flip_vertically_on_load(true);

	//load our shaders and our Objects
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
		//-----------------------------------------------------------------------------------------------------------------------------------------//

		//input section
		processInput(window);
		//-----------------------------------------------------------------------------------------------------------------------------------------//
		
		//frameBuffer #1: 
		//we render the scene with the camera translated under the objects, which will allow us to have a copy of the scene that has a lower perpsective
		//allowing us to have a simple reflection simulation
		//rendering section
		WatBuff->BindReflectionFBO();
		glEnable(GL_DEPTH_TEST);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader activation
		bpShader.ActivateShader();

		//pass the camera projection to our shader
		//Calculate the camera downward translation
		float distance = 2 * (cam.Position.y - 1.0f);
		cam.Position.y -= distance;
		//flip the camera's pitch to look upwards
		cam.Pitch = -cam.Pitch;				   // rotate the camera's pitch 180 degrees around
		cam.ProcessMouseMovement(0, 0, false); //disable the mouse inputs and the camera constraints, so we can flip rotate the camera 
		glm::mat4 view = cam.GetViewMatrix();  //update the camera rotation

		//now reset the camera position by reverting the changes we just made
		cam.Pitch = -cam.Pitch;
		cam.Position.y += distance;
		cam.ProcessMouseMovement(0, 0, true);

		//set the fustrum for our scene view, remember we are using a perspective view
		//take notice this will be the only time we set the projection, as it will not change throught the application
		glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_WIDTH, 0.1f, 100.0f);
		
		//send the MVP matrix to our GPU, this will be the final camera output
		bpShader.setMat4("view", view);
		bpShader.setMat4("projection", projection);

		RenderScene(bpShader, model, model2, ourModel);

		//end of the frameBuffer
		WatBuff->UnbindCurrentFBO();

		//-----------------------------------------------------------------------------------------------------------------------------------------//
		
		//Frame buffer #2
		//this is just a print of the scene without any changes, this will be used for the refraction later
		WatBuff->BindRefractionFBO();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//load our model
		//notice that we have to reset the camera every time we set a frame buffer, as it does a weird bug if we don't
		//TODO investigate about this issue
		model = glm::mat4(1.0f);
		view = cam.GetViewMatrix();
		bpShader.setMat4("view", view);

		RenderScene(bpShader, model, model2, ourModel);

		//end of the frameBuffer
		WatBuff->UnbindCurrentFBO();

		//-----------------------------------------------------------------------------------------------------------------------------------------//

		//Now we render everything as normal
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

		//load our model
		model = glm::mat4(1.0f);
		view = cam.GetViewMatrix();
		bpShader.setMat4("view", view);

		RenderScene(bpShader, model, model2, ourModel);


		//-----------------------------------------------------------------------------------------------------------------------------------------//

		//Water plane Render
		waterShader.ActivateShader();

		//Uniform attachments for our Shaders
		waterShader.setMat4("view", view);
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("model", glm::mat4(1.0f));
		waterShader.SetInt("reflectionTexture", 0);
		waterShader.SetInt("refractionTexture", 1);
		waterShader.SetInt("dudvMap", 2);
		waterShader.SetInt("normalMap", 3);
		//Here we hardcode a light source for the meanwhile 
		//TODO add a light-source system
		waterShader.setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
		waterShader.setVec3("lightPos", glm::vec3(10.0f, 10.0f, 5.0f));

		//Here we prepare the water Movement 
		float WaveSpeed = 0.03f;
		float watMovement = 0.0f;
		//we interpolate the DuDv map at every frame
		watMovement += WaveSpeed * glfwGetTime();
		watMovement = fmod(watMovement, 1); //make sure that when the texture reaches 1 is set back to 0
		waterShader.SetFloat("moveFactor", watMovement);
		waterShader.setVec3("cameraPos", cam.Position);

		//create the plane 
		glBindVertexArray(waterVAO);
		//attach the textures to our plane
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, loc_reflection);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, loc_refraction);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudvMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap);

		//draw the the plane to our screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0); //always good practice to turn off the buffer after you are done with the object
		//check all events and swap the buffers
		glfwSwapBuffers(window); //Swap the color buffer
		glfwPollEvents(); //check if any events have been triggered, like mouse or keyboard inputs
	}
}

NeptShark::~NeptShark()
{
	//Delete everything when the program is closed
	glDeleteVertexArrays(1, &waterVAO);
	glDeleteBuffers(1, &waterVBO);
	glDeleteTextures(1, &loc_reflection);
	glDeleteTextures(1, &loc_refraction);
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

int NeptShark::TextureLoader(unsigned int texture, const char* filename )
{
	//generate and bind the texture buffer
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//linking the image I want to use 
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	//test if we have passed the file sucesfuly
	if (data)
	{
		//if yes, load it as a 2D texture
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
	//generate the quad and bind the verticies to the buffer and the vertex Array obj
	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterVBO);
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

	//tell OpenGl how to use the data bussed
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));

}

void NeptShark::RenderScene(ShaderComp shader, glm::mat4 model, glm::mat4 model2,  Model object)
{
	//render the first backpack
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	object.Draw(shader);

	//render the second backpack
	model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
	model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model2);
	object.Draw(shader);
}
