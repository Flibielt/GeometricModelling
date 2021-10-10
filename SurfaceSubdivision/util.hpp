#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>

static int WIN_WIDTH = 800;
static int WIN_HEIGHT = 800;

bool checkOpenGLError();
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
std::string readShaderSource(const char* filePath);
GLuint createShaderProgram();


#endif