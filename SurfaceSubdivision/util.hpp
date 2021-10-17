#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include <iostream>
#include <string>
#include <fstream>

static int WIN_WIDTH = 800;
static int WIN_HEIGHT = 800;

// camera
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern bool firstMouse, enableMouse;
extern float yaw, pitch, lastX, lastY, deltaTime, lastTime;

bool checkOpenGLError();
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
std::string readShaderSource(const char* filePath);
GLuint createShaderProgram();

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

#endif