// Vectors and matrices structures

#ifndef __CVECTORMATRIX_H__
#define __CVECTORMATRIX_H__

// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3 
{
public:
	CVector3();
	~CVector3();
	CVector3(float X, float Y, float Z)
	{ 
		x = X; y = Y; z = Z;
	}
	float x, y, z;
	void Set(float X, float Y, float Z);
	void Add(CVector3 source);
};

// This is our 4D point class. 
class CVector4 
{
public:
	CVector4();
	~CVector4();
	CVector4(float R, float G, float B, float A)
	{ 
		r = R; g = G; b = B, a = A;
	}
	float r, g, b, a;
	void Set(float R, float G, float B, float A);
	void Add(CVector4 source);
};


// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
	float x, y;
	void Set(float X, float Y);
};

class CObject
{
public:
	CVector3 *m_pVertices;			// This stores all the object's vertices
	CVector2 *m_pTextureCoords;		// This stores all the objects UV coordinates
	UINT	 *m_pIndices;			// This stores the face indices for the object

	int m_numOfTriangles;			// This stores the....  Just read the variable name :)
};

// This is a simple class to demonstrate using interleaved arrays (UV and vertex info together)
class CVertex
{
public:
	CVector2 m_textureCoord;
	CVector3 m_vertex;	
};


#endif /*__CVECTORMATRIX_H__*/