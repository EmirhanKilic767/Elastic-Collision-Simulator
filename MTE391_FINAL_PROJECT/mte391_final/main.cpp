/* THIS IS THE OPENGL PROJECT THAT SIMULATE THE ELASTIC COLLISION.
 * USER INPUT KEYS:
 * W - S  Zoom in / out for camera
 * UP - DOWN - RIGHT - LEFT  Move and rotate the camera
 * Z - X  Creates ball object with different sizes
 * 1 - 2 - 3  Changes the textures ( IT IS STRONGLY RECOMMEND TO EXPERIENCE THIS FEATURE. CLICK THE 1- 2- 3 KEYS AND ENJOY! )
 * BACKSPACE - Deletes the ball objects
 * LEFT MOUSE CLICK - Deletes the ball objects
 * */

#include <iostream>
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "ShaderManager.h"
#include "MatrixHandler.h"
#include "CollisionHandler.h"
#include "BallObjectHandler.h"
#include "WallObjectHandler.h"
#include "Camera.h"
#include "TextureManager.h"
#include "GLFW_Handler.h"

using namespace std;
const float PI = 3.141592f;
void setWalls(vector<Setters_Getters*> *list, glm::vec3 pos, float dimension); // Function to initialize wall objects

int main()
{
    // time related variables
    double timeSinceAction = glfwGetTime();
    double deltaTime;

    // GL-related variables
    GLfloat ProjectionMatrix[16] = { 2.42f, 0.0f, 0.0f, 0.0f,    //projection matrix to transform 3d coordinates to 2d screen coordinates
                      0.0f, 2.42f, 0.0f, 0.0f,
                      0.0f, 0.0f, -1.0f, -1.0f,
                      0.0f, 0.0f, -0.2f, 0.0f };
    GLfloat Cam[3] = { 0.0f, 0.0f, 0.0f }; //camera position in 3d scene x y z coordinates
    GLfloat Colours[3]; //colors

    //variables to store the locations of uniform var. in shaders
    GLint locationMV; // Location of the model-view matrix uniform in the shader.
    GLint locationP;   //Location of the projection matrix uniform in the shader
    GLint locationCam; //Location of the camera position uniform in the shader.
    GLint locationColor; // Location of the object color uniform in the shader.


    //Window dimensions
    GLuint WIDTH = 1280, HEIGHT =960;

    GLFW_Handler* glfwHandler = new GLFW_Handler(WIDTH,HEIGHT); // GLFW handler

    std::vector<Setters_Getters*> objectList; // vector to store pointers to objects

    //ShaderManager *Shader = new ShaderManager("/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Shaders/vertexshader.vert", "/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Shaders/fragmentshader.frag");
    ShaderManager *Shader = new ShaderManager("./Shaders/vertexshader.vert", "./Shaders/fragmentshader.frag");// initilazing file paths to vertex and fragment shaders
    MatrixHandler *MVstack = new MatrixHandler(); //to manage matrix operations
    MVstack->init();

    Camera *camera = new Camera(140.0f); //camera object

    TextureManager* textureManager = new TextureManager(); //texture manager
    //textureManager->loadTexture("/Users/emirhankilic/Desktop/3.1/mte391/final2/mte391_final/Textures/ball.jpg"); // loading a texture
    textureManager->loadTexture("./Textures/ball.jpg"); // loading a texture


    CollisionHandler *collisionHandler = new CollisionHandler(); // Collision handler initialization

    // linking variables to shader
    locationMV = glGetUniformLocation(Shader->programID, "MV");
    locationP = glGetUniformLocation(Shader->programID, "P");
    locationCam = glGetUniformLocation(Shader->programID, "cameraPosition");
    locationColor = glGetUniformLocation(Shader->programID, "objectColor");

    Setters_Getters* oPointer;  // Declaring a pointer to  object of type Setters_Getters
    vector<Setters_Getters*>* vPointer; // Declaring a pointer to a vector of pointers to Setters_Getters objects
    vPointer = &objectList; // Assigning the address of the objectList vector to vPointer

    setWalls(vPointer, glm::vec3(0.0f, 0.0f, 0.0f), 50.0f); // setting walls in sceen

    glm::mat4 viewMatrix; // 4*4 matrix for camera's view

    while (!glfwWindowShouldClose(glfwHandler->getWindow())) {
        glfwPollEvents();// Event handling
        glm::vec4 cam_era = glm::vec4(0.0, 0.0, 0.0, 1.0); // Camera position

        deltaTime = glfwGetTime() - timeSinceAction;// Time calculation

        glfwHandler->handleKeyInput(deltaTime, objectList, textureManager, timeSinceAction);// Handling key inputs

        // GL calls
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glUseProgram(Shader->programID);

        // Send static variables to vertexshader
        glUniformMatrix4fv(locationP, 1, GL_FALSE, ProjectionMatrix);

        // TIME calc
        collisionHandler->calculateTime();
        camera->handle_key(glfwHandler->getWindow());

        // Transform calculations and rendering
        MVstack->push();
        MVstack->translate(glm::vec3(0.0f, -3.0f, -10.0f));
        MVstack->translate(glm::vec3(0.0f, 0.0f, -camera->getRad()));
        MVstack->rotationX(camera->getTheta());
        MVstack->rotationY(camera->getPhi());
        // View matrix calculation
        viewMatrix = glm::make_mat4(MVstack->getCurrentMatrix());
        cam_era = glm::inverse(viewMatrix) * cam_era;
        // Updating camera position
        Cam[0] = cam_era.x;
        Cam[1] = cam_era.y;
        Cam[2] = cam_era.z;
        glUniform3fv(locationCam, 1, Cam);
        // Collision calculations and resolution
        if (!glfwGetKey(glfwHandler->getWindow(), GLFW_KEY_C)) {
            collisionHandler->calculateMovement(vPointer, glfwHandler->getWindow());
            collisionHandler->resolveCollision(vPointer);
        }

        // Rendering each object in the list
        for (int i = 0; i < vPointer->size(); i++) {
            oPointer = objectList[i];

            MVstack->push();

            MVstack->translate(oPointer->getPosition());
            MVstack->rotAxis(oPointer->getOrientation(), oPointer->getAngularPosition());

            glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack->getCurrentMatrix());

            //random colours if there is no texture
            Colours[0] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            Colours[1] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            Colours[2] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            glUniform3fv(locationColor, 1, Colours);

            // Rendering the object
            oPointer->render();
            MVstack->pop(); //removing the top matrix from matrix stack
        }

        MVstack->pop();
        MVstack->push();//adding new matrix onto matrix stack

        glfwHandler->swap(); //swapping buffers
    }

    //frees the memory
    delete glfwHandler;
    delete Shader;
    delete camera;
    delete collisionHandler;
    delete textureManager;

    return 0;
}

void setWalls(vector<Setters_Getters*>* list, glm::vec3 pos, float dimension)
{
    WallObjectHandler* pLeft;
    WallObjectHandler* pRight;
    WallObjectHandler* pBack;
    WallObjectHandler* pFront;

    // bottom wall
    list->push_back(new WallObjectHandler(pos, 10.0f, glm::vec2(dimension, dimension)));
    pos = glm::vec3(0.0f, 0.1f, 0.0f);
    // left wall
    pLeft = new WallObjectHandler(glm::vec3(pos.x - dimension / 2, pos.y + dimension / 2, pos.z), 10.0f, glm::vec2(dimension, dimension)); // Creating a new wall object
    pLeft->setOrientation(glm::vec3(0.0f, 0.0f, 1.0f));// Setting the orientation of the pRight object to a specific vector
    pLeft->setNormal(glm::vec3(1.0f, 0.0f, 0.0f)); // Setting the normal vector
    pLeft->setAngularPosition(PI / 2); //rotate 90 degree
    list->push_back(pLeft); // add the wall object to  object list

    // right wall
    pRight = new WallObjectHandler(glm::vec3(pos.x + dimension / 2, pos.y + dimension / 2, pos.z), 30.0f, glm::vec2(dimension, dimension));
    pRight->setOrientation(glm::vec3(0.0f, 0.0f, -1.0f));
    pRight->setNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
    pRight->setAngularPosition(PI / 2);
    list->push_back(pRight);

    // back wall
    pBack = new WallObjectHandler(glm::vec3(pos.x, pos.y + dimension/ 2, pos.z + dimension / 2), 30.0f, glm::vec2(dimension, dimension));
    pBack->setOrientation(glm::vec3(1.0f, 0.0f, 0.0f));
    pBack->setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    pBack->setAngularPosition(PI / 2);
    list->push_back(pBack);

    // front wall
    pFront = new WallObjectHandler(glm::vec3(pos.x, pos.y + dimension / 2, pos.z - dimension / 2), 30.0f, glm::vec2(dimension, dimension));
    pFront->setOrientation(glm::vec3(-1.0f, 0.0f, 0.0f));
    pFront->setNormal(glm::vec3(0.0f, 0.0f, -1.0f));
    pFront->setAngularPosition(PI / 2);
    list->push_back(pFront);

}
