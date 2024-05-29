// scene03

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;
static GLUquadricObj *sphere;

extern Model_3DS scene03_walls_3ds;
extern Model_3DS scene03_temple_3ds;

extern CSkybox scene03_skybox;
extern CCamPath scene03_campath;

static CCamera cam3;
static CLight light3;

static CCaption scene03_caption;

int totalscene03keyframes;
float scenetime3;

extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];


static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

extern CPicture scene03_screen;
extern CPicture scene04_screen;
extern CTexture mask;

static KeyFrame noisefade[4];
static KeyFrame k0;
static KeyFrame k1;
static KeyFrame k2;
static KeyFrame k3;

void seq_scene03()
{
	cam3.Sequence(scene03_campath.kfArray, scene03_campath.kfindex, 0, scenetime3);

	scene03_skybox.Draw();

	light3.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,5, 0, 20));
	light3.EnableLight();

	glPushMatrix();
		glEnable(GL_DEPTH_TEST);
		scene03_walls_3ds.pos.x = 0;
		scene03_walls_3ds.pos.y = 0;
		scene03_walls_3ds.pos.z = 0;
		glEnable(GL_NORMALIZE);
		scene03_walls_3ds.Draw();
		scene03_temple_3ds.scale = 0.6f;
		scene03_temple_3ds.pos.y = 0.051f;
		scene03_temple_3ds.Draw();
	glPopMatrix();

	noise_sequence(noisefade, 4, 0, scenetime3);
	scene03_caption.Draw(scenetime3);	// "Harsh words and violent blows"

	crossfade(scene03_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime3);
	crossfade(scene04_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime3);
}

bool init_seq_scene03()
{
	k0.SetKeyFrame(8997.007f, 0, 0, 0, 0, 0, 0);
	k1.SetKeyFrame(15000.816f, 0, 0, 0, 1, 1, 1);
	k2.SetKeyFrame(21006.803000f, 0, 0, 0, 1, 1, 1);
	k3.SetKeyFrame(23998.186000f, 0, 0, 0, 0, 0, 0);
	noisefade[0] = k0;
	noisefade[1] = k1;
	noisefade[2] = k2;
	noisefade[3] = k3;
// transition effect

	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(400, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeOut2[0] = k0;
	ColorFadeOut2[1] = k1;

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(400, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeOut[0] = k0;
	ColorFadeOut[1] = k1;

	k0.SetKeyFrame(27002.90300f-600, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(27002.90300f-600, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	sphere=gluNewQuadric();							//
	gluQuadricNormals(sphere, GLU_SMOOTH);			// Create Smooth Normals
	gluQuadricOrientation(sphere, GLU_OUTSIDE);		//
	gluQuadricTexture(sphere, GL_TRUE);				// Create Texture Coords
	gluQuadricDrawStyle(sphere, GL_FILL);				//

	scene03_caption.Create(2999.002000f, -10, 0, 0,	"So many schemes",
					   8997.007000f, -10, 0, 0,		"and so many lies.",
					   15000.816000f, -10, 0, 0,	"Too many whens",
					   21006.803000f, -10, 0, 0,	"and too many whys.",
					   23998.186000f, -10, 0, 0,	"",
					   4000);	
	scenetime3 = fTime;
	return true;
}