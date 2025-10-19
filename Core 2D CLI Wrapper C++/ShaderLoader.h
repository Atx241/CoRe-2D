#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
bool LoadShader(std::string vPath, std::string fPath, unsigned int& shaderProgram) {
	std::ifstream vStream;
	std::ifstream fStream;

	vStream.open(vPath);
	fStream.open(fPath);

	std::string vSource;
	std::string fSource;

	if (vStream.is_open()) {
		for (std::string line; std::getline(vStream, line);) {
			vSource += line + "\n";
		}
		vSource += "\0";
	}
	else {
		std::cout << "Cannot read vertex shader file!";
		return false;
	}
	if (fStream.is_open()) {
		for (std::string line; std::getline(fStream, line);) {
			fSource += line + "\n";
		}
		fSource += "\0";
	}
	else {
		std::cout << "Cannot read fragment shader file!";
		return false;
	}

	const char* vSourceCh[] = { vSource.c_str() };
	const char* fSourceCh[] = { fSource.c_str() };

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, vSourceCh, NULL);
	glShaderSource(fShader, 1, fSourceCh, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	int success;
	bool succeded = true;
	char infoLog[512];

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "VERTEX SHADER FAILED TO COMPILE\n" << infoLog << std::endl;
		succeded = false;
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER FAILED TO COMPILE\n" << infoLog << std::endl;
		succeded = false;
	}

	if (!succeded) {
		return false;
	}

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return true;
}