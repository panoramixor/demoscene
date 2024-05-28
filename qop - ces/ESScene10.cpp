// scene10

#include "stdafx.h"
#define TIME_OFFSET 4000
/*
and the sand goes down
secrets remain untold
*/
CCaption cap25a;
CCaption cap25b;
CCaption cap26a;
CCaption cap26b;

extern bool g_bHighQuality;

CPicture basetexto15a;
KeyFrame kf_bt15a[4];
CPicture basetexto15b;
KeyFrame kf_bt15b[4];
CPicture basetexto15c;
KeyFrame kf_bt15c[4];
CPicture basetexto15d;
KeyFrame kf_bt15d[4];
CPicture basetexto15e;
KeyFrame kf_bt15e[4];
CPicture basetexto15f;
KeyFrame kf_bt15f[4];
CPicture basetexto15g;
KeyFrame kf_bt15g[4];
CPicture basetexto15h;
KeyFrame kf_bt15h[4];
CPicture basetexto15i;
KeyFrame kf_bt15i[4];
CPicture basetexto15j;
KeyFrame kf_bt15j[4];
CPicture basetexto15k;
KeyFrame kf_bt15k[4];
CPicture basetexto15l;
KeyFrame kf_bt15l[4];
CPicture basetexto15m;
KeyFrame kf_bt15m[4];

CPicture basetexto16a;
KeyFrame kf_bt16a[4];
CPicture basetexto16b;
KeyFrame kf_bt16b[4];
CPicture basetexto16c;
KeyFrame kf_bt16c[4];
CPicture basetexto16d;
KeyFrame kf_bt16d[4];
CPicture basetexto16e;
KeyFrame kf_bt16e[4];
CPicture basetexto16f;
KeyFrame kf_bt16f[4];
CPicture basetexto16g;
KeyFrame kf_bt16g[4];
CPicture basetexto16h;
KeyFrame kf_bt16h[4];
CPicture basetexto16i;
KeyFrame kf_bt16i[4];
CPicture basetexto16j;
KeyFrame kf_bt16j[4];
CPicture basetexto16k;
KeyFrame kf_bt16k[4];
CPicture basetexto16l;
KeyFrame kf_bt16l[4];
CPicture basetexto16m;
KeyFrame kf_bt16m[4];

/*CPicture basetexto17;
KeyFrame kf_bt17[5];*/

CTexture fb10;

/* Load */
bool CScene10::Load(char *filename)
{
	scenetime = 0;
	b_init = false;
	model.Load("./data/scene10/10new.3ds");
	fader.Create(0,0,-5.f, 12.0f,9.0f, "./data/fader.bmp", 0xff, 0x0, 0xff);

	fader_out[0].SetKeyFrame(0,0,0,-1,1,1,1);
	fader_out[1].SetKeyFrame(2000,0,0,-1,0,0,0);
	fader_out[2].SetKeyFrame(31000, 0,0,-1, 0,0,0);
	fader_out[3].SetKeyFrame(32000, 0,0,-1, 1,1,1);

	center.Load("./data/scene10/crap.jpg");

	efectito.SetType(3);

	model.SetActiveCamera(0);
	model.SetFrameRate(120);

	basetexto15a.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15a[0].SetKeyFrame(TIME_OFFSET + 1000,0.f,3.8f,-20, 0,0,0);
	kf_bt15a[1].SetKeyFrame(TIME_OFFSET + 1500,0.f,3.8f,-10, 1,1,1);
	kf_bt15a[2].SetKeyFrame(TIME_OFFSET + 10900,0.f,3.8f,-10, 1,1,1);
	kf_bt15a[3].SetKeyFrame(TIME_OFFSET + 11200,0.f,3.8f,7, 0,0,0);

	basetexto15b.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15b[0].SetKeyFrame(TIME_OFFSET + 1300,1.3f,1.5f,7, 0,0,0);
	kf_bt15b[1].SetKeyFrame(TIME_OFFSET + 1700,1.3f,1.5f,-10, 1,1,1);
	kf_bt15b[2].SetKeyFrame(TIME_OFFSET + 11000,1.3f,1.5f,-10, 1,1,1);
	kf_bt15b[3].SetKeyFrame(TIME_OFFSET + 11500,1.3f,1.5f,-20, 0,0,0);

	basetexto15c.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15c[0].SetKeyFrame(TIME_OFFSET + 500,4.3f,1.9f,-15, 0,0,0);
	kf_bt15c[1].SetKeyFrame(TIME_OFFSET + 1400,4.3f,1.9f,-10, 1,1,1);
	kf_bt15c[2].SetKeyFrame(TIME_OFFSET + 10700,4.3f,1.9f,-10, 1,1,1);
	kf_bt15c[3].SetKeyFrame(TIME_OFFSET + 11300,4.3f,1.9f,7, 0,0,0);

	basetexto15d.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15d[0].SetKeyFrame(TIME_OFFSET + 1200,1.7f,2.7f,7, 0,0,0);
	kf_bt15d[1].SetKeyFrame(TIME_OFFSET + 1600,1.7f,2.7f,-8, 1,1,1);
	kf_bt15d[2].SetKeyFrame(TIME_OFFSET + 11000,1.7f,2.7f,-8, 1,1,1);
	kf_bt15d[3].SetKeyFrame(TIME_OFFSET + 11400,1.7f,2.7f,-12, 0,0,0);

	basetexto15e.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15e[0].SetKeyFrame(TIME_OFFSET + 1000,4.7f,3.5f,-15, 0,0,0);
	kf_bt15e[1].SetKeyFrame(TIME_OFFSET + 1500,4.7f,3.5f,-10, 1,1,1);
	kf_bt15e[2].SetKeyFrame(TIME_OFFSET + 11000,4.7f,3.5f,-10, 1,1,1);
	kf_bt15e[3].SetKeyFrame(TIME_OFFSET + 11600,4.7f,3.5f,8, 0,0,0);

	basetexto15f.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15f[0].SetKeyFrame(TIME_OFFSET + 800,1.8f,3.1f,-15, 0,0,0);
	kf_bt15f[1].SetKeyFrame(TIME_OFFSET + 1200,1.8f,3.1f,-12, 1,1,1);
	kf_bt15f[2].SetKeyFrame(TIME_OFFSET + 11100,1.8f,3.1f,-12, 1,1,1);
	kf_bt15f[3].SetKeyFrame(TIME_OFFSET + 11700,1.8f,3.1f,-20, 0,0,0);

	basetexto15g.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15g[0].SetKeyFrame(TIME_OFFSET + 300,3.5f,3.4f,7, 0,0,0);
	kf_bt15g[1].SetKeyFrame(TIME_OFFSET + 900,3.5f,3.4f,-12, 1,1,1);
	kf_bt15g[2].SetKeyFrame(TIME_OFFSET + 11400,3.5f,3.4f,-12, 1,1,1);
	kf_bt15g[3].SetKeyFrame(TIME_OFFSET + 11900,3.5f,3.4f,7, 0,0,0);

	basetexto15h.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15h[0].SetKeyFrame(TIME_OFFSET + 1000,5.7f,3.2f,-15, 0,0,0);
	kf_bt15h[1].SetKeyFrame(TIME_OFFSET + 1600,5.7f,3.2f,-10, 1,1,1);
	kf_bt15h[2].SetKeyFrame(TIME_OFFSET + 11000,5.7f,3.2f,-10, 1,1,1);
	kf_bt15h[3].SetKeyFrame(TIME_OFFSET + 11500,5.7f,3.2f,4, 0,0,0);

	basetexto15i.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15i[0].SetKeyFrame(TIME_OFFSET + 1400,6.4f,1.2f,-15, 0,0,0);
	kf_bt15i[1].SetKeyFrame(TIME_OFFSET + 1900,6.4f,1.2f,-10, 1,1,1);
	kf_bt15i[2].SetKeyFrame(TIME_OFFSET + 11100,6.4f,1.2f,-10, 1,1,1);
	kf_bt15i[3].SetKeyFrame(TIME_OFFSET + 11700,6.4f,1.2f,-20, 0,0,0);

	basetexto15j.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15j[0].SetKeyFrame(TIME_OFFSET + 1700,4.5f,1.4f,7, 0,0,0);
	kf_bt15j[1].SetKeyFrame(TIME_OFFSET + 2000,4.5f,1.4f,-12, 1,1,1);
	kf_bt15j[2].SetKeyFrame(TIME_OFFSET + 11500,4.5f,1.4f,-12, 1,1,1);
	kf_bt15j[3].SetKeyFrame(TIME_OFFSET + 11900,4.5f,1.4f,8, 0,0,0);

	basetexto15k.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15k[0].SetKeyFrame(TIME_OFFSET + 800,5.7f,3.7f,7, 0,0,0);
	kf_bt15k[1].SetKeyFrame(TIME_OFFSET + 1200,5.7f,3.7f,-12, 1,1,1);
	kf_bt15k[2].SetKeyFrame(TIME_OFFSET + 11600,5.7f,3.7f,-12, 1,1,1);
	kf_bt15k[3].SetKeyFrame(TIME_OFFSET + 11800,5.7f,3.7f,-20, 0,0,0);

	basetexto15l.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15l[0].SetKeyFrame(TIME_OFFSET + 1200,-0.4f,3.1f,7, 0,0,0);
	kf_bt15l[1].SetKeyFrame(TIME_OFFSET + 1700,-0.4f,3.1f,-10, 1,1,1);
	kf_bt15l[2].SetKeyFrame(TIME_OFFSET + 11400,-0.4f,3.1f,-10, 1,1,1);
	kf_bt15l[3].SetKeyFrame(TIME_OFFSET + 11900,-0.4f,3.1f,8, 0,0,0);

	basetexto15m.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt15m[0].SetKeyFrame(TIME_OFFSET + 1800,6.8f,2.6f,7, 0,0,0);
	kf_bt15m[1].SetKeyFrame(TIME_OFFSET + 2000,6.8f,2.6f,-10, 1,1,1);
	kf_bt15m[2].SetKeyFrame(TIME_OFFSET + 11000,6.8f,2.6f,-10, 1,1,1);
	kf_bt15m[3].SetKeyFrame(TIME_OFFSET + 11600,6.8f,2.6f,-20, 0,0,0);

	//base texto cap26

	basetexto16a.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16a[0].SetKeyFrame(TIME_OFFSET + 6000,-5.f,-2.8f,-20, 0,0,0);
	kf_bt16a[1].SetKeyFrame(TIME_OFFSET + 7000,-5.f,-2.8f,-10, 1,1,1);
	kf_bt16a[2].SetKeyFrame(TIME_OFFSET + 14800,-5.f,-2.8f,-10, 1,1,1);
	kf_bt16a[3].SetKeyFrame(TIME_OFFSET + 15200,-5.f,-2.8f,7, 0,0,0);

	basetexto16b.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16b[0].SetKeyFrame(TIME_OFFSET + 5500,-9.4f,-1.8f,7, 0,0,0);
	kf_bt16b[1].SetKeyFrame(TIME_OFFSET + 6500,-9.4f,-1.8f,-10, 1,1,1);
	kf_bt16b[2].SetKeyFrame(TIME_OFFSET + 15700,-9.4f,-1.8f,-10, 1,1,1);
	kf_bt16b[3].SetKeyFrame(TIME_OFFSET + 16000,-9.4f,-1.8f,-20, 0,0,0);

	basetexto16c.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16c[0].SetKeyFrame(TIME_OFFSET + 6500,-9.0f,-3.2f,7, 0,0,0);
	kf_bt16c[1].SetKeyFrame(TIME_OFFSET + 7000,-9.0f,-3.2f,-10, 1,1,1);
	kf_bt16c[2].SetKeyFrame(TIME_OFFSET + 15100,-9.0f,-3.2f,-10, 1,1,1);
	kf_bt16c[3].SetKeyFrame(TIME_OFFSET + 15600,-9.0f,-3.2f,-20, 0,0,0);

	basetexto16d.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16d[0].SetKeyFrame(TIME_OFFSET + 7000,-7.0f,-3.8f,-15, 0,0,0);
	kf_bt16d[1].SetKeyFrame(TIME_OFFSET + 7400,-7.0f,-3.8f,-10, 1,1,1);
	kf_bt16d[2].SetKeyFrame(TIME_OFFSET + 14900,-7.0f,-3.8f,-10, 1,1,1);
	kf_bt16d[3].SetKeyFrame(TIME_OFFSET + 15400,-7.0f,-3.8f,-20, 0,0,0);

	basetexto16e.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16e[0].SetKeyFrame(TIME_OFFSET + 6700,-6.0f,-2.2f,-15, 0,0,0);
	kf_bt16e[1].SetKeyFrame(TIME_OFFSET + 7000,-6.0f,-2.2f,-10, 1,1,1);
	kf_bt16e[2].SetKeyFrame(TIME_OFFSET + 15800,-6.0f,-2.2f,-10, 1,1,1);
	kf_bt16e[3].SetKeyFrame(TIME_OFFSET + 16000,-6.0f,-2.2f,-20, 0,0,0);

	basetexto16f.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16f[0].SetKeyFrame(TIME_OFFSET + 6200,-8.0f,-2.8f,8, 0,0,0);
	kf_bt16f[1].SetKeyFrame(TIME_OFFSET + 6700,-8.0f,-2.8f,-10, 1,1,1);
	kf_bt16f[2].SetKeyFrame(TIME_OFFSET + 15200,-8.0f,-2.8f,-10, 1,1,1);
	kf_bt16f[3].SetKeyFrame(TIME_OFFSET + 15600,-8.0f,-2.8f,8, 0,0,0);

	basetexto16g.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16g[0].SetKeyFrame(TIME_OFFSET + 6700,-6.4f,-2.7f,8, 0,0,0);
	kf_bt16g[1].SetKeyFrame(TIME_OFFSET + 7100,-6.4f,-2.7f,-10, 1,1,1);
	kf_bt16g[2].SetKeyFrame(TIME_OFFSET + 15300,-6.4f,-2.7f,-10, 1,1,1);
	kf_bt16g[3].SetKeyFrame(TIME_OFFSET + 15900,-6.4f,-2.7f,-20, 0,0,0);

	basetexto16h.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16h[0].SetKeyFrame(TIME_OFFSET + 6200,-4.2f,-3.1f,8, 0,0,0);
	kf_bt16h[1].SetKeyFrame(TIME_OFFSET + 6700,-4.2f,-3.1f,-10, 1,1,1);
	kf_bt16h[2].SetKeyFrame(TIME_OFFSET + 14800,-4.2f,-3.1f,-10, 1,1,1);
	kf_bt16h[3].SetKeyFrame(TIME_OFFSET + 15200,-4.2f,-3.1f,8, 0,0,0);

	basetexto16i.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16i[0].SetKeyFrame(TIME_OFFSET + 6200,-7.6f,-1.9f,8, 0,0,0);
	kf_bt16i[1].SetKeyFrame(TIME_OFFSET + 6700,-7.6f,-1.9f,-10, 1,1,1);
	kf_bt16i[2].SetKeyFrame(TIME_OFFSET + 15200,-7.6f,-1.9f,-10, 1,1,1);
	kf_bt16i[3].SetKeyFrame(TIME_OFFSET + 15700,-7.6f,-1.9f,8, 0,0,0);

	basetexto16j.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16j[0].SetKeyFrame(TIME_OFFSET + 6500,-6.6f,-1.4f,8, 0,0,0);
	kf_bt16j[1].SetKeyFrame(TIME_OFFSET + 7000,-6.6f,-1.4f,-10, 1,1,1);
	kf_bt16j[2].SetKeyFrame(TIME_OFFSET + 15500,-6.6f,-1.4f,-10, 1,1,1);
	kf_bt16j[3].SetKeyFrame(TIME_OFFSET + 16000,-6.6f,-1.4f,8, 0,0,0);

	basetexto16k.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16k[0].SetKeyFrame(TIME_OFFSET + 6500,-5.1f,-2.4f,8, 0,0,0);
	kf_bt16k[1].SetKeyFrame(TIME_OFFSET + 7000,-5.1f,-2.4f,-10, 1,1,1);
	kf_bt16k[2].SetKeyFrame(TIME_OFFSET + 15100,-5.1f,-2.4f,-10, 1,1,1);
	kf_bt16k[3].SetKeyFrame(TIME_OFFSET + 15600,-5.1f,-2.4f,8, 0,0,0);

	basetexto16l.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16l[0].SetKeyFrame(TIME_OFFSET + 6500,-7.6f,-1.7f,8, 0,0,0);
	kf_bt16l[1].SetKeyFrame(TIME_OFFSET + 7000,-7.6f,-1.7f,-10, 1,1,1);
	kf_bt16l[2].SetKeyFrame(TIME_OFFSET + 14700,-7.6f,-1.7f,-10, 1,1,1);
	kf_bt16l[3].SetKeyFrame(TIME_OFFSET + 15000,-7.6f,-1.7f,8, 0,0,0);

	basetexto16m.Create(0,0,-2.f,4.0f,3.0f,"./data/scene10/s10_04.tga", 255, 255, 255);
	kf_bt16m[0].SetKeyFrame(TIME_OFFSET + 6500,-8.2f,-1.7f,8, 0,0,0);
	kf_bt16m[1].SetKeyFrame(TIME_OFFSET + 7000,-8.2f,-1.7f,-10, 1,1,1);
	kf_bt16m[2].SetKeyFrame(TIME_OFFSET + 15700,-8.2f,-1.7f,-10, 1,1,1);
	kf_bt16m[3].SetKeyFrame(TIME_OFFSET + 16000,-8.2f,-1.7f,8, 0,0,0);

	/*basetexto17.Create(0,0,-3.f,6.0f,4.5f,"./data/scene10/s10_02.tga", 255, 255, 255);
	kf_bt17[0].SetKeyFrame(0,-2.f,0,0, 0,0,0);
	kf_bt17[1].SetKeyFrame(6500,-2.f,0,0, 0,0,0);
	kf_bt17[2].SetKeyFrame(8000,0,0,0, 1,1,1);
	kf_bt17[3].SetKeyFrame(12000,0,0,0, 1,1,1);
	kf_bt17[4].SetKeyFrame(13000,4.f,0,0, 0,0,0);*/

	cap25a.Create(TIME_OFFSET + 3000, 7500, -0.07f, 0.20f, -1, "and the sand");
	cap25b.Create(TIME_OFFSET + 3500, 7500, 0.17f, 0.15f, -1, 0.0025f, "goes down");
	cap26a.Create(TIME_OFFSET + 8000, 6000, -0.81f, -0.2f, -1, 0.0025f, "secrets");
	cap26b.Create(TIME_OFFSET + 8500, 6500, -0.81f, -0.25f, -1, "remain untold");

	return true;
}

void CScene10::Init(float sTime, unsigned int take)
{
	if(!b_init) {
		scenetime = sTime;
	}
	b_init = true;
	fxswitch = take%2;
//	model.SetActiveCamera((take%15)%2);
}

/* Play */
void CScene10::Play(float sTime)
{
	if(g_bHighQuality) {
	glViewport(0,0,render_to_texture_width,render_to_texture_height);
	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glDepthMask(FALSE);
	glRotatef(90, 1.0,0,0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glRotatef(sTime*50, 0, (float)sin(sTime*10)+10,0);
	center.Use();
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	for(int i=0;i<=14;i++) {
		glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
		glRotatef(360/14, 0,1,0);
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 0.0f+sTime*2); 
			glVertex3f(-20, -250, 0);
			glTexCoord2f(0.0f, 10.0f+sTime*2); 
			glVertex3f(-20, 0, 0);
			glTexCoord2f(1.0f, 10.0f+sTime*2); 
			glVertex3f(20, 0, 0); 
			glTexCoord2f(1.0f, 0.0f+sTime*2); 
			glVertex3f(20, -250, 0);
		glEnd();
	}
	glPopMatrix(); 
	glDepthMask(TRUE);

	tex_fb.LoadFrameBuffer(render_to_texture_width, render_to_texture_height);

	glViewport(0,0,window_width,window_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	glEnable(GL_CULL_FACE);
	model.Draw(sTime);
	glDisable(GL_CULL_FACE);

	glPushMatrix();
	glDepthMask(FALSE);
	glRotatef(90, 1.0,0,0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glRotatef(sTime*50, 0, (float)sin(sTime*10)+10,0);
	center.Use();
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	for(int i=0;i<=14;i++) {
		glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
		glRotatef(360/14, 0,1,0);
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 0.0f+sTime*2); 
			glVertex3f(-20, -250, 0);
			glTexCoord2f(0.0f, 10.0f+sTime*2); 
			glVertex3f(-20, 0, 0);
			glTexCoord2f(1.0f, 10.0f+sTime*2); 
			glVertex3f(20, 0, 0); 
			glTexCoord2f(1.0f, 0.0f+sTime*2); 
			glVertex3f(20, -250, 0);
		glEnd();
	}
	glPopMatrix(); 
	glDepthMask(TRUE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	tex_fb.Use();
	efectito.Draw(sTime);
	glPopAttrib();

	basetexto15a.Sequence(kf_bt15a,4,0,scenetime);
	basetexto15b.Sequence(kf_bt15b,4,0,scenetime);
	basetexto15c.Sequence(kf_bt15c,4,0,scenetime);
	basetexto15d.Sequence(kf_bt15d,4,0,scenetime);
	basetexto15e.Sequence(kf_bt15e,4,0,scenetime);
	basetexto15f.Sequence(kf_bt15f,4,0,scenetime);
	basetexto15g.Sequence(kf_bt15g,4,0,scenetime);
	basetexto15h.Sequence(kf_bt15h,4,0,scenetime);
	basetexto15i.Sequence(kf_bt15i,4,0,scenetime);
	basetexto15j.Sequence(kf_bt15j,4,0,scenetime);
	basetexto15k.Sequence(kf_bt15k,4,0,scenetime);
	basetexto15l.Sequence(kf_bt15l,4,0,scenetime);
	basetexto15m.Sequence(kf_bt15m,4,0,scenetime);

	basetexto16a.Sequence(kf_bt16a,4,0,scenetime);
	basetexto16b.Sequence(kf_bt16b,4,0,scenetime);
	basetexto16c.Sequence(kf_bt16c,4,0,scenetime);
	basetexto16d.Sequence(kf_bt16d,4,0,scenetime);
	basetexto16e.Sequence(kf_bt16e,4,0,scenetime);
	basetexto16f.Sequence(kf_bt16f,4,0,scenetime);
	basetexto16g.Sequence(kf_bt16g,4,0,scenetime);
	basetexto16h.Sequence(kf_bt16h,4,0,scenetime);
	basetexto16i.Sequence(kf_bt16i,4,0,scenetime);
	basetexto16j.Sequence(kf_bt16j,4,0,scenetime);
	basetexto16k.Sequence(kf_bt16k,4,0,scenetime);
	basetexto16l.Sequence(kf_bt16l,4,0,scenetime);
	basetexto16m.Sequence(kf_bt16m,4,0,scenetime);

	//basetexto17.Sequence(kf_bt17,5,0,scenetime);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	cap25a.Draw(scenetime);
	cap25b.Draw(scenetime);
	cap26a.Draw(scenetime);
	cap26b.Draw(scenetime);
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();

/*
	glEnable(GL_CULL_FACE);
	model.Draw(sTime);

	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glDepthMask(FALSE);
	glRotatef(90, 1.0,0,0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glRotatef(sTime*50, 0,sin(sTime*10)+10,0);
	center.Use();
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	for(int i=0;i<=14;i++) {
		glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
		glRotatef(360/14, 0,1,0);
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 0.0f+sTime*2); 
			glVertex3f(-20, -250, 0);
			glTexCoord2f(0.0f, 10.0f+sTime*2); 
			glVertex3f(-20, 250, 0);
			glTexCoord2f(1.0f, 10.0f+sTime*2); 
			glVertex3f(20, 250, 0); 
			glTexCoord2f(1.0f, 0.0f+sTime*2); 
			glVertex3f(20, -250, 0);
		glEnd();
	}
	glPopMatrix(); 
	glDepthMask(TRUE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	fader.Sequence(fader_out, 4, 0, scenetime);
	glPopAttrib();
*/
}
