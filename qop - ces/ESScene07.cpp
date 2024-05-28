// scene07

#include "stdafx.h"

/*
and the absent claim
the neverending cry
*/

extern bool g_bHighQuality;

CParticles particles7;
CTexture tex_particle7;

CPicture basetexto8;
KeyFrame kf_bt8[5];

CCaption cap13;
CCaption cap14;
CCaption cap15;
CCaption cap16;
CCaption cap17;
CCaption cap18;
CCaption cap19;

/* Load */
bool CScene07::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene07/07_01.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(17010, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(18010, 0,0,-1, 1,1,1);

	basetexto8.Create(1.7f,0,-3.f,6.0f,4.5f,"./data/scene07/s07_01.tga", 255, 255, 255);
	kf_bt8[0].SetKeyFrame(0,0,0,-2, 0,0,0);
	kf_bt8[1].SetKeyFrame(1000,0,0,-2, 0,0,0);
	kf_bt8[2].SetKeyFrame(2000,0,0,0, 1,1,1);
	kf_bt8[3].SetKeyFrame(14000,0,0,0, 1,1,1);
	kf_bt8[4].SetKeyFrame(15000,0,0,-3, 0,0,0);

	cap13.Create(2000, 8000, 0.50f, 0.17f, -1, "and");
	cap14.Create(3000, 7000, 0.60f, 0.12f, -1, "the");
	cap15.Create(4000, 6000, 0.40f, 0.06f, -1, 0.004f, "absent");
	cap16.Create(5000, 5000, 0.47f, 0.01f, -1, "claim");
	cap17.Create(8000, 7000, 0.42f, -0.09f, -1, 0.004f, "the");
	cap18.Create(9000, 6000, 0.37f, -0.14f, -1, "neverending");
	cap19.Create(10000, 5000, 0.49f, -0.19f, -1, "cry");

	if(GLEW_ARB_point_sprite) {	
		tex_particle7.Load("./data/scene07/particle.tga");
		particles7.Create(1000, 0.3f, &tex_particle7);
		particles7.SetSize(350.f);
	}
	model.SetActiveCamera(0);
	model.SetFrameRate(250);

	efectito.SetType(3);
	return true;
}

void CScene07::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%2;
	if(fxswitch == 1)
		fxswitch = -1;
	efectito2.SetType(fxswitch);
}

/* Play */
void CScene07::Play(float sTime)
{
	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		model.Draw(sTime);
	//	glFlush();
		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glPushMatrix();
	model.Draw(sTime);
	glPopMatrix();
	if(GLEW_ARB_point_sprite) {
		particles7.Play(sTime);
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();

	basetexto8.Sequence(kf_bt8,5,0,scenetime);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap13.Draw(scenetime);
	cap14.Draw(scenetime);
	cap15.Draw(scenetime);
	cap16.Draw(scenetime);
	cap17.Draw(scenetime);
	cap18.Draw(scenetime);
	cap19.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
