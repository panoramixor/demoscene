//	scene03

#include "stdafx.h"

extern 	CFMod music;

CDevilTexture ringblack;

/* Load */
bool CScene03::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene03/scene03b.3ds");
	model.SetFrameRate(100);
	ringblack.Load("./data/scene03/ringblack.bmp");
	model.SetActiveCamera(1);

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
	kf->time = 37.412788f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 38.412788f;
	fx_fadein.Set(3, kf);

	delete kf;

	return true;
}

void CScene03::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		glClearColor(1,1,1,1);
		scenetime = sTime;
	}
	b_init = true;
	model.SetActiveCamera(take%2);
}

/* Play */
void CScene03::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	model.Draw(sTime);
	glPushMatrix();
	glRotatef(sTime*60, 0,0, 1);
	float width = 40.f;
	float height = 40.f;
	float x=-width/2.0f;
	float y=-height/2.0f;
	float z= 0;
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	DisableAllTextures();
	float levels = music.GetVolumeLevels();
	ringblack.Bind();
	glColor4f(1, 1, 1, 1);

	glScalef(2+sin(sTime), 2+sin(sTime), 2+sin(sTime));
	glBegin(GL_QUADS);
	for(unsigned int i=0;i<20;i++) {
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height, z+0.2f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y, z+0.2f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y, z+0.2f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height, z+0.2f*i*levels);

	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	for(unsigned int i=0;i<70;i++) {
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width-100, y+height, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width-100, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x-100, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x-100, y+height, z+0.4f*i*levels);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width-100, y+height-106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width-100, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x-100, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x-100, y+height-106, z+0.4f*i*levels);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width-100, y+height+106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width-100, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x-100, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x-100, y+height+106, z+0.4f*i*levels);
//
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height-106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height-106, z+0.4f*i*levels);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height+106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height+106, z+0.4f*i*levels);
//
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width+100, y+height, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width+100, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x+100, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x+100, y+height, z+0.4f*i*levels);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width+100, y+height-106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width+100, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x+100, y-106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x+100, y+height-106, z+0.4f*i*levels);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width+100, y+height+106, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width+100, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x+100, y+106, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x+100, y+height+106, z+0.4f*i*levels);

	}
	glEnd();
	glPopMatrix();

}

void CScene03::PostRender(float sTime)
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
