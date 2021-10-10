#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#include "util.hpp"

GLint dragged = -1;

GLfloat xNorm;
GLfloat yNorm;

static std::vector<glm::vec3> myPoints =
{
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
};

GLuint VBO;
GLuint VAO;

GLuint renderingProgram;

void updateData()
{
	std::vector<glm::vec3> allPoints;

	allPoints.insert(std::end(allPoints), std::begin(myPoints), std::end(myPoints));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(glm::vec3), allPoints.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	GLint i;

	xNorm = xPos / (WIN_WIDTH / 2) - 1.0f;
	yNorm = (WIN_HEIGHT - yPos) / (WIN_HEIGHT / 2) - 1.0f;

	if (dragged >= 0)
	{
		myPoints.at(dragged).x = xNorm;
		myPoints.at(dragged).y = yNorm;

		updateData();
	}

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, myPoints.size() * sizeof(glm::vec3), myPoints.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void cleanUpScene()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(renderingProgram);
}

void display(GLFWwindow* window, double currentTime) {
	unsigned int colorLoc;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glUseProgram(renderingProgram);

	glBindVertexArray(VAO);

	colorLoc = glGetUniformLocation(renderingProgram, "color");
	glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
	glPointSize(10.0f);
	glDrawArrays(GL_POINTS, 0, myPoints.size());
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	colorLoc = glGetUniformLocation(renderingProgram, "color");
	glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
	glLineWidth(4.0f);
	glDrawArrays( GL_LINE_STRIP, 0, myPoints.size());

	glBindVertexArray(0);
}

int main(void) {

	if (!glfwInit()) { exit(EXIT_FAILURE); }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Drag and drop", NULL, NULL);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	cleanUpScene();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}