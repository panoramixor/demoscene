// Vectors structures

#ifndef __CVECTORMATRIX_H__
#define __CVECTORMATRIX_H__

// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3 
{
public:
	CVector3();
	~CVector3();
	float x, y, z;
	void Set(float X, float Y, float Z);
	void Add(CVector3 source);
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
	float x, y;
};

#endif /*__CVECTORMATRIX_H__*/