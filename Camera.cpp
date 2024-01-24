#include "Camera.h"


Camera::Camera(float _rad)
{
    // Initializing phi, theta, and radial distance
	phi = 0.0f;
	theta = PI / 4.0f;
	rad = _rad;

	zoom = 20.0f; // zoom level
}

Camera::~Camera()
{
}

glm::vec3 Camera::getPosition() {
    glm::vec3 position;

     // Calculeting the camera position based on spherical coordinates/ spherical-to-Cartesian coordinate transformation.
    position.x = rad * sin(theta) * cos(phi);
    position.y = rad * cos(theta);
    position.z = rad * sin(theta) * sin(phi);

    return position;
}


void Camera::handle_key(GLFWwindow *window)
{
    //Time calculations
	newTime = glfwGetTime();
	deltaTime = newTime - currTime;
	currTime = newTime;


    if (glfwGetKey(window, GLFW_KEY_W))
    {
        if (rad > 0.0f){
            rad -= deltaTime*zoom; // zoom in
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        rad += deltaTime*zoom;  //zoom out
    }

    if (glfwGetKey(window, GLFW_KEY_UP)) {
        theta += deltaTime*PI / 2.0; // Rotate 90 degrees per second
        if (theta >= PI / 2.0) theta = PI / 2.0; // Clamp at 90
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        theta -= deltaTime*PI / 2.0; // Rotate 90 degrees per second
        if (theta < 0.1f) theta = 0.1f;      // Clamp at -90
    }


    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        phi -= deltaTime*PI / 2.0; // Rotate 90 degrees per second (pi/2)
        phi = fmod(phi, PI*2.0); // Wrap around at 360 degrees (2*pi)
        if (phi < 0.0) phi += PI*2.0; // If phi<0, then fmod(phi,2*pi)<0
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT))
    {
        phi += deltaTime*PI / 2.0; // Rotate 90 degrees per second (pi/2)
        phi = fmod(phi, PI*2.0);

    }

}