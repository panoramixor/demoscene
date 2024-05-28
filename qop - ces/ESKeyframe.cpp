// Animation frame class

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

#ifdef EDITOR
void KeyFrame::getEditorValues()
{
	char kfmyindex[10], posx[10], posy[10], posz[10], rotx[10], roty[10], rotz[10], kftime[100] /*, kltime[100]*/;
	if(GetDlgItemText(hEditDlg, IDC_POSX, posx, 10) == NULL) strcpy(posx, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_POSY, posy, 10) == NULL) strcpy(posy, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_POSZ, posz, 10) == NULL) strcpy(posz, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_ROTX, rotx, 10) == NULL) strcpy(rotx, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_ROTY, roty, 10) == NULL) strcpy(roty, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_ROTZ, rotz, 10) == NULL) strcpy(rotz, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_KFTIME, kftime, 10) == NULL) strcpy(kftime, "0.00000");
	if(GetDlgItemText(hEditDlg, IDC_KFNUMBER, kfmyindex, 10) == NULL) strcpy(kfmyindex, "0");
	index = atoi(kfmyindex);
	keyframeTime = atoi(kftime);
	positionX = atof(posx);
	positionY = atof(posy);
	positionZ = atof(posz);
	rotationX = atof(rotx);
	rotationY = atof(roty);
	rotationZ = atof(rotz);
	if(SendDlgItemMessage(hEditDlg, IDC_LINEAR, BM_GETCHECK, 0, 0))
		kinterpolationmode = ES_ANIMATE_LINEAR;
	if(SendDlgItemMessage(hEditDlg, IDC_EASED, BM_GETCHECK, 0, 0))
		kinterpolationmode = ES_ANIMATE_EASED;
	if(SendDlgItemMessage(hEditDlg, IDC_NONE, BM_GETCHECK, 0, 0))
		kinterpolationmode = ES_ANIMATE_NONE;
}
#endif