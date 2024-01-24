#include "WallObjectHandler.h"

WallObjectHandler::WallObjectHandler(glm::vec3 _pos, float _mass, glm::vec2 _dim)
{
	position = _pos; //position of the wall
	mass = _mass; //mass
	dim = _dim; // dimension of the wall
	centerOfMass = position; // The center of mass is in the objects origin as default
	oType = 'W'; //Object type identifier for wall
	normal = { 0.0f, 1.0f, 0.0f }; //Normal axis of the wall
	velocity = { 0, 0, 0 }; //Initial velocity
	acceleration = { 0, 0, 0 };// Initial acceleration
	orientation = { 0.0f, 1.0f, 0.0f };// Initial orientation
	rotAxis = { 0.0f, 1.0f, 0.0f };// Initial rotation axis
	angularVelocity = 2.0f; // Initial angular velocity
	angularAcceleration = 0.0f;// Initial angular acceleration

    // Default color of the wall
	color.x = 0.7;
	color.y = 0.7;
	color.z = 0.7;

	GLfloat Wall_Vertices[] = {  // Vertices and indices for a simple rectangular wall
		_dim.x / 2.0f, 0.0f, _dim.y / 2.0f, 0.0f, 1.0f, 0.0f,
		-_dim.x / 2.0f, 0.0f, -_dim.y / 2.0f, 0.0f, 1.0f, 0.0f,
		-_dim.x / 2.0f, 0.0f, _dim.y / 2.0f, 0.0f, 1.0f, 0.0f,
		_dim.x / 2.0f, 0.0f, -_dim.y / 2.0f, 0.0f, 1.0f, 0.0f
	};

	static const GLuint Wall_Indices[] = {
		0, 1, 2, // Base triangle 1
		0, 3, 1, // Base triangle 2

	};

    //  the number of vertices and triangles
	num_vertex = 4;
	num_triangles = 2;

// Allocating memory for vertex and index arrays
	vertexarray = new GLfloat[6 * num_vertex];
	indexarray = new GLuint[3 * num_triangles];

    // Copying vertex and index data
	for (int i = 0; i<6 * num_vertex; i++) {
		vertexarray[i] =  Wall_Vertices[i];
	}
	for (int i = 0; i<3 * num_triangles; i++) {
		indexarray[i] = Wall_Indices[i];
	}

    // Generating vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(VAO));
	glBindVertexArray(VAO);

    // Generating buffer IDs for vertex and index buffers
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

    // Activating the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * num_vertex * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);

    // Enabling vertex attributes
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals

    // Specifying vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals

    // Bind and fill the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * num_triangles*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

    // Binding the Vertex Array Object (VAO) for rendering
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

WallObjectHandler::~WallObjectHandler()
{
}

void WallObjectHandler::render()
{
    // rendering wall object
	glBindVertexArray(VAO);
    glColor3f(color.x,color.y,color.z);
	glDrawElements(GL_TRIANGLES, 3 * num_triangles, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
