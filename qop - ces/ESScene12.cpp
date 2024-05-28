// scene12

#include "stdafx.h"
#define PLASMASIZE 256

extern CVector3 *p_pixels;
extern bool g_bHighQuality;
GLuint texture2[2];
CTexture fondo;

/*
sometimes it's so bright 
we cannot see our faces
*/
CCaption cap20;
CCaption cap21;

CPicture basetexto9;
KeyFrame kf_bt9[4];
CPicture basetexto10;
KeyFrame kf_bt10[4];
CPicture basetexto11;
KeyFrame kf_bt11[4];

/* Load */
bool CScene12::Load(char *filename)
{
	scenetime = 0;
	fxswitch = 0;
	b_init = false;
	model.Load("./data/scene12/12.3ds");
	fondo.Load("./data/scene12/nebulaFondo.jpg");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);
	model.SetFrameRate(30);
	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(31000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(32000, 0,0,-1, 1,1,1);
	model.SetActiveCamera(0);
	p_pixels = new CVector3[PLASMASIZE*PLASMASIZE];
/*
	for (int i=0; i<PLASMASIZE*PLASMASIZE;i++) 
		p_pixels[i].Set(0,0,0);
*/	
	efectito.SetType(5);

	basetexto9.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt9[0].SetKeyFrame(1000,-2.f,-2.77f,0, 0,0,0);
	kf_bt9[1].SetKeyFrame(3000,0,-2.77f,0, 1,1,1);
	kf_bt9[2].SetKeyFrame(20000,0,-2.77f,0, 1,1,1);
	kf_bt9[3].SetKeyFrame(20500,-2.f,-2.77f,0, 0,0,0);

	basetexto10.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt10[0].SetKeyFrame(1000,-2.f,-2.77f,0, 0,0,0);
	kf_bt10[1].SetKeyFrame(3300,1.2f,-2.77f,0, 1,1,1);
	kf_bt10[2].SetKeyFrame(19500,1.2f,-2.77f,0, 1,1,1);
	kf_bt10[3].SetKeyFrame(20500,-2.f,-2.77f,0, 0,0,0);

	basetexto11.Create(0,0,-3.f,6.0f,4.5f,"./data/scene08/s08_01.tga", 255, 255, 255);
	kf_bt11[0].SetKeyFrame(1000,-2.f,-2.77f,0, 0,0,0);
	kf_bt11[1].SetKeyFrame(3600,2.4f,-2.77f,0, 1,1,1);
	kf_bt11[2].SetKeyFrame(19000,2.4f,-2.77f,0, 1,1,1);
	kf_bt11[3].SetKeyFrame(20500,-2.f,-2.77f,0, 0,0,0);

	cap20.Create(5000, 10000, -0.85f, -0.4f, -1, "sometimes it's so bright");
	cap21.Create(9000, 10000, -0.85f, -0.45f, -1, "we cannot see our faces");

	return true;
}

void CScene12::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%3;
	if(fxswitch == 1)
		fxswitch = 0;
	efectito2.SetType(fxswitch);
	model.SetActiveCamera(take%3);
}

/* Play */
void CScene12::Play(float sTime)
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
	glPushMatrix();
	float z = -1;
	float width = 1.865f*2;
	float height = 1.4f*2;
	float x = - width  / 2;
	float y = - height / 2;
	//tex_fb.Use();
	fondo.Use();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
//	glDepthMask(GL_TRUE);
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
	float realtime = sTime - scenetime;
	glEnable(GL_BLEND);
		for(unsigned int i=0;i<5;i++) {
//			glScalef(i+1, i+1, i+1);
			glLoadIdentity();
			glRotatef((180/5)* (float)i, 0,0,1);
			glPushMatrix();
			glRotatef((180/5), 0,1,0);
			glBegin(GL_TRIANGLES);		
			glTexCoord2f(1.0f, 1.0f+realtime/2); 
			//glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(x+width, y+height, z);
			glTexCoord2f(0.0f, 1.0f+realtime/2); 
			//glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(x, y+height, z);
			glTexCoord2f(0.0f, 0.0f+realtime/4); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(0.0f, 0.0f+realtime/4); 
			//glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(x, y, z);
			glTexCoord2f(1.0f, 0.0f+realtime/2); 
			//glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(x+width, y, z);
			glTexCoord2f(1.0f, 1.0f+realtime/2); 
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

/*
	// plasma is back again!
	for(int pX = 0; pX < PLASMASIZE; pX++) {
		for(int pY = 0; pY < PLASMASIZE; pY++)
		{
			float value = sin(dist(pX + sTime * 40, pY, PLASMASIZE, PLASMASIZE) / 8.0)
							+ sin(dist(pX, pY, PLASMASIZE/2, PLASMASIZE/2) / 8.0)
							+ sin(dist(pX, pY + sTime *40 / 7, 128.0, PLASMASIZE/2) / 7.0)
							+ sin(dist(pX, pY, 128.0, 50.0) / 8.0);
			float color = float(value); // * 32;

			p_pixels[pX*PLASMASIZE+pY].Set(0.5f+color, 0.5f+color/59.f, 0.5f+color/3.f);
		}    
	}
	
	if(!glIsTexture(texture2[0]))
		glGenTextures(1, &texture2[0]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture2[0]);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, PLASMASIZE, PLASMASIZE, GL_RGB, GL_FLOAT, p_pixels);
/*
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
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_BLEND);
//	int i=0;
		for(unsigned int i=0;i<10;i++) {
			glScalef(i+1, i+1, i+1);
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
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	efectito2.Draw(sTime);
	glPopAttrib();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	basetexto9.Sequence(kf_bt9,4,0,scenetime);
	basetexto10.Sequence(kf_bt10,4,0,scenetime);
	basetexto11.Sequence(kf_bt11,4,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap20.Draw(scenetime);
	cap21.Draw(scenetime);
	glPopAttrib();

//	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
//	glPopAttrib();

}
