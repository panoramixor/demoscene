#include "stdafx.h"

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
		case ES_ANIMATE_EASED:
			KeyFrameEased(source, destination, timepassed);
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

void KeyFrame::KeyFrameEased(KeyFrame source, KeyFrame destination, float timepassed)
{
	ResetKeyFrameData();
	
	float kftime = (timepassed / Difference(source.keyframeTime, destination.keyframeTime));
//	if(kftime > 1) kftime = 1;
	if(kftime < 0) kftime = 0;
	keyframeTime = kftime;
	positionX = source.positionX - LinearEaseInterpolate(source.positionX, destination.positionX, kftime);
	positionY = source.positionY - LinearEaseInterpolate(source.positionY, destination.positionY, kftime);
	positionZ = source.positionZ - LinearEaseInterpolate(source.positionZ, destination.positionZ, kftime);
	rotationX = source.rotationX - LinearEaseInterpolate(source.rotationX, destination.rotationX, kftime);
	rotationY = source.rotationY - LinearEaseInterpolate(source.rotationY, destination.rotationY, kftime);
	rotationZ = source.rotationZ - LinearEaseInterpolate(source.rotationZ, destination.rotationZ, kftime);
	return;
}
