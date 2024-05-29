// animation Keyframe class

#ifndef KEYFRAME_H
#define KEYFRAME_H

class KeyFrame
{
	public:
				KeyFrame();				// Constructor
				virtual ~KeyFrame();		// Destructor

				float keyframeTime;
				float positionX;
				float positionY;
				float positionZ;
				float rotationX;
				float rotationY;
				float rotationZ;
				char name[MAX_PATH];
				int	  index;
				int kinterpolationmode;
				void SetKeyFrame(float skeyframeTime, float spositionX, float spositionY, float spositionZ, float srotationX, float srotationY, float srotationZ);
				void KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed, int index, float deltatime);
				void KeyFrameInterpolateLinear(KeyFrame source, KeyFrame destination, float timepassed);
				void KeyFrameInterpolateCurve(int index, float deltatime, float timepassed, float totaltime);
				void KeyFrameInterpolateNone(int index);
				void ResetKeyFrameData();
};

#endif KEYFRAME_H
