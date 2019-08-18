#ifndef SYSTEM_H
#define SYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"

class System
{
private:
	// Screen
	const GLint WIDTH = 600, HEIGHT = 600;
	int screenWidth, screenHeight;
	GLfloat left, right, up;
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 translation;


public:
	GLFWwindow* window;
	Shader coreShader;

public:
	System();
	~System();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	int GLFWInit();
	int OpenGLSetup();
	int SystemSetup();

	void DoMovement();
	void Run();

	void Finish();

};

#endif