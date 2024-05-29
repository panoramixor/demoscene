// class for interpolation of animation keyframes

#include "stdafx.h"

CInterpolator::CInterpolator()
{
	current_keyframe = 0;
	last_keyframe = 0;
	time_offset = 0.0f;
	keyframe_array = 0;
}

CInterpolator::~CInterpolator()
{
    if(keyframe_array)
	    delete [] keyframe_array;
}

void CInterpolator::SetLen(unsigned int len)
{
	keyframe_array = new Keyframe[len];
	last_keyframe = len;
}

void CInterpolator::Set(unsigned int index, Keyframe *keyframe)
{
	current_keyframe=0;
	if(index < last_keyframe)
		keyframe_array[index] = *keyframe;
}

Keyframe *CInterpolator::Evaluate(float sTime)
{
	Keyframe source;
	Keyframe dest;

	for(unsigned int i=current_keyframe;i<last_keyframe;i++) {
			if((sTime+0.0002f >= keyframe_array[i].time) && (sTime+0.0002f < keyframe_array[i+1].time)){
					if(current_keyframe != i) {
						current_keyframe = i;
					}
				break;
			}
	}

	source = keyframe_array[current_keyframe];
	dest = keyframe_array[current_keyframe+1];

	float animation_time = (dest.time-source.time);
	float relative_time = sTime-source.time;

	float current_time = relative_time/animation_time;
	if(current_time > 1.0f) {
//		time_offset += relative_time;
		current_time = 1.0f;
	} else if(current_time < 0.0f) {
		current_time = 0.0f;
	}

	current_frame.time = relative_time;
	current_frame.color.Set(LinearInterpolate(source.color.r, dest.color.r, current_time), LinearInterpolate(source.color.g, dest.color.g, current_time), LinearInterpolate(source.color.b, dest.color.b, current_time), LinearInterpolate(source.color.a, dest.color.a, current_time));
	current_frame.position.Set(LinearInterpolate(source.position.x, dest.position.x, current_time), LinearInterpolate(source.position.y, dest.position.y, current_time), LinearInterpolate(source.position.z, dest.position.z, current_time));
	current_frame.rotation.Set(LinearInterpolate(source.rotation.x, dest.rotation.x, current_time), LinearInterpolate(source.rotation.y, dest.rotation.y, current_time), LinearInterpolate(source.rotation.z, dest.rotation.z, current_time));
	current_frame.scale.Set(LinearInterpolate(source.scale.x, dest.scale.x, current_time), LinearInterpolate(source.scale.y, dest.scale.y, current_time), LinearInterpolate(source.scale.z, dest.scale.z, current_time));

	return &current_frame;
}