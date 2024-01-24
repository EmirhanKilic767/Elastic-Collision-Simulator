#include "ShaderManager.h"
#include <fstream>

ShaderManager::ShaderManager(const char *vertexShaderFile, const char *fragmentShaderFile) // Constructor for ShaderManager class
        : programID(0)
{
    // Reading shader source files
    std::string vertexShaderSource = readShaderFile(vertexShaderFile);
    std::string fragmentShaderSource = readShaderFile(fragmentShaderFile);

    // Compiling vertex and fragment shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    // Linking shaders to create a program
    linkShaders(vertexShader, fragmentShader);

    // Cleaning up by deleting individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Destructor for ShaderManager class
ShaderManager::~ShaderManager() {
    // Delete the program if it exists
    if (programID)
        glDeleteProgram(programID);
}

// Compiling a shader of the specified type and return its ID
GLuint ShaderManager::compileShader(GLenum shaderType, const char *shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
// Checking if the shader compilation was successful
    checkShaderCompile(shader, (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment");

    return shader;
}

// Check if shader compilation was successful
void ShaderManager::checkShaderCompile(GLuint shader, const char *shaderType) {
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
 // Printing  error message if compilation fails
    if (compiled == GL_FALSE) {
        char log[4096];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        std::cout << shaderType << " shader compile error" << std::endl;
    }
}
// Reading the contents of a shader file
std::string ShaderManager::readShaderFile(const char *filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot open shader file: " << filename << std::endl;
        return "";
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void ShaderManager::linkShaders(GLuint vertexShader, GLuint fragmentShader) {
    // Creating a shader program
    programID = glCreateProgram();
    // Attaching vertex and fragment shaders to the program
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    // Link the shaders to the program
    glLinkProgram(programID);

    // Check if the program linking was successful
    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE) {
        char log[4096];
        glGetProgramInfoLog(programID, sizeof(log), NULL, log);
        std::cout << "ShaderManager program linking error" << std::endl;
    }
}

