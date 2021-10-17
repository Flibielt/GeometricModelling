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
#include "SubdivisionLine.hpp"
#include "SubdivisionSurface.hpp"

GLint dragged = -1;

GLfloat xNorm;
GLfloat yNorm;

static std::vector<glm::vec3> myPoints =
{
	glm::vec3(-0.5f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.0f, 0.0f)
};

/* Vegyük fel a kamera pozicót tároló változót, illetve a tengelyekhezz szükséges vektorokat. */
/*
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f), up = glm::vec3(0.0f, 1.0f, 0.0f),
	cameraMoving = glm::vec3(0.0, 0.0, -1.0);
*/
float deltaTime = 0.0f, lastTime = 0.0f;

/** Vetítési és kamera mátrixok felvétele. */
glm::mat4 view;
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
bool firstMouse = true;
bool enableMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

std::vector<glm::vec3> points;
std::vector<SubdivisionSurface> surfaces;
std::vector<SubdivisionLine> lines;

GLuint VBO;
GLuint VAO;

GLuint renderingProgram;

void createSurface()
{
	glm::vec3 pointA = glm::vec3(-0.5f, 0.0f, 0.0f);
	glm::vec3 pointB = glm::vec3(0.0f, 0.0f, 0.5f);
	glm::vec3 pointC = glm::vec3(0.5f, 0.0f, 0.0f);
	glm::vec3 pointD = glm::vec3(0.0f, 0.5f, 0.0f);

	points.push_back(pointA);
	points.push_back(pointB);
	points.push_back(pointC);
	points.push_back(pointD);

	SubdivisionSurface surface1 = SubdivisionSurface(pointA, pointB, pointD);
	SubdivisionSurface surface2 = SubdivisionSurface(pointB, pointC, pointD);
	SubdivisionSurface surface3 = SubdivisionSurface(pointA, pointC, pointD);
	SubdivisionSurface surface4 = SubdivisionSurface(pointA, pointB, pointC);

	surfaces.push_back(surface1);
	surfaces.push_back(surface2);
	surfaces.push_back(surface3);
	surfaces.push_back(surface4);

	SubdivisionLine lineA = SubdivisionLine(&pointA, &pointD, &surface3, &surface1);
	SubdivisionLine lineB = SubdivisionLine(&pointA, &pointB, &surface1, &surface4);
	SubdivisionLine lineC = SubdivisionLine(&pointB, &pointD, &surface1, &surface2);
	SubdivisionLine lineD = SubdivisionLine(&pointB, &pointC, &surface2, &surface4);
	SubdivisionLine lineE = SubdivisionLine(&pointC, &pointD, &surface2, &surface3);
	SubdivisionLine lineF = SubdivisionLine(&pointA, &pointC, &surface4, &surface3);

	lines.push_back(lineA);
	lines.push_back(lineB);
	lines.push_back(lineC);
	lines.push_back(lineD);
	lines.push_back(lineE);
	lines.push_back(lineF);
}

void updateData()
{
	std::vector<glm::vec3> allPoints;

	for (int i = 0; i < surfaces.size(); i++)
	{
		allPoints.insert(std::end(allPoints), std::begin(surfaces[i].points), std::end(surfaces[i].points));
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(glm::vec3), allPoints.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		enableMouse = !enableMouse;
}

/** Kiszámoljuk a kamera mátrixokat. */
void computeCameraMatrices()
{
	/* A paraméterek rendre: az új koordinátarendszerünk középpontja (hol a kamera), merre néz a kamera, mit tekintünk ,,fölfele" iránynak */
	//view = glm::lookAt(cameraPos, cameraPos + cameraMoving, up);
	//projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
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

	// Mouse movement
	if (enableMouse == false)
		return;
	
	if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.03f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

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

	for (int i = 0; i < surfaces.size(); i++)
	{
		int start = (i * 3) - 1;
		colorLoc = glGetUniformLocation(renderingProgram, "color");
		glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, start, surfaces[i].points.size());
		//glBindVertexArray(0);

		//glBindVertexArray(VAO);
		colorLoc = glGetUniformLocation(renderingProgram, "color");
		glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
		glLineWidth(4.0f);
		glDrawArrays(GL_LINE_LOOP, start, surfaces[i].points.size());
		//glBindVertexArray(0);
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	unsigned int modelLoc = glGetUniformLocation(renderingProgram, "model");
	

	//computeCameraMatrices();
	
	unsigned int projectionLoc = glGetUniformLocation(renderingProgram, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(0);
	
}

int main(void) {

	if (!glfwInit()) { exit(EXIT_FAILURE); }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Surface Subversion", NULL, NULL);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	createSurface();
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