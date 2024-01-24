#pragma once
#include "Setters_Getters.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextureManager.h"


// BallObjectHandler is a class representing a wall object with inherited Setters_Getters functionality
class BallObjectHandler :
	public Setters_Getters
{
public:
	// Creates a sphere  
	BallObjectHandler(glm::vec3 _pos, float _mass, float _rad,TextureManager *textureManager); // Parameterized constructor to create a wall object with given position, mass, and dimensions
	~BallObjectHandler();

	BallObjectHandler()
	{

        // Default constructor initializes member variables
        VAO = 0;
        vertexbuffer = 0;
        indexbuffer = 0;
        vertexarray = NULL;
        indexarray = NULL;
        num_vertex = 0;
        num_triangles = 0;
	};

	void createBall(float radius, int segments,TextureManager *textureManager); // creating ball method
	void render(); // render the ball object

	float getRadius(){return radius;} // get radius of the ball object

private:
	GLuint VAO{};          // Vertex array object, the main handle for geometry
	int num_vertex{}; // Number of vertices in the vertex array
	int num_triangles{};  // Number of triangles in the index array
	GLuint vertexbuffer{}; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer{};  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
	GLfloat *vertexarray{}; // Vertex array
	GLuint *indexarray{};   // Element index array

	float radius; //radius
};