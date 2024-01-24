#pragma once
#include <cmath>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(float _rad);
	~Camera();

	void handle_key(GLFWwindow *window); //handle keys for camera object
    //getters
	float getPhi(){ return phi; }
	float getTheta(){ return theta; }
	float getRad(){ return rad; }
    glm::vec3 getPosition();

private:
    //angles
	float phi; //azimuthal
	float theta; //polar
	float rad;//radial

	float zoom;
    //Time related variables
	double newTime = 0.0;
	double deltaTime = 0.0;
	double currTime = 0.0;
	const float PI = 3.14159265358979f;
};

