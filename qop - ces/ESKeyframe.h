// Animation frame class

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
				void KeyFrame::KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed, int index, float deltatime);
				void KeyFrameInterpolateLinear(KeyFrame source, KeyFrame destination, float timepassed);
				void KeyFrameInterpolateCurve(int index, float deltatime, float timepassed, float totaltime);
				void KeyFrame::KeyFrameEased(KeyFrame source, KeyFrame destination, float timepassed);
				void KeyFrame::KeyFrameInterpolateNone(int index);
				void ResetKeyFrameData();
#ifdef EDITOR
				void getEditorValues();
#endif

};
/*

class KeyFramer
{
	public:
		
			KeyFramer();				// Constructor
			virtual ~KeyFramer();	// Destructor
//			KeyFrame Interpolate(KeyFrame current, KeyFrame next, float fps);
//			float GetInterpolationScalar(float kI, float kD, float pof);
//			KeyFrame SetKeyFrame(KeyFrame current, float kTime, float kRotX, float kRotY, float kRotZ, float kTransX, float kTransY, float kTransZ);
//			KeyFrame Interpolator;


};
*/
#endif KEYFRAME_H
