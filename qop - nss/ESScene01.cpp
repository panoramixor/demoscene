// scene01

#include "stdafx.h"

extern 	CFMod music;

CDevilTexture texture;

CCube cubos[100];
CModel3DS model2;
CModel3DS model3;

/* Load */
bool CScene01::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	texture.Load("./data/reflect.jpg");

	model3.Load("./data/scene01/firulete2.3ds");
	model3.SetActiveCamera(0);

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
	kf->time = 19.014999f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 19.214999f;
	fx_fadein.Set(3, kf);

	delete kf;

	return true;
}

void CScene01::Init(float sTime, unsigned int take)
{
	glClearColor(1,1,1,1);
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
}

/* Play */
void CScene01::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	float levels = music.GetVolumeLevels();
	if (levels > 1.0f) { levels = 0.1f; }
	model3.Draw((float)sTime);
}

void CScene01::PostRender(float sTime)
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