// scene04

#include "stdafx.h"

extern 	CFMod music;


CCube cubos;
CDevilTexture texturecubos;
CDevilTexture scene04ring;
CDevilTexture credits;
CInterpolator creditsfade;


/* Load */
bool CScene04::Load(char *filename)
{

	scenetime = 0;
	b_init = false;
	cubos.Create(0.1f, 0.1f, 0.1f);
	texturecubos.Load("./data/scene04/texturecubos.bmp");
	scene04ring.Load("./data/scene04/ringblac.bmp");
	credits.Load("./data/credits.bmp");
	model.Load("./data/scene04/scene04.3ds");
	activecamera=0;
	model.SetActiveCamera(activecamera);
	Keyframe *kf = new Keyframe;

	// fade in
	fx_fadein.SetLen(4);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 0.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 1.0f;
	fx_fadein.Set(1, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 22.28482f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 23.28482f;
	fx_fadein.Set(3, kf);

	creditsfade.SetLen(2);
	kf->position.Set(10, 0, 0);
	kf->time = 5;
	creditsfade.Set(0, kf);

	kf->position.Set(0, 0, 0);
	kf->time = 8;
	creditsfade.Set(1, kf);

	delete kf;
	return true;
}

void CScene04::Init(float sTime, unsigned int take)
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
	model.SetActiveCamera(activecamera);
}

/* Play */
void CScene04::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model.Draw(sTime);

	float levels = music.GetVolumeLevels();
	if (levels > 1.0f) { levels = 0.1f; }

	glDisable(GL_LIGHTING);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	glDepthMask(0);
	glEnable(GL_BLEND);
	glPushMatrix();	
	glScalef(10, 10, 10);

	float k = 0.25f;
	glPointSize(2.0f);
	glDisable(GL_DEPTH_TEST);
	DisableAllTextures();

	for (int j = 0 ; j <= 16 ; j ++)
	{
		glTranslatef(0.2f,0.2f,0);
		glRotatef(sTime*j, 0, 1, 1);
		glPushMatrix();	
		for (int i = 0 ; i <= 80 ; i ++)
		{			
			glTranslatef(0.5f,0,0);
			glRotatef(sTime*((float)i/10.f),1,1,1);

			float h_mod = float((sinf (powf((float)(i+sTime)/5.0f,2)+powf((float)(j+sTime)/5.0f,2)))*(cosf(sTime*(j*i)/500.f)));
			if(h_mod < 0)
				h_mod = 0.0f;
			glTranslatef(0,h_mod*(levels/10),0);
			glColor4f(h_mod, h_mod, h_mod, h_mod);
			texturecubos.Bind();
			cubos.Draw(sTime);
		}
		glPopMatrix();	
	}
	glPopMatrix();	

	float width = 240.f;
	float height = 240.f;
	float x=-width/2.0f;
	float y=-height/2.0f;
	float z= 0;

	scene04ring.Bind();
	glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
	for(unsigned int i=0;i<15;i++) {
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y+height, z+0.4f*i*levels);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y, z+0.4f*i*levels);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y+height, z+0.4f*i*levels);
	}
	glEnd();
	glDepthMask(1);
}

void CScene04::PostRender(float sTime)
{
	Keyframe *kf;
	float width;
	float height;
	float x;
	float y;
	float z;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 800/600, 0.01f, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	kf = creditsfade.Evaluate(sTime-scenetime);
	glTranslatef(kf->position.x, kf->position.y, kf->position.z);
	width = 1.4f;
	height = 1.4f;
	x=-width/2.0f;
	y=-height/2.0f;
	z=-1;
	credits.Bind();
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glColor4f(1,1,1,1);
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



	width = 2;
	height = 2;
	x=-width/2.0f;
	y=-height/2.0f;
	z=-1;
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
