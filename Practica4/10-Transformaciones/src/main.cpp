//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint VBO, VAO, EBO;

struct Vertex {
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
};

// This is for the render with index element
Vertex vertices[] =
{	//frente														
	{ glm::vec3(-0.5f, -0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },	//0
	{ glm::vec3(0.5f , -0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },	//1
	{ glm::vec3(0.5f ,  0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },	//2
	{ glm::vec3(-0.5f,  0.5f, 0.5f) , glm::vec3(1.0f, 0.0f, 0.0f) },	//3

	//atras															
	{ glm::vec3(0.5f , -0.5f, -0.5f), glm::vec3(0.952, 0.729, 0.086) },	//4
	{ glm::vec3(0.5f ,  0.5f, -0.5f), glm::vec3(0.952, 0.729, 0.086) },	//5
	{ glm::vec3(-0.5f , 0.5f, -0.5f) ,glm::vec3(0.952, 0.729, 0.086) },	//6
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(0.952, 0.729, 0.086) },	//7

	//arriba														
	{ glm::vec3(0.5f , 0.5f, -0.5f), glm::vec3(0.956, 0.968, 0.113) },		//8
	{ glm::vec3(-0.5f , 0.5f, -0.5f), glm::vec3(0.956, 0.968, 0.113) },	//9
	{ glm::vec3(0.5f , 0.5f, 0.5f) ,glm::vec3(0.956, 0.968, 0.113) },		//10
	{ glm::vec3(-0.5f , 0.5f, 0.5f),glm::vec3(0.956, 0.968, 0.113f) },		//11

	//abajo											
	{ glm::vec3(-0.5f , -0.5f, 0.5f), glm::vec3(0.356, 0.968, 0.113) },	//12
	{ glm::vec3(0.5f ,  -0.5f, 0.5f), glm::vec3(0.356, 0.968, 0.113) },	//13
	{ glm::vec3(0.5f , -0.5f, -0.5f) ,glm::vec3(0.356, 0.968, 0.113) },	//14
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(0.356, 0.968, 0.113) },	//15

	//izquierda										
	{ glm::vec3(-0.5f , -0.5f, 0.5f), glm::vec3(0.113, 0.368, 0.968) },	//16
	{ glm::vec3(-0.5f ,  0.5f, 0.5f), glm::vec3(0.113, 0.368, 0.968) },	//17
	{ glm::vec3(-0.5f , 0.5f, -0.5f) ,glm::vec3(0.113, 0.368, 0.968) },	//18
	{ glm::vec3(-0.5f , -0.5f, -0.5f),glm::vec3(0.113, 0.368, 0.968) },	//19

	//derecha										
	{ glm::vec3(0.5f , -0.5f, 0.5f), glm::vec3(0.709, 0.203, 0.717) },		//20
	{ glm::vec3(0.5f ,  0.5f, 0.5f), glm::vec3(0.709, 0.203, 0.717) },		//21
	{ glm::vec3(0.5f , -0.5f, -0.5f),glm::vec3(0.709, 0.203, 0.717) },		//22
	{ glm::vec3(0.5f , 0.5f, -0.5f),glm::vec3(0.709, 0.203, 0.717) },		//23
};

GLuint indices[] = {  // Note that we start from 0!
//frente
	0, 1, 2,
	2, 3, 0,
	//atras
	4, 7, 6,
	6, 5, 4,
	//arriba
	8, 9, 11,
	11, 10, 8,
	//abajo
	12, 15, 14,
	14, 13, 12,
	//izquierda
	16, 17, 18,
	18, 19, 16,
	//derecha
	20, 22, 23,
	23, 21, 20
};

Shader shader;

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);
void cubo();

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shader.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	cubo();
}

void cubo() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// This is for the render with index element
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// This is for the render with index element
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*)sizeof(vertices[0].m_Pos));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();


	glm::vec3 cubePositions[] = {
		//posiciones del cubo
		//humano
		glm::vec3(-3.0f, 1.4f, -5.0f), //1
		glm::vec3(-3.0f, 1.125f, -5.0f),	//2
		glm::vec3(-3.0f, 0.5f, -5.0f), //3
		glm::vec3(-3.0f,-0.1f, -5.0f),	//4
		glm::vec3(-3.3f, -0.7f, -5.0f), //5
		glm::vec3(-2.7f, -0.7f, -5.0f),	//6
		glm::vec3(-3.45f, -1.3f, -5.0f), //7
		glm::vec3(-2.55f, -1.3f, -5.0f),	//8
		glm::vec3(-3.7f, 0.8f, -5.0f), //9
		glm::vec3(-3.8f, 0.2f,-5.0f),	//10
		glm::vec3(-2.3f, 0.8f,-5.0f), //11
		glm::vec3(-2.2f, 0.2f, -5.0f),	//12
		glm::vec3(-1.5f, 0.0f,-5.0f), //13
		glm::vec3(-1.8f, 0.0f, -5.0f),	//14
		//perro

		glm::vec3(0.5f, 0.6f, -8.0f),	//15
		glm::vec3(1.1f, 0.08f, -8.0f), //16
		glm::vec3(0.45f, -0.6f,-8.0f),	//17
		glm::vec3(1.75f, -0.6f,-8.0f), //18
		glm::vec3(2.15f, 0.28f, -8.0f),	//19
		glm::vec3(0.1f, 0.5f,-8.0f), //20
		glm::vec3(0.65f, 0.95f, -8.0f),	//21
	};

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader.turnOn();

		GLuint modelLoc = shader.getUniformLocation("model");
		GLuint viewLoc = shader.getUniformLocation("view");
		GLuint projLoc = shader.getUniformLocation("projection");

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);
		float scale = 1.0f;

		/*for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			model = glm::rotate(model, (float)i / 3.1416f, glm::vec3(1.0f, 0.3f, 0.0f));
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// This is for the render with index element
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
			scale += 0.2f;

		};
		*/

		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//humano
		//1
		model = glm::translate(model, cubePositions[0]);
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		//2
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[1]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		
		//3
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[2]);
		model = glm::scale(model, glm::vec3(0.9f, 1.0f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//4
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[3]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.2f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//5
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[4]);
		model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//6
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[5]);
		model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//7
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[6]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//8
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[7]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//9
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[8]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//10
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[9]);
		model = glm::scale(model, glm::vec3(0.18f, 0.8f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);


		//11
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[10]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		// 12
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[11]);
		model = glm::scale(model, glm::vec3(0.18f, 0.8f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//13
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[12]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.15f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//14
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[13]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.5f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);


		//perrito

		//15
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[14]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		// 16
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[15]);
		model = glm::scale(model, glm::vec3(1.7f, 0.6f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//17
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[16]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//18
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[17]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//19
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[18]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		// 20
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[19]);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		// 21
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[20]);
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);

		//model = glm::mat4(1.0f);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// This is for the render with index element
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)0);
		glBindVertexArray(0);

		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}