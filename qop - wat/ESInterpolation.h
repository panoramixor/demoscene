#ifndef __CINTER_H__
#define __CINTER_H__

class CEaseInOutInterpolation
{
	public:
		//	private:
		float _value;
		float _target;
		float _remainingtime;
		float _totaltime;
		float _speed;
		float _acceleration;

		CEaseInOutInterpolation();
		~CEaseInOutInterpolation();

		bool Setup(float from, float to, float time);
		bool Interpolate(float deltatime);
		float GetValue();
		float Speed();
};


class CEaseInOutInterpolateKeyframe
{
	public:
		CEaseInOutInterpolateKeyframe();
		~CEaseInOutInterpolateKeyframe();

		CEaseInOutInterpolation iposx;
		CEaseInOutInterpolation iposy;
		CEaseInOutInterpolation iposz;

		CEaseInOutInterpolation irotx;
		CEaseInOutInterpolation iroty;
		CEaseInOutInterpolation irotz;

		void Setup(KeyFrame isource, KeyFrame idest, float time);
		CVector3 InterpolatePos(float deltatime);
		CVector3 InterpolateRot(float deltatime);
};


#endif /*__CINTER_H__*/