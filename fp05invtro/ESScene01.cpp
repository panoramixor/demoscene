// scene01

#include "stdafx.h"

extern 	CFMod music;


#define TEXT_LINES 16

typedef struct _maskedbmp {
	CDevilTexture textobj;
	CDevilTexture *textmask;
} maskedbmp;

maskedbmp text[TEXT_LINES];

CDevilTexture texture;
CDevilTexture texturecubos;
CDevilTexture logofp;
CDevilTexture textloading;
CCube cubos[100];

/* Load */
void Barra(float status)
{
	glLoadIdentity();
	gluPerspective(130, 1280/1024, 0.1f, 2000.0);
	glTranslatef(0,0,-2);
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,1);
	glBegin(GL_QUADS);		
		glVertex3f(-2.0f, -0.70f,0);		
		glVertex3f(-2.0f, -0.30f,0);		
		glVertex3f( 0.4f*status, -0.30f,0);		
		glVertex3f( 0.4f*status,-0.70f,0);
	glEnd();	
	glEnable(GL_TEXTURE_2D);
	glfwSwapBuffers();
}

bool CScene01::Load(char *filename)
{
	scenetime = 0;
	b_init = false;	
	textloading.Load("./data/loading.bmp");
	textloading.Bind();
	glEnable(GL_TEXTURE_2D);
	
	glLoadIdentity();
	gluPerspective(130, 1280/1024, 0.1f, 2000.0);
	glTranslatef(0,0,-2);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(-2.5f,-2.5f,0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(-2.5f,2.5f,0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(2.5f,2.5f,0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(2.5f,-2.5f,0);
	glEnd();	
	glfwSwapBuffers();

	Barra(0);

	texture.Load("./data/gris.bmp");
	texturecubos.Load("./data/gris.bmp");
	logofp.Load("./data/invtro.bmp");
	for (int i = 0 ; i < (TEXT_LINES - 1) ; i++)
	{
		char pepe[1024];
		Barra(float(i)/float(TEXT_LINES));
		sprintf(pepe, "./data/text%d.bmp", i+1);
		text[i].textobj.Load(pepe);		
		text[i].textmask = text[i].textobj.CreateMask(255, 0, 255);		
	}
	cubos[0].Create(0.8f,30.0f,0.8f);
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
	unsigned char *logoptr = logofp.GetData();

	glLoadIdentity();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	float limitTime = 95;
	if (sTime > (limitTime + 12)) { glfwTerminate(); }
	glLoadIdentity();
	gluPerspective(130, 1280/1024, 0.1f, 2000.0);
	
	if (sTime > limitTime) {
		glTranslatef(0, 0, -(sTime-limitTime)*10);		
	}

	float levels = music.GetVolumeLevels();
	if (levels > 1.0f) { levels = 0.1f; }

	glPushMatrix();	
	glTranslatef(-10,4,-5);
	float k = 0.25f;
 	glDisable(GL_TEXTURE_2D);
	glPointSize(2.0f);
	glDisable(GL_DEPTH_TEST);

	for (int j = 0 ; j <= 16 ; j ++)
	{		
		for (int i = 0 ; i <= 80 ; i ++)
		{			
			float h_mod = float((sinf (powf((float)(i+sTime)/5.0f,2)+powf((float)(j+sTime)/5.0f,2)))*(cosf(sTime*(j*i)/500.f)));
			if (sTime > (limitTime - 5))
			{
				glColor4f(1.0f - h_mod*levels + (sTime - (limitTime - 5))*0.25f, 1.0f - h_mod*levels + (sTime - (limitTime - 5))*0.25f, 1.0f - h_mod*levels + (sTime - (limitTime - 5))*0.25f, 1.0f - h_mod*levels + (sTime - (limitTime - 5))*0.25f);
			} 
			else
			{
	 		   glColor4f(1.0f - h_mod*levels, 1.0f - h_mod*levels, 1.0f - h_mod*levels,1.0f - h_mod*levels);
			}
			glBegin(GL_POINTS);
				glVertex3f(i*k,j*k,0);	  				
			glEnd();
		}
	}
	glPopMatrix();	

	float qwidth=2;
	float qheight=2;
	float qlenght=2;

	float x = 0;
	float y = 0;
	float z = 0;

	glPushMatrix();	
	glTranslatef(80,-6,0);
	glRotatef(30,1,0,0);

	glEnable(GL_TEXTURE_2D);
	texturecubos.Bind();
	glTranslatef(x, y, z);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_BLEND);
	glPushMatrix();
	int renglones = 80;
	float k_sep = 8.0f;

	glTranslatef(-40-50*cosf(sTime/5),levels, -100-(50*sinf(sTime/5)));
	glTranslatef(-100, -10, 0);

	for(int j=0;j<renglones;j++) {		
		int ncubos = 80;
		for(int i=0;i<ncubos;i++) {
			int pix_weight = logoptr[(j*80*4)+i*4];
				glPushMatrix();
					glTranslatef(k_sep*(i*0.5f)*0.5f, 0, k_sep*(j*0.5f)*0.5f);
					glTranslatef(0, sin((k_sep*(i*0.5f))*(k_sep*(j*0.5f)))*(cos(sTime*2)/2), 0);
					// constante de separacion = k_sep*((i/5)*0.5f)
					float h_mod = float((sinf (powf((float)i/5.0f,2)+powf((float)j/5.0f,2)))*(cosf(sTime*(j*i)/500.f))/2);
					
					if(pix_weight) {						
						glScalef(0.9f*2.0f, h_mod*levels*1.2f+0.3f, 0.9f*2.0f);
					} else {
						glScalef(0.9f*2.0f, h_mod*levels+0.01f, 0.9f*2.0f);
					}
										
					float tmp = (float(pix_weight)/655.f) +0.5f;
					if (tmp > 1.0f) { tmp = 1.0f; }
					if (tmp < 0.0f) { tmp = 0.0f; }

					if (sTime > (limitTime + 10))
					{
						// fade out
						tmp += float((sTime - (limitTime + 10))*1.0f);
						if(pix_weight) {
							glColor4f(tmp, tmp, tmp,(0.9f*h_mod*(i/20.f)+0.5f) - tmp);
						} else {
							glColor4f(0.1f + (sTime - (limitTime + 10))*0.25f, 0.1f + (sTime - (limitTime + 10))*0.25f, 0.1f + (sTime - (limitTime + 10))*0.25f,0.9f*h_mod*(i/20.f) - (sTime - (limitTime + 10)));
						}
					}
					else if (sTime < 10)
					{
						// fade in
						tmp = (sTime < 5) ? sTime/((sTime-1)*10.0f) : sTime/10.0f;
						if(pix_weight) {
							glColor4f(tmp, tmp, tmp,(0.9f*h_mod*(i/20.f)+0.5f)*tmp);
						} else {
							glColor4f(0.1f, 0.1f, 0.1f,(0.9f*h_mod*(i/20.f))*tmp);
						}
					}
					else
					{
						if(pix_weight) {
							glColor4f(tmp, tmp, tmp,0.9f*h_mod*(i/20.f)+0.5f);
						} else {
							glColor4f(0.1f, 0.1f, 0.1f,0.9f*h_mod*(i/20.f));
						}
					}
				
					cubos[0].Draw(sTime);
				glPopMatrix();
		}
	}
	glPopMatrix();
	glPopMatrix();

	if (sTime < limitTime) {
		if(levels > 0.9f) {
			glPushMatrix();
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
			glEnable(GL_BLEND);
			glTranslatef(0,0,-1);
			float quadwidth = 5;
			float quadheight = 5;
			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
				glVertex3f(-quadwidth/2,-quadheight/2,0);
				glVertex3f(-quadwidth/2,quadheight/2,0);
				glVertex3f(quadwidth/2,quadheight/2,0);
				glVertex3f(quadwidth/2,-quadheight/2,0);
			glEnd();
			glPopMatrix();
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_DST_COLOR,GL_ZERO);
		glEnable(GL_BLEND);
		glTranslatef(tan(sTime)/5*levels,2.5f+(cos(sTime)*levels),-2);
		float quadwidth = 5;
		float quadheight = 5;
		
		text[int(int(sTime)*((TEXT_LINES - 1)/limitTime))].textmask->Bind();
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-quadwidth/2,-quadheight/2,0);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-quadwidth/2,quadheight/2,0);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(quadwidth/2,quadheight/2,0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(quadwidth/2,-quadheight/2,0);
		glEnd();
		

		if (levels < 0.9f)
		{
			glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_BLEND);

			text[int(int(sTime)*((TEXT_LINES - 1)/limitTime))].textobj.Bind();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); 
				glVertex3f(-quadwidth/2,-quadheight/2,0);
				glTexCoord2f(0.0f, 1.0f); 
				glVertex3f(-quadwidth/2,quadheight/2,0);
				glTexCoord2f(1.0f, 1.0f); 
				glVertex3f(quadwidth/2,quadheight/2,0);
				glTexCoord2f(1.0f, 0.0f); 
				glVertex3f(quadwidth/2,-quadheight/2,0);
			glEnd();
		}
		glDepthMask(GL_TRUE);
		glPopMatrix();
	}
}
