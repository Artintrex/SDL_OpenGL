/*
A template for building multi-platform graphic applications using SDL2, OpenGL, GLEW, GLM, STB libraries. 

This software is under the MIT license.

Copyright (c) 2019 Egemen Gungor
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "system.h"
#include "stb_image.h"

SDL_Window* gWindow = NULL;

//Nintendo MakeFloat functions
Float2 MakeFloat2(float x, float y) {
	return Float2(x, y);
}
Float3 MakeFloat3(float x, float y, float z) {
	return Float3(x, y, z);
}
Float4 MakeFloat4(float r, float g, float b, float w) {
	return Float4(r, g, b, w);
}

//Ortho Matrix Conversion from Nintendo API
glm::mat4 OrthographicRightHanded(float width, float height, float nearClip, float farClip) {
	float depth = (farClip - nearClip);

	return glm::mat4(
		2.0f / width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		2.0f / height,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		-1.0f / depth,
		0.0f,

		0.0f,
		0.0f,
		-nearClip / depth,
		1.0f);
}

//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

//Controller Functions
//Api includes nn::hid hid_NpadJoy, hid_Vibration, hid_NpadSixAxisSensor, hid_TouchScreen
void InitController();
void UninitController();
void UpdateController();
bool GetControllerPress(int button);
bool GetControllerTrigger(int button);
Float2 GetControllerLeftStick();
Float2 GetControllerRightStick();
void SetControllerLeftVibration(int frame);
void SetControllerRightVibration(int frame);
Float3 GetControllerLeftAcceleration();
Float3 GetControllerRightAcceleration();
Float3 GetControllerLeftAngle();
Float3 GetControllerRightAngle();
bool GetControllerTouchScreen();
Float2 GetControllerTouchScreenPosition();


//OpenGL context
SDL_GLContext gContext;

//Graphics program
GLuint gProgramID = 0;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();
	

	//Stuff from Switch template
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 330\n"
"precision highp float;\n"

"uniform mat4 uProjection;\n"

"layout( location = 0 ) in vec3 inPosition;\n"
"layout( location = 1 ) in vec4 inColor;\n"
"layout( location = 2 ) in vec2 inTexCoord;\n"

"out vec4 vColor;\n"
"out vec2 vTexCoord;\n"

"void main() {\n"
"    vColor = inColor;\n"
"    vTexCoord = inTexCoord;\n"
"    gl_Position = vec4(inPosition, 1.0) * uProjection;\n"
"}\n"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			"#version 330\n"
"precision highp float;\n"

"uniform sampler2D uSampler;\n"
"uniform bool uTextureEnable;\n"

"in vec4 vColor;\n"
"in vec2 vTexCoord;\n"

"out vec4 outColor;\n"

"void main() {\n"
"    if(uTextureEnable)\n"
"		outColor = vColor * texture(uSampler, vTexCoord);\n"
"    else\n"
"		outColor = vColor;\n"
"}\n"
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);


			//Link program
			glLinkProgram(gProgramID);
			glUseProgram(gProgramID);


			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				printProgramLog(gProgramID);
				success = false;
			}
			else
			{

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				//glEnableVertexAttribArray(3);

				glm::mat4 projection = OrthographicRightHanded((float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT, 0.0f, 1.0f);
				
				glUniformMatrix4fv(glGetUniformLocation(gProgramID, "uProjection"), 1, GL_TRUE, (float*)& projection);

				glUniform1i(glGetUniformLocation(gProgramID, "uTextureEnable"), 0);

			}
		}
	}
	
	return success;
}

void handleKeys(unsigned char key, int x, int y)
{

}

void close()
{
	//Deallocate program
	glDeleteProgram(gProgramID);

	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

//Swaps Buffers
void SwapBuffers() {
	SDL_GL_SwapWindow(gWindow);
}

//Loads Textures from the drive
unsigned int LoadTexture(const char* FileName)
{

	int width, height, nrChannels;
	unsigned char* data = stbi_load(FileName, &width, &height, &nrChannels, 0);

	unsigned int texture;
	unsigned int format = GL_RGB;

	// Generate texture and set parameters
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (nrChannels == 4)format = GL_RGBA;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		printf("Failed to load texture %s", FileName);
	}

	stbi_image_free(data);

	return texture;
}

void UnloadTexture(unsigned int Texture)
{

	glDeleteTextures(1, &Texture);

}

void SetTexture(unsigned int Texture)
{
	if (Texture == 0)
	{
		glUniform1i(glGetUniformLocation(gProgramID, "uTextureEnable"), 0);
	}
	else
	{
		glUniform1i(glGetUniformLocation(gProgramID, "uTextureEnable"), 1);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
}