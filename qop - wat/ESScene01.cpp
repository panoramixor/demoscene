// scene01

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern Model_3DS scene01_3ds;

static CCamera cam1;
static CLight light1;
extern CSkybox scene01_skybox;
static CCaption scene01_caption;

static GLUquadricObj *tube;

extern CTexture scene01_envmap0;

float scenetime1;

extern CCamPath scene01_campath;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

extern CPicture scene01_screen;
extern CPicture scene02_screen;
//extern CPicture mask;
extern CTexture mask;

static KeyFrame comein[4];
static KeyFrame noisefade[4];
static KeyFrame k0;
static KeyFrame k1;
static KeyFrame k2;
static KeyFrame k3;

void seq_scene01()
{
	cam1.Sequence(scene01_campath.kfArray, scene01_campath.kfindex, 0, scenetime1);

	glPushMatrix();
	scene01_skybox.Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	light1.SetUpLight(GL_LIGHT3, CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(1, 1, 1, 1), CreateVector(0,1, 0, 1));
	light1.EnableLight();
	scene01_3ds.scale = 7;
	scene01_3ds.pos.x = 0;
	scene01_3ds.pos.y = 0;
	scene01_3ds.pos.z = 0;
	glEnable(GL_NORMALIZE);
	glPushMatrix();
	scene01_3ds.Draw();
	glPopMatrix();
	glDisable(GL_LIGHTING);

	glCullFace(GL_FRONT);
	glTranslatef(0,0.4f,0);
	glRotatef(90,1,0,0);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	scene01_envmap0.Use();
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	gluQuadricOrientation(tube, GLU_INSIDE);		//
	//------------------------
	gluCylinder(tube, 0.199f, 0.199f, 10, 32, 1);
	glColor4f(0.4f, 0.4f, 0.4f, 0.4f);
	gluQuadricOrientation(tube, GLU_OUTSIDE);		//
	gluCylinder(tube, 0.2f, 0.2f, 10, 32, 1);	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();

	noise_sequence(noisefade, 4, 0, scenetime1);
	glDisable(GL_CULL_FACE);
	scene01_caption.Draw(scenetime1);
	crossfade(scene01_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime1);
	crossfade(scene02_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime1);
}

bool init_seq_scene01()
{
	scene01_caption.Create(2999.002000f, -10, 0, 0,	 "Harsh words",
					   8997.007000f, -10, 0,  0,	 "and violent blows",
					   15000.816000f, -10, 0, 0,   "Hidden secrets",
					   21006.803000f, -10, 0, 0,   "no one knows.",
					   23998.186000f, -10, 0, 0,   "",
					   4000);	

	k0.SetKeyFrame(8997.007f, 0, 0, 0, 0, 0, 0);
	k1.SetKeyFrame(15000.816f, 0, 0, 0, 1, 1, 1);
	k2.SetKeyFrame(21006.803000f, 0, 0, 0, 1, 1, 1);
	k3.SetKeyFrame(23998.186000f, 0, 0, 0, 0, 0, 0);
	noisefade[0] = k0;
	noisefade[1] = k1;
	noisefade[2] = k2;
	noisefade[3] = k3;

	tube=gluNewQuadric();							//
	gluQuadricNormals(tube, GLU_FLAT);			// Create Smooth Normals
	gluQuadricOrientation(tube, GLU_OUTSIDE);		//
	gluQuadricTexture(tube, GL_TRUE);				// Create Texture Coords
	gluQuadricDrawStyle(tube, GL_FILL);				//

// transition effect

	k0.SetKeyFrame(0, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(600, 0, 0, 0, 0, 0, 0);		//  full color

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

	scenetime1 = fTime;
	return true;
}