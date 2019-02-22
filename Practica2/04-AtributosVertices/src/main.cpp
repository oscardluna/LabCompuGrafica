
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


bool render1 = true;

GLuint VBO, VAO, VBO2, VAO2, VBO3, VAO3, VBO4, VAO4, VBO5, VAO5, VBO6, VAO6, VBO7, VAO7, VBO8, VAO8, VBO9, VAO9, VBO10, VAO10, VBO11, VAO11, VBO12, VAO12, VBO13, VAO13, VBO14, VAO14;


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

	Vertex vertices[] =
	{
		{ { -0.5f, 0.43f, 0.0f },{ 0.98f, 0.98f, 0.82f } },
		{ { 0.5f, 0.43f, 0.0f }, { 0.98f, 0.98f, 0.82f }},
		{ { -0.5f, -0.5f, 0.0f }, { 0.98f, 0.98f, 0.82f }}
	};

	
	Vertex vertices2[] =
	{
		{ { 0.5f, 0.43f, 0.0f } ,{ 0.98f, 0.98f, 0.82f } },
		{ { -0.5f, -0.5f, 0.0f } ,{ 0.98f, 0.98f, 0.82f }},
		{ { 0.5f,  -0.5f, 0.0f } ,{ 0.98f, 0.98f, 0.82f } }
	};

	Vertex vertices3[] =
	{
		{ { -0.75f, 0.5f, 0.0f } ,{ 0.98f, 0.98f, 0.82f } },
		{ { 0.75f, 0.5f, 0.0f } ,{ 0.98f, 0.98f, 0.82f } },
		{ { -0.0f,  0.9f, 0.0f } ,{ 0.98f, 0.98f, 0.82f } }
	};

	 /* P U E R T A*/
	Vertex vertices4[] =
	{
		{ { -0.4f, -0.5f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } },
		{ { -0.4f, 0.1f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } },
		{ { 0.0f,  -0.5f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } }
	};

	Vertex vertices5[] =
	{
		{ { -0.4f, 0.1f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } },
		{ { 0.0f, 0.1f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } },
		{ { 0.0f,  -0.5f, 0.0f } ,{ 0.8f, 0.4f, 0.1f } }
	};

	/*V E N T A N A*/
	Vertex vertices6[] =
	{
		{ { 0.2f, 0.4f, 0.0f } ,{ 0.67f, 0.84f, 0.90f } },
		{ { 0.2f, 0.2f, 0.0f } ,{ 0.67f, 0.84f, 0.90f } },
		{ { 0.4f, 0.2f, 0.0f } ,{ 0.67f, 0.84f, 0.90f } }

	};

	Vertex vertices7[] =
	{
		{ { 0.2f, 0.4f, 0.0f } ,{0.67f, 0.84f, 0.90f } },
		{ { 0.4f, 0.4f, 0.0f } ,{ 0.67f, 0.84f, 0.90f } },
		{ { 0.4f, 0.2f, 0.0f } ,{ 0.67f, 0.84f, 0.90f } }
	};

	/*V E N T A N A  T E C H O */

	Vertex vertices8[] =
	{
		{ { -0.1f, 0.8f, 0.0f } ,{ 0.52f, 0.80f, 0.98f } },
		{ { -0.1f, 0.6f, 0.0f } ,{ 0.52f, 0.80f, 0.98f} },
		{ { 0.1f, 0.6f, 0.0f } ,{ 0.52f, 0.80f, 0.98f } }
	};

	Vertex vertices9[] =
	{
		{ { -0.1f, 0.8f, 0.0f } ,{ 0.52f, 0.80f, 0.98f } },
		{ { 0.1f, 0.8f, 0.0f } ,{ 0.52f, 0.80f, 0.98f } },
		{ { 0.1f, 0.6f, 0.0f } ,{ 0.52f, 0.80f, 0.98f } }
	};

	/* F A C H A D A*/
	Vertex vertices10[] =
	{
		{ { -0.5f, 0.43f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } },
		{ { 0.5f, 0.43f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } },
		{ { -0.5f, 0.5f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } }
	};

	Vertex vertices11[] =
	{
		{ { 0.5f, 0.5f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } },
		{ { -0.5f, 0.5f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } },
		{ { 0.5f, 0.43f, 0.0f } ,{ 0.62f, 0.32f, 0.17f } }
	};

	/* C O N T O R N O  P U E R T A*/
	Vertex vertices12[] =
	{
		{ { -0.37f, -0.47f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } },
		{ { -0.37f, 0.07f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } },
		{ { -0.03f,  -0.47f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } }
	};

	Vertex vertices13[] =
	{
		{ { -0.37f, 0.07f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } },
		{ { -0.03f, 0.07f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } },
		{ { -0.03f,  -0.47f, 0.0f } ,{ 0.64f, 0.16f, 0.16f } }
	};




	const size_t bufferSize = sizeof(vertices);
	const size_t vertexSize = sizeof(vertices[0]);
	const size_t rgbOffset = sizeof(vertices[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize << std::endl;
	std::cout << "Vertex Size:" << vertexSize << std::endl;
	std::cout << "Buffer size:" << rgbOffset << std::endl;

	//Esto es para el primer triangulo
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Esto es para el segundo triangulo

	const size_t bufferSize2 = sizeof(vertices2);
	const size_t vertexSize2 = sizeof(vertices2[0]);
	const size_t rgbOffset2 = sizeof(vertices2[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize2 << std::endl;
	std::cout << "Vertex Size:" << vertexSize2 << std::endl;
	std::cout << "Buffer size:" << rgbOffset2 << std::endl;
	glGenBuffers(1, &VBO2);

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);


	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize2, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize2,
		(GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//Esto es para el tercer triángulo
	
	const size_t bufferSize3 = sizeof(vertices3);
	const size_t vertexSize3 = sizeof(vertices3[0]);
	const size_t rgbOffset3 = sizeof(vertices3[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize3 << std::endl;
	std::cout << "Vertex Size:" << vertexSize3 << std::endl;
	std::cout << "Buffer size:" << rgbOffset3 << std::endl;
	glGenBuffers(1, &VBO3);

	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);


	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices3, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize3, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize3, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Esto es para el cuarto triangulo
	const size_t bufferSize4 = sizeof(vertices4);
	const size_t vertexSize4 = sizeof(vertices4[0]);
	const size_t rgbOffset4 = sizeof(vertices4[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize4 << std::endl;
	std::cout << "Vertex Size:" << vertexSize4 << std::endl;
	std::cout << "Buffer size:" << rgbOffset4 << std::endl;
	glGenBuffers(1, &VBO4);

	glGenVertexArrays(1, &VAO4);
	glBindVertexArray(VAO4);


	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices4, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize4, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize4, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Esto es para el quinto triangulo

	const size_t bufferSize5 = sizeof(vertices5);
	const size_t vertexSize5 = sizeof(vertices5[0]);
	const size_t rgbOffset5 = sizeof(vertices5[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize5 << std::endl;
	std::cout << "Vertex Size:" << vertexSize5 << std::endl;
	std::cout << "Buffer size:" << rgbOffset5 << std::endl;
	glGenBuffers(1, &VBO5);

	glGenVertexArrays(1, &VAO5);
	glBindVertexArray(VAO5);


	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices5, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize5, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize5, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Sexto triángulo
	const size_t bufferSize6 = sizeof(vertices6);
	const size_t vertexSize6 = sizeof(vertices6[0]);
	const size_t rgbOffset6 = sizeof(vertices6[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize6 << std::endl;
	std::cout << "Vertex Size:" << vertexSize6 << std::endl;
	std::cout << "Buffer size:" << rgbOffset6 << std::endl;
	glGenBuffers(1, &VBO6);

	glGenVertexArrays(1, &VAO6);
	glBindVertexArray(VAO6);


	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices6, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize6, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Septimo triángulo

	const size_t bufferSize7 = sizeof(vertices7);
	const size_t vertexSize7 = sizeof(vertices7[0]);
	const size_t rgbOffset7 = sizeof(vertices7[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize7 << std::endl;
	std::cout << "Vertex Size:" << vertexSize7 << std::endl;
	std::cout << "Buffer size:" << rgbOffset7 << std::endl;
	glGenBuffers(1, &VBO7);

	glGenVertexArrays(1, &VAO7);
	glBindVertexArray(VAO7);


	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices7, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize7, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize7, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Octavo
	const size_t bufferSize8 = sizeof(vertices8);
	const size_t vertexSize8 = sizeof(vertices8[0]);
	const size_t rgbOffset8 = sizeof(vertices8[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize8 << std::endl;
	std::cout << "Vertex Size:" << vertexSize8 << std::endl;
	std::cout << "Buffer size:" << rgbOffset8 << std::endl;
	glGenBuffers(1, &VBO8);

	glGenVertexArrays(1, &VAO8);
	glBindVertexArray(VAO8);


	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices8, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize8, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize8, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Noveno
	const size_t bufferSize9 = sizeof(vertices9);
	const size_t vertexSize9 = sizeof(vertices9[0]);
	const size_t rgbOffset9 = sizeof(vertices9[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize9 << std::endl;
	std::cout << "Vertex Size:" << vertexSize9 << std::endl;
	std::cout << "Buffer size:" << rgbOffset9 << std::endl;
	glGenBuffers(1, &VBO9);

	glGenVertexArrays(1, &VAO9);
	glBindVertexArray(VAO9);


	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices9, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize9, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Noveno
	const size_t bufferSize10 = sizeof(vertices10);
	const size_t vertexSize10 = sizeof(vertices10[0]);
	const size_t rgbOffset10 = sizeof(vertices10[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize10 << std::endl;
	std::cout << "Vertex Size:" << vertexSize10 << std::endl;
	std::cout << "Buffer size:" << rgbOffset10 << std::endl;
	glGenBuffers(1, &VBO10);

	glGenVertexArrays(1, &VAO10);
	glBindVertexArray(VAO10);


	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices10, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize10, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Noveno
	const size_t bufferSize11 = sizeof(vertices11);
	const size_t vertexSize11 = sizeof(vertices11[0]);
	const size_t rgbOffset11 = sizeof(vertices11[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize11 << std::endl;
	std::cout << "Vertex Size:" << vertexSize11 << std::endl;
	std::cout << "Buffer size:" << rgbOffset11 << std::endl;
	glGenBuffers(1, &VBO11);

	glGenVertexArrays(1, &VAO11);
	glBindVertexArray(VAO11);


	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices11, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize11, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize11, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 



	//Noveno
	const size_t bufferSize12 = sizeof(vertices12);
	const size_t vertexSize12 = sizeof(vertices12[0]);
	const size_t rgbOffset12 = sizeof(vertices12[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize12 << std::endl;
	std::cout << "Vertex Size:" << vertexSize12 << std::endl;
	std::cout << "Buffer size:" << rgbOffset12 << std::endl;
	glGenBuffers(1, &VBO12);

	glGenVertexArrays(1, &VAO12);
	glBindVertexArray(VAO12);


	glBindBuffer(GL_ARRAY_BUFFER, VBO12);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices12, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize12, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize12, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//Noveno
	const size_t bufferSize13 = sizeof(vertices13);
	const size_t vertexSize13 = sizeof(vertices13[0]);
	const size_t rgbOffset13 = sizeof(vertices13[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize13 << std::endl;
	std::cout << "Vertex Size:" << vertexSize13 << std::endl;
	std::cout << "Buffer size:" << rgbOffset13 << std::endl;
	glGenBuffers(1, &VBO13);

	glGenVertexArrays(1, &VAO13);
	glBindVertexArray(VAO13);


	glBindBuffer(GL_ARRAY_BUFFER, VBO13);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices13, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize13, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize13, (GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

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
		case GLFW_KEY_F:
			render1 = false;
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

		glUseProgram(shaderProgram);

		if (render1)
		{
			glBindVertexArray(VAO);
			//Si queremos dibujar  mas triangulos debemos cambiar el 3 por el numero de vertices

			glDrawArrays(GL_TRIANGLES, 0, 144);
			glDrawArrays(GL_TRIANGLES, 0, 3);

		}
		else
		{
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO4);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO5);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO6);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO7);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO8);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO9);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO10);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO11);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO12);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO13);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO14);
		glDrawArrays(GL_TRIANGLES, 0, 3);


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


