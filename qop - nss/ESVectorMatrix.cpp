// Vectors and matrices structures

#include "stdafx.h"

void CVector2::Set(float X, float Y)
{
	x = X;
	y = Y;
}


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

CVector4::CVector4()
{
	r = g = b = a = 0.0f;
}

CVector4::~CVector4()
{
	
}

void CVector4::Set(float R, float G, float B, float A)
{
	r = R;
	g = G;
	b = B;
	a = A;
}

void CVector4::Add(CVector4 source)
{
	r += source.r;
	g += source.g;
	b += source.b;
	a += source.a;
}


