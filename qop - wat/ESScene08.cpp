// scene08

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern Model_3DS scene08_3ds;

static CCamera cam8;
static CLight light8;

extern CCamPath scene08_campath;

static CCaption scene08_caption;
float scenetime8;

extern CPicture scene08_screen;
extern CPicture scene09_screen;
extern CTexture mask;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

static KeyFrame k0, k1;

void seq_scene08()
{
	cam8.Sequence(scene08_campath.kfArray, scene08_campath.kfindex, 0, scenetime8);

	light8.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,-1, 1, 0));
	light8.EnableLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	scene08_3ds.Draw();
	scene08_caption.Draw(scenetime8);

	crossfade(scene08_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime8);
	crossfade(scene09_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime8);
}

bool init_seq_scene08()
{
	scene08_caption.Create(2999.002000f, -10, 0, 0,	"My soul has burned out",
					   8997.007000f, -10, 0, 0,		"and the ashes have been sifted.",
					   15000.816000f, -10, 0, 0,	"Still here I am",
					   21006.803000f, -10, 0, 0,	"Warped and twisted.",
					   23998.186000f, -10, 0, 0,	"Warped and twisted.",
					   4000);	

	// transition effect
	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(24015.96400f-500, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(24015.96400f-500, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(24015.96400f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;
	
	scenetime8 = fTime;
	return true;
}