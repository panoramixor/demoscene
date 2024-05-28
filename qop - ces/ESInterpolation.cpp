#include "stdafx.h"

CEaseInOutInterpolation::CEaseInOutInterpolation()
{
	_speed = 0.0f;
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
	_remainingTime = _totalTime = time;
	_totalTime = time;
	return true;
}

bool CEaseInOutInterpolation::Interpolate(float deltaTime)
{
	_remainingTime -= deltaTime;
	if(_remainingTime < _totalTime/2)
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
	return (_remainingTime <= 0);
}

float CEaseInOutInterpolation::GetValue()
{
	return _value;
}


