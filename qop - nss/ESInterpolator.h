// class for interpolation of animation keyframes

#ifndef __CINTER_H__
#define __CINTER_H__


typedef struct tagKeyframe {
	float time;
	CVector3 position;
	CVector3 rotation;
	CVector3 scale;
	CVector4 color;
} Keyframe;

class CInterpolator
{
public:
	CInterpolator();
	~CInterpolator();
	Keyframe *Evaluate(float sTime);
	void SetLen(unsigned int len);
	void Set(unsigned int index, Keyframe *keyframe);

private:
	Keyframe *keyframe_array;
	Keyframe current_frame;
	float time_offset;
	unsigned int current_keyframe;
	unsigned int last_keyframe;
};

#endif /*__CINTER_H__*/