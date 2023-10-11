#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"		

#define WIDTH 800
#define HEIGHT 600
#define ERROR_LOG(errorMessage) 			       \
	do { 									       \
		std::cout << errorMessage << std::endl;    \
		glfwTerminate(); 					       \
		return -1; 							       \
	} while (0)

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0f);\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 0.0f);\n"
"}";

// program handle, shader handle, complie and link program
static void createShaderProgram(GLuint &shaderProgram) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
#ifdef __WIN64
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::VALIDATE_FAILED\n" << infoLog << std::endl;
		exit(-1);
	}
#endif // __WIN64
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// vertex specification
GLuint VAO, VBO;
static void createVertexBuffer() {
	GLfloat vertices[] = { -0.5f, -0.5f,  0.0f,
						    0.0f,  0.5f,  0.0f,
						    0.5f, -0.5f,  0.0f };
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void setViewport(GLFWwindow *window) {
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
}

static void renderScreen(GLFWwindow *window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glfwSwapBuffers(window);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac
#endif
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLTutorial1", NULL, NULL);
	if (window == NULL) {
		ERROR_LOG("Failed to create window!");
	}
	glfwMakeContextCurrent(window);
	if (GLEW_OK != glewInit()) {
		ERROR_LOG("Failed to init glew!");
	}
	createVertexBuffer();
	Shader shader = Shader(".\\src\\shaders\\core.vs", ".\\src\\shaders\\core.fs");
	//GLuint shaderProgram = glCreateProgram();
	//createShaderProgram(shaderProgram);

	while (!glfwWindowShouldClose(window)) {
		//glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glfwSwapBuffers(window);
		//glfwPollEvents();

		setViewport(window);
		glfwPollEvents();
		shader.useProgram();
		//glUseProgram(shaderProgram);
		renderScreen(window);
	}
	glfwTerminate();
	std::cout << "Hello world!" << std::endl;
	std::cin.get();
	return 0;
}
