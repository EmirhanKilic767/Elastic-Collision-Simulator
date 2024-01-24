#include "MatrixHandler.h"


//Constructor to create one matrix and set it to identity
MatrixHandler::MatrixHandler() {
    currentMatrix = new Matrix;
    init();
}

MatrixHandler::~MatrixHandler() {
    flush();
}

// Getting the pointer to the topmost (current) matrix array.
float *MatrixHandler::getCurrentMatrix() const {
    return currentMatrix ? currentMatrix->m : nullptr;
}

// Set the current matrix to the identity matrix
void MatrixHandler::init() const {
    currentMatrix->previous = nullptr;
    currentMatrix->m[0]=1.0f;
    currentMatrix->m[1]=0.0f;
    currentMatrix->m[2]=0.0f;
    currentMatrix->m[3]=0.0f;
    currentMatrix->m[4]=0.0f;
    currentMatrix->m[5]=1.0f;
    currentMatrix->m[6]=0.0f;
    currentMatrix->m[7]=0.0f;
    currentMatrix->m[8]=0.0f;
    currentMatrix->m[9]=0.0f;
    currentMatrix->m[10]=1.0f;
    currentMatrix->m[11]=0.0f;
    currentMatrix->m[12]=0.0f;
    currentMatrix->m[13]=0.0f;
    currentMatrix->m[14]=0.0f;
    currentMatrix->m[15]=1.0f;
}

// Multiplying the current matrix with a rotation around X axes
void MatrixHandler::rotationX(float angle){
    float rx[16];
	rx[0]= 1.0f;
    rx[1]=0.0f;
    rx[2]=0.0f;
    rx[3]=0.0f;
	rx[4]= 0.0f;
    rx[5]=cosf(angle);
    rx[6]=sinf(angle);
    rx[7]= 0.0f;
	rx[8]= 0.0f;
    rx[9]=-sinf(angle);
    rx[10]=cosf(angle);
    rx[11]=0.0f;
	rx[12]=0.0f;
    rx[13]=0.0f;
    rx[14]=0.0f;
    rx[15]=1.0f;
    matrixMultiplication(currentMatrix->m, rx, currentMatrix->m);
}

// Multiplying  the current matrix with a rotation around Y axes
void MatrixHandler::rotationY(float angle){
    float ry[16];
    ry[0]= cosf(angle);
    ry[1]=0.0f;
    ry[2]=-sinf(angle);
    ry[3]=0.0f;
	ry[4]= 0.0f;
    ry[5]=1.0f;
    ry[6]=0.0f;
    ry[7]= 0.0f;
	ry[8]= sinf(angle);
    ry[9]=0.0f;
    ry[10]=cosf(angle);
    ry[11]=0.0f;
	ry[12]=0.0f;
    ry[13]=0.0f;
    ry[14]=0.0f;
    ry[15]=1.0f;
    matrixMultiplication(currentMatrix->m, ry, currentMatrix->m);
}

// Multiplying  the current matrix with a rotation around Z axes
void MatrixHandler::rotationZ(float angle) {
    float rz[16];
	rz[0]= cosf(angle);
    rz[1]=sinf(angle);
    rz[2]=0.0f;
    rz[3]=0.0f;
	rz[4]= -sinf(angle);
    rz[5]=cosf(angle);
    rz[6]=0.0f;
    rz[7]= 0.0f;
	rz[8]= 0.0f;
    rz[9]=0.0f;
    rz[10]=1.0f;
    rz[11]=0.0f;
	rz[12]=0.0f;
    rz[13]=0.0f;
    rz[14]=0.0f;
    rz[15]=1.0f;
    matrixMultiplication(currentMatrix->m, rz, currentMatrix->m);
}

void MatrixHandler::rotAxis(glm::vec3 axis, float angle) {

	axis = glm::normalize(axis); //normalizing the axis

	float ra[16]; //rotation matrix

    // Create a rotation matrix using glm::rotate
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), angle, axis);

    // Extracting the elements of the rotation matrix
    ra[0]  = rotationMatrix[0][0];
    ra[1]  = rotationMatrix[1][0];
    ra[2]  = rotationMatrix[2][0];
    ra[3]  = rotationMatrix[3][0];
    ra[4]  = rotationMatrix[0][1];
    ra[5]  = rotationMatrix[1][1];
    ra[6]  = rotationMatrix[2][1];
    ra[7]  = rotationMatrix[3][1];
    ra[8]  = rotationMatrix[0][2];
    ra[9]  = rotationMatrix[1][2];
    ra[10] = rotationMatrix[2][2];
    ra[11] = rotationMatrix[3][2];
    ra[12] = rotationMatrix[0][3];
    ra[13] = rotationMatrix[1][3];
    ra[14] = rotationMatrix[2][3];
    ra[15] = rotationMatrix[3][3];

    matrixMultiplication(currentMatrix->m, ra, currentMatrix->m);
}

// Multiplying the current matrix with a uniform scaling
void MatrixHandler::scale(float s) {
    float s_mtx[16] = {0};
    s_mtx[0] = s;
    s_mtx[5] = s;
    s_mtx[10] = s;
    s_mtx[15] = 1.0f;
    matrixMultiplication(currentMatrix->m, s_mtx, currentMatrix->m);
}

// Multiplying the current matrix with a translation
void MatrixHandler::translate(glm::vec3 pos) {
    float t_mtx[16] = {0};
    t_mtx[0] = 1.0f;
    t_mtx[5] = 1.0f;
    t_mtx[10] = 1.0f;
    t_mtx[15] = 1.0f;
    t_mtx[12] = pos.x;
    t_mtx[13] = pos.y;
    t_mtx[14] = pos.z;
    matrixMultiplication(currentMatrix->m, t_mtx, currentMatrix->m);
}

// Adding a new level on the stack, by making a copy of the current matrix and
// adding it to the top of the linked list, making it the new current matrix.
void MatrixHandler::push() {
    Matrix *new_matrix;
    int i;

    new_matrix = new Matrix;

    for(i=0; i<16; i++) {
        new_matrix->m[i] = currentMatrix->m[i];
    }
    new_matrix->previous = currentMatrix;
    currentMatrix = new_matrix;
}

// Remove one element from the stack, exposing the element below.
void MatrixHandler::pop() {
    Matrix *previous_matrix;

    previous_matrix = currentMatrix;

    if(previous_matrix == nullptr) {
        std::cerr <<"Stack underflow: Pop attempted on empty stack.\n";
        return;
    }
    currentMatrix = currentMatrix->previous;
    delete previous_matrix;
}

// Remove all elements from the stack
void MatrixHandler::flush() {
    while(currentMatrix != nullptr) {
        pop();
    }
}

// Multiply two matrices.
void MatrixHandler::matrixMultiplication(const float M1[], const float M2[], float Mout[]) {

	float Mtemp[16]; //temporariy matrix
	int i, j;
    // Performing the multiplication matrix 1 * matrix 2
    // (i is row index, j is column index)
	for(i=0; i<4; i++) {
		for(j=0; j<4; j++) {
			Mtemp[i+j*4] = M1[i]*M2[j*4] + M1[i+4]*M2[1+j*4]
				+ M1[i+8]*M2[2+j*4] + M1[i+12]*M2[3+j*4];
		}
	}
	// Copy the result to the output variable
	for(i=0; i<16; i++) {
		Mout[i] = Mtemp[i];
	}
}
