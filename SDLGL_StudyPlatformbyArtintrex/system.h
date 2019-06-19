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

