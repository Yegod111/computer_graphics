#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

typedef void(*check_status)(GLuint, GLenum, GLint *);
typedef void(*read_log)(GLuint, GLsizei, GLsizei *, GLchar *);

#define CHECK_ERROR_AND_LOG(check_status, read_log, obj, checkStatus, errorMessage)	   \
	do {																			   \
		GLint success;																   \
		check_status(obj, checkStatus, &success);									   \
		if (!success) {																   \
			GLchar infoLog[1024];													   \
			read_log(obj, 1024, NULL, infoLog);										   \
			std::cout << errorMessage << std::endl;                                    \
			std::cout << infoLog << std::endl;                                         \
			exit(-1);																   \
		}																			   \
	} while (0)

class Shader {
private:
	GLuint shaderProgram;
public:
	Shader(const char *vertexPath, const char *fragmentPath) {
		shaderProgram = glCreateProgram();
		if (shaderProgram == 0) {
			std::cout << "Error creatnig shader program!" << std::endl;
			exit(1);
		}
		compileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
		compileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
		linkShaderProgram();
	}
	~Shader() {
		glDeleteProgram(shaderProgram);
	}
	void useProgram() {
		glUseProgram(shaderProgram);
	}
private:
	void compileShaderFromFile(const char *path, GLenum shaderType) {
		std::string code;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			code = shaderStream.str();
		}
		catch (std::ifstream::failure &e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const GLchar *shaderCode = code.c_str();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		checkErrors(shader, "SHADER", GL_COMPILE_STATUS);
		glAttachShader(shaderProgram, shader);
		glDeleteShader(shader);
	}
	void checkErrors(GLuint obj, std::string type, GLenum checkStatus) {
		if (type == "SHADER") {
			CHECK_ERROR_AND_LOG(glGetShaderiv, glGetShaderInfoLog, obj, checkStatus, "ERROR::SHADER::COMPILATION_FAILED");
		} else if (type == "PROGRAM") {
			CHECK_ERROR_AND_LOG(glGetProgramiv, glGetProgramInfoLog, obj, checkStatus, "ERROR::PROMGRAM::FAILED");
		}
	}
	void linkShaderProgram() {
		glLinkProgram(shaderProgram);
		checkErrors(shaderProgram, "PROGRAM", GL_LINK_STATUS);
#ifdef __WIN64
		glValidateProgram(shaderProgram);
		checkErrors(shaderProgram, "PROGRAM", GL_VALIDATE_STATUS);
#endif // __WIN64
	}
};