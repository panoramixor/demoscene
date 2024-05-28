// Title scene

#include "stdafx.h"

extern bool g_bHighQuality;

CPicture base;
KeyFrame kf_base[5];
COverlayFX efectito2;

/* Load */
bool CScene01::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene01/01.3ds");
	model.SetFrameRate(90);
	
	qoplogo.Create(0,0,-4.f, 10.0f,7.5f , "./data/scene01/s01_logo.tga", 0xff, 0x0, 0xff);
	isbackto.Create(0,0.4f,-3.f, 4.0f,1.0f, "./data/scene01/s01_isback.tga", 0xff, 0x0, 0xff);
	title.Create(0,-1,-4.f, 10.0f ,7.5f , "./data/scene01/s01_title.tga", 0xff, 0x0, 0xff);
	base.Create(0,-0.2f,-4.f,10.0f,7.5f , "./data/scene01/s01_soporte.tga", 0xff, 0x0, 0xff);
	
	kf_base[0].SetKeyFrame(0,0,0,0, 0,0,0);
	kf_base[1].SetKeyFrame(5000,0,0,0, 0,0,0);
	kf_base[2].SetKeyFrame(6000,0,0,0, 0.5f,0.5f,0.5f);
	kf_base[3].SetKeyFrame(10000,0,0,0, 0.5f,0.5f,0.5f);
	kf_base[4].SetKeyFrame(11000,0,0,0, 0,0,0);

	fader.Create(0,0,-5.f, 12.0f,9.0f , "./data/fader.bmp", 0xff, 0x0, 0xff);

	fadein[0].SetKeyFrame(5000,0,0,-1,0,0,0);
	fadein[1].SetKeyFrame(6000,0,0,-1,1,1,1);
	fadein[2].SetKeyFrame(9000,0,0,-1,1,1,1);
	fadein[3].SetKeyFrame(12000,0,0,0,0,0,0);

	fadein2[0].SetKeyFrame(7000,0,0,-1,0,0,0);
	fadein2[1].SetKeyFrame(8000,0,0,-1,1,1,1);
	fadein2[2].SetKeyFrame(9000,0,0,-1,1,1,1);
	fadein2[3].SetKeyFrame(10000,0,0,-1,0,0,0);

	fadein3[0].SetKeyFrame(9000,0,1,0,0,0,0);
	fadein3[1].SetKeyFrame(11000,0,1,-1,1,1,1);
	fadein3[2].SetKeyFrame(12000,0,1,-1,1,1,1);
	fadein3[3].SetKeyFrame(17900,0,1,-1,1,1,1);
	fadein3[4].SetKeyFrame(19900,0,1,0,0,0,0);

	fader_out[0].SetKeyFrame(4000,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(9000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(18900,0,0,-1,0,0,0);
	fader_out[3].SetKeyFrame(19900,0,0,-1,1,1,1);
	model.SetActiveCamera(0);

	efectito.SetType(0);
	efectito2.SetType(4);
	return true;
}

void CScene01::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
}

/* Play */
void CScene01::Play(float sTime)
{
	
	if(g_bHighQuality) {
		glViewport(0,0, render_to_texture_width, render_to_texture_height);

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		qoplogo.Sequence(fadein, 4, 0, scenetime);
		isbackto.Sequence(fadein2, 4, 0, scenetime);
		title.Sequence(fadein3, 5, 0, scenetime);
		glPopAttrib();	
		glPopMatrix();

		tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

		glViewport(0,0,window_width,window_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

//	glLoadIdentity();

	float width = 1.865f ;
	float height = 1.4f ;
	float x = -(width*0.5f);
	float y = -(width*0.5f);
	float z = -1;

	float width_sobre10 = width/20.f;
	float height_sobre10 = height/20.f;

	float current_x = x;
	float current_y = y;

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glLoadIdentity();
	glTranslatef(-1, 0,0);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glBegin(GL_QUADS);
	for(int j=0;j<40;j++) {
		for(int i=0;i<40;i++) {
			glColor4f(j*i/1600.f, j*i/1600.f, j*i/1600.f, j*i/1600.f);
			glVertex3f(current_x+ (float)(sin((i+1)*sTime))/20, current_y , z- (float)(i* (float)sin(sTime)/40));
			glVertex3f(current_x+ (float)(sin((i+1)*sTime))/20, current_y+height_sobre10, z- (float)(i*sin(sTime)/40));
			glVertex3f(current_x+width_sobre10+((float)sin((i+1)*sTime))/20, current_y+height_sobre10, z-(i* (float)sin(sTime)/40));
			glVertex3f(current_x+width_sobre10+((float)sin((i+1)*sTime))/20, current_y , z-(i* (float)sin(sTime)/40));
			current_y += height_sobre10*1.1f;
		}
		current_x += width_sobre10*1.1f;
		current_y = y;
	}
	glEnd();

	base.Sequence(kf_base,5,0,scenetime);
	qoplogo.Sequence(fadein, 4, 0, scenetime);
	
	isbackto.Sequence(fadein2, 4, 0, scenetime);

	title.Sequence(fadein3, 5, 0, scenetime);


	efectito.Draw(sTime);

	tex_fb.Use();	
	efectito2.Draw(sTime);

	fader.Sequence(fader_out, 5, 0, scenetime);

}
