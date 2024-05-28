#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

CPixelShow logo;
CPixelShow text1;
CPixelShow text2;
CPixelShow text3;
CPixelShow text4;
CPixelShow credits;

CTexture planet_flare;

extern float fTime;

static bool inited = false;
KeyFrame logo_kfarray[2];
KeyFrame header_kfarray[2];
KeyFrame text1_kfarray[2];
KeyFrame text2_kfarray[2];
KeyFrame text3_kfarray[2];
KeyFrame text4_kfarray[2];
KeyFrame credits_kfarray[3];

CTexture face;

KeyFrame k0, k1;

GLint starfield_list;

#define MAX_STARS 30000
#define STAR_SIZE 0.015f

float start_time;

GLUquadricObj* cilindric;

float cosftime = (float)cos(fTime);
float sinftime = (float)sin(fTime);

void RingMe(int i)
{
	glColor4f(sinftime,1,1,1-(i*0.011f));
	glPushMatrix();
	glTranslatef(0,0,(float)sin(fTime/i));
	glRotatef(sinftime*(i*9),-2-fTime,2*fTime,-2+fTime);
	glRotatef((float)(i*8),1,0,1);
	gluCylinder(cilindric, i*0.4, i*0.4, 0.8, 8, 4);
	glPopMatrix();

	return;
}

// Startup Stuff.
bool Init_RotateMe()
{
	cilindric = gluNewQuadric();

	gluQuadricDrawStyle(cilindric, GLU_LINE);
	gluQuadricTexture(cilindric, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	return true;
}

void RotateMe()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float cosftime = cosf(fTime);
	float sinftime = sinf(fTime);

	glPushMatrix();
		glTranslatef(0, 0, -5);
		glColor4f(1,1,1,1);
		glColor4f(1-sinf(fTime),-1+sinf(fTime)*2,cosf(fTime),1);
		gluLookAt(-sinf(fTime),-sinf(fTime),cos(fTime*3),(sinf(fTime)*2),(sinf(fTime)*2),6.0+(sinf(fTime)*2),0.5,0.5,0.5);
		glEnable(GL_BLEND);	
		glScalef(0.4f,0.4f,0.4f);
		glEnable(GL_TEXTURE_2D);
		planet_flare.Use();
		
		for(int e=0;e<=80;e++) {
			RingMe(e);
		}
	glPopMatrix();

}

bool init()
{
	logo.Create(640, 384, LOGO);
	text1.Create(320, 96, TEXT1);
	text2.Create(320, 96, TEXT2);
	text3.Create(320, 96, TEXT3);
	text4.Create(320, 64, TEXT4);
	credits.Create(256, 32, CREDITS);

#define TOPX -0.05f

	// LOGO Keyframes
	k0.SetKeyFrame(0, 1, -0.41f, -1, 1, 1, 1);		
	k1.SetKeyFrame(3000, -0.55f, -0.41f, -1, 1, 1, 1);
	logo_kfarray[0] = k0;
	logo_kfarray[1] = k1;

	// TEXT1 keyframes
	k0.SetKeyFrame(3000, 0.55f, 0.25f, -1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(6000, TOPX, 0.25f, -1, 1, 1, 1);		//
	text1_kfarray[0] = k0;
	text1_kfarray[1] = k1;


	// TEXT2 keyframes
	k0.SetKeyFrame(6000, 0.55f, 0.05f, -1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(9000, TOPX, 0.05f, -1, 1, 1, 1);		//
	text2_kfarray[0] = k0;
	text2_kfarray[1] = k1;

	// TEXT3 keyframes
	k0.SetKeyFrame( 9000, 0.55f, -0.15f, -1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(12000, TOPX, -0.15f, -1, 1, 1, 1);		//
	text3_kfarray[0] = k0;
	text3_kfarray[1] = k1;

	// TEXT4 keyframes
	k0.SetKeyFrame(12000, 0.55f, -0.4f, -1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(15000, TOPX, -0.4f, -1, 1, 1, 1);		//
	text4_kfarray[0] = k0;
	text4_kfarray[1] = k1;

	// Credits keyframes
	k0.SetKeyFrame(15000, -0.55f, 1, -1, 1, 1, 1);		//	fadein
	k1.SetKeyFrame(18000, -0.55f, 0.35f, -1, 1, 1, 1);		//
	credits_kfarray[0] = k0;
	credits_kfarray[1] = k1;
	credits_kfarray[2] = k1;

	// populate starfield
	planet_flare.LoadBMPResource(FLARE);
	static int i = 0;
	starfield_list = glGenLists(1);
	glNewList(starfield_list, GL_COMPILE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	planet_flare.Use();
	glBegin(GL_QUADS);
	for(i=0; i<MAX_STARS; i++)
	{
		float sx = -(float(rand()%41)*(rand()%41)/100)+(float(rand()%42)*(rand()%21)/100);
		float sy = -(float(rand()%32)*(rand()%30)/100)+(float(rand()%31)*(rand()%29)/100);
		float sz = +(float(rand()%60)*(rand()%280)/100)-(float(rand()%62)*(rand()%280)/100);
		glColor4f(1,1-sy/10,1+sx/10,sz/2);
		glTexCoord2f(0,0);
		glVertex3f(sx-STAR_SIZE,sy,sz);
		glTexCoord2f(1,0);
		glVertex3f(sx,sy,sz);
		glTexCoord2f(1,1);
		glVertex3f(sx,sy-STAR_SIZE,sz);
		glTexCoord2f(0,1);
		glVertex3f(sx-STAR_SIZE,sy-STAR_SIZE,sz);
	}
	glEnd();
	glDisable(GL_BLEND);
	glEndList();

	Init_RotateMe();
	start_time = fTime;
	return true;
}

bool Draw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RotateMe();

	glLoadIdentity();
	glTranslatef(0,0,-10-fTime/10);
	glRotatef(fTime*4,0,0,1);
	glCallList(starfield_list);

	glLoadIdentity();
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	logo.Sequence(logo_kfarray, 2, 1, start_time);
	text1.Sequence(text1_kfarray, 2, 1, start_time);
	text2.Sequence(text2_kfarray, 2, 1, start_time);
	text3.Sequence(text3_kfarray, 2, 1, start_time);
	text4.Sequence(text4_kfarray, 2, 1, start_time);
	glLoadIdentity();
	credits.Sequence(credits_kfarray, 3, 0, start_time);

	return false;
}