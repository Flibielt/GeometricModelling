#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

static int WIN_WIDTH = 800;
static int WIN_HEIGHT = 800;

static int SUBDIVISION_POINT_COUNT = 4;
static std::vector<float> SUBDIVISION_SCHEME = { -((float) 1 / 20), (float) 11 / 20, (float) 11 / 20, -((float) 1 / 20) };
bool started_subdivision = false;

GLint dragged = -1;

GLfloat xNorm;
GLfloat yNorm;

static std::vector<glm::vec3> myPoints =
{
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
};

/* Vertex buffer objektum �s vertex array objektum az adatt�rol�shoz.*/
GLuint VBO;
GLuint VAO;

GLuint renderingProgram;

bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log: " << log << std::endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log: " << log << std::endl;
		free(log);
	}
}

std::string readShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	std::string line = "";

	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram() {

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string vertShaderStr = readShaderSource("vertexShader.glsl");
	std::string fragShaderStr = readShaderSource("fragmentShader.glsl");

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		printShaderLog(vShader);
	}


	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		printShaderLog(fShader);
	}

	// Shader program objektum l�trehoz�sa. Elt�roljuk az ID �rt�ket.
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		printProgramLog(vfProgram);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return vfProgram;
}

GLfloat dist2(glm::vec3 P1, glm::vec3 P2)
{
	GLfloat t1 = P1.x - P2.x;
	GLfloat t2 = P1.y - P2.y;

	return t1 * t1 + t2 * t2;
}

GLint getActivePoint(std::vector<glm::vec3> p, GLint size, GLfloat sens, GLfloat x, GLfloat y)
{

	GLint i;
	GLfloat s = sens * sens;

	xNorm = x / (WIN_WIDTH / 2) - 1.0f;
	yNorm = y / (WIN_HEIGHT / 2) - 1.0f;
	glm::vec3 P = glm::vec3(xNorm, yNorm, 0.0f);

	for (i = 0; i < size; i++) {
		if (dist2(p[i], P) < s) {
			return i;
		}
	}
	return -1;

}

void updateData()
{
	std::vector<glm::vec3> allPoints;

	allPoints.insert(std::end(allPoints), std::begin(myPoints), std::end(myPoints));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(glm::vec3), allPoints.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int normalize_index(int index)
{
	int normalized_index;
	if (index < 0)
	{
		normalized_index = myPoints.size() + index;
	}
	else if (index >= myPoints.size())
	{
		normalized_index = index - myPoints.size();
	}
	else
	{
		return index;
	}

	return normalized_index;
}

glm::vec3 subdivision(int start_index)
{
	float new_point_x, new_point_y;
	glm::vec3 new_point = {0.0f, 0.0f, 0.0f};

	new_point_x = 0.0f;
	new_point_y = 0.0f;
	for (int index = 0; index < SUBDIVISION_POINT_COUNT; index++)
	{
		int normalized_index = normalize_index(start_index + index);
		glm::vec3 point = myPoints[normalized_index];
		float point_x = point[0];
		float point_y = point[1];

		new_point_x += SUBDIVISION_SCHEME[index] * point_x;
		new_point_y += SUBDIVISION_SCHEME[index] * point_y;
	}

	new_point[0] = new_point_x;
	new_point[1] = new_point_y;

	return new_point;
}

void add_points_from_subdivision(std::vector<glm::vec3> new_points)
{
	std::vector<glm::vec3> points;
	for (int index = 0; index < new_points.size(); index++)
	{
		points.push_back(new_points[index]);
		points.push_back(myPoints[index]);
	}

	myPoints.clear();
	myPoints = points;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::vector<glm::vec3> new_points;
	glm::vec3 new_point;

	// Start subdivision
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		// The subdivision needs at least 4 points
		if (myPoints.size() > 3)
		{
			int count = myPoints.size();
			for (int index = -2; index < count - 2; index++)
			{
				new_point = subdivision(index);
				new_points.push_back(new_point);
			}

			add_points_from_subdivision(new_points);
			updateData();
			started_subdivision = true;
		}
	}
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
	GLint i;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if ((i = getActivePoint(myPoints, myPoints.size(), 0.1f, x, WIN_HEIGHT - y)) != -1)
		{
			dragged = i;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		dragged = -1;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		if ((i = getActivePoint(myPoints, myPoints.size(), 0.1f, x, WIN_HEIGHT - y)) != -1)
		{
			myPoints.erase(myPoints.begin() + i);
		}
		else
		{
			myPoints.push_back(glm::vec3(xNorm, yNorm, 0.0f));
		}

		updateData();
	}
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();

	/* L�trehozzuk a sz�ks�ges Vertex buffer �s vertex array objektumot. */
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	/* T�pus meghat�roz�sa: a GL_ARRAY_BUFFER neves�tett csatol�ponthoz kapcsoljuk a buffert (ide ker�lnek a vertex adatok). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/* M�soljuk az adatokat a pufferbe! Megadjuk az aktu�lisan csatolt puffert,  azt hogy h�ny b�jt adatot m�solunk,
	a m�soland� adatot, majd a feldolgoz�s m�dj�t is meghat�rozzuk: most az adat nem v�ltozik a felt�lt�s ut�n */
	glBufferData(GL_ARRAY_BUFFER, myPoints.size() * sizeof(glm::vec3), myPoints.data(), GL_STATIC_DRAW);

	/* A puffer k�sz, lecsatoljuk, m�r nem szeretn�nk m�dos�tani. */
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Csatoljuk a vertex array objektumunkat a konfigur�l�shoz. */
	glBindVertexArray(VAO);

	/* Vertex buffer objektum �jracsatol�sa. */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/* Ezen adatok szolg�lj�k a 0 index� vertex attrib�tumot (itt: poz�ci�).
	Els�k�nt megadjuk ezt az azonos�t�sz�mot.
	Ut�na az attrib�tum m�ret�t (vec3, l�ttuk a shaderben).
	Harmadik az adat t�pusa.
	Negyedik az adat normaliz�l�sa, ez maradhat FALSE jelen p�ld�ban.
	Az attrib�tum �rt�kek hogyan k�vetkeznek egym�s ut�n? Milyen l�p�sk�z ut�n tal�lom a k�vetkez� vertex adatait?
	V�g�l megadom azt, hogy honnan kezd�dnek az �rt�kek a pufferben. Most r�gt�n, a legelej�t�l veszem �ket.*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	/* Enged�lyezz�k az im�nt defini�lt 0 index� attrib�tumot. */
	glEnableVertexAttribArray(0);

	/* Lev�lasztjuk a vertex array objektumot �s a puufert is.*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/** A jelenet�nk ut�ni takar�t�s. */
void cleanUpScene()
{
	/** T�r�lj�k a vertex puffer �s vertex array objektumokat. */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	/** T�r�lj�k a shader programot. */
	glDeleteProgram(renderingProgram);
}

void display(GLFWwindow* window, double currentTime) {
	unsigned int colorLoc;
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // fontos lehet minden egyes alkalommal t�r�lni!

	// aktiv�ljuk a shader-program objektumunkat.
	glUseProgram(renderingProgram);

	/*Csatoljuk a vertex array objektumunkat. */
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
	if (started_subdivision)
	{
		glDrawArrays( GL_LINE_LOOP, 0, myPoints.size());
	}
	else
	{
		glDrawArrays( GL_LINE_STRIP, 0, myPoints.size());
	}

	/* Lev�lasztjuk, nehogy b�rmilyen �rt�k fel�l�r�djon.*/
	glBindVertexArray(0);
}

int main(void) {

	/* Pr�b�ljuk meg inicializ�lni a GLFW-t! */
	if (!glfwInit()) { exit(EXIT_FAILURE); }

	/* A k�v�nt OpenGL verzi� (4.3) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* Pr�b�ljuk meg l�trehozni az ablakunkat. */
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Drag and drop", NULL, NULL);

	/* V�lasszuk ki az ablakunk OpenGL kontextus�t, hogy haszn�lhassuk. */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	/* Incializ�ljuk a GLEW-t, hogy el�rhet�v� v�ljanak az OpenGL f�ggv�nyek. */
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	/* Az alkalmaz�shoz kapcsol�d� el�k�sz�t� l�p�sek, pl. hozd l�tre a shader objektumokat. */
	init(window);

	while (!glfwWindowShouldClose(window)) {
		/* a k�d, amellyel rajzolni tudunk a GLFWwindow ojektumunkba. */
		display(window, glfwGetTime());
		/* double buffered */
		glfwSwapBuffers(window);
		/* esem�nyek kezel�se az ablakunkkal kapcsolatban, pl. gombnyom�s */
		glfwPollEvents();
	}

	/* t�r�lj�k a GLFW ablakot. */
	glfwDestroyWindow(window);
	/* Le�ll�tjuk a GLFW-t */

	cleanUpScene();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}