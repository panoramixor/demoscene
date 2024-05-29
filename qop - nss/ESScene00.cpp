// title scene

#include "stdafx.h"

extern 	CFMod music;

CDevilTexture lines;

CScene00::CScene00()
{

}

CScene00::~CScene00()
{

}


/* Load */
bool CScene00::Load(char *filename)
{
	scenetime = 0;
	b_init = false;

	model.Load("./data/scene00/base2_01.3ds");
	qoplogo.Load("./data/qoplogo.jpg");
	lines.Load("./data/scene00/lines.bmp");

	// "not"
	txtnotanim.SetLen(2);
	Keyframe *kf = new Keyframe;

	kf->color.Set(0.0f,0.0f,0.0f,0.0f);
	kf->time = 9.0f;
	txtnotanim.Set(0, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 11.0f;
	txtnotanim.Set(1, kf);

	// "so"
	txtsoanim.SetLen(2);
	kf->color.Set(0.0f,0.0f,0.0f,0.0f);
	kf->time = 15.0f;
	txtsoanim.Set(0, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 17.0f;
	txtsoanim.Set(1, kf);

	// "soon"
	txtletrasanim.SetLen(2);
	kf->color.Set(0.0f,0.0f,0.0f,0.0f);
	kf->time = 19.0f;
	txtletrasanim.Set(0, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 21.0f;
	txtletrasanim.Set(1, kf);

	// fade in
	fx_fadein.SetLen(4);
	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 3.0f;
	fx_fadein.Set(0, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 7.0f;
	fx_fadein.Set(1, kf);

	kf->color.Set(0,0,0,0);
	kf->time = 27.867958f;
	fx_fadein.Set(2, kf);

	kf->color.Set(1.0f,1.0f,1.0f,1.0f);
	kf->time = 28.867958f;
	fx_fadein.Set(3, kf);

	delete kf;
	return true;
}

void CScene00::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		glClearColor(1, 1, 1, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
//		scenetime = sTime;
		b_init = true;
	}
	switch(take) {
		case 0:
			break;

		case 1:
			break;

		case 2:
			break;
	}
}

/* Play */
void CScene00::Play(float sTime, CRenderToTexture *framebuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, window_width/window_height, 0.01f, 4000.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	float width = 3;
	float height = 0.6f;
	float x=-width/2.0f;
	float y=-0.5f;
	float z=-2;
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glEnable(GL_BLEND);
	DisableAllTextures();
	lines.Bind();
	glDisable(GL_DEPTH_TEST);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);		
	for(unsigned int i=0;i<5;i++) {
			glTexCoord2f(1.0f+sTime, 0.0f); 
			glVertex3f(x+width, y+height, z-0.001f*i);
			glTexCoord2f(1.0f+sTime, 1.0f); 
			glVertex3f(x+width, y, z-0.001f*i);
			glTexCoord2f(0.0f+sTime, 1.0f); 
			glVertex3f(x, y, z-0.001f*i);
			glTexCoord2f(0.0f+sTime, 0.0f); 
			glVertex3f(x, y+height, z-0.001f*i);
	}
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1,1,1,1);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	model.Draw(sTime);
	glPopAttrib();

}

void CScene00::PostRender(float sTime)
{
	float width;
	float height;
	float x;
	float y;
	float z;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, window_width/window_height, 0.01f, 4000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	width = 0.73f;
	height = width/1.53f;
	x=-width/2.0f-0.3f;
	y=-height/2.0f+0.4f;
	z=-1;
	
	qoplogo.Bind();
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, window_width/window_height, 0.01f, 4000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1,1,1,1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
//	glBlendEquationEXT(GL_FUNC_SUBTRACT_EXT);
//	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glLoadIdentity();
	Keyframe *kf = txtnotanim.Evaluate(sTime-scenetime);
	glColor4f(kf->color.r,kf->color.g,kf->color.b,kf->color.a);
	title_not.SetPos(-0.5f,0.2f,0);
	title_not.Print("not");

	glLoadIdentity();
	kf = txtsoanim.Evaluate(sTime-scenetime);
	glColor4f(kf->color.r,kf->color.g,kf->color.b,kf->color.a);
	title_so.SetPos(-0.5f,0.16f,0);
	title_so.Print("so");
	
	glLoadIdentity();
	kf = txtletrasanim.Evaluate(sTime-scenetime);
	glColor4f(kf->color.r,kf->color.g,kf->color.b,kf->color.a);
	title_soon.SetPos(-0.5f,0.12f,0);
	title_soon.Print("soon");

	glLoadIdentity();
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
