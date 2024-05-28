// scene05

#include "stdafx.h"

extern bool g_bHighQuality;

/*
fearing
the truth
*/

CParticles particles5;
CTexture tex_particle5;

CCaption cap09;
CCaption cap10;

/* Load */
bool CScene05::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene05/05b.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);

//	photo.Create(0,0,-5.f, 72.0f,9.0f, "./data/scene05/photo.bmp", 0xff, 0x1, 0xff);

	scroller[0].SetKeyFrame(0,30,0,-1,0.5f,0.5f,0.5f);
	scroller[1].SetKeyFrame(8000,0.75f,0,-1,0.8f,0.8f,0.8f);
	scroller[2].SetKeyFrame(16000,-30,0,-1,0.5f,0.5f,0.5f);

	center.Load("./data/scene05/02.tga");
	if(GLEW_ARB_point_sprite) {
		tex_particle5.Load("./data/scene05/particle.tga");
		particles5.Create(10000, 0.003f, &tex_particle5);
		particles5.SetSize(15.0f);
//		particles5.SetSize(1.5f);
		particles5.SetBlend(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}

	model.SetActiveCamera(0);
	model.SetFrameRate(220);

	efectito.SetType(3);
	efectito2.SetType(0);

	cap09.Create(4000, 10000, -0.70f, -0.05f, -1, 0.0039f, "fearing");
	cap10.Create(7000, 7000, -0.70f, -0.12f, -1, "the    truth");

	return true;
}

void CScene05::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
		glClearColor(0,0,0,0);
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch==1)
		fxswitch=0;
	model.SetActiveCamera((take%15)%2);
//	model.SetCurrentFrame(1);
}

/* Play */
void CScene05::Play(float sTime)
{
/*
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	photo.Sequence(scroller, 3, 0, scenetime);
	glPopAttrib();
*/
//	model.SetRenderMode(GL_LINES);

//	model.SetPos(0,0,0);// cuando hay texto que ver, camara 3
	
	if (sTime >= scenetime+4 && sTime <= scenetime+15) {
		model.SetPos(20,0,-40);
	}
	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		glEnable(GL_CULL_FACE);
		model.Draw(sTime);
		glDisable(GL_CULL_FACE);
		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);
	if(GLEW_ARB_point_sprite) {
		particles5.Play(sTime);
	}

//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
//	glPopAttrib();

//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
//	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap09.Draw(scenetime);
	cap10.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
