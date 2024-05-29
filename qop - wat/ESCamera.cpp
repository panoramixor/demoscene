// Camera class

extern float fTime;

#include "stdafx.h"

extern float fDeltaTime;
float lasttime=0;

CCamera::CCamera()
{
	//Init with standard OGL values:
	pos.Set(0,0,0);
	viewdir.Set(0.0f, 0.0f,-1.0f);
	ViewDirChanged = false;
	RotatedX = RotatedY = RotatedZ = 0.0;
	keyframer_index = 0;
	rotiny = 0;
}

void CCamera::SetKeyframerIndex(int newindex)
{
	keyframer_index = newindex;
}

void CCamera::GetViewDir( void )
{
	SF3dVector Step1, Step2;
	//Rotate around Y-axis:
	Step1.x = (GLfloat)cos( (RotatedY + 90.0) * PIdiv180);
	Step1.z = (GLfloat)-sin( (RotatedY + 90.0) * PIdiv180);
	//Rotate around X-axis:
	double cosX = (GLfloat)cos (RotatedX * PIdiv180);
	Step2.x = Step1.x * (GLfloat)cosX;
	Step2.z = Step1.z * (GLfloat)cosX;
	Step2.y = (GLfloat)sin(RotatedX * PIdiv180);
	//Rotation around Z-axis not yet implemented, so:
	ViewDir = Step2;
	viewdir.x = Step2.x;
	viewdir.y = Step2.y;
	viewdir.z = Step2.z;
}


// the method below needs testing, it's supposed to be used by the future implementation of a billboard class
// currentry only used by the particles class
CVector3 CCamera::GetCameraAngle( void )
{
	CVector3 res;
	res.x = RotatedX;
	res.y = RotatedY;
	res.z = RotatedZ;
	return res;
}


void CCamera::RotateY (GLfloat Angle)
{
	RotatedY += Angle;
	ViewDirChanged = true;
}

void CCamera::RotateX (GLfloat Angle)
{
	RotatedX += Angle;
	ViewDirChanged = true;
}

void CCamera::RotateZ (GLfloat Angle)
{
	RotatedZ += Angle;
	ViewDirChanged = true;
}

void CCamera::Render( void )
{
	RotationQuat.CreateFromAxisAngle(0, 0, 1, RotatedZ);
	RotationQuat.ConvertToMatrix(RotationMatrix);
	glMultMatrixf(RotationMatrix);
	
	RotationQuat.CreateFromAxisAngle(1, 0, 0, RotatedX);
	RotationQuat.ConvertToMatrix(RotationMatrix);
	glMultMatrixf(RotationMatrix);

	RotationQuat.CreateFromAxisAngle(0, 1, 0, RotatedY);
	RotationQuat.ConvertToMatrix(RotationMatrix);
	glMultMatrixf(RotationMatrix);

	glTranslatef( -pos.x, -pos.y, -pos.z);
}

void CCamera::PointTo(GLfloat point_x, GLfloat point_y, GLfloat point_z)
{
	gluLookAt(-Position.x, -Position.y, -Position.z, point_x, point_y, point_z, 0, 1, 0);

}

void CCamera::MoveForwards( GLfloat Distance )
{
	if (ViewDirChanged) GetViewDir();
	// SF3dVector MoveVector;
	CVector3 movevector;
	movevector.x = viewdir.x * -Distance;
	movevector.y = viewdir.y * -Distance;
	movevector.z = viewdir.z * -Distance;
	pos.Add(movevector);
	Position.z += Distance;
}

void CCamera::StrafeRight ( GLfloat Distance )
{
	if (ViewDirChanged) GetViewDir();
	CVector3 movevector;
	movevector.z = -viewdir.x * -Distance;
	movevector.y = 0.0;
	movevector.x = viewdir.z * -Distance;
	pos.Add(movevector);
	Position.x += Distance;
}

void CCamera::StrafeLeft ( GLfloat Distance )
{
	StrafeRight(-Distance);
}

void CCamera::MoveUp( GLfloat Distance )
{
	if (ViewDirChanged) GetViewDir();
	CVector3 movevector;
	movevector.x = 0;
	movevector.y = ViewDir.z * -Distance;
	movevector.z = -ViewDir.y * -Distance;
	pos.Add(movevector);
	Position.y += Distance;
	pos.y += Distance;
}

void CCamera::MoveDown( GLfloat Distance )
{
	MoveUp(-Distance);
}

void CCamera::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	CQuaternion q1, q2;
	float myscenetime = (fTime-scenetime)*1000-lasttime;
	if(keyframer_index < MAX_KEYFRAMER_INDEX)
	{
		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, fDeltaTime);
		if (kd.keyframeTime <= 1.0f)
		{
				RotatedX = -Difference(SceneKeyFrames[keyframer_index].rotationX, kd.rotationX);
				RotatedY = -Difference(SceneKeyFrames[keyframer_index].rotationY, kd.rotationY);
				RotatedZ = -Difference(SceneKeyFrames[keyframer_index].rotationZ, kd.rotationZ);
				pos.x = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
				pos.y = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
				pos.z = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);
		}
		else
		{
				keyframer_index++;//= 2;
				kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, fDeltaTime);
				RotatedX = -Difference(SceneKeyFrames[keyframer_index].rotationX, kd.rotationX);
				RotatedY = -Difference(SceneKeyFrames[keyframer_index].rotationY, kd.rotationY);
				RotatedZ = -Difference(SceneKeyFrames[keyframer_index].rotationZ, kd.rotationZ);
				pos.x = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
				pos.y = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
				pos.z = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);

		}
	}
	else
	{
				if (looper != 0)
				{
					keyframer_index = looper;
					lasttime += SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
				}
	}
	Render();
}
