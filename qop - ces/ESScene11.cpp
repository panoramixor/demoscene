// scene11

#include "stdafx.h"
/*
inside me
getting darker

once again 
*/
CCaption cap27;
CCaption cap28;
CCaption cap29;

extern bool g_bHighQuality;

/* Load */
bool CScene11::Load(char *filename)
{
	scenetime = 0;
	fxswitch = 0;
	b_init = false;
	model.Load("./data/scene11/11.3ds");
	model.SetFrameRate(30);
	
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);
	
	model.SetActiveCamera(0);
	efectito.SetType(3);

	cap27.Create(2000, 6000, -0.80f, 0.2f, -1, "inside me");
	cap28.Create(6000, 5000, -0.80f, 0.2f, -1, "getting darker");
	cap29.Create(11000, 5000, 0.40f, -0.35f, -1, "once again");
	return true;
}

void CScene11::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch == 1)
		fxswitch = -1;
	efectito2.SetType(fxswitch);
	model.SetActiveCamera(take%2);
}

/* Play */
void CScene11::Play(float sTime)
{
	if(g_bHighQuality) {
	glViewport(0,0,render_to_texture_width,render_to_texture_height);
	glEnable(GL_CULL_FACE);
//	model.SetCurrentFrame(0);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE, GL_DST_COLOR);
	glEnable(GL_BLEND);
	glColor3f(0.5f,0.5f,0.5f);
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
	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap27.Draw(scenetime);
	cap28.Draw(scenetime);
	cap29.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
