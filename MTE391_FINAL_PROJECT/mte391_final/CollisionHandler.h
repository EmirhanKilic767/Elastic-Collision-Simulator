#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Setters_Getters.h"
#include "BallObjectHandler.h"
#include "WallObjectHandler.h"
#include <vector>

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	void calculateMovement(vector<Setters_Getters*> *theEntityList, GLFWwindow *window); //movement calculation method for balls (walls are fixed)

	void integrate(Setters_Getters *E); // integration method, from acc to velocity, velocity to displacement

	void resolveCollision(vector<Setters_Getters*> *theEntityList); // collision detection method

	void calculateTime(); //calculate time
	
	float getTime(){ return currTime; }

private:

    float currTime;          // Current time in the simulation
    float deltaTime;         // Time difference between frames
    glm::vec3 currPos;       // Current position vector
    glm::vec3 currAcc;       // Current acceleration vector
    glm::vec3 currVel;       // Current velocity vector
    glm::vec3 gravity;        // Gravity vector
    glm::vec3 currRotVel;    // Current rotational velocity vector

    glm::vec3 currOrient;    // Current orientation vector
    float currAngVel;        // Current angular velocity
    float currAngAcc;        // Current angular acceleration
    float currAngPos;        // Current angular position

    WallObjectHandler *tempWall;   // Temporary WallObjectHandler for collision detection
    BallObjectHandler *tempBall1;  // Temporary BallObjectHandler for collision detection
    BallObjectHandler *tempBall2;  // Temporary BallObjectHandler for collision detection

    float elasticity = 1.2f;   // Coefficient of restitution, elasticity. You can change it desired elesticity value
    float m1, m2;             // Masses of colliding objects
    float v3, v4;             // Final velocities after collision resolution

};

