// keyframe class

#include "stdafx.h"

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
		KeyFrameInterpolateLinear(source, destination, timepassed);
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
