// Minimal Quaternion class

#include "stdafx.h"

#define PI				3.14159265358979323846f

CQuaternion::CQuaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

CQuaternion::~CQuaternion()
{

}

CQuaternion::CQuaternion(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

// this function takes axis/angle and converts it to a quaternion
void CQuaternion::CreateFromAxisAngle(float X, float Y, float Z, float degree) 
{ 
	// First we want to convert the degrees to radians 
	// since the angle is assumed to be in radians
	float angle = float((degree / 180.0f) * PI);

	// Here we calculate the sin( theta / 2) once for optimization
	float result = (float)sin( angle / 2.0f );
		
	// Calcualte the w value by cos( theta / 2 )
	w = (float)cos( angle / 2.0f );

	// Calculate the x, y and z of the quaternion
	x = float(X * result);
	y = float(Y * result);
	z = float(Z * result);
}

//	This function converts a quaternion to a rotation matrix
MATRIX CQuaternion::ConvertToMatrix()
{
	// Make sure the matrix has allocated memory to store the rotation data
	// This function is a necessity when it comes to doing almost anything
	// with quaternions.  Since we are working with OpenGL, which uses a 4x4
	// homogeneous matrix, we need to have a way to take our quaternion and
	// convert it to a rotation matrix to modify the current model view matrix.
	// We pass in a 4x4 matrix, which is a 1D array of 16 floats.  This is how OpenGL
	// allows us to pass in a matrix to glMultMatrixf(), so we use a single dimensioned array.
	// After about 300 trees murdered and 20 packs of chalk depleted, the
	// mathematicians came up with these equations for a quaternion to matrix converion:
	//
	//     ª        2     2												 ª
    //     ª 1 - (2y  + 2z )   2xy + 2zw         2xz - 2yw			0	 ª
    //     ª															 ª
    //     ª                          2     2							 ª
    // M = ª 2xy - 2zw         1 - (2x  + 2z )   2zy + 2xw			0	 ª
    //     ª															 ª
    //     ª                                            2     2			 ª
    //     ª 2xz + 2yw         2yz - 2xw         1 - (2x  + 2y )	0	 ª
    //     ª															 ª
	//     ª															 ª
	//     ª 0				   0				 0					1	 |													 ª
	//     ª															 ª
	// 
	// This is of course a 4x4 matrix.  Notice that a rotational matrix can just
	// be a 3x3 matrix, but since OpenGL uses a 4x4 matrix, we need to conform to the man.
	// Remember that the identity matrix of a 4x4 matrix has a diagonal of 1's, where
	// the rest of the indices are 0.  That is where we get the 0's lining the sides, and
	// the 1 at the bottom-right corner.  Since OpenGL matrices are row by column, we fill
	// in our matrix accordingly below.
/*	
	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );  
	pMatrix[ 1] = 2.0f * ( x * y - w * z );  
	pMatrix[ 2] = 2.0f * ( x * z + w * y );  
	pMatrix[ 3] = 0.0f;  

	// Second row
	pMatrix[ 4] = 2.0f * ( x * y + w * z );  
	pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );  
	pMatrix[ 6] = 2.0f * ( y * z - w * x );  
	pMatrix[ 7] = 0.0f;  

	// Third row
	pMatrix[ 8] = 2.0f * ( x * z - w * y );  
	pMatrix[ 9] = 2.0f * ( y * z + w * x );  
	pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );  
	pMatrix[11] = 0.0f;  

	// Fourth row
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;
*/
	return CreateMatrix(1.0f - 2.0f * ( y * y + z * z ), 2.0f * ( x * y - w * z ), 2.0f * ( x * z + w * y ), 0,
				 2.0f * ( x * y + w * z ), 1.0f - 2.0f * ( x * x + z * z ), 2.0f * ( y * z - w * x ), 0,
				 2.0f * ( x * z - w * y ), 2.0f * ( y * z + w * x ), 1.0f - 2.0f * ( x * x + y * y ), 0,
				 0, 0, 0, 1.0f);

}


void CQuaternion::ConvertToMatrix(float *pMatrix)
{
	// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );  
	pMatrix[ 1] = 2.0f * ( x * y - w * z );  
	pMatrix[ 2] = 2.0f * ( x * z + w * y );  
	pMatrix[ 3] = 0.0f;  

	// Second row
	pMatrix[ 4] = 2.0f * ( x * y + w * z );  
	pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );  
	pMatrix[ 6] = 2.0f * ( y * z - w * x );  
	pMatrix[ 7] = 0.0f;  

	// Third row
	pMatrix[ 8] = 2.0f * ( x * z - w * y );  
	pMatrix[ 9] = 2.0f * ( y * z + w * x );  
	pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );  
	pMatrix[11] = 0.0f;  

	// Fourth row
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;
}

void CQuaternion::Slerp(CQuaternion &q1, CQuaternion &q2, float t)
{
	// This function is the milk and honey of our quaternion code, the rest of
	// the functions are an appendage to what is done here.  Everyone understands
	// the terms, "matrix to quaternion", "quaternion to matrix", "create quaternion matrix",
	// "quaternion multiplication", etc.. but "SLERP" is the stumbling block, even a little 
	// bit after hearing what it stands for, "Spherical Linear Interpolation".  What that
	// means is that we have 2 quaternions (or rotations) and we want to interpolate between 
	// them.  The reason what it's called "spherical" is that quaternions deal with a sphere.  
	// Linear interpolation just deals with 2 points primarily, where when dealing with angles
	// and rotations, we need to use sin() and cos() for interpolation.  If we wanted to use
	// quaternions for camera rotations, which have much more instant and jerky changes in 
	// rotations, we would use Spherical-Cubic Interpolation.  The equation for SLERP is this:
	//
	// q = (((b.a)^-1)^t)a
	//
	// Go here for an a detailed explanation and proofs of this equation:
	//
	// http://www.magic-software.com/Documentation/quat.pdf
	//
	// Now, Let's code it

	// Here we do a check to make sure the 2 quaternions aren't the same, return q1 if they are
	if(q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w) 
	{
		x = q1.x;
		y = q1.y;
		z = q1.z;
		w = q1.w;
		return;
	}

	// Following the (b.a) part of the equation, we do a dot product between q1 and q2.
	// We can do a dot product because the same math applied for a 3D vector as a 4D vector.
	float result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	// If the dot product is less than 0, the angle is greater than 90 degrees
	if(result < 0.0f)
	{
		// Negate the second quaternion and the result of the dot product
		q2 = CQuaternion(-q2.x, -q2.y, -q2.z, -q2.w);
		result = -result;
	}

	// Set the first and second scale for the interpolation
	float scale0 = 1 - t, scale1 = t;

	// Next, we want to actually calculate the spherical interpolation.  Since this
	// calculation is quite computationally expensive, we want to only perform it
	// if the angle between the 2 quaternions is large enough to warrant it.  If the
	// angle is fairly small, we can actually just do a simpler linear interpolation
	// of the 2 quaternions, and skip all the complex math.  We create a "delta" value
	// of 0.1 to say that if the cosine of the angle (result of the dot product) between
	// the 2 quaternions is smaller than 0.1, then we do NOT want to perform the full on 
	// interpolation using.  This is because you won't really notice the difference.

	// Check if the angle between the 2 quaternions was big enough to warrant such calculations
	if(1 - result > 0.1f)
	{
		// Get the angle between the 2 quaternions, and then store the sin() of that angle
		float theta = (float)acos(result);
		float sinTheta = (float)sin(theta);

		// Calculate the scale for q1 and q2, according to the angle and it's sine value
		scale0 = (float)sin( ( 1 - t ) * theta) / sinTheta;
		scale1 = (float)sin( ( t * theta) ) / sinTheta;
	}	

	// Calculate the x, y, z and w values for the quaternion by using a special
	// form of linear interpolation for quaternions.
	x = (scale0 * q1.x) + (scale1 * q2.x);
	y = (scale0 * q1.y) + (scale1 * q2.y);
	z = (scale0 * q1.z) + (scale1 * q2.z);
	w = (scale0 * q1.w) + (scale1 * q2.w);
}

