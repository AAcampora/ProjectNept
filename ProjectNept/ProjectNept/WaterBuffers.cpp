#include "WaterBuffers.h"


WaterFrameBuffers::WaterFrameBuffers()
{
	InitReflectionFBO();
	InitRefractionFBO();
}

void WaterFrameBuffers::CleanUp()
{
	glDeleteFramebuffers(1, &reflectionFBO);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionRBO);
	glDeleteFramebuffers(1, &refractionFBO);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionRBO);
}

void WaterFrameBuffers::BindReflectionFBO()
{
	BindFBO(reflectionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

int WaterFrameBuffers::GetReflectionTexture()
{
	return reflectionTexture;
}

void WaterFrameBuffers::BindRefractionFBO()
{
	BindFBO(refractionFBO, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

int WaterFrameBuffers::GetRefractionTexture()
{
	return refractionTexture;
}

int WaterFrameBuffers::GetRefractionRBO()
{
	return refractionRBO;
}

void WaterFrameBuffers::UnbindCurrentFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800.0f, 600.0f);
}

void WaterFrameBuffers::InitReflectionFBO()
{
	reflectionFBO = CreateFBO();
	reflectionTexture = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionRBO = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	UnbindCurrentFBO();
}

void WaterFrameBuffers::InitRefractionFBO()
{
	refractionFBO = CreateFBO();
	refractionTexture = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	reflectionRBO = CreateDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	UnbindCurrentFBO();
}

void WaterFrameBuffers::BindFBO(int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

int WaterFrameBuffers::CreateFBO()
{
	unsigned int FBO;
	//create the frameBuffer
	glGenFramebuffers(1, &FBO);
	//bind it
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//attach the color to it
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return FBO;
}

int WaterFrameBuffers::CreateTextureAttachment(int width, int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	return texture;
}

int WaterFrameBuffers::CreateDepthTextureAttachment(int width, int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

	return texture;
}

int WaterFrameBuffers::CreateDepthBufferAttachment(int width, int height)
{
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	return RBO;
}
