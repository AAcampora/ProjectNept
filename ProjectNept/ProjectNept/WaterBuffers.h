#ifndef WATERBUFFER
#define WATERBUFFER
#include <glad/glad.h>
#include<GLFW/glfw3.h>

//this class handles the definition and the processing of the frame buffers used by the water texture
class WaterFrameBuffers
{

public:
	WaterFrameBuffers();
	void CleanUp();

	//Reflection functions
	void BindReflectionFBO();
	int GetReflectionTexture();

	//refraction functions
	void BindRefractionFBO();
	int GetRefractionTexture();
	int GetRefractionRBO();

	void UnbindCurrentFBO();
	
private:

	//Params used by our water buffer resolution when drawn on screen
	const int REFLECTION_WIDTH = 320;
	const int REFLECTION_HEIGHT = 180;

	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;

	//texture Params
	unsigned int reflectionFBO;
	unsigned int reflectionTexture;
	unsigned int reflectionRBO;

	unsigned int refractionFBO;
	unsigned int refractionTexture;
	unsigned int refractionRBO;

	//FrameBuffer Initalizations
	void InitReflectionFBO();
	void InitRefractionFBO();

	//Frame Buffer general Functions
	void BindFBO(int frameBuffer, int width, int height);
	int CreateFBO();
	int CreateTextureAttachment(int width, int height);
	int CreateDepthTextureAttachment(int width, int height);
	int CreateDepthBufferAttachment(int width, int height);

};


#endif // WaterBuffer

