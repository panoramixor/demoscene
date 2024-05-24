#include "FmodWrapper.h"
#include <math.h>

#define stream_buffer_size 80

/* Constructor */
CFMod::CFMod()
{

}

bool CFMod::isPlaying()
{
	FMOD_BOOL res = false;
	FMOD_Channel_IsPlaying(hChannel, &res);
	return res;
}


void CFMod::Load(char *filename, int outputtype)
{
			// inactive audio
			FMOD_System_Create(&hFmod);
			FMOD_System_SetOutput(hFmod, FMOD_OUTPUTTYPE_AUTODETECT);
//			FMOD_System_SetStreamBufferSize(hFmod, stream_buffer_size, FMOD_TIMEUNIT_MS);
			 // Initialize FMOD.
			if (!FMOD_System_Init(hFmod, 8, FMOD_INIT_NORMAL, 0))
			{
				FMOD_System_CreateStream(hFmod, filename, FMOD_ACCURATETIME | FMOD_LOOP_OFF, 0, &hStream);
				
				FMOD_System_GetMasterChannelGroup(hFmod, &hChGrp);
				FMOD_ChannelGroup_GetDSP(hChGrp, 0, &hDsp);
				FMOD_System_CreateDSPByType(hFmod, FMOD_DSP_TYPE_FFT, &hDsp);

				FMOD_ChannelGroup_AddDSP(hChGrp, 0, hDsp);
				FMOD_System_PlaySound(hFmod, hStream, hChGrp, true, &hChannel);

				FMOD_DSP_SetActive(hDsp, true);
				FMOD_DSP_SetParameterInt(hDsp, FMOD_DSP_FFT_WINDOWSIZE, stream_buffer_size);
			} else {

			}
}


float CFMod::GetVolumeLevels()
{
	FMOD_DSP_PARAMETER_FFT* paramFFT;
	FMOD_RESULT res = FMOD_DSP_GetParameterData(hDsp, FMOD_DSP_FFT_SPECTRUMDATA, (void**)&paramFFT, 0, 0, 0);
	if((res == FMOD_OK) && (paramFFT->length))
	{
		float j = *paramFFT->spectrum[0];
		float k = *paramFFT->spectrum[0];
		return (float)(j+k)/2.f;
	}
	return 0.1f;
}

void CFMod::FadeOutMusic(float sTime, float seconds)
{
	/*
	int newvolume;
	if(fadestart < 0)
		fadestart = sTime;
	float limit = (sTime-fadestart)/seconds;
	newvolume = LinearInterpolate(volume, 0, limit);
	FSOUND_SetVolume(channels, newvolume);
	if(newvolume < 1)
		glfwTerminate();
		*/
}


void CFMod::SetSyncCallback(FMOD_CHANNELCONTROL_CALLBACK Callback)
{
	FMOD_Channel_SetCallback(hChannel,Callback);
}

void CFMod::AddSyncPoint(unsigned int pcm, char *name)
{
	FMOD_Sound_AddSyncPoint(hStream, pcm, FMOD_TIMEUNIT_PCM, name, 0);
}

void CFMod::Play()
{
	FMOD_Channel_SetPaused(hChannel, false);
}

void CFMod::Stop()
{
		FMOD_Channel_Stop(hChannel);
}

/* Destructor */
CFMod::~CFMod()
{

}

void CFMod::SetTime(float sTime)
{
	FMOD_Channel_SetPosition(hChannel, (unsigned int)sTime, FMOD_TIMEUNIT_MS);
}

float CFMod::GetTime()
{
	unsigned int pos;
	FMOD_Channel_GetPosition(hChannel, &pos, FMOD_TIMEUNIT_MS);
	return((float)pos/(float)1000.f);
}
