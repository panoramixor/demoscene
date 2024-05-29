// streaming music class using Fmod

#ifndef __CFMOD_H__
#define __CFMOD_H__

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
	void SetSyncCallback(FSOUND_STREAMCALLBACK Callback);
	void AddSyncPoint(unsigned int pcm, char *name);
	void SetTime(float sTime);
	void FadeOutMusic(float sTime, float seconds);
private:
	FMUSIC_MODULE * hMod;
	FSOUND_STREAM * hStream;
	int volume;
	bool bMusic;
	float fadestart;
	int channels;
};

#endif /*__CFMOD_H__*/