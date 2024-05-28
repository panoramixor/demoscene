// scene09

#include "stdafx.h"
#define TIME_OFFSET 2000

/*
you told me
not to worry

but then again 
you managed
to escape
*/
CCaption cap22a;
CCaption cap22b;
CCaption cap23a;
CCaption cap23b;
CCaption cap23c;
CCaption cap24a;
CCaption cap24b;

extern bool g_bHighQuality;

CPicture basetexto12;
KeyFrame kf_bt12[4];
CPicture basetexto13;
KeyFrame kf_bt13[5];
CPicture basetexto14;
KeyFrame kf_bt14[5];

CPicture scene09gfx;

/* Load */
bool CScene09::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene09/09new.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	scene09gfx.Create(0,0,-5.f, 12.0f,9.0f, "./data/scene09/0002.jpg", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(31000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(32000, 0,0,-1, 1,1,1);

	center.Load("./data/scene09/0002.jpg");

	model.SetActiveCamera(0);
	model.SetFrameRate(250);

	efectito.SetType(5);

	basetexto12.Create(0,0,-3.f,6.0f,4.5f,"./data/scene09/s09_01.tga", 255, 255, 255);
	kf_bt12[0].SetKeyFrame(TIME_OFFSET + 7000,-3.f,-0.4f,0, 0,0,0);
	kf_bt12[1].SetKeyFrame(TIME_OFFSET + 8000,-3.f,-0.4f,0, 1,1,1);
	kf_bt12[2].SetKeyFrame(TIME_OFFSET + 24000,-3.f,-0.4f,0, 1,1,1);
	kf_bt12[3].SetKeyFrame(TIME_OFFSET + 25000,-3.f,-0.4f,0, 0,0,0);

	basetexto13.Create(0,0,-3.f,6.0f,4.5f,"./data/scene09/s09_04.tga", 255, 255, 255);
	kf_bt13[0].SetKeyFrame(TIME_OFFSET + 0,-0.35f,0,-4, 0,0,0);
	kf_bt13[1].SetKeyFrame(TIME_OFFSET + 15000,4.0f,-3.1f,-4, 0,0,0);
	kf_bt13[2].SetKeyFrame(TIME_OFFSET + 16000,4.0f,-3.1f,-4, 1,1,1);
	kf_bt13[3].SetKeyFrame(TIME_OFFSET + 26000,4.0f,-3.1f,-4, 1,1,1);
	kf_bt13[4].SetKeyFrame(TIME_OFFSET + 27000,4.0f,-3.1f,-4, 0,0,0);

	basetexto14.Create(0,0,-3.f,6.0f,4.5f,"./data/scene09/s09_04.tga", 255, 255, 255);
	kf_bt14[0].SetKeyFrame(TIME_OFFSET + 0,-0.35f,0,-4, 0,0,0);
	kf_bt14[1].SetKeyFrame(TIME_OFFSET + 15000,2.7f,-4.5f,-4, 0,0,0);
	kf_bt14[2].SetKeyFrame(TIME_OFFSET + 16000,2.7f,-4.5f,-4, 1,1,1);
	kf_bt14[3].SetKeyFrame(TIME_OFFSET + 26000,2.7f,-4.5f,-4, 1,1,1);
	kf_bt14[4].SetKeyFrame(TIME_OFFSET + 27500,2.7f,-4.5f,-4, 0,0,0);

	cap22a.Create(TIME_OFFSET + 9000, 18000, -0.90f, 0.4f, -1, 0.002f, "you told me");
	cap22b.Create(TIME_OFFSET + 9000, 18000, -0.90f, 0.35f, -1, "not to worry");
	cap23a.Create(TIME_OFFSET + 17000, 10000, 0.54f, -0.2f, -1, "but");
	cap23b.Create(TIME_OFFSET + 17200, 10000, 0.67f, -0.22f, -1, 0.0025f, "then");
	cap23c.Create(TIME_OFFSET + 17400, 10000, 0.54f, -0.28f, -1, 0.004f, "again");
	cap24a.Create(TIME_OFFSET + 18000, 9000, 0.0f, -0.40f, -1, 0.0025f, "you managed");
	cap24b.Create(TIME_OFFSET + 18200, 9000, 0.0f, -0.47f, -1, 0.004f, "to escape");

	return true;
}

void CScene09::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%2;
	efectito2.SetType(fxswitch);
	model.SetActiveCamera((take%15)%2);
}

/* Play */
void CScene09::Play(float sTime)
{
	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		model.Draw(sTime);
//	glFlush();
/*
	glPushMatrix();
	glDepthMask(FALSE);
	glRotatef(sTime*50, sin(sTime*10)+10,sin(sTime*10)+10,sin(sTime*10)+10);
	glEnable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);
	center.Use();
//	tex_fb.Use();
//	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	for(int i=0;i<=80;i++) {
		glRotatef(360/80, 1.0,1,1);
		glScalef(1.1f, 1.1f, 1.1f);
	glPushMatrix();
		glColor4f(0.4f,0.4f,0.4f,1.3f);
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f+sTime/2, 0.0f+sTime/2); 
			glVertex3f(-1, -1, 0);
			glTexCoord2f(0.0f+sTime/2, 1.0f+sTime/2); 
			glVertex3f(-1, 1, 0);
			glTexCoord2f(1.0f+sTime/2, 1.0f+sTime/2); 
			glVertex3f(1, 1, 0); 
			glTexCoord2f(1.0f+sTime/2, 0.0f+sTime/2); 
			glVertex3f(1, -1, 0);
		glEnd();
	glPopMatrix(); 
	}
	glPopMatrix(); 
	glDepthMask(TRUE);
*/
		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
/*
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();
*/

	glPushMatrix();
	float z = -1;
	float width = 1.865f*2;
	float height = 1.4f*2;
	float x = - width  / 2;
	float y = - height / 2;
	center.Use();
	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	unsigned int max_layers = 10;
	if(g_bHighQuality)
		max_layers = 20;

		for(unsigned int i=0;i<max_layers;i++) {
//			glScalef(i+1, i+1, i+1);
			glLoadIdentity();
			glRotatef((360/20)* (float)i, 0,0,1);
			glPushMatrix();
			glRotatef((360/20), 0,1,0);
			glBegin(GL_TRIANGLES);		
			glTexCoord2f(1.0f+sTime/2, 1.0f/*+sTime/2*/); 
			//glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+height, z);
			glTexCoord2f(0.0f+sTime/2, 1.0f/*+sTime/2*/); 
			//glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y+height, z);
			glTexCoord2f(0.0f+sTime/2, 0.0f/*+sTime/2*/); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(0.0f+sTime/2, 0.0f/*+sTime/2*/); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(1.0f+sTime/2, 0.0f/*+sTime/2*/); 
			//glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y, z);
			glTexCoord2f(1.0f+sTime/2, 1.0f/*+sTime/2*/); 
			//glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+height, z);
			width += 0.01f/(i+1);
			height += 0.0075f/(i+1);
			x = -(width  * 0.5f);
			y = -(height * 0.5f);
			glPopMatrix();
			glEnd();
		}
	glPopMatrix();
//	glDepthMask(GL_FALSE);

//	glLoadIdentity();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	model.Draw(sTime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	basetexto12.Sequence(kf_bt12,4,0,scenetime);
	basetexto13.Sequence(kf_bt13,5,0,scenetime);
	basetexto14.Sequence(kf_bt14,5,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap22a.Draw(scenetime);
	cap22b.Draw(scenetime);
	cap23a.Draw(scenetime);
	cap23b.Draw(scenetime);
	cap23c.Draw(scenetime);
	cap24a.Draw(scenetime);
	cap24b.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();


}
