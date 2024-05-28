// scene14

#include "stdafx.h"

extern bool g_bHighQuality;

/* Load */
bool CScene14::Load(char *filename)
{
	scenetime = 0;
	fxswitch = 0;
	b_init = false;
	model.Load("./data/scene14/14_txt.3ds");
	model_box.Load("./data/scene14/14_box.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	model.SetFrameRate(100);
	model_box.SetFrameRate(100);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);
	model.SetActiveCamera(1);
	model_box.SetActiveCamera(1);
	efectito.SetType(5);
	return true;
}

void CScene14::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
		glClearColor(0,0,0,0);
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch == 1)
		fxswitch = 0;
//	model.SetCurrentFrame(0);
	efectito2.SetType(fxswitch);
	//	model.SetActiveCamera(take%2);
}

CScene14::~CScene14() {
}

/* Play */
void CScene14::Play(float sTime)
{
	if(g_bHighQuality) {
	glViewport(0,0,render_to_texture_width,render_to_texture_height);

	glEnable(GL_CULL_FACE);
	model_box.Draw(sTime);
	glDisable(GL_CULL_FACE);

	tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

	glViewport(0,0,window_width,window_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	glEnable(GL_CULL_FACE);
	model_box.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);
/*
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();
*/
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
