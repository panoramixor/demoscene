// scene02

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;
extern Model_3DS scene02_3ds;
extern void	CaptureScreen();
static CCamera cam2;
static CLight light2;

static CCaption scene02_caption;
extern CCamPath scene02_campath;

float scenetime2;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

extern CPicture scene02_screen;
extern CPicture scene03_screen;
//extern CPicture mask;
extern CTexture mask;

static KeyFrame comein[2];
static KeyFrame noisefade[4];

static KeyFrame k0;
static KeyFrame k1;

extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];

void seq_scene02()
{
	cam2.Sequence(scene02_campath.kfArray, scene02_campath.kfindex, 0, scenetime2);

	glPushMatrix();
		light2.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,4, 0, 20));
		light2.EnableLight();
		glEnable(GL_DEPTH_TEST);
		scene02_3ds.pos.x = 0;
		scene02_3ds.pos.y = 0;
		scene02_3ds.pos.z = 0;
		glEnable(GL_NORMALIZE);
		scene02_3ds.Draw();
	glPopMatrix();
	scene02_caption.Draw(scenetime2);	// "Harsh words and violent blows"

	crossfade(scene02_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime2);
	crossfade(scene03_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime2);
}

bool init_seq_scene02()
{
	k0.SetKeyFrame(0, 36, 0, 0, 1, 1, 1);
	k1.SetKeyFrame(23998.186000f, -32, 0, 0, 1, 1, 1);
	comein[0] = k0;
	comein[1] = k1;

// transition effect

	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(600, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(600, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(24015.964f-600, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24015.964f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(24015.964f-600, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(24015.964f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scene02_caption.Create(2999.002000f, -10, 0, 0,	 "Teeth are clenched",
					   8997.007000f, -10, 0, 0,	 "hands are fisted.",
					   15000.816000f, -10, 0, 0,   "Deep inside I'm",
					   21006.803000f, -10, 0, 0,   "Warped and twisted.",
					   23998.186000f, -10, 0, 0,   "Warped and twisted.",
					   4000);	

	
	scenetime2 = fTime;
	return true;
}