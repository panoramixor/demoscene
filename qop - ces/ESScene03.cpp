// scene03

#include "stdafx.h"

extern bool g_bHighQuality;
/*
of knowing to little
*/

CCaption cap03;
CCaption cap04;
CCaption cap05;
CCaption cap06;

CPicture basetexto2;
KeyFrame kf_bt2[4];
CPicture basetexto3;
KeyFrame kf_bt3[5];
CPicture basetexto4;
KeyFrame kf_bt4[5];

/* Load */
bool CScene03::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene03/03.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
//	photo.Create(0,0,-5.f, 72.0f,9.0f, "./data/scene03/photo.bmp", 0xff, 0x1, 0xff);

	basetexto2.Create(0,0,-2.f,4.0f,3.0f,"./data/scene03/s03_01.tga", 255, 255, 255);
	kf_bt2[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt2[1].SetKeyFrame(0001,0,0,0, 1,1,1);
	kf_bt2[2].SetKeyFrame(11000,0,0,0, 1,1,1);
	kf_bt2[3].SetKeyFrame(11500,0,0,0, 0,0,0);

	basetexto3.Create(0,0,-3.f,6.0f,4.5f,"./data/scene03/s03_02.tga", 255, 255, 255);
	kf_bt3[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt3[1].SetKeyFrame(0700,0,0,0, 0,0,0);
	kf_bt3[2].SetKeyFrame(0701,0,0,0, 1,1,1);
	kf_bt3[3].SetKeyFrame(11000,0,0,0, 1,1,1);
	kf_bt3[4].SetKeyFrame(11500,0,0,0, 0,0,0);

	basetexto4.Create(0,0,-4.f,10.0f,7.5f,"./data/scene03/s03_03.tga", 255, 255, 255);
	kf_bt4[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt4[1].SetKeyFrame(1000,0,0,0, 0,0,0);
	kf_bt4[2].SetKeyFrame(1001,0,0,0, 1,1,1);
	kf_bt4[3].SetKeyFrame(11000,0,0,0, 1,1,1);
	kf_bt4[4].SetKeyFrame(11500,0,0,0, 0,0,0);

	model.SetFrameRate(90);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);
/*
	scroller[0].SetKeyFrame(0,30,0,-1,0.3f,0.3f,0.3f);
	scroller[1].SetKeyFrame(8000,0.75f,0,-1,0.5f,0.5f,0.5f);
	scroller[2].SetKeyFrame(16000,-30,0,-1,0.3f,0.3f,0.3f);
*/
	efectito.SetType(3);
	model.SetActiveCamera(0);

	cap03.Create(1000, 10000, 0.71f, 0.4f, -1,  "of");
	cap04.Create(1500, 9000, 0.48f, 0.35f, -1, "knowing");
	cap05.Create(2000, 8000, 0.70f, 0.3f, -1, "too");
	cap06.Create(2500, 7000, 0.68f, 0.25f, -1, "little");

	return true;
}

void CScene03::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch == 1)
		fxswitch = 0;

	efectito2.SetType(fxswitch);
	model.SetActiveCamera((take%15)%2);
//	model.SetCurrentFrame(250);
//	model.SetCurrentFrame(1);
}

/* Play */
void CScene03::Play(float sTime)
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

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();

	basetexto4.Sequence(kf_bt4,5,0,scenetime);
	basetexto3.Sequence(kf_bt3,5,0,scenetime);
	basetexto2.Sequence(kf_bt2,4,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap03.Draw(scenetime);
	cap04.Draw(scenetime);
	cap05.Draw(scenetime);
	cap06.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
