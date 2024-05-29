// scene sequencer
//-----------------------------------------------------------------------------------------------
// sequencer.ini file management by madbit
// aditional coding by panoramix

#include "stdafx.h"
#include "fmod.h"

FILE* INIFile;

#define SEQ_LOOP           -2
#define SEQ_STOP           0

#define SEQ_SCENE00	   1
#define SEQ_SCENE01    2
#define SEQ_SCENE02	   3
#define SEQ_SCENE03	   4
#define SEQ_SCENE04	   5
#define SEQ_SCENE05    6
#define SEQ_SCENE06	   7
#define SEQ_SCENE07	   8
#define SEQ_SCENE08	   9
#define SEQ_SCENE09	   10
#define SEQ_SCENE10	   11
#define SEQ_SCENE11	   12

int sequencer_totalscenes;
int sequencer_index;

typedef struct tagSequencerLine {
	int sequence_endtime;
	int sequence_reserved1;
	int sequence_reserved2;
	int sequence_number;
} SequencerLine;

SequencerLine demosequence[MAX_SCENEKEYFRAMES];

bool bStopDemo     = false;									// True when all sequences played

extern FMUSIC_MODULE *hMod;													// Handle to the module
extern FSOUND_STREAM *hStream;												// Handle to the mp3
extern int stream_buffer_size;													// used to calculate accurate sync.

extern void Stop_Music();
extern void Init_Music(char *filename);
extern void Seek_Music(int newtime);

extern float		  fTime;										 // Time passed since first frame
extern float		  fDeltaTime;									  // Time passed since last frame

KeyFrame ColorFadeIn[2];
KeyFrame ColorFadeOut[2];
KeyFrame c0;
KeyFrame c1;

extern bool global_showfps;

extern bool init_seq_scene00();
extern void seq_scene00();

extern bool init_seq_scene01();
extern void seq_scene01();

extern bool init_seq_scene02();
extern void seq_scene02();

extern bool init_seq_scene03();
extern void seq_scene03();

extern bool init_seq_scene04();
extern void seq_scene04();

extern bool init_seq_scene05();
extern void seq_scene05();

extern bool init_seq_scene06();
extern void seq_scene06();

extern bool init_seq_scene07();
extern void seq_scene07();

extern bool init_seq_scene08();
extern void seq_scene08();

extern bool init_seq_scene09();
extern void seq_scene09();

extern bool init_seq_scene10();
extern void seq_scene10();

extern int FSOUND_GetCurrentVU();

extern CPicture scene01_screen;
extern CPicture scene02_screen;
extern CPicture scene03_screen;
extern CPicture scene04_screen;
extern CPicture scene05_screen;
extern CPicture scene06_screen;
extern CPicture scene07_screen;
extern CPicture scene08_screen;
extern CPicture scene09_screen;
extern CPicture scene10_screen;

long m_Time;

bool  bInitSequence = true;

//extern CPicture mask;

CPicture seq_picture[MAX_TIMES];

//-----------------------------------------------------------------------------------------------
// SequenceLoad(): Load the hole ini file (return number of sequences for this demo)
//-----------------------------------------------------------------------------------------------
int SequenceLoad() 
{
	int i=0;
	INIFile = fopen("./data/script/sequencer.ini", "r");
	while(i<=sizeof(demosequence)) {
		fscanf(INIFile, "%d %d %d %d", &demosequence[i].sequence_endtime, &demosequence[i].sequence_reserved1, &demosequence[i].sequence_reserved2, &demosequence[i].sequence_number);
		i++;
		if(demosequence[i-1].sequence_number == 0) break;
	}
	fclose(INIFile);
	sequencer_totalscenes = i;
	return i;
}


//-----------------------------------------------------------------------------------------------
// GetMusicPosition(): Query music player status and update member variables. 
//-----------------------------------------------------------------------------------------------
long GetStreamPosition()
{

#ifdef CAPTURE
	m_Time = int(fTime*1000);
#else
	m_Time = FSOUND_Stream_GetTime(hStream);
#endif

	if (m_Time >= (demosequence[sequencer_index].sequence_endtime))
	{
		bInitSequence = false;
		reset_noise();
		sequencer_index++;
	}

	return m_Time;
}

//-----------------------------------------------------------------------------------------------
// Sequencer()
//-----------------------------------------------------------------------------------------------
bool Sequencer()
{
	static bool not_first_time = false;
	if (!not_first_time)
	{
		not_first_time = true;
		bInitSequence = false;
	}

	GetStreamPosition();
	
	// Render current sequence.
	switch (demosequence[sequencer_index].sequence_number)
	{
		case SEQ_SCENE00: 
			if(m_Time != 0) {
				if (!bInitSequence) {
					init_seq_scene00();
					bInitSequence = true;
				}
				seq_scene00();
			}
			break;

		case SEQ_SCENE01: 
			if (!bInitSequence) {
				init_seq_scene01();
				bInitSequence = true;
			}
			seq_scene01();
			break;

		case SEQ_SCENE02: 
			if (!bInitSequence) {
				init_seq_scene02();
				bInitSequence = true;
			}
			seq_scene02();
			break;

		case SEQ_SCENE03: 
			if (!bInitSequence) {
				init_seq_scene03();
				bInitSequence = true;
			}
			seq_scene03();
			break;

		case SEQ_SCENE04: 
			if (!bInitSequence) {
				init_seq_scene04();
				bInitSequence = true;
			}
			seq_scene04();
			break;

		case SEQ_SCENE05: 
			if (!bInitSequence) {
				init_seq_scene05();
				bInitSequence = true;
			}
			seq_scene05();
			break;

		case SEQ_SCENE06: 
			if (!bInitSequence) {
				init_seq_scene06();
				bInitSequence = true;
			}
			seq_scene06();
			break;

		case SEQ_SCENE07: 
			if (!bInitSequence) {
				init_seq_scene07();
				bInitSequence = true;
			}
			seq_scene07();
			break;

		case SEQ_SCENE08: 
			if (!bInitSequence) {
				init_seq_scene08();
				bInitSequence = true;
			}
			seq_scene08();
			break;

		case SEQ_SCENE09: 
			if (!bInitSequence) {
				init_seq_scene09();
				bInitSequence = true;
			}
			seq_scene09();
			break;

		case SEQ_SCENE10: 
			if (!bInitSequence) {
				init_seq_scene10();
				bInitSequence = true;
			}
			seq_scene10();
			break;

		case SEQ_LOOP:
			fseek(INIFile, 0, SEEK_SET);											 // Loop demo
//			FMUSIC_SetOrder(hMod, 0);
			FSOUND_Stream_SetPosition(hStream,0);
			break;

		case SEQ_STOP:
			// Stop the music.
			Stop_Music();
			bStopDemo = true;														 // Stop demo
			break;
	}	   

	if(global_showfps) {
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
	}

	float thisy;
	glLoadIdentity();
	glColor4f(1,1,1,1);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glBegin(GL_LINES);
		thisy = 0.8f;
		float thisx = (float)(sin(fTime*2)/10);
		glVertex3f(thisx-15,thisy+0.15f,-1);
		glVertex3f(thisx-1,thisy+0.15f,-1);
		glVertex3f(thisx-1,thisy+0.15f,-1);
		glVertex3f(thisx-0.931f,thisy+0.08f,-1);
		glVertex3f(thisx-15,thisy+0.08f,-1);
		glVertex3f(thisx+15,thisy+0.08f,-1);
		glVertex3f(thisx+0.931f,-thisy-0.051f,-1);
		glVertex3f(thisx+1,-thisy-0.12f,-1);
		glVertex3f(thisx+1,-thisy-0.12f,-1);
		glVertex3f(thisx+15,-thisy-0.12f,-1);
		glVertex3f(thisx-15,-thisy-0.05f,-1);
		glVertex3f(thisx+15,-thisy-0.05f,-1);
		float otherthisx = (float)(sin(fTime*3)/20);
		glVertex3f(otherthisx-15,thisy+0.15f,-1);
		glVertex3f(otherthisx-1,thisy+0.15f,-1);
		glVertex3f(otherthisx-1,thisy+0.15f,-1);
		glVertex3f(otherthisx-0.931f,thisy+0.08f,-1);
		glVertex3f(otherthisx-15,thisy+0.08f,-1);
		glVertex3f(otherthisx+15,thisy+0.08f,-1);
		glVertex3f(otherthisx+0.931f,-thisy-0.051f,-1);
		glVertex3f(otherthisx+1,-thisy-0.12f,-1);
		glVertex3f(otherthisx+1,-thisy-0.12f,-1);
		glVertex3f(otherthisx+15,-thisy-0.12f,-1);
		glVertex3f(otherthisx-15,-thisy-0.05f,-1);
		glVertex3f(otherthisx+15,-thisy-0.05f,-1);
	glEnd();
	float fcolor = 1.3f;

	glBlendFunc(GL_DST_COLOR,GL_ZERO);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
		glColor4f(1.5f-fcolor,1.5f-fcolor,1.5f-fcolor,0.5f*fcolor);
		glBegin(GL_QUADS);		
		glVertex3f(-15,	0.8f+0.08f,			-1);
		glVertex3f(-15,	0.8f+2, -1);
		glVertex3f(15, 0.8f+2, -1); 
		glVertex3f(15, 0.8f+0.08f,			-1);
	glEnd();

	glBegin(GL_QUADS);
		glColor4f(1.5f-fcolor,1.5f-fcolor,1.5f-fcolor,0.5f*fcolor);
		glBegin(GL_QUADS);		
		glVertex3f(-15,	-0.8f-0.051f,			-1);
		glVertex3f(-15,	-0.8f-2, -1);
		glVertex3f(15, -0.8f-2, -1); 
		glVertex3f(15, -0.8f-0.051f,			-1);
	glEnd();

	glPopAttrib();
	glPopMatrix();

	return bStopDemo;
}
