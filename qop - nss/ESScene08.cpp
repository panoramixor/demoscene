// Scene08

#include "stdafx.h"

extern 	CFMod music;

/* Load */
bool CScene08::Load(char *filename)
{
	scenetime = 0;
	b_init = false;

	Keyframe *kf = new Keyframe;

	model.Load("./data/scene08/scene08.3ds");
	model.SetFrameRate(160);

	// fade in
	fx_fadein.SetLen(4);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 0.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 1.0f;
	fx_fadein.Set(1, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 18.203781f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 19.203781f;
	fx_fadein.Set(3, kf);

	delete kf;
	return true;
}

void CScene08::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		glClearColor(1, 1, 1, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
		scenetime = sTime;
		b_init = true;
	}
	switch(take%2) {
		case 0:
			activecamera=0;
			break;

		case 1:
			activecamera=1;
			break;
	}
	model.SetActiveCamera((int)activecamera);
}

/* Play */
void CScene08::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model.Draw(sTime);
}

void CScene08::PostRender(float sTime)
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
