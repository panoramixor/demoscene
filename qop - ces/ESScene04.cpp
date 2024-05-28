// scene04

#include "stdafx.h"

extern bool g_bHighQuality;

/*
filling the hours
with nonsense
*/
CCaption cap07a;
CCaption cap07b;
CCaption cap07c;
CCaption cap08a;
CCaption cap08b;

CPicture basetexto5a;
KeyFrame kf_bt5a[5];
CPicture basetexto5b;
KeyFrame kf_bt5b[5];

/* Load */
bool CScene04::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene04/04.3ds");
	
	basetexto5a.Create(0,-0.3f,-2.f,4.0f,3.0f,"./data/scene04/s04_02.tga", 255, 255, 255);
	kf_bt5a[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt5a[1].SetKeyFrame(2000,0,0,0, 0,0,0);
	kf_bt5a[2].SetKeyFrame(3000,0,0,0, 1,1,1);
	kf_bt5a[3].SetKeyFrame(9000,0,0,0, 1,1,1);
	kf_bt5a[4].SetKeyFrame(10000,0,0,0, 0,0,0);

	basetexto5b.Create(0,-1.2f,-2.f,4.0f,3.0f,"./data/scene04/s04_02.tga", 255, 255, 255);
	kf_bt5b[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt5b[1].SetKeyFrame(5000,0,0,0, 0,0,0);
	kf_bt5b[2].SetKeyFrame(6000,0,0,0, 1,1,1);
	kf_bt5b[3].SetKeyFrame(12000,0,0,0, 1,1,1);
	kf_bt5b[4].SetKeyFrame(13000,0,0,0, 0,0,0);

	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);

	efectito.SetType(3);
	model.SetActiveCamera(0);
	model.SetFrameRate(220);

	cap07a.Create(3000, 8000, 0.10f, 0.23f, -1, 0.005f, "filling");
	cap07b.Create(3300, 8000, 0.32f, 0.16f, -1, "the");
	cap07c.Create(3900, 8000, 0.32f, 0.09f, -1, 0.004f, "hours");
	cap08a.Create(6000, 6000, 0.41f, -0.3f, -1, 0.004f, "with");
	cap08b.Create(6000, 6000, 0.24f, -0.35f, -1, "nonsense");

	return true;
}

void CScene04::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
//	fxswitch = take%4;
//	if(fxswitch==1)
//		fxswitch=0;
//	model.SetActiveCamera((take%15)%2);
//	model.SetCurrentFrame(1);
}

/* Play */
void CScene04::Play(float sTime)
{
	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		model.Draw(sTime);
	//	glFlush();
		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	model.Draw(sTime);
	// test code
//	glPushMatrix();
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);
	/*
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(10, 10, 0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(10, 10 + 20, 1);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(10 + 20, 10 + 20, 1); 
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(10 + 20, 10, 1);
	glEnd();*/
//	glPopMatrix(); 
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	basetexto5a.Sequence(kf_bt5a,5,0,scenetime);
	basetexto5b.Sequence(kf_bt5b,5,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap07a.Draw(scenetime);
	cap07b.Draw(scenetime);
	cap07c.Draw(scenetime);
	cap08a.Draw(scenetime);
	cap08b.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
