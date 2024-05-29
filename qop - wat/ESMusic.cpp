// fMod sound system implementation

#include "stdafx.h"

FMUSIC_MODULE * hMod;
FSOUND_STREAM * hStream;

bool bBeat;

int stream_buffer_size=80;

extern int m_Order;
extern void ShutDown();
extern void FatalError(char *formatstring, char *datastring);
//extern bool bStopDemo;

int channels;
void BeatOn()
{
	if (bBeat == true) bBeat = false;
	if (bBeat == false) bBeat = true;
}

static bool bMusic = false;

// TODO: callback al final del tema?
signed char streamcallback(FSOUND_STREAM *stream, void *buff, int len, int param) 
{
	return 1;
}

void Init_Music(char *filename, int outputtype)
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

void Seek_Music(int newtime)
{
	FSOUND_Stream_SetTime(hStream, newtime);
}

int Music_GetTime()
{
//	FSOUND_Update();
	return	FSOUND_Stream_GetTime(hStream);
}

float GetMusicVolume()
{
	float j, k;
	FSOUND_GetCurrentLevels(channels, &j, &k);
	return (float)(j+k)/800;
}

void Play_Music()
{
	LogLine("FMOD: Unpausing...\n", NULL);
	channels = FSOUND_Stream_PlayEx(FSOUND_FREE,hStream, 0, FALSE);
	FSOUND_SetPan(FSOUND_ALL, FSOUND_STEREOPAN);
}

void Stop_Music()
{

	if(bMusic)
	{
		while(1) {
			if(FSOUND_Stream_Stop(hStream))
				if(FSOUND_Stream_Close(hStream)) {
					FSOUND_Close();
					if(FSOUND_Stream_GetOpenState(hStream) == 0)
						break;
				}
		};
		bMusic = false;
	}
}

