/**********************************************************

	Project: EngineSuffer
	Desc:    Keyframe class, handles creation/destruction
		 and interpolation of keyframe objects.

	Original Author: panoramix

**********************************************************/

#include "stdafx.h"
extern CEaseInOutInterpolateKeyframe InterpolatorArray[MAX_SCENEKEYFRAMES*2];
extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND hEditDlg;
float v1, v2;

KeyFrame::KeyFrame()
{
	// Initialization
	ResetKeyFrameData();
}

KeyFrame::~KeyFrame()
{

}

void KeyFrame::SetKeyFrame(float skeyframeTime, float spositionX, float spositionY, float spositionZ, float srotationX, float srotationY, float srotationZ)
{
	keyframeTime = skeyframeTime;
	positionX = spositionX;	
	positionY = spositionY;
	positionZ = spositionZ;
	rotationX = srotationX;
	rotationY = srotationY;
	rotationZ = srotationZ;
}

void KeyFrame::ResetKeyFrameData()
{
	keyframeTime = 0;
	positionX = 0;	
	positionY = 0;
	positionZ = 0;
	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	kinterpolationmode = 1;
}


void KeyFrame::KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed, int index, float deltatime)
{
	switch(source.kinterpolationmode)
	{
		case ES_ANIMATE_NONE:
			KeyFrameInterpolateNone(index);
			break;

		case ES_ANIMATE_EASED:
			KeyFrameInterpolateCurve(index, deltatime, timepassed, Difference(source.keyframeTime, destination.keyframeTime));
			break;

		case ES_ANIMATE_LINEAR:
		default:
			KeyFrameInterpolateLinear(source, destination, timepassed);
			break;

		case ES_ANIMATE_COLORS:
			KeyFrameInterpolateLinear(source, destination, timepassed);
			break;
	}
}

void KeyFrame::KeyFrameInterpolateLinear(KeyFrame source, KeyFrame destination, float timepassed)
{
	ResetKeyFrameData();
	
	float kftime = (timepassed / Difference(source.keyframeTime, destination.keyframeTime));
//	if(kftime > 1) kftime = 1;
	if(kftime < 0) kftime = 0;
	keyframeTime = kftime;
	positionX = source.positionX - LinearInterpolate(source.positionX, destination.positionX, kftime);
	positionY = source.positionY - LinearInterpolate(source.positionY, destination.positionY, kftime);
	positionZ = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, kftime);
	rotationX = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, kftime);
	rotationY = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, kftime);
	rotationZ = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, kftime);
	return;
}
 
void KeyFrame::KeyFrameInterpolateCurve(int index, float deltatime, float timepassed, float totaltime)
{
	ResetKeyFrameData();
	CVector3 P1, P2, P3, P4;
	CVector3 R1, R2, R3, R4;
	CVector3 result;

	KeyFrame source = KeyFrameArray[index];
	KeyFrame destination = KeyFrameArray[index+4];

	float kftime = (timepassed / Difference(source.keyframeTime, destination.keyframeTime));
	if(kftime > 1) kftime = 1;
	if(kftime < 0) kftime = 0;
	keyframeTime = kftime;

	// source point
	P1.Set(source.positionX, source.positionY, source.positionY);
	R1.Set(source.rotationX, source.rotationY, source.rotationZ);


	// genetate controls point from linear interpolating 1/3 of total vector lenght
	// this control point is for position

	P2.x = source.positionX - LinearInterpolate(source.positionX, destination.positionX, 0.333333f);
	P2.y = source.positionY - LinearInterpolate(source.positionY, destination.positionY, 0.333333f);
	P2.z = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, 0.333333f);

	// this control point is for rotation
	R2.x = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, 0.333333f);
	R2.y = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, 0.333333f);
	R2.z = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, 0.333333f);

	// this control point is for position
	P3.x = source.positionX - LinearInterpolate(source.positionX, destination.positionX, 0.666666f);
	P3.y = source.positionY - LinearInterpolate(source.positionY, destination.positionY, 0.666666f);
	P3.z = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, 0.666666f);

	// this control point is for rotation
	R3.x = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, 0.666666f);
	R3.y = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, 0.666666f);
	R3.z = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, 0.666666f);

	// destination point
	P4.Set(destination.positionX, destination.positionY, destination.positionZ);
	R4.Set(destination.rotationX, destination.rotationY, destination.rotationZ);

	result = SplineInterpolate(P2, P1, P4, P3, kftime);
	
	positionX = result.x;
	positionY = result.y;
	positionZ = result.z;

	result = SplineInterpolate(R2, R1, R4, R3, kftime);

	rotationX = result.x;
	rotationY = result.y;
	rotationZ = result.z;
	return;
}

/*
void KeyFrame::KeyFrameInterpolateCurve(int index, float deltatime, float timepassed, float totaltime)
{
		float kftime = (float) (timepassed / totaltime);
		if(kftime > 1) kftime = 1;
		if(kftime < 0) kftime = 0;
		keyframeTime = kftime;
		CVector3 kfposition, kfrotation;
		kfposition = InterpolatorArray[index].InterpolatePos(deltatime*1000);
		kfrotation = InterpolatorArray[index].InterpolateRot(deltatime*1000);
		
		positionX = KeyFrameArray[index].positionX - kfposition.x;
		positionY = KeyFrameArray[index].positionY - kfposition.y;
		positionZ = KeyFrameArray[index].positionZ - kfposition.z;

		rotationX = KeyFrameArray[index].rotationX - kfrotation.x;
		rotationY = KeyFrameArray[index].rotationY - kfrotation.y;
		rotationZ = KeyFrameArray[index].rotationZ - kfrotation.z;
		return;
}
*/


void KeyFrame::KeyFrameInterpolateNone(int index)
{
		
		keyframeTime = 1;
		positionX = KeyFrameArray[index+1].positionX;
		positionY = KeyFrameArray[index+1].positionY;
		positionZ = KeyFrameArray[index+1].positionZ;

		rotationX = KeyFrameArray[index+1].rotationX;
		rotationY = KeyFrameArray[index+1].rotationX;
		rotationZ = KeyFrameArray[index+1].rotationX;
		return;
}

/*
void KeyFrame::KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed)
{
//	ResetKeyFrameData();
	float kftime = (float) (timepassed / Difference(source.keyframeTime, destination.keyframeTime));
	if(kftime > 1) kftime = 1;
	if(kftime < 0) kftime = 0;
	keyframeTime = kftime;
	positionX = source.positionX - LinearInterpolate(source.positionX, destination.positionX, kftime);
	positionY = source.positionY - LinearInterpolate(source.positionY, destination.positionY, kftime);
	positionZ = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, kftime);
	rotationX = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, kftime);
	rotationY = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, kftime);
	rotationZ = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, kftime);
	return;
}
*/

/*
void KeyFrame::KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed)
{
	ResetKeyFrameData();
	CVector3 P1, P2, P3, P4;
	CVector3 R1, R2, R3, R4;
	CVector3 result;

	float kftime = (timepassed / Difference(source.keyframeTime, destination.keyframeTime));
	if(kftime > 1) kftime = 1;
	if(kftime < 0) kftime = 0;
	keyframeTime = kftime;

	// source point
	P1.Set(source.positionX, source.positionY, source.positionY);
	R1.Set(source.rotationX, source.rotationY, source.rotationZ);


	// genetate controls point from linear interpolating 1/3 of total vector lenght
	// this control point is for position

	P2.x = source.positionX - LinearInterpolate(source.positionX, destination.positionX, 0.333333f);
	P2.y = source.positionY - LinearInterpolate(source.positionY, destination.positionY, 0.333333f);
	P2.z = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, 0.333333f);

	// this control point is for rotation
	R2.x = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, 0.333333f);
	R2.y = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, 0.333333f);
	R2.z = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, 0.333333f);

	// this control point is for position
	P3.x = source.positionX - LinearInterpolate(source.positionX, destination.positionX, 0.666666f);
	P3.y = source.positionY - LinearInterpolate(source.positionY, destination.positionY, 0.666666f);
	P3.z = source.positionZ - LinearInterpolate(source.positionZ, destination.positionZ, 0.666666f);

	// this control point is for rotation
	R3.x = source.rotationX - LinearInterpolate(source.rotationX, destination.rotationX, 0.666666f);
	R3.y = source.rotationY - LinearInterpolate(source.rotationY, destination.rotationY, 0.666666f);
	R3.z = source.rotationZ - LinearInterpolate(source.rotationZ, destination.rotationZ, 0.666666f);

	// destination point
	P4.Set(destination.positionX, destination.positionY, destination.positionZ);
	R4.Set(destination.rotationX, destination.rotationY, destination.rotationZ);

	result = SplineInterpolate(P2, P1, P4, P3, kftime);
	
	positionX = result.x;
	positionY = result.y;
	positionZ = result.z;

	result = SplineInterpolate(R2, R1, R4, R3, kftime);

	rotationX = result.x;
	rotationY = result.y;
	rotationZ = result.z;
	return;
}
*/
/*
float KeyFrame::EaseInOutInterpolationInterpolate(float source, float destination, float deltatime, float remainingtime, float totaltime)
{
	float acceleration = Difference(source,destination)/(totaltime*totaltime/4);
	if(remainingtime < totaltime/2)
	{
		// Decceleration
		speed -= acceleration * deltatime;
	}
	else
	{
		// Acceleration
		speed += acceleration * deltatime;
	}
	LogLine("ease interpolation: %f\n", speed * deltatime);
	return (speed * deltatime);
}
*/

