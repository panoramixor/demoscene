// scene13

#include "stdafx.h"

CParticles particles;
CTexture tex_particle;
GLuint texture[2];
CVector3 *p_pixels;

CPicture base_credits;
CPicture txt_credits;
CPicture logos_credits;

extern bool g_bHighQuality;

/* Load */
bool CScene13::Load(char *filename)
{
	scenetime = 0;
	fxswitch = 0;
	b_init = false;
	model.Load("./data/scene13/13.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	model.SetFrameRate(300);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(20000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(21000, 0,0,-1, 1,1,1);
	model.SetActiveCamera(0);
	if(GLEW_ARB_point_sprite) {
		tex_particle.Load("./data/scene13/particle.tga");
		particles.Create(10000, 0.01f, &tex_particle);
		particles.SetSize(100.f);
	}
	p_pixels = new CVector3[64*64];
	efectito.SetType(3);
	model.SetRenderMode(GL_LINE_STRIP);

	base_credits.Create(0,0,-3.f,6.0f,4.5f,"./data/scene13/credits_base.tga", 255, 255, 255);
	txt_credits.Create(0,0,-3.f,6.0f,4.5f,"./data/scene13/credits_txt.tga", 255, 255, 255);
	logos_credits.Create(0,0,-3.f,6.0f,4.5f,"./data/scene13/credits_logos.tga", 255, 255, 255);

	return true;
}

void CScene13::Init(float sTime, unsigned int take)
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
	efectito2.SetType(0);
	model.SetActiveCamera(take%2);
}

CScene13::~CScene13() {
	delete [] p_pixels;
}

/* Play */
void CScene13::Play(float sTime)
{
	if(g_bHighQuality) {
	glViewport(0,0,render_to_texture_width,render_to_texture_height);

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

	glViewport(0,0,window_width,window_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
/*
	// plasma is back!
	for(int x = 0; x < 64; x++) {
		for(int y = 0; y < 64; y++)
		{
			float value = sin(dist(x + sTime * 40, y, 64, 64) / 8.0)
							+ sin(dist(x, y, 32.0, 32.0) / 8.0)
							+ sin(dist(x, y + sTime *40 / 7, 128.0, 32) / 7.0)
							+ sin(dist(x, y, 128.0, 50.0) / 8.0);
			float color = float(value); // * 32;

			p_pixels[x*64+y].Set(0.5f+color/3.f, 0.5f+color/59.f, 0.5f+color);
		}    
	}
	if(!glIsTexture(texture[0]))
		glGenTextures(1, &texture[0]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 64, 64, GL_RGB, GL_FLOAT, p_pixels);

	glPushMatrix();
	float z = -1;
	float width = 1.865f*2;
	float height = 1.4f*2;
	float x = - width  / 2;
	float y = - height / 2;
//	tex_fb.Use();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
//	glDepthMask(GL_TRUE);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.2f, 0.5f, 0.2f, 0.2f);
	glEnable(GL_BLEND);
		for(unsigned int i=0;i<10;i++) {
//			glScalef(i+1, i+1, i+1);
			glLoadIdentity();
			glRotatef(((70+sin(sTime))/10)*i, 0,0,1);
			glPushMatrix();
			glRotatef(((30*sin(sTime))/10), 0,1,0);
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
	*/
/*
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito.Draw(sTime);
	glPopAttrib();
*/
	glEnable(GL_DEPTH_TEST);

//	glDrawPixels(64, 64, GL_RGB, GL_FLOAT, p_pixels);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	base_credits.Draw();
	glPopAttrib();

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);
	if(GLEW_ARB_point_sprite) {
		particles.Play(sTime);
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
    logos_credits.Draw();
	glPopAttrib();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	tex_fb.Use();
	efectito2.Draw(sTime);
	glPopAttrib();

	txt_credits.Draw();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

}
