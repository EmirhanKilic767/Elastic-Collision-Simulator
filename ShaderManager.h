#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

class ShaderManager
{
public:
    ShaderManager(const char *vertexShaderFile, const char *fragmentShaderFile); // Constructor to initialize the ShaderManager with vertex and fragment shader files
    ~ShaderManager();// Destructor to clean up resources
    GLuint programID; // OpenGL program ID for the linked shaders

private:

    // Compiling a shader of the specified type and return its ID
    GLuint compileShader(GLenum shaderType, const char *shaderSource);

    // Checking if shader compilation was successful
    void checkShaderCompile(GLuint shader, const char *shaderType);

    // Reading the contents of a shader file and return it as a string
    std::string readShaderFile(const char *filename);

    // Linking vertex and fragment shaders to create an OpenGL program
    void linkShaders(GLuint vertexShader, GLuint fragmentShader);
};


