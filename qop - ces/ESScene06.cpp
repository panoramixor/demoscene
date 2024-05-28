// scene06

#include "stdafx.h"


/*
even when i don't know 
if there could be such a thing
*/
CCaption cap11;
CCaption cap12;

extern bool g_bHighQuality;

CPicture basetexto6;
KeyFrame kf_bt6[4];
CPicture basetexto7;
KeyFrame kf_bt7[5];


/* Load */
bool CScene06::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene06/06.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);

	basetexto6.Create(0,0,-2.f,4.0f,3.0f,"./data/scene06/s06_01.tga",0xff, 0xff, 0xff);
	kf_bt6[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt6[1].SetKeyFrame(2000,0,-1,0, 1,1,1);
	kf_bt6[2].SetKeyFrame(11000,0,-1,0, 1,1,1);
	kf_bt6[3].SetKeyFrame(11001,0,-1,0, 0,0,0);

	basetexto7.Create(0,0,-3.f,6.0f,4.5f,"./data/scene06/s06_01.tga", 255, 255, 255);
	kf_bt7[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt7[1].SetKeyFrame(3000,0,0,0, 0,0,0);
	kf_bt7[2].SetKeyFrame(6000,0,-2,0, 1,1,1);
	kf_bt7[3].SetKeyFrame(11000,0,-2,0, 1,1,1);
	kf_bt7[4].SetKeyFrame(11001,0,-2,0, 0,0,0);

	model.SetActiveCamera(0);
	model.SetFrameRate(50);

	efectito.SetType(4);
	efectito2.SetType(3);
	efectito3.SetType(0);

	cap11.Create(2000, 10000, -0.70f, -0.03f, -1, "even when i don't know");
	cap12.Create(6000, 6000, -0.70f, -0.15f, -1, "if there could be such a thing");

	return true;
}

void CScene06::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
}

/* Play */
void CScene06::Play(float sTime)
{
	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		model.Draw(sTime);

		// highpass filter (thanks to ithaqua/stravaganza for showing me the light) :P

		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_ONE, GL_DST_COLOR);
		glEnable(GL_BLEND);
		glColor3f(0.1f,0.1f,0.1f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			float step = 0.0458f;
			float z = -1;
			float width = 1.865f;
			float height = 1.4f;
			float x = - width  / 2;
			float y = - height / 2;
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y + height, z);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x + width, y + height, z); 
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x + width, y, z);
		glEnd();

		// back to normal
		glBlendEquation(GL_FUNC_ADD);
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
	tex_fb.Use();
	efectito2.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito3.Draw(sTime);
	glPopAttrib();

	basetexto6.Sequence(kf_bt6,4,0,scenetime);
	basetexto7.Sequence(kf_bt7,5,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap11.Draw(scenetime);
	cap12.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
