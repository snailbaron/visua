#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include "shader.hpp"

std::string ReadFile(const char *fileName)
{
    std::ifstream fs(fileName);
    if (!fs.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return "";
    }
    std::string text((std::istreambuf_iterator<char>(fs)), (std::istreambuf_iterator<char>()));
    return text;
}

void CompileShader(const std::string &text, GLuint shaderId)
{
    const GLchar *textPtr = text.c_str();
    glShaderSource(shaderId, 1, &textPtr, nullptr);
    glCompileShader(shaderId);

    GLint compileResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
    GLint logLen;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        std::vector<char> log(logLen + 1);
        glGetShaderInfoLog(shaderId, logLen, nullptr, &log[0]);
        std::cout << "Log:" << std::endl << &log[0] << std::endl;
    } else {
        std::cout << "Log is empty" << std::endl;
    }
}

GLuint LoadShaders(const char *vertFile, const char *fragFile)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderText = ReadFile(vertFile);
    if (vertShaderText.empty()) {
        return 0;
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderText = ReadFile(fragFile);
    if (fragShaderText.empty()) {
        return 0;
    }

    std::cout << "Compiling vertex shader: " << vertFile << std::endl;
    CompileShader(vertShaderText, vertShader);

    std::cout << "Compiling fragment shader: " << fragFile << std::endl;
    CompileShader(fragShaderText, fragShader);

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    GLint linkLogLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &linkLogLen);
    if (linkLogLen > 0) {
        std::vector<char> log(linkLogLen + 1);
        glGetProgramInfoLog(program, linkLogLen, nullptr, &log[0]);
        std::cout << "Link log:" << std::endl << &log[0] << std::endl;
    } else {
        std::cout << "Link log is empty" << std::endl;
    }

    glDeleteShader(fragShader);
    glDeleteShader(vertShader);

    return program;
}