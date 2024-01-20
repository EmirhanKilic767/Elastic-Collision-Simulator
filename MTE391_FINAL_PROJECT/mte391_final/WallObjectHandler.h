#pragma once
#include "Setters_Getters.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// WallObjectHandler is a class representing a wall object with inherited Setters_Getters functionality
class WallObjectHandler :
	public Setters_Getters
{
public:
	WallObjectHandler()
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

    // Parameterized constructor to create a wall object with given position, mass, and dimensions
	WallObjectHandler(glm::vec3 _pos, float _mass, glm::vec2 _dim);
	~WallObjectHandler();

    //getters and setters
	glm::vec3 getNormal() { return normal; }
	void setNormal(glm::vec3 n){ normal = n; }

	void render();  // Method to render the wall object
	glm::vec2 getDim(){ return dim; }

private:
	GLuint VAO;     // Vertex array object
	int num_vertex; // Number of vertices in the vertex array
	int num_triangles;  // Number of triangles in the index array
	GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
	GLfloat *vertexarray; // Vertex array
	GLuint *indexarray;   // Element index array

	glm::vec2 dim;
	glm::vec3 normal;
};
