// scene05

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern Model_3DS scene05_3ds;

static CCamera cam5;
static CLight light5;

extern CCamPath scene05_campath;

float scenetime5;
extern CPicture scene05_picture0;

extern CPicture scene05_screen;
extern CPicture scene06_screen;
extern CTexture mask;

static CCaption scene05_caption;

static KeyFrame k0, k1;
KeyFrame ColorFade[2];

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

static KeyFrame comein[2];

void seq_scene05()
{
	glLoadIdentity();
	glTranslatef(0,0,-2);
	cam5.Sequence(scene05_campath.kfArray, scene05_campath.kfindex, 0, scenetime5);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	scene05_3ds.scale = 0.2f;
	scene05_3ds.Draw();

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	scene05_caption.Draw(scenetime5);
	crossfade(scene05_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime5);
	crossfade(scene06_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime5);
}

bool init_seq_scene05()
{
	k0.SetKeyFrame(0, 1, 1, 1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(2000, 0, 0, 0, 0, 0, 0);		//

	ColorFade[0] = k0;
	ColorFade[1] = k1;

	k0.SetKeyFrame(8997.007f, 8.2f, 1.48f, 0, 1, 1, 1);
	k1.SetKeyFrame(23998.186000f, -3, 1.48f, 0, 1, 1, 1);
	comein[0] = k0;
	comein[1] = k1;


	scene05_caption.Create(2999.002000f, -10, 0, 0,	"Sleeping awake",
					   8997.007000f, -10, 0, 0,		"and choking on a dream.",
					   15000.816000f, -10, 0, 0,	"Listening",
					   21006.803000f, -10, 0, 0,	"to my silent screams.",
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

	k0.SetKeyFrame(24015.96400f-600, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(24015.96400f-600, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	
	scenetime5 = fTime;
	return true;
}