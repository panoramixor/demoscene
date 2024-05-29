// Scene06

#include "stdafx.h"

extern 	CFMod music;
CDevilTexture lines2;

/* Load */
bool CScene06::Load(char *filename)
{
	scenetime = 0;
	b_init = false;

	Keyframe *kf = new Keyframe;

	lines2.Load("./data/scene06/lines.bmp");

	model.Load("./data/scene06/scene06.3ds");
	model.SetFrameRate(200);

	// fade in
	fx_fadein.SetLen(4);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 0.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 1.0f;
	fx_fadein.Set(1, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 38.02153f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 39.02153f;
	fx_fadein.Set(3, kf);

	delete kf;
	return true;
}

void CScene06::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		glClearColor(1,1, 1, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
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
void CScene06::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLoadIdentity();
	float width = 3;
	float height = 0.6f;
	float x=-width/2.0f;
	float y=0.5f;
	float z=-2;
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	DisableAllTextures();
	lines2.Bind();
	float levels = music.GetVolumeLevels();
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
	for(unsigned int i=0;i<5;i++) {
			glTexCoord2f(1.0f+(sTime*levels), 0.0f); 
			glVertex3f(x+width, y+height, z+i*(levels/5.f));
			glTexCoord2f(1.0f+(sTime*levels), 1.0f); 
			glVertex3f(x+width, y, z+i*(levels/5.f));
			glTexCoord2f(0.0f+(sTime*levels), 1.0f); 
			glVertex3f(x, y, z+i*(levels/5.f));
			glTexCoord2f(0.0f+(sTime*levels), 0.0f); 
			glVertex3f(x, y+height, z+i*(levels/5.f));

			glTexCoord2f(1.0f+(sTime*levels), 0.0f); 
			glVertex3f(x+width, -y-height, z+i*(levels/5.f));
			glTexCoord2f(1.0f+(sTime*levels), 1.0f); 
			glVertex3f(x+width, -y, z+i*(levels/5.f));
			glTexCoord2f(0.0f+(sTime*levels), 1.0f); 
			glVertex3f(x, -y, z+i*(levels/5.f));
			glTexCoord2f(0.0f+(sTime*levels), 0.0f); 
			glVertex3f(x, -y-height, z+i*(levels/5.f));
	}
	glEnd();
	glPopAttrib();
	model.Draw(sTime);
}

void CScene06::PostRender(float sTime)
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
