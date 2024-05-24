#ifndef __CFMOD_H__
#define __CFMOD_H__

#include <fmod.h>


class CFMod
{
public:
	CFMod();
	~CFMod();
	void Load(char *filename, int outputtype);
	float GetVolumeLevels();
	void Play();
	void Pause();
	void Stop();
	float GetTime();
	void SetSyncCallback(FMOD_CHANNELCONTROL_CALLBACK Callback);
	void AddSyncPoint(unsigned int pcm, char *name);
	void SetTime(float sTime);
	void FadeOutMusic(float sTime, float seconds);
	bool isPlaying();
//private:

//	FMUSIC_MODULE * hMod;
	FMOD_DSP *hDsp;
	FMOD_SOUND* hStream;
	FMOD_SYSTEM* hFmod;
	FMOD_CHANNELGROUP* hChGrp;
	FMOD_CHANNEL* hChannel;
	int volume;
};

#endif /*__CFMOD_H__*/