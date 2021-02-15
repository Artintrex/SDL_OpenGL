#pragma once

//Include SDL, GLEW and OPENGL 
#include "main.h"
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stdio.h>
#include <string>


typedef glm::vec2 Float2;
typedef glm::vec3 Float3;
typedef glm::vec4 Float4;

//Nintendo style prepares vectors
Float2 MakeFloat2(float x, float y);
Float3 MakeFloat3(float x, float y, float z);
Float4 MakeFloat4(float r, float g, float b, float w);

//Texture functions
unsigned int LoadTexture(const char* FileName);
void UnloadTexture(unsigned int Texture);
void SetTexture(unsigned int Texture);


//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes rendering program and clear color
bool initGL();

//Input handler
void handleKeys(unsigned char key, int x, int y);

//Frees media and shuts down SDL
void close();

//SDL SwapBuffer
void SwapBuffers();

