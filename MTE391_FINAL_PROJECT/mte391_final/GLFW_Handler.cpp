#include "GLFW_Handler.h"
#include <iostream>

using namespace std;

GLFW_Handler::GLFW_Handler(GLuint width, GLuint height) : width(width), height(height), window(nullptr) {
    // Starting GLEW extension handler
    if (!glfwInit()) {
        cerr << "ERROR: could not start GLFW3" << endl;
        throw std::runtime_error("GLFW initialization failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Creating GLFW window and selecting context
    window = glfwCreateWindow(width, height, "FINAL PROJECT", NULL, NULL);
    if (!window) {
        cerr << "ERROR: could not open window with GLFW3" << endl;
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window);

    // Starting GLEW extension handler
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "ERROR: GLEW initialization failed" << endl;
        throw std::runtime_error("GLEW initialization failed");
    }
}


void GLFW_Handler::handleKeyInput(double& deltaTime, std::vector<Setters_Getters*>& objectList, TextureManager* textureManager, double& timeSinceAction) {
    // generating random coordinates
    float rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    // Left mouse button press to remove the last object
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && deltaTime > 0.2 && objectList.size() > 1) {
        if(size(objectList) > 5){
            objectList.erase(objectList.end() - 1);
            timeSinceAction = glfwGetTime();
        }
    }
    //Changing textures
    if (glfwGetKey(window, GLFW_KEY_1) && deltaTime > 0.2) {
        //textureManager->loadTexture("/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Textures/rainbowl.jpg");  // Loading rainbow texture
        textureManager->loadTexture("./Textures/rainbowl.jpg");  // Loading rainbow texture
        objectList.push_back(new BallObjectHandler(glm::vec3(0.5f * rand1, 8.0f, 0.5f * rand2), 10.0f, 0.5f, textureManager)); // Adding a ball object to the scene
        timeSinceAction = glfwGetTime();
        glBindVertexArray(0); //binding
    }
    if (glfwGetKey(window, GLFW_KEY_2) && deltaTime > 0.2) {
        //textureManager->loadTexture("/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Textures/ball.jpg");
        textureManager->loadTexture("./Textures/ball.jpg");
        objectList.push_back(new BallObjectHandler(glm::vec3(0.5f * rand1, 8.0f, 0.5f * rand2), 10.0f, 0.5f, textureManager));
        timeSinceAction = glfwGetTime();
        glBindVertexArray(0);
    }
    if (glfwGetKey(window, GLFW_KEY_3) && deltaTime > 0.2) {
        //textureManager->loadTexture("/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Textures/tennisball.jpg");
        textureManager->loadTexture("./Textures/tennisball.jpg");
        objectList.push_back(new BallObjectHandler(glm::vec3(0.5f * rand1, 8.0f, 0.5f * rand2), 10.0f, 0.5f, textureManager));
        timeSinceAction = glfwGetTime();
        glBindVertexArray(0);
    }

    // Adding balls to scene
    if (glfwGetKey(window, GLFW_KEY_Z) && deltaTime > 0.1) {
        objectList.push_back(new BallObjectHandler(glm::vec3(0.5f * rand1, 8.0f, 0.5f * rand2), 60.0f, 2.5f, textureManager));
        timeSinceAction = glfwGetTime();
        glBindVertexArray(0);
    }
    if (glfwGetKey(window, GLFW_KEY_X) && deltaTime > 0.1) {
        textureManager->bindTexture(0);
        objectList.push_back(new BallObjectHandler(glm::vec3(0.5f * rand1, 8.0f, 0.5f * rand2), 10.0f, 0.5f, textureManager));
        timeSinceAction = glfwGetTime();
        glBindVertexArray(0);
    }
    // Removing object
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) && objectList.size() > 1 && deltaTime > 0.2) {
        if(size(objectList) > 5){
            objectList.erase(objectList.end() - 1);
            timeSinceAction = glfwGetTime();
        }

    }
}


void GLFW_Handler::swap() {
    glfwSwapBuffers(window); // Swapping the front and back buffers
}

GLFW_Handler::~GLFW_Handler() {
    glfwTerminate(); // Terminate GLFW
}

//getters
GLFWwindow* GLFW_Handler::getWindow() {
    return window;
}

GLuint GLFW_Handler::getWidth() {
    return width;
}

GLuint GLFW_Handler::getHeight() {
    return height;
}
