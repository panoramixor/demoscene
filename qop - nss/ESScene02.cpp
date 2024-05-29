// scene02

#include "stdafx.h"

extern 	CFMod music;

CDevilTexture ringblack02;

/* Load */
bool CScene02::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene02/canios.3ds");
	model.SetActiveCamera(1);
	ringblack02.Load("./data/scene02/ringblack.bmp");

	Keyframe *kf = new Keyframe;

	// fade in
	fx_fadein.SetLen(5);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 0.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 0.2f;
	fx_fadein.Set(1, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 19.387413f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 20.387413f;
	fx_fadein.Set(3, kf);

	delete kf;

	return true;
}

void CScene02::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
		b_init = true;
	}
	glClearColor(1,1,1,1);
}

/* Play */
void CScene02::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model.Draw(sTime);
	glRotatef(sTime*60, 0,0, 1);
	float width = 450.f;
	float height = 450.f;
	float x=-width/2.0f;
	float y=-height/2.0f;
	float z= -180;
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	DisableAllTextures();
	ringblack02.Bind();
	glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
	glDepthMask(0);
	glBegin(GL_QUADS);
	for(unsigned int i=0;i<36;i++) {
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height, z+10*i);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y, z+10*i);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y, z+10*i);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height, z+10*i);
	}
	glEnd();
	glDepthMask(1);

}

void CScene02::PostRender(float sTime)
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