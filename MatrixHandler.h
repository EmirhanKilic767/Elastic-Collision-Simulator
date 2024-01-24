#pragma once
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


class Matrix { //Matrix class
public:
  float m[16];  // Array to store matrix elements
  Matrix *previous;  // Pointer to the previous matrix in the stack
};

class MatrixHandler {

public:
    //current matrix object derived from Matrix class
    Matrix *currentMatrix;

    //Constructor to create one matrix and set it to identity
    MatrixHandler();

    //Delete all elements from the stack
    ~MatrixHandler();

    // Get the pointer to the topmost (current) matrix array
    float* getCurrentMatrix() const;

    // Set the topmost (current) matrix to the identity matrix
    void init() const;

    // Multiply the topmost (current) matrix with a rotation around X
    void rotationX(float angle);

    // Multiply the topmost (current) matrix with a rotation around Y
    void rotationY(float angle);

    // Multiply the topmost (current) matrix with a rotation around Z
    void rotationZ(float angle);

	void rotAxis(glm::vec3 axis, float angle);

    // Multiply the topmost (current) matrix with a uniform scaling
    void scale(float s);

    // Multiply the topmost (current) matrix with a translation
    void translate(glm::vec3 pos);

    // Add a new level on the stack, making a copy of the topmost matrix
    void push();

    // Remove one element from the stack, exposing the element below
    void pop();

    // Remove all elements except the last one from the stack
    void flush();

    //Multiplying two matrices
    void matrixMultiplication(const float M1[], const float M2[], float Mout[]);


};
