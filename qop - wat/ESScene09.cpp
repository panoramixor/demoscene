// scene09

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern Model_3DS scene02_3ds;

static CCamera cam9;
static CLight light9;

extern CCamPath scene09_campath;

static CCaption scene09_caption0;
static CCaption scene09_caption1;
static CCaption scene09_caption2;
static CCaption scene09_caption3;
static CCaption scene09_caption4;
static CCaption scene09_caption5;
static CCaption scene09_caption6;
static CCaption scene09_caption7;

extern CPicture scene09_picture0;
extern CTexture ESFrameBufferTexture;

float scenetime9;

static KeyFrame comein[4];
static KeyFrame k0;
static KeyFrame k1;
static KeyFrame k2;
static KeyFrame k3;

extern CPicture scene09_screen;
extern CPicture scene10_screen;
extern CTexture mask;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

void seq_scene09()
{
	glLoadIdentity();
	cam9.Sequence(scene09_campath.kfArray, scene09_campath.kfindex, 0, scenetime9);

	glPushMatrix();
		light9.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,4, 0, 20));
		light9.EnableLight();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		scene02_3ds.Draw();
	glPopMatrix();
	
	scene09_caption0.Draw(scenetime9,1);
	scene09_caption1.Draw(scenetime9,1);
	scene09_caption2.Draw(scenetime9,1);
	scene09_caption3.Draw(scenetime9,1);
	scene09_caption4.Draw(scenetime9,1);
	scene09_caption5.Draw(scenetime9,1);
	scene09_caption6.Draw(scenetime9,1);
	scene09_caption7.Draw(scenetime9,1);

	crossfade(scene09_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime9);
	crossfade(scene10_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime9);

}

bool init_seq_scene09()
{
	k0.SetKeyFrame(8997.007f, 2, 0, 0, 1, 1, 1);
	k1.SetKeyFrame(15000.816f, 0, 0, 0, 1, 1, 1);
	k2.SetKeyFrame(21006.803000f, 0, 0, 0, 1, 1, 1);
	k3.SetKeyFrame(23998.186000f, 2, 0, 0, 1, 1, 1);
	comein[0] = k0;
	comein[1] = k1;
	comein[2] = k2;
	comein[3] = k3;
	
	scene09_caption0.Create( 0000, -20,10*2, -8,"sunflower",
						     1000, -20, 8*2, -8, "aardbei",
						     2000, -20, 6*2, -8, "farbrausch",
					         3000, -20, 4*2, -8, "fairlight",
					         4000, -20, 2*2, -8, "popsy team"
							,1000);	
							
	scene09_caption1.Create( 0000+250, -1.333f, 10*2, -8, "moppi",
							 1000+250, -1.333f, 8*2, -8, "cocoon",
							 2000+250, -1.333f, 6*2, -8, "calodox",
					         3000+250, -1.333f, 4*2, -8, "haujobb",
					         4000+250, -1.333f, 2*2, -8, "tbl"
							,1000);	

	scene09_caption2.Create( 0000+500, 17.333f,10*2, -8, "bomb",
						     1000+500, 17.333f, 8*2, -8, "tpolm",
						     2000+500, 17.333f, 6*2, -8, "mfx",
					         3000+500, 17.333f, 4*2, -8, "taat",
					         4000+500, 17.333f, 2*2, -8, "exceed"
							,1000);	

	scene09_caption3.Create( 0000+750, 36, 10*2, -8, "aegis corp",
							 1000+750, 36, 8*2, -8, "razor 1911",
							 2000+750, 36, 6*2, -8, "scoopex",
					         3000+750, 36, 4*2, -8, "blasphemy",
					         4000+750, 36, 2*2, -8, "threepixels"
							,1000);	

	scene09_caption4.Create( 5000, -20, 0, -8, "black maiden",
						     6000, -20, -2*2, -8, "critical mass",
						     7000, -20, -4*2, -8, "spinning kids",
					         8000, -20, -6*2, -8, "madwizards",
					         9000, -20, -8*2, -8, "smash designs"
							,1000);	

	scene09_caption5.Create( 5000+250, -1.333f, 0, -8, "satori",
							 6000+250, -1.333f, -2*2, -8, "kolor",
							 7000+250, -1.333f, -4*2, -8, "kewlers",
					         8000+250, -1.333f, -6*2, -8, "trsi",
					         9000+250, -1.333f, -8*2, -8, "cncd"
							,1000);	

	scene09_caption6.Create( 5000+500, 17.333f, 0, -8, "t-rex",
						     6000+500, 17.333f, -2*2, -8, "boah",
						     7000+500, 17.333f, -4*2, -8, "rgba",
					         8000+500, 17.333f, -6*2, -8, "orion",
					         9000+500, 17.333f, -8*2, -8, "3state"
							,1000);	

	scene09_caption7.Create( 5000+750, 36, 0, -8, "maturefurk",
							 6000+750, 36, -2*2, -8, "byterapers",
							 7000+750, 36, -4*2, -8, "halcyon",
					         8000+750, 36, -6*2, -8, "katastro.fi",
					         9000+750, 36, -8*2, -8, "nomad"
							,1000);	


	// transition effect
	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(500, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(12229.66000f-500, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(12229.66000f, 0, 0, 0, 1, 1, 1);			//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(12229.66000f-500, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(12229.66000f, 0, 0, 0, 0, 0, 0);			//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scenetime9 = fTime;
	return true;
}