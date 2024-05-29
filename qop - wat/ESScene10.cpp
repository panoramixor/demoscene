// scene10

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern Model_3DS scene04_3ds;

static CCamera cam10;
static CLight light10;

extern CCamPath scene10_campath;

extern CPicture scene10_theend;
extern CPicture scene10_credits;

extern CTexture ESFrameBufferTexture;

float scenetime10;

static KeyFrame scrollup[2];
static KeyFrame k0;
static KeyFrame k1;

static CCaption scene10_caption0;
static CCaption scene10_caption1;
static CCaption scene10_caption2;

extern CPicture scene10_screen;
extern CTexture mask;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

void seq_scene10()
{
	glLoadIdentity();
	cam10.Sequence(scene10_campath.kfArray, scene10_campath.kfindex, 0, scenetime10);
	glPushMatrix();
		light10.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,4, 0, 20));
		light10.EnableLight();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		scene04_3ds.Draw();
	glPopMatrix();
	scene10_credits.Draw();
	scene10_caption0.Draw(scenetime10, 1);
	scene10_caption1.Draw(scenetime10, 1);
	scene10_caption2.Draw(scenetime10, 1);

	crossfade(scene10_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime10);
	scene10_theend.Sequence(ColorFadeIn, 2, 0, scenetime10);
}

bool init_seq_scene10()
{
	k0.SetKeyFrame(0, 0, 0, 2, 1, 1, 1);
	k1.SetKeyFrame(2900, 0, 0, 0, 1, 1, 1);
	scrollup[0] = k0;
	scrollup[1] = k1;

	scene10_caption0.Create(2000, -10, 0, 0,	"coded by panoramix",
					        4000, -10, 0,  0,	"portions of scene sequencer by madbit",
					        6000, -10, 0, 0,	"portions of quaternion class by digiben",
					        8000, -10, 0, 0,	"portions of 3ds loader by matthew fairfax",
					       10000, -10, 0, 0,	"fmod library by Firelight",
					        2000);	

	scene10_caption1.Create(2000+10000, -10, 0, 0,	 "poem by skitzo-bimbo",
					        4000+10000, -10, 0, 0,	 "music by rave-n and panoramix",
					        6000+10000, -10, 0, 0,   "3d modeling by panoramix",
					        8000+10000, -10, 0, 0,   "2d art by nova",
					       10000+10000, -10, 0, 0,   "textures from all arround the web...",
							2000);	

	scene10_caption2.Create(2000+20000, -10, 0, 0,	 "qop at flashparty 2003",
					        4000+20000, -10, 0, 0,	 "see you next time...",
					        6000+20000, -10, 0, 0,   "",
					        8000+20000, -10, 0, 0,   "",
					       10000+20000, -10, 0, 0,   "",
							2000);	

	// transition effect
	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(27002.90300f-500, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 1, 1, 1);			//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(27002.90300f-500, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 0, 0, 0);			//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scenetime10 = fTime;
	return true;
}