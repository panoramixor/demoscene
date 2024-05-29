#include "Stdafx.h"

CEaseInOutInterpolation::CEaseInOutInterpolation()
{
	_speed = 0.0f;
	_remainingtime = 0;
}

CEaseInOutInterpolation::~CEaseInOutInterpolation()
{
}

bool CEaseInOutInterpolation::Setup(float from, float to, float time)
{
	if(time <= 0)
	{
		return false;
	}
	_value = from;
	_target = to;
	_speed = 0.0f;
	// derived from x=x0 + v0*t + a*t*t/2
	_acceleration = (to-from)/(time*time/4);
	CEaseInOutInterpolation::_remainingtime = _totaltime = time;
	_totaltime = time;
	return true;
}

bool CEaseInOutInterpolation::Interpolate(float deltaTime)
{
	_remainingtime -= deltaTime;
	if(_remainingtime < _totaltime /2)
	{
		// Decceleration
		_speed -= _acceleration * deltaTime;
	}
	else
	{
		// Acceleration
		_speed += _acceleration * deltaTime;
	}
	_value = _speed * deltaTime;
	return (_remainingtime <= 0);
}

float CEaseInOutInterpolation::GetValue()
{
	return _value;
}

float CEaseInOutInterpolation::Speed()
{
	printf("%f\n", _speed);
	return _speed;
}


CEaseInOutInterpolateKeyframe::CEaseInOutInterpolateKeyframe()
{

}

CEaseInOutInterpolateKeyframe::~CEaseInOutInterpolateKeyframe()
{

}



void CEaseInOutInterpolateKeyframe::Setup(KeyFrame isource, KeyFrame idest, float time)
{
	iposx.Setup(isource.positionX, idest.positionX, time);
	iposy.Setup(isource.positionY, idest.positionY, time);
	iposz.Setup(isource.positionZ, idest.positionZ, time);

	irotx.Setup(isource.rotationX, idest.rotationX, time);
	iroty.Setup(isource.rotationY, idest.rotationY, time);
	irotz.Setup(isource.rotationZ, idest.rotationZ, time);
}

CVector3 CEaseInOutInterpolateKeyframe::InterpolatePos(float deltatime)
{
	CVector3 result;
	iposx.Interpolate(deltatime);
	iposy.Interpolate(deltatime);
	iposz.Interpolate(deltatime);

	result.x = iposx.GetValue();
	result.y = iposy.GetValue();
	result.z = iposz.GetValue();
	return result;
}

CVector3 CEaseInOutInterpolateKeyframe::InterpolateRot(float deltatime)
{
	CVector3 result;
	irotx.Interpolate(deltatime);
	iroty.Interpolate(deltatime);
	irotz.Interpolate(deltatime);

	result.x = irotx.GetValue();
	result.y = iroty.GetValue();
	result.z = irotz.GetValue();
	return result;
}
