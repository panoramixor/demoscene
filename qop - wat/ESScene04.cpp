// scene04

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;
extern Model_3DS scene04_3ds;

static CCamera cam4;
static CLight light4;

extern CCamPath scene04_campath;

static CCaption scene04_caption;

float scenetime4;

//extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];
static KeyFrame k0;
static KeyFrame k1;

extern CPicture scene04_screen;
extern CPicture scene05_screen;
extern CTexture mask;

void seq_scene04()
{
//	glLoadIdentity();

	cam4.Sequence(scene04_campath.kfArray, scene04_campath.kfindex, 1, scenetime4);

	glPushMatrix();
		light4.EnableLight();
		light4.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0, (float)sin(fTime*60), 0, 1));
		glEnable(GL_DEPTH_TEST);
		scene04_3ds.scale = 5;
		glEnable(GL_NORMALIZE);
		scene04_3ds.Draw();
	glPopMatrix();

	scene04_caption.Draw(scenetime4);
	crossfade(scene04_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime4);
	crossfade(scene05_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime4);

}

bool init_seq_scene04()
{
	scene04_caption.Create(2999.002000f, -10, 0, 0,	"I am not special",
					   8997.007000f, -10, 0, 0,		"and I am not gifted.",
					   15000.816000f, -10, 0, 0,	"I'm just me",
					   21006.803000f, -10, 0, 0,	"Warped and twisted.",
					   23998.186000f, -10, 0, 0,	"",
					   4000);	

// transition effect

	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(600, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(600, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(24015.96400f-400, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(24015.96400f-400, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scenetime4 = fTime;
	return true;
}