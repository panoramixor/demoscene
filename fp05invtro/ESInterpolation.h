#ifndef __CINTER_H__
#define __CINTER_H__

class CEaseInOutInterpolation
{
	public:
		bool Setup(float from, float to, float time)
		{
			if(time <= 0)
				return false;
			_value = from;
			_target = to;
			_speed = 0.0f;

			_acceleration = (to-from)/((time*time)/4);
			_remainingtime = _totaltime = time;
			return true;
		}

		bool Interpolate(float deltatime)
		{
			_remainingtime -= deltatime;
			if(_remainingtime < _totaltime/2) _speed -= _acceleration * deltatime;
			else _speed += _acceleration * deltatime;
			_value += _speed * deltatime;
			return(_remainingtime <= 0);
		}

		float GetValue()
		{
			printf("%f\n",_value);
			return _value;
		}

		float Speed()
		{
			printf("%f\n",_speed);
			return _speed;
		}


	private:
		float _value;
		float _target;
		float _remainingtime;
		float _totaltime;
		float _speed;
		float _acceleration;
};


class CEaseInOutInterpolateKeyframe
{
	public:
		CEaseInOutInterpolation iposx;
		CEaseInOutInterpolation iposy;
		CEaseInOutInterpolation iposz;

		CEaseInOutInterpolation irotx;
		CEaseInOutInterpolation iroty;
		CEaseInOutInterpolation irotz;


		void Setup(KeyFrame isource, KeyFrame idest, float time)
		{
			iposx.Setup(isource.positionX, idest.positionX, time);
			iposy.Setup(isource.positionY, idest.positionY, time);
			iposz.Setup(isource.positionZ, idest.positionZ, time);

			irotx.Setup(isource.rotationX, idest.rotationX, time);
			iroty.Setup(isource.rotationY, idest.rotationY, time);
			irotz.Setup(isource.rotationZ, idest.rotationZ, time);
		}

		CVector3 InterpolatePos(float deltatime)
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

		CVector3 InterpolateRot(float deltatime)
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
};


#endif /*__CINTER_H__*/