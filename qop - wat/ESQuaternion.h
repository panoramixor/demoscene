#ifndef __CQUATERNION_H__
#define __CQUATERNION_H__

class CQuaternion
{
public:
	CQuaternion();
	~CQuaternion();
	CQuaternion(float X, float Y, float Z, float W);
	void CreateFromAxisAngle(float X, float Y, float Z, float degree);
	void ConvertToMatrix(float *pMatrix);
	MATRIX ConvertToMatrix();
	void Slerp(CQuaternion &q1, CQuaternion &q2, float t);

private:
	float x;
	float y;
	float z;
	float w;
};

#endif /*__CQUATERNION_H__*/