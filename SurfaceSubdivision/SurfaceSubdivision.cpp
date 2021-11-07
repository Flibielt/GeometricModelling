#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <list>

#include "util.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "ObjReader.hpp"
#include "ModifiedButterflySubdivision.hpp"


static std::vector<glm::vec3> myPoints =
{
	glm::vec3(-0.5f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.0f, 0.0f)
};


glm::mat4 view;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

std::vector<glm::vec3> points;
std::vector<Edge> edges;

GLuint VBO;
GLuint VAO;

GLuint renderingProgram;

ObjReader objReader;
ModifiedButterflySubdivision modifiedButterfly;

void updateData()
{
	std::vector<glm::vec3> allPoints;

	for (Edge edge : edges)
	{
		allPoints.push_back(edge.pStartPoint->point);
		allPoints.push_back(edge.pEndPoint->point);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(glm::vec3), allPoints.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	int start = 0;
	int size = edges.size();
	for (int i = 0; i < edges.size(); i++)
	{
		colorLoc = glGetUniformLocation(renderingProgram, "color");
		glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
		glLineWidth(4.0f);
		glDrawArrays(GL_LINES, start, sizeof(glm::vec3) * 2);

		start += 2;
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	unsigned int modelLoc = glGetUniformLocation(renderingProgram, "model");
	
	unsigned int projectionLoc = glGetUniformLocation(renderingProgram, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(0);
	
}

int main(void) {

	objReader.readFile("bunny.obj");
	//edges = objReader.getEdges();

	modifiedButterfly.vertices = objReader.getVertices();
	modifiedButterfly.edges = objReader.getEdges();
	modifiedButterfly.faces = objReader.getFaces();
	modifiedButterfly.subdivide();

	edges = modifiedButterfly.edges;
	//edges = objReader.getEdges();

	if (!glfwInit()) { exit(EXIT_FAILURE); }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Surface Subversion", NULL, NULL);
	//glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	updateData();

	while (!glfwWindowShouldClose(window)) {

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		unsigned int viewLoc = glGetUniformLocation(renderingProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	cleanUpScene();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}