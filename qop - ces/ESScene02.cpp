// scene02

#include "stdafx.h"

extern bool g_bHighQuality;

CCaption cap01;
CCaption cap02;

CTexture tex_locura;

CPicture basetexto;
KeyFrame kf_bt[5];

/* Load */
bool CScene02::Load(char *filename)
{
	scenetime = 0;
	fxswitch = 0;
	b_init = false;
	model.Load("./data/scene02/02.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	basetexto.Create(0,0,-2.f,4.0f,3.0f,"./data/scene02/s02_01.tga", 255, 255, 255);
	kf_bt[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_bt[1].SetKeyFrame(4500,0,0,0, 0,0,0);
	kf_bt[2].SetKeyFrame(4501,0,0,0, 1,1,1);
	kf_bt[3].SetKeyFrame(15000,0,0,0, 1,1,1);
	kf_bt[4].SetKeyFrame(15001,0,0,0, 0,0,0);
	tex_locura.Load("./data/scene02/locura.jpg");
	model.SetFrameRate(70);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(15000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(16000, 0,0,-1, 1,1,1);
	model.SetActiveCamera(0);
//	model.SetCurrentFrame(200);
	cap01.Create(5000, 10000, -0.80f, -0.3f, -1, 0.006f, "once again");
	cap02.Create(7000, 8000, -0.80f, -0.35f, -1, "the certainty");

	fx_radial_blur.SetType(3);
	return true;
}

void CScene02::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
		glClearColor(0,0,0,0);
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch == 1)
		fxswitch = -1;
	fx_lines.SetType(fxswitch);
	model.SetActiveCamera((take%15)%2);
}

/* Play */
void CScene02::Play(float sTime)
{
		glViewport(0,0,render_to_texture_width,render_to_texture_height);

		glPushMatrix();
		float z = -4;
		float width = 1.865f*2;
		float height = 1.4f*2;
		float x = - width  / 2;
		float y = - height / 2;
	//	glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
	//	glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
		tex_locura.Use();
		glColor4f(0.5f, 0.5f, 0.5f, 0);

			for(unsigned int i=0;i<500;i++) {
				glLoadIdentity();
				glScalef(0.9f, 0.9f, 0.9f);
				glRotatef((360.f/100.f)* (float)i, 0,0,1);
				glPushMatrix();
				glRotatef((360/10)+sTime* (float)sin(sTime), 1,1,1);
				glBegin(GL_LINE_STRIP);		
				glTexCoord2f(1.0f+sTime/2, 1.0f); 
				//glTexCoord2f(1.0f, 1.0f); 
				glVertex3f(x+width, y+height, z);
				glTexCoord2f(0.0f+sTime/2, 1.0f); 
				//glTexCoord2f(0.0f, 1.0f); 
				glVertex3f(x, y+height, z);
				glTexCoord2f(0.0f+sTime/2, 0.0f); 
				//glTexCoord2f(0.0f, 0.0f); 
				glVertex3f(x, y, z);
				glTexCoord2f(0.0f+sTime/2, 0.0f+sTime/2); 
				//glTexCoord2f(0.0f, 0.0f); 
				glVertex3f(x, y, z);
				glTexCoord2f(1.0f+sTime/2, 0.0f+sTime/2); 
				//glTexCoord2f(1.0f, 0.0f); 
				glVertex3f(x+width, y, z);
				glTexCoord2f(1.0f+sTime/2, 1.0f+sTime/2); 
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
	if(g_bHighQuality) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		model.Draw(sTime);
		glDisable(GL_CULL_FACE);

	}
	tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

	glViewport(0,0,window_width,window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glPushMatrix();
	z = -1;
	width = 1.865f*2;
	height = 1.4f*2;
	x = - width  / 2;
	y = - height / 2;
	tex_fb.Use();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.9f, 0.9f, 0.9f, 0.9f);
	glEnable(GL_BLEND);
		for(unsigned int i=0;i<10;i++) {
			glLoadIdentity();
			glScalef((float)i+1, (float)i+1, (float)i+1);
			glRotatef((90/20)* (float)i, 0,0,1);
			glPushMatrix();
			glRotatef((180/20), 0,1,0);
			glBegin(GL_TRIANGLES);		
			glTexCoord2f(1.0f+sTime/2, 1.0f); 
			//glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+height, z);
			glTexCoord2f(0.0f+sTime/2, 1.0f); 
			//glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y+height, z);
			glTexCoord2f(0.0f+sTime/2, 0.0f); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(0.0f+sTime/2, 0.0f+sTime/2); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(1.0f+sTime/2, 0.0f+sTime/2); 
			//glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y, z);
			glTexCoord2f(1.0f+sTime/2, 1.0f+sTime/2); 
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

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	fx_radial_blur.Draw(sTime);
//	glPopAttrib();

//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fx_lines.Draw(sTime);
//	glPopAttrib();
	
	//glViewport(0,0,window_width,window_height);
	// aca va el sequence de basetexto
	basetexto.Sequence(kf_bt,5,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap01.Draw(scenetime);
	cap02.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
