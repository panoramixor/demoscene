// Scene09

#include "stdafx.h"

extern 	CFMod music;

CScene *scenes[9];
unsigned int activescene =0;

extern CScene01 scene01;
extern CScene02 scene02;
extern CScene03 scene03;
extern CScene10 scene04;
extern CScene05 scene05;
extern CScene06 scene06;
extern CScene07 scene07;
extern CScene08 scene08;

/* Load */
bool CScene09::Load(char *filename)
{
	scenetime = 0;
	b_init = false;

	model.Load("./data/scene09/scene09.3ds");

	Keyframe *kf = new Keyframe;

	// fade in
	fx_fadein.SetLen(4);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 0.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 1.0f;
	fx_fadein.Set(1, kf);


/*
	kf->color.Set(0,0,0,0);
	kf->time = 1.400295f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1,1,1,1);
	kf->time = 2.400295f;
	fx_fadein.Set(3, kf);

	kf->color.Set(1,1,1,1);
	kf->time = 2.400295f;
	fx_fadein.Set(4, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 3.400295f;
	fx_fadein.Set(5, kf);
//
	kf->color.Set(0,0,0,0);
	kf->time = 4.8005f;
	fx_fadein.Set(6, kf);

	kf->color.Set(1,1,1,1);
	kf->time = 5.8005f;
	fx_fadein.Set(7, kf);

	kf->color.Set(1,1,1,1);
	kf->time = 5.8005f;
	fx_fadein.Set(8, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 6.8005f;
	fx_fadein.Set(9, kf);


//	kf->color.Set(0,0,0,0);
//	kf->time = 3.400295f;
//	fx_fadein.Set(9, kf);

*/
	kf->color.Set(0,0,0,0);
	kf->time = 18.217041f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 19.217041f;
	fx_fadein.Set(3, kf);

	scenes[1]=&scene01;
	scenes[2]=&scene02;
	scenes[3]=&scene03;
	scenes[4]=&scene04;
	scenes[5]=&scene05;
	scenes[6]=&scene06;
	scenes[7]=&scene07;
	scenes[8]=&scene08;

	delete kf;
	return true;
}

void CScene09::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		glClearColor(1, 1, 1, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
		scenetime = sTime;
		b_init = true;
	} 
	activescene++;
		/*
#ifdef _DEBUG
		LogLine("DEBUG -> %f\n", sTime-scenetime);
#endif*/
	if(activescene > 8)
		activescene = 0;
}

/* Play */
void CScene09::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(activescene == 0)
		model.Draw(sTime);
	else {
		scenes[activescene]->Play(sTime, 0);
	}
}

void CScene09::PostRender(float sTime)
{

	Keyframe *kf;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 800/600, 0.01f, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float width = 2;
	float height = 2;
	float x=-width/2.0f;
	float y=-height/2.0f;
	float z=-1;
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	DisableAllTextures();
	glDisable(GL_DEPTH_TEST);
	kf = fx_fadein.Evaluate(sTime-scenetime);
//	glColor4f(1, 1, 1, kf->color.a);
	glColor4f(kf->color.r, kf->color.g, kf->color.b, kf->color.a);
	glBegin(GL_QUADS);		
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height, z-0.001f);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y, z-0.001f);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y, z-0.001f);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height, z-0.001f);
	glEnd();
}
