// Vector math functions 

#ifndef __VECTOR_MATRIX_H__
#define __VECTOR_MATRIX_H__


/* enum structure used to clarify the code
   we also use vectors for colors */
enum coord{X=0, R=0, Y=1, G=1, Z=2, B=2, W=3, A=3};

/* Vector structure */
typedef struct
{
	float c[4];
}VECTOR;

/* Matrix structure: matrices a 4x4 */
typedef struct
{
	float c[4][4];
}MATRIX;

/* Create a vector structure */
VECTOR CreateVector(float, float, float, float);

/* Multiply a vector by a vector */
float VectorByVector(VECTOR *v1, VECTOR *v2);

/* Dot product */
float VectorDotProduct(VECTOR *v1, VECTOR *v2);

/* Functions used to create (and to allocate) a matrix */
MATRIX CreateMatrixEmpty();
MATRIX CreateMatrixIdentity();
MATRIX CreateMatrix(float c00, float c01, float c02, float c03,
					 float c10, float c11, float c12, float c13,
					 float c20, float c21, float c22, float c23,
					 float c30, float c31, float c32, float c33);

/* Multiply a 4x4 matrix by another 4x4 matrix and return the result */
MATRIX MatrixByMatrix(MATRIX *mat1, MATRIX *mat2);

/* Multiply a 4x4 matrix by a vector and return the result */
VECTOR MatrixByVector(MATRIX *mat, VECTOR *vec);

/* Multiply a vector by a 4x4 matrix and return the result */
VECTOR VectorByMatrix(VECTOR *vec, MATRIX *mat);

/* Normalize a vector (length = 1) */
void VectorNormalize(VECTOR *v);

/* Return the cross product of two vectors */
VECTOR VectorCrossProduct(VECTOR *v1, VECTOR *v2);

/* Add two vectors and return the result */
VECTOR VectorAdd(VECTOR *v1, VECTOR *v2);

#endif /*__VECTOR_MATRIX_H__*/