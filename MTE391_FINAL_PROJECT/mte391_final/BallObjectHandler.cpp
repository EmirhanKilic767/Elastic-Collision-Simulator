#include "BallObjectHandler.h"

BallObjectHandler::BallObjectHandler(glm::vec3 _pos, float _mass, float _rad, TextureManager *textureManager)
{
	oType = 'B';//Object type identifier for ball
    position = _pos; //position of the wall
    mass = _mass; //mass
	radius = _rad; //radius of the ball
	centerOfMass = position; // The center of mass is in the objects origin as default

	createBall(_rad, 32, textureManager); //creating ball with texture

	velocity = { 0, 0, 0 }; //Initial velocity
	acceleration = { 0, 0, 0 };// Initial acceleration
	orientation = { 0.0f, 1.0f, 0.0f };// Initial orientation
	rotAxis = { 0.0f, 1.0f, 0.0f };// Initial rotation axis
	angularPosition = 0.0f;// Initial angular position
	angularVelocity = 0.0f;// Initial angular velocity
	angularAcceleration = 0.0f;// Initial angular acceleration

	}


BallObjectHandler::~BallObjectHandler()
{
}


void BallObjectHandler::createBall(float radius, int segments, TextureManager *textureManager) {
	float PI = 3.14159265358979323846;
	int i, j, base, i0;
	float x, y, z, R;
	double theta, phi;
	int vsegs, hsegs; // vertical segments and horizontal segments
	int stride = 8;


	vsegs = segments;
	if (vsegs < 2) vsegs = 2;
	hsegs = vsegs * 2;
	num_vertex = 1 + (vsegs - 1) * (hsegs + 1) + 1; // top + middle + bottom
	num_triangles = hsegs + (vsegs - 2) * hsegs * 2 + hsegs; // top + middle + bottom
	vertexarray = new float[num_vertex * 8];
	indexarray = new GLuint[num_triangles * 3];

	// The vertex array: 3D xyz, 3D normal, 2D st (8 floats per vertex)
	// First vertex: top pole (+z is "up" in object local coords)
	vertexarray[0] = 0.0f;
	vertexarray[1] = 0.0f;
	vertexarray[2] = radius;
	vertexarray[3] = 0.0f;
	vertexarray[4] = 0.0f;
	vertexarray[5] = 1.0f;
	vertexarray[6] = 0.5f;
	vertexarray[7] = 1.0f;
	// Last vertex: bottom pole
	base = (num_vertex - 1)*stride;
	vertexarray[base] = 0.0f;
	vertexarray[base + 1] = 0.0f;
	vertexarray[base + 2] = -radius;
	vertexarray[base + 3] = 0.0f;
	vertexarray[base + 4] = 0.0f;
	vertexarray[base + 5] = -1.0f;
	vertexarray[base + 6] = 0.5f;
	vertexarray[base + 7] = 0.0f;
	// All other vertices:
	// vsegs-1 latitude rings of hsegs+1 vertices each
	for (j = 0; j<vsegs - 1; j++) { // vsegs-1 latitude rings of vertices
		theta = (double)(j + 1) / vsegs*PI;
		z = cos(theta);
		R = sin(theta);
		for (i = 0; i <= hsegs; i++) { // hsegs+1 vertices in each ring (duplicate for texcoords)
			phi = (double)i / hsegs*2.0*PI;
			x = R*cos(phi);
			y = R*sin(phi);
			base = (1 + j*(hsegs + 1) + i)*stride;
			vertexarray[base] = radius*x;
			vertexarray[base + 1] = radius*y;
			vertexarray[base + 2] = radius*z;
			vertexarray[base + 3] = x;
			vertexarray[base + 4] = y;
			vertexarray[base + 5] = z;
			vertexarray[base + 6] = (float)i / hsegs;
			vertexarray[base + 7] = 1.0f - (float)(j + 1) / vsegs;
		}
	}

	// The index array: triplets of integers, one for each triangle
	// Top cap
	for (i = 0; i<hsegs; i++) {
		indexarray[3 * i] = 0;
		indexarray[3 * i + 1] = 1 + i;
		indexarray[3 * i + 2] = 2 + i;
	}
	// Middle part
	for (j = 0; j<vsegs - 2; j++) {
		for (i = 0; i<hsegs; i++) {
			base = 3 * (hsegs + 2 * (j*hsegs + i));
			i0 = 1 + j*(hsegs + 1) + i;
			indexarray[base] = i0;
			indexarray[base + 1] = i0 + hsegs + 1;
			indexarray[base + 2] = i0 + 1;
			indexarray[base + 3] = i0 + 1;
			indexarray[base + 4] = i0 + hsegs + 1;
			indexarray[base + 5] = i0 + hsegs + 2;
		}
	}
	// Bottom cap
	base = 3 * (hsegs + 2 * (vsegs - 2)*hsegs);
	for (i = 0; i<hsegs; i++) {
		indexarray[base + 3 * i] = num_vertex - 1;
		indexarray[base + 3 * i + 1] = num_vertex - 2 - i;
		indexarray[base + 3 * i + 2] = num_vertex - 3 - i;
	}

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(VAO));
	glBindVertexArray(VAO);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

	// Activating the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 8 * num_vertex * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
	// Specifying how many attribute arrays in VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activating the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	//  vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * num_triangles*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

    textureManager->bindTexture(0); //binding texture

    // Binding the Vertex Array Object (VAO) for rendering
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

};


void BallObjectHandler::render()
{
    // rendering ball object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * num_triangles, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}