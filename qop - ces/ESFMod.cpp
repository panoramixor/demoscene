// FMOD wrapper

#include "stdafx.h"

#define stream_buffer_size 80

/* Constructor */
CFMod::CFMod()
{
	bMusic = false;
	channels = 0;
}

void CFMod::Load(char *filename, int outputtype)
{
		if(!bMusic)
		{
			// inactive audio
			FSOUND_SetMaxHardwareChannels(4);
			FSOUND_SetMinHardwareChannels(2);
			FSOUND_SetMixer(FSOUND_MIXER_QUALITY_MMXP5);
			FSOUND_SetOutput(outputtype);	
			FSOUND_SetBufferSize(stream_buffer_size);
			if (FSOUND_Init(44100, 2, FSOUND_INIT_GLOBALFOCUS))
			{
				LogLine("FMOD: Using %s\n", (char *)FSOUND_GetDriverName(FSOUND_GetDriver()));
				hStream = FSOUND_Stream_Open(filename, FSOUND_MPEGACCURATE | FSOUND_LOOP_OFF | FSOUND_NONBLOCKING, 0, 0);
				FSOUND_SetPanSeperation(1.0f);
				FSOUND_SetSFXMasterVolume(255);
				switch(FSOUND_Stream_GetOpenState(hStream))
				{
					case -2:
						while(FSOUND_Stream_GetOpenState(hStream) == -2)
						{
							Sleep(stream_buffer_size);
							LogLine("FMOD: Stream is openning... waiting %dms\n", stream_buffer_size);
						}
						break;
					case -3:
						FatalError("Cant find %s!", filename);
						break;
				}
					
			} else {
				FatalError("FMOD: Init Error!", NULL);
			}
		}
}


float CFMod::GetVolumeLevels()
{
	float j, k;
	FSOUND_GetCurrentLevels(channels, &j, &k);
	return (float)sqrt(j*j+k*k);
}


void CFMod::SetSyncCallback(FSOUND_STREAMCALLBACK Callback)
{
	FSOUND_Stream_SetSyncCallback(hStream,Callback,0);
}

void CFMod::AddSyncPoint(unsigned int pcm, char *name)
{
	FSOUND_Stream_AddSyncPoint(hStream, pcm, name);
}

void CFMod::Play()
{
	LogLine("FMOD: Unpausing...\n", NULL);
	channels = FSOUND_Stream_PlayEx(FSOUND_FREE,hStream, 0, FALSE);
	FSOUND_SetPan(FSOUND_ALL, FSOUND_STEREOPAN);
	bMusic = true;
	//	FSOUND_SetPaused(FSOUND_ALL, FALSE);
//	FSOUND_SetFrequency(channels, 44100);
}

void CFMod::Stop()
{
	signed char res = FALSE;
	if(bMusic)
	{
	//	FMUSIC_StopSong(hMod);
	//	FMUSIC_FreeSong(hMod);

		do {
			res = FSOUND_Stream_Stop(hStream);
			Sleep(stream_buffer_size);
			LogLine("FMOD: Stream is closing... waiting %dms\n", stream_buffer_size);
		} while (res == FALSE);

		FSOUND_Stream_Close(hStream);
		FSOUND_Close();

//		FSOUND_Stream_GetOpenState(hStream);	
		LogLine("FMOD: Shutdown... \n", NULL);
		bMusic = false;
	}
}



/* Destructor */
CFMod::~CFMod()
{

}

void CFMod::SetTime(float sTime)
{
	FSOUND_Stream_SetTime(hStream, (int)sTime*1000);
}

float CFMod::GetTime()
{
	return((float)FSOUND_Stream_GetTime(hStream)/(float)1000);
}
