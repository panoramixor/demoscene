// scene07

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;
extern Model_3DS scene07_3ds;

static CCamera cam7;
static CLight light7;
extern CSkybox scene07_skybox;

static CCaption scene07_caption;

extern CCamPath scene07_campath;

float scenetime7;

extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];

extern CPicture scene07_screen;
extern CPicture scene08_screen;
extern CTexture mask;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

static KeyFrame k0, k1;

void seq_scene07()
{

	glLoadIdentity();

	cam7.Sequence(scene07_campath.kfArray, scene07_campath.kfindex, 0, scenetime7);

	glPushMatrix();
	scene07_skybox.Draw();
	glPopMatrix();

	glPushMatrix();
		light7.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,10, 0, 20));
		light7.EnableLight();
		glEnable(GL_DEPTH_TEST);
		scene07_3ds.pos.x = 0;
		scene07_3ds.pos.y = 0;
		scene07_3ds.pos.z = 0;
		scene07_3ds.Draw();
	glPopMatrix();

	scene07_caption.Draw(scenetime7);
	crossfade(scene07_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime7);
	crossfade(scene08_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime7);
}

bool init_seq_scene07()
{
	scene07_caption.Create(2999.002000f, -10, 0, 0,	"Cried out, wasted",
					   8997.007000f, -10, 0, 0,		"empty and hollow.",
					   15000.816000f, -10, 0, 0,	"Today is just",
					   21006.803000f, -10, 0, 0,	"yesterday's tomorrow.",
					   23998.186000f, -10, 0, 0,	"",
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

	scenetime7 = fTime;
	return true;
}