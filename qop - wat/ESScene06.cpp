// scene06

#include "stdafx.h"

extern float fTime;

extern Model_3DS scene06_building;
extern Model_3DS scene06_terrain;

int mcount=0;
int mindex=0;

static CCamera cam6;
extern CSkybox scene06_skybox;

static CLight light6;

extern CPicture scene06_screen;
extern CPicture scene07_screen;
extern CTexture mask;

extern CCamPath scene06_campath;

static CCaption scene06_caption;

int totalscene06keyframes;
float scenetime6;

static KeyFrame Scene06[6];

extern CPicture scene06_screen;
extern CPicture scene07_screen;

extern CTexture water;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

static KeyFrame k0, k1;


void seq_scene06()
{
	glLoadIdentity();
	glTranslatef(0,0,-5);

	cam6.Sequence(scene06_campath.kfArray, scene06_campath.kfindex, 0, scenetime6);

	glDisable(GL_LIGHTING);
	scene06_skybox.Draw();

	glEnable(GL_DEPTH_TEST);
	scene06_terrain.scale = 1;
	scene06_terrain.pos.y = -4.5f;
	scene06_terrain.pos.x = -0.5f;
	glPushMatrix();
	scene06_terrain.Draw();
	glPopMatrix();

	scene06_building.scale = 3;
	scene06_building.pos.y = -2.26f;
	scene06_building.pos.x = 1.5f;
	scene06_building.pos.z = -1.3f;
	glPushMatrix();
	scene06_building.Draw();
	glPopMatrix();

	// here comes the water!!;]
	glPushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
	float x, y, z;
	float width, height;
	x = -10;
	y = -10;
	z = 1.95f;
	width = 500;
	height = 500;
	glRotatef(90, 1, 0, 0);
	water.Use();
	glBegin(GL_QUADS);		
		// layer1
		glTexCoord2f(0.0f+(fTime*0.2f), width+(fTime*0.2f)); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f+(fTime*0.2f), 0.0f+(fTime*0.2f)); glVertex3f(x,			y + height, z);
		glTexCoord2f(width+(fTime*0.2f), 0.0f+(fTime*0.2f)); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(width+(fTime*0.2f), width+(fTime*0.2f)); glVertex3f(x + width, y,			z);

		// layer2
		glTexCoord2f(0.0f+(float)sin(fTime)/4, width-(fTime*0.2f)); glVertex3f(x,			y,			z-0.02f);
		glTexCoord2f(0.0f+ (float)sin(fTime)/4, 0.0f-(fTime*0.2f)); glVertex3f(x,			y + height , z-0.02f);
		glTexCoord2f(width+ (float)sin(fTime)/4, 0.0f-(fTime*0.2f)); glVertex3f(x + width, y + height, z-0.02f);
		glTexCoord2f(width+ (float)sin(fTime)/4, width-(fTime*0.2f)); glVertex3f(x + width, y,			z-0.02f);
	glEnd();
	glPopMatrix();

	scene06_caption.Draw(scenetime6);
	crossfade(scene06_screen, mask, ColorFadeOut, ColorFadeOut2, scenetime6);
	crossfade(scene07_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime6);
}

bool init_seq_scene06()
{
	scene06_caption.Create(2999.002000f, -10, 0, 0,	 "I'd call my mind",
					   8997.007000f, -10, 0, 0,	 "but the numbers unlisted.",
					   15000.816000f, -10, 0, 0,   "I'm lost in someone so",
					   21006.803000f, -10, 0, 0,   "Warped and twisted.",
					   23998.186000f, -10, 0, 0,   "Warped and twisted.",
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

	k0.SetKeyFrame(27002.90300f-500, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(27002.90300f-500, 0, 0, 0, 1, 1, 1);		//	full transparent
	k1.SetKeyFrame(27002.90300f, 0, 0, 0, 0, 0, 0);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scenetime6 = fTime;
	return true;
}