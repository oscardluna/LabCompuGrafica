//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shaders code
const GLchar* vertexShaderSource = { "#version 400\n"

"layout(location=0) in vec3 position;\n"
"layout(location=1) in vec3 color;\n"
"out vec3 ourColor;\n"

"void main(void)\n"
"{\n"
"  gl_Position = vec4(position, 1.0);\n"
"  ourColor = color;\n"
"}\n" };
const GLchar* fragmentShaderSource = { "#version 400\n"

"in vec3 ourColor;\n"
"out vec4 out_Color;\n"

"void main(void)\n"
"{\n"
"  out_Color = vec4(ourColor, 1.0);\n"
"}\n" };

GLuint VBO, VAO, EBO;
GLint vertexShader, fragmentShader, shaderProgram;

typedef struct {
	float XYZ[3];
	float RGB[3];
} Vertex;

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
void creaRectangulo();
void creaEstrella();
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

	// Build and compile our shader program
	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
			<< std::endl;
	}
	/*
	// This is for the render with index element
	Vertex vertices[] =
	{
		{ { -0.5f, -0.5f, 0.0f } , { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f , -0.5f, 0.0f } , { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f , 0.5f , 0.0f } , { 0.0f, 0.0f, 1.0f } },
		{ {-0.5f , 0.5f , 0.0f } , { 1.0f, 0.0f, 1.0f } },
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};

	const size_t bufferSize = sizeof(vertices);
	const size_t vertexSize = sizeof(vertices[0]);
	const size_t rgbOffset = sizeof(vertices[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize << std::endl;
	std::cout << "Vertex Size:" << vertexSize << std::endl;
	std::cout << "Buffer size:" << rgbOffset << std::endl;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// This is for the render with index element
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

	// This is for the render with index element
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	*/

	//creaRectangulo();
	creaEstrella();
}

void creaEstrella() {

	Vertex vertices[] = {
		{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },//0
		//parte de arriba
		{ { -0.2f, 0.8f, 0.0f } ,{ 1.0f, 0.0f, 1.0f } },//1
		{ { 0.2f, 0.8f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },//2
		{ { 0.0f , 0.8f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },//3
		{ { 0.0f , 1.0f , 0.0f } ,{ 0.2f, 1.0f, 1.0f } },//4

		//parte derecha
		{ { 0.8f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 1.0f } },//5
		{ { 0.8f, -0.2f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },//6
		{ { 0.8f , 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },//7
		{ { 1.0f , 0.0f , 0.0f } ,{ 0.2f, 1.0f, 1.0f } },//8

		//parte abajo
		{ { -0.2f, -0.8f, 0.0f } ,{ 1.0f, 0.0f, 1.0f } },//9
		{ { 0.2f, -0.8f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },//10
		{ { 0.0f , -0.8f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },//11
		{ { 0.0f , -1.0f , 0.0f } ,{ 0.2f, 1.0f, 1.0f } },//12

		//parte izquierda
		{ { -0.8f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 1.0f } },//13
		{ { -0.8f, -0.2f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },//14
		{ { -0.8f , 0.0f, 0.0f } ,{ 1.0f, 1.0f, 0.0f } },//15
		{ { -1.0f , 0.0f , 0.0f } ,{ 0.2f, 1.0f, 1.0f } },//16
	};

	GLuint indices[] = {
		//Arriba
		//sentido antihorario
		0,3,1,
		0,2,3,
		3,4,1,
		3,2,4,
		//derecha
		0,7,5,
		0,6,7,
		7,8,5,
		7,6,8,
		//abajo
		0,11,9,
		0,10,11,
		11,12,9,
		11,10,12,
		//izquierda
		0,13,15,
		0,15,14,
		15,13,16,
		15,16,14
	};

	const size_t VertexSize = sizeof(vertices);
	const size_t StrideSize = sizeof(vertices[0]);
	const size_t OffsetPos = sizeof(vertices[0].XYZ);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	//enlaza el buffer de los datos al vertex array, ocupar VBO como un arreglo de 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//
	glBufferData(GL_ARRAY_BUFFER, VertexSize, vertices, GL_STATIC_DRAW);
	//configura la estructura de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, StrideSize, 0);
	//configurar color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, StrideSize, (GLvoid*)OffsetPos);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//vuelve a su configuracion inicial
	glBindVertexArray(0);


}

void creaRectangulo() {
	Vertex vertices[] = {


	{ { -0.5f, -0.5f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f , -0.5f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f , 0.5f , 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f , 0.5f , 0.0f } ,{ 1.0f, 0.0f, 1.0f } },
	};

	GLuint indices[] = {
		0,1,2,
		0,2,3

	};

	const size_t VertexSize = sizeof(vertices);
	const size_t StrideSize = sizeof(vertices[0]);
	const size_t OffsetPos = sizeof(vertices[0].XYZ);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	//enlaza el buffer de los datos al vertex array, ocupar VBO como un arreglo de 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//
	glBufferData(GL_ARRAY_BUFFER, VertexSize, vertices, GL_STATIC_DRAW);
	//configura la estructura de datos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, StrideSize, 0);
	//configurar color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, StrideSize, (GLvoid*)OffsetPos);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//vuelve a su configuracion inicial
	glBindVertexArray(0);

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	glUseProgram(0);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);

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
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// This is for the render with index element
		//parametros: primitiva,num indices,tipo de dato, apuntador
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
		//12 indices por punta de estrella
		//glDrawArrays(GL_TRIANGLES, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

