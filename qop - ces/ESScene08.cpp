// scene08

#include "stdafx.h"
/*
sometimes it's so bright 
we cannot see our faces
*/

//CCaption cap20;
//CCaption cap21;

extern bool g_bHighQuality;
/*
CPicture basetexto9;
KeyFrame kf_bt9[4];
CPicture basetexto10;
KeyFrame kf_bt10[4];
CPicture basetexto11;
KeyFrame kf_bt11[4];
*/

CTexture scene08texture;

/* Load */
bool CScene08::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene08/08new.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	scene08texture.Load("./data/scene08/04.jpg");

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);
/*	
	basetexto9.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt9[0].SetKeyFrame(0,-2.f,-2.77f,0, 0,0,0);
	kf_bt9[1].SetKeyFrame(2000,0,-2.77f,0, 1,1,1);
	kf_bt9[2].SetKeyFrame(14000,0,-2.77f,0, 1,1,1);
	kf_bt9[3].SetKeyFrame(14500,-2.f,-2.77f,0, 0,0,0);

	basetexto10.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt10[0].SetKeyFrame(0,-2.f,-2.77f,0, 0,0,0);
	kf_bt10[1].SetKeyFrame(2300,1.2f,-2.77f,0, 1,1,1);
	kf_bt10[2].SetKeyFrame(13500,1.2f,-2.77f,0, 1,1,1);
	kf_bt10[3].SetKeyFrame(14500,-2.f,-2.77f,0, 0,0,0);

	basetexto11.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt11[0].SetKeyFrame(0,-2.f,-2.77f,0, 0,0,0);
	kf_bt11[1].SetKeyFrame(2600,2.4f,-2.77f,0, 1,1,1);
	kf_bt11[2].SetKeyFrame(13000,2.4f,-2.77f,0, 1,1,1);
	kf_bt11[3].SetKeyFrame(14500,-2.f,-2.77f,0, 0,0,0);
*/
	model.SetActiveCamera(0);
	model.SetFrameRate(250);

	efectito.SetType(3);

//	cap20.Create(3000, 10000, -0.85f, -0.4f, -1, "sometimes it's so bright");
//	cap21.Create(7000, 6000, -0.85f, -0.45f, -1, "we cannot see our faces");

	return true;
}

void CScene08::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
//	fxswitch = take%4;
	fxswitch = take%2;
	if(fxswitch == 1)
		fxswitch = -1;
	efectito2.SetType(fxswitch);
}

/* Play */
void CScene08::Play(float sTime)
{
//	if(g_bHighQuality) {
		glViewport(0,0,render_to_texture_width,render_to_texture_height);
		model.Draw(sTime);
	//	glFlush();
		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
	glPushMatrix();
	float z = -1;
	float width = 1.865f*2;
	float height = 1.4f*2;
	float x = - width  / 2;
	float y = - height / 2;
	tex_fb.Use();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
//	glDepthMask(GL_TRUE);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
	glEnable(GL_BLEND);
		for(unsigned int i=0;i<10;i++) {
//			glScalef(i+1, i+1, i+1);
			glLoadIdentity();
			glRotatef((180/10)*(float)i, 0,0,1);
			glPushMatrix();
			glRotatef((180/10), 0,1,0);
			glBegin(GL_TRIANGLES);		
			glTexCoord2f(1.0f, 1.0f+sTime/2); 
			//glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+height, z);
			glTexCoord2f(0.0f, 1.0f+sTime/2); 
			//glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y+height, z);
			glTexCoord2f(0.0f, 0.0f+sTime/2); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(0.0f, 0.0f+sTime/2); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(1.0f, 0.0f+sTime/2); 
			//glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y, z);
			glTexCoord2f(1.0f, 1.0f+sTime/2); 
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

	glEnable(GL_DEPTH_TEST);
	model.Draw(sTime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();
/*
	basetexto9.Sequence(kf_bt9,4,0,scenetime);
	basetexto10.Sequence(kf_bt10,4,0,scenetime);
	basetexto11.Sequence(kf_bt11,4,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap20.Draw(scenetime);
	cap21.Draw(scenetime);
	glPopAttrib();
*/
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
