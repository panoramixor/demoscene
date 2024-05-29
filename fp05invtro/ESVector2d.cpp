#include "stdafx.h"

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//- VECTOR_INT DEFINITIONS -----------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//

// vector assignment
const VECTOR2D_INT VECTOR2D_INT::
	operator=(const VECTOR2D_INT &vec)
	{
	v[0]= vec.v[0];
	v[1]= vec.v[1];

	return *this;
	}

// vecector equality
const bool VECTOR2D_INT::
	operator==(const VECTOR2D_INT &vec)
	{	return ((v[0]==vec.v[0]) && (v[1]==vec.v[1]));	}

// vecector inequality
const bool VECTOR2D_INT::
	operator!=(const VECTOR2D_INT &vec)
	{	return !(*this==vec);	}

// vector add
const VECTOR2D_INT VECTOR2D_INT::
	operator+(const VECTOR2D_INT &vec)
	{	return VECTOR2D_INT(v[0]+vec.v[0], v[1]+vec.v[1]);	}

// vector add (opposite of negation)
const VECTOR2D_INT VECTOR2D_INT::
	operator+(void)
	{	return VECTOR2D_INT(*this);	}

// vector increment
const VECTOR2D_INT VECTOR2D_INT::
	operator+=(const VECTOR2D_INT& vec)
	{    
	v[0]+= vec.v[0];
	v[1]+= vec.v[1];
	return *this;
	}

// vector subtraction
const VECTOR2D_INT VECTOR2D_INT::
	operator-(const VECTOR2D_INT& vec)
	{	return VECTOR2D_INT(v[0]-vec.v[0], v[1]-vec.v[1]);	}
     
// vector negation
const VECTOR2D_INT VECTOR2D_INT::
	operator-()
	{	return VECTOR2D_INT(-v[0], -v[1]);	}

// scalar self-multiply
const VECTOR2D_INT VECTOR2D_INT::
	operator*=(const int &s)
	{
	v[0]*= s;
	v[1]*= s;    
	return *this;
	}

// scalar self-divecide
const VECTOR2D_INT VECTOR2D_INT::
	operator/=(const int &s)
	{
	int recip;
	recip= 1/s; // for speed, one divecision

	v[0]*= recip;
	v[1]*= recip;
	return *this;
	}

// post multiply by scalar
const VECTOR2D_INT VECTOR2D_INT::
	operator*(const int &s)
	{	return VECTOR2D_INT(v[0]*s, v[1]*s);	}

const VECTOR2D_INT VECTOR2D_INT::
	operator*(const VECTOR2D_INT& vec)
	{	return VECTOR2D_INT(v[0]*vec.v[0], v[1]*vec.v[1]);	}

// divide by scalar
const VECTOR2D_INT VECTOR2D_INT::
	operator/(int s)
	{
	s = 1/s;
	
	return VECTOR2D_INT(v[0]*s, v[1]*s);
	}
/*
// vector assignment
const VECTOR2D_INT VECTOR2D_INT::
	operator=(const VECTOR2D_FLOAT &vec)
	{
	v[0]= (int)vec.v[0];
	v[1]= (int)vec.v[1];

	return *this;
	}

// vecector equality
const bool VECTOR2D_INT::
	operator==(const VECTOR2D_FLOAT &vec)
	{	return ((v[0]==vec.v[0]) && (v[1]==vec.v[1]));	}

// vecector inequality
const bool VECTOR2D_INT::
	operator!=(const VECTOR2D_FLOAT &vec)
	{	return !(*this==vec);	}

// vector add
const VECTOR2D_INT VECTOR2D_INT::
	operator+(const VECTOR2D_FLOAT &vec)
	{	return VECTOR2D_INT(v[0]+(int)vec.v[0], v[1]+(int)vec.v[1]);	}

// vector increment
const VECTOR2D_INT VECTOR2D_INT::
	operator+=(const VECTOR2D_FLOAT& vec)
	{    
	v[0]+= (int)vec.v[0];
	v[1]+= (int)vec.v[1];
	return *this;
	}

// vector subtraction
const VECTOR2D_INT VECTOR2D_INT::
	operator-(const VECTOR2D_FLOAT& vec)
	{	return VECTOR2D_INT(v[0]-(int)vec.v[0], v[1]-(int)vec.v[1]);	}
*/
// scalar self-multiply
const VECTOR2D_INT VECTOR2D_INT::
	operator*=(const float &s)
	{
	v[0]*= (int)s;
	v[1]*= (int)s;    
	return *this;
	}

// scalar self-divecide
const VECTOR2D_INT VECTOR2D_INT::
	operator/=(const float &s)
	{
	float recip;
	recip= 1/s; // for speed, one divecision

	v[0]*= (int)recip;
	v[1]*= (int)recip;
	return *this;
	}

// post multiply by scalar
const VECTOR2D_INT VECTOR2D_INT::
	operator*(const float &s)
	{	return VECTOR2D_INT((int)(v[0]*s), (int)(v[1]*s));	}
/*
const VECTOR2D_INT VECTOR2D_INT::
	operator*(const VECTOR2D_FLOAT& vec)
	{	return VECTOR2D_INT((int)(v[0]*vec.v[0]), (int)(v[1]*vec.v[1]));	}
*/
// divide by scalar
const VECTOR2D_INT VECTOR2D_INT::
	operator/(float s)
	{
	s = 1/s;
	
	return VECTOR2D_INT((int)(v[0]*s), (int)(v[1]*s));
	}

//------------------------------------------------------------------//
//- void VECTOR::SetProperties(int, int) ---------------------------//
//------------------------------------------------------------------//
//- Sets the vector's information.								   -//
//------------------------------------------------------------------//
void VECTOR2D_INT::
	SetProperties(int X, int Y)
	{
	v[0]= X;
	v[1]= Y;
	}

