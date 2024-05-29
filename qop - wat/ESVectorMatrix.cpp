// Vectors and matrices structures

#include "stdafx.h"

CVector3::CVector3()
{
	x = y = z = 0.0f;
}

CVector3::~CVector3()
{
	
}

void CVector3::Set(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

void CVector3::Add(CVector3 source)
{
	x += source.x;
	y += source.y;
	z += source.z;
}

