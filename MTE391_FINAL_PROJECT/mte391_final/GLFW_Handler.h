#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "Setters_Getters.h"
#include "TextureManager.h"
#include "BallObjectHandler.h"

class GLFW_Handler {

private:
    GLFWwindow* window;
    GLuint width;
    GLuint height;


public:
    GLFW_Handler(GLuint width, GLuint height);
    ~GLFW_Handler();

    // Swap buffers
    void swap();

    //Handling key input method
    void handleKeyInput(double& deltaTime, std::vector<Setters_Getters*>& objectList, TextureManager* textureManager, double& timeSinceAction);

    // Getters
    GLFWwindow* getWindow();
    GLuint getWidth();
    GLuint getHeight();
};




