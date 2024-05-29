// Vectors and matrices structures

#include "stdafx.h"

/* Create a vector structure */
VECTOR CreateVector(float Xp, float Yp, float Zp, float Wp)
{
	VECTOR v;
	v.c[X] = Xp;
	v.c[Y] = Yp;
	v.c[Z] = Zp;
	v.c[W] = Wp;

	return v;
}

/* Multiply a vector by a vector */
float VectorByVector(VECTOR *v1, VECTOR *v2)
{
	return (v1->c[X]*v2->c[X] + v1->c[Y]*v2->c[Y] + v1->c[Z]*v2->c[Z] + v1->c[W]*v2->c[W]);
}

/* Dot product */
float VectorDotProduct(VECTOR *v1, VECTOR *v2)
{
	return (v1->c[X]*v2->c[X] + v1->c[Y]*v2->c[Y] + v1->c[Z]*v2->c[Z]);
}

/* Functions used to create (and to allocate) a matrix */
MATRIX CreateMatrixEmpty()
{
	return CreateMatrix(0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0);
}

MATRIX CreateMatrixIdentity()
{
	return CreateMatrix(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
}

MATRIX CreateMatrix(float c00, float c01, float c02, float c03,
					 float c10, float c11, float c12, float c13,
					 float c20, float c21, float c22, float c23,
					 float c30, float c31, float c32, float c33)
{
	MATRIX mat;

	mat.c[0][0] = c00; mat.c[0][1] = c01;
	mat.c[0][2] = c02; mat.c[0][3] = c03;
	mat.c[1][0] = c10; mat.c[1][1] = c11;
	mat.c[1][2] = c12; mat.c[1][3] = c13;
	mat.c[2][0] = c20; mat.c[2][1] = c21;
	mat.c[2][2] = c22; mat.c[2][3] = c23;
	mat.c[3][0] = c30; mat.c[3][1] = c31;
	mat.c[3][2] = c32; mat.c[3][3] = c33;

	return mat;
}

/* Multiply a 4x4 matrix by another 4x4 matrix and return the result */
MATRIX MatrixByMatrix(MATRIX *mat1, MATRIX *mat2)
{
	MATRIX res;
	int row, col, i;

	res = CreateMatrixEmpty();
	for(row = 0; row < 4; row++)
		for(col = 0; col < 4; col++)
			for(i = 0; i < 4; i++)
				res.c[row][col]+=mat1->c[row][i]*mat2->c[i][col];
 
	return res;
}

/* Multiply a 4x4 matrix by a vector and return the result */
VECTOR MatrixByVector(MATRIX *mat, VECTOR *vec)
{
	VECTOR res;
	int row, i;

	res = CreateVector(0, 0, 0, 0);
	for(row = 0; row < 4; row++)
		for(i = 0; i < 4; i++)
			res.c[row]+=mat->c[row][i]*vec->c[i];

	return res;
}

/* Multiply a vector by a 4x4 matrix and return the result */
VECTOR VectorByMatrix(VECTOR *vec, MATRIX *mat)
{
	VECTOR res;
	int col, i;

	res = CreateVector(0, 0, 0, 0);
	for(col = 0; col < 4; col++)
		for(i = 0; i < 4; i++)
			res.c[col]+=vec->c[i]*mat->c[i][col];

	return res;
}

/* Normalize a vector (length = 1) */
void VectorNormalize(VECTOR *v)
{
	float length = (float)sqrt(v->c[X]*v->c[X] + v->c[Y]*v->c[Y] + v->c[Z]*v->c[Z]);
	v->c[X]/=length;
	v->c[Y]/=length;
	v->c[Z]/=length;
}

/* Return the cross product of two vectors */
VECTOR VectorCrossProduct(VECTOR *v1, VECTOR *v2)
{
	return CreateVector(v1->c[Y]*v2->c[Z]-v1->c[Z]*v2->c[Y], v1->c[Z]*v2->c[X]-v1->c[X]*v2->c[Z], v1->c[X]*v2->c[Y]-v1->c[Y]*v2->c[X], 1);
}

/* Add two vectors and return the result */
VECTOR VectorAdd(VECTOR *v1, VECTOR *v2)
{
	return CreateVector(v1->c[X]+v2->c[X], v1->c[Y]+v2->c[Y], v1->c[Z]+v2->c[Z], 1);
}
