
#include "stdafx.h"

extern float sTime;

/* Constructor */
COverlayFX::COverlayFX()
{
	b_enabled = true;
	dlist = 0;
	fxtype = -1;
}

/* Disable all the FX */
void COverlayFX::DisableAllFX()
{

}

/* Disable */
void COverlayFX::Disable()
{
	b_enabled = false;
}

void COverlayFX::SetType(int type)
{
	fxtype = type;
}

/* Enable */
void COverlayFX::Enable()
{
	b_enabled = true;
}

void COverlayFX::Draw(float sTime)
{

	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
//	glEnable(GL_CULL_FACE); 
	float step;

	float x = 0;
	float y = 0;
	float width = 2;
	float height = 2;
	x = x - width  / 2;
	y = y - height / 2;
	float z = -1;

	if(b_enabled) {
		switch(fxtype) {
			case 0:
				current_framebuffer.LoadFrameBuffer(2, 512);
//				glReadPixels((window_width/2)-1, 0, 1, window_height, GL_RGB, GL_FLOAT, p_pixels);
				glLoadIdentity();
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
		//		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
				glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
				glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
				glLoadIdentity();
				glBegin(GL_QUADS);
					step = 0.0458f;
					z = -1;
					width = 1.865f;
					height = 1.4f;
					x = - width  / 2;
					y = - height / 2;
					glTexCoord2f(0.0f, 0.0f); 
					glVertex3f(x, y, z);
					glTexCoord2f(0.0f, 1.0f); 
					glVertex3f(x, y + height, z);
					glTexCoord2f(1.0f, 1.0f); 
					glVertex3f(x + width, y + height, z); 
					glTexCoord2f(1.0f, 0.0f); 
					glVertex3f(x + width, y, z);
				glEnd();

/*
				glBegin(GL_LINES);
					step = 0.0018f;
					for(unsigned int i=0;i<window_height;i++) {
						glColor4f(p_pixels[i*3], p_pixels[(i*3)+1], p_pixels[(i*3)+2], 0.3f);
						glVertex3f(-1,0.7f-(i*step),-1.f);
						glVertex3f(1,0.7f-(i*step),-1.f);

						glVertex3f(-1,-0.7f+(i*step),-1.f);
						glVertex3f(1,-0.7f+(i*step),-1.f);
					}
				glEnd();
				
*/
				break;

			case 1:
				glLoadIdentity();
//				glBlendEquation(GL_MAX);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
				glColor4f(0,0,0,0);
				glBegin(GL_QUADS);		
					glVertex3f(x, y, z);
					glVertex3f(x, y + height, z);
					glVertex3f(x + width, y + height, z); 
					glVertex3f(x + width, y, z);
				glEnd();
//				glBlendEquation(GL_FUNC_ADD);
				break;

			case 2:
				current_framebuffer.LoadFrameBuffer(2, 512);
//				glReadPixels((window_width/2)-1, 0, 1, window_height, GL_RGB, GL_FLOAT, p_pixels);
				glLoadIdentity();
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
/*
				glBegin(GL_LINES);
					step = 0.0018f;
					for(unsigned int i=0;i<window_height;i++) {
						glColor4f(p_pixels[i*3]/2, p_pixels[(i*3)+1]/2, p_pixels[(i*3)+2]/2, 1.f);
						glVertex3f(-1,0.7f-(i*step),-1.f);
						glVertex3f(1,0.7f-(i*step),-1.f);

						glVertex3f(-1,-0.7f+(i*step),-1.f);
						glVertex3f(1,-0.7f+(i*step),-1.f);

					}
				glEnd();
				*/
				glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
				glBegin(GL_TRIANGLES);
					step = 0.0458f;
					z = -1;
					width = 1.865f;
					height = 1.4f;
					x = - width  / 2;
					y = - height / 2;

					glTexCoord2f(1.0f, 1.0f); 
					glVertex3f(x+width, y+height, z);
					glTexCoord2f(0.0f, 1.0f); 
					glVertex3f(x, y+height, z);
					glTexCoord2f(0.0f, 0.0f); 
					glVertex3f(x, y, z);
					glTexCoord2f(0.0f, 0.0f); 
					glVertex3f(x, y, z);
					glTexCoord2f(1.0f, 0.0f); 
					glVertex3f(x+width, y, z);
					glTexCoord2f(1.0f, 1.0f); 
					glVertex3f(x+width, y+height, z);
				/*
					glTexCoord2f(0.0f, 0.0f); 
					glVertex3f(x, y, z);
					glTexCoord2f(0.0f, 1.0f); 
					glVertex3f(x, y + height, z);
					glTexCoord2f(1.0f, 1.0f); 
					glVertex3f(x + width, y + height, z); 
					glTexCoord2f(1.0f, 0.0f); 
					glVertex3f(x + width, y, z);
				*/
				glEnd();

				break;

			case 3:

				if(!dlist) {

					dlist = glGenLists(1);
					glNewList(dlist, GL_COMPILE);	
					glDepthMask(GL_FALSE);
					glColor4f(0.1f,0.1f,0.1f,0.1f);
//					glColor4f(1,1,1,1);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_LIGHTING);
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glPushMatrix();
					glLoadIdentity();

					glBegin(GL_TRIANGLES);
						step = 0.0158f;
						z = -1;
						width = 1.55f;
						height = 1.1475f;
						x = - width  * 0.5f;
						y = - height * 0.5f;

						for(unsigned int i=0;i<10;i++) {
							glTexCoord2f(1.0f, 1.0f); 
							glVertex3f(x+width, y+height, z);
							glTexCoord2f(0.0f, 1.0f); 
							glVertex3f(x, y+height, z);
							glTexCoord2f(0.0f, 0.0f); 
							glVertex3f(x, y, z);
							glTexCoord2f(0.0f, 0.0f); 
							glVertex3f(x, y, z);
							glTexCoord2f(1.0f, 0.0f); 
							glVertex3f(x+width, y, z);
							glTexCoord2f(1.0f, 1.0f); 
							glVertex3f(x+width, y+height, z);
							width += 0.001f;
							height += 0.00075f;
							x = -(width  * 0.5f);
							y = -(height * 0.5f);
							z -= step/100;
						}
					glEnd();

					glPopMatrix();
					glDepthMask(GL_TRUE);

					glEndList();
					
				} else {
					glCallList(dlist);	
				}

				break;

			case 4:
				if(!dlist) {
					dlist = glGenLists(1);
					glNewList(dlist, GL_COMPILE);	
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glColor3f(0.5f,0.5f,0.5f);
//					glColor4f(1,1,1,1);
					glPushMatrix();
					glLoadIdentity();
/*
					CVector2 *p_texcoord = new CVector2[4];
					CVector3 *p_vertex = new CVector3[4];

					p_texcoord[0].Set(1.0f, 1.0f);
					p_texcoord[1].Set(0.0f, 1.0f);
					p_texcoord[2].Set(1.0f, 1.0f);
					p_texcoord[3].Set(1.0f, 1.0f);
*/
					glBegin(GL_TRIANGLES);
						step = 0.0458f;
						z = -1;
						width = 1.865f;
						height = 1.4f;
						x = - width  / 2;
						y = - height / 2;
						int i = 0; 

						for(unsigned int i=0;i<5;i++) {
							glTexCoord2f(1.0f, 1.0f); 
							glVertex3f(x+width, y+height, z);
							glTexCoord2f(0.0f, 1.0f); 
							glVertex3f(x, y+height, z);
							glTexCoord2f(0.0f, 0.0f); 
							glVertex3f(x, y, z);
							glTexCoord2f(0.0f, 0.0f); 
							glVertex3f(x, y, z);
							glTexCoord2f(1.0f, 0.0f); 
							glVertex3f(x+width, y, z);
							glTexCoord2f(1.0f, 1.0f); 
							glVertex3f(x+width, y+height, z);
							width += 0.01f;
							height += 0.0075f;
							x = -(width  / 2);
							y = -(height / 2);
						}
					glEnd();
					glPopMatrix();

					glEndList();

				} else {
					glCallList(dlist);	
				}
				break;
		case 5:
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_LIGHTING);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glColor3f(0.1f,0.1f,0.1f);
//				glColor4f(1,1,1,1);
				glPushMatrix();
				glLoadIdentity();
/*
					CVector2 *p_texcoord = new CVector2[4];
					CVector3 *p_vertex = new CVector3[4];

					p_texcoord[0].Set(1.0f, 1.0f);
					p_texcoord[1].Set(0.0f, 1.0f);
					p_texcoord[2].Set(1.0f, 1.0f);
					p_texcoord[3].Set(1.0f, 1.0f);
*/
				glBegin(GL_TRIANGLES);
					step = 0.0458f;
					z = -1;
					width = 1.865f;
					height = 1.4f;
					x = - width  / 2;
					y = - height / 2;
					
					for(unsigned int i=0;i<40;i++) {
						glTexCoord2f(1.0f, 1.0f); 
						glVertex3f(x+width, y+height, z);
						glTexCoord2f(0.0f, 1.0f); 
						glVertex3f(x, y+height, z);
						glTexCoord2f(0.0f, 0.0f); 
						glVertex3f(x, y, z);
						glTexCoord2f(0.0f, 0.0f); 
						glVertex3f(x, y, z);
						glTexCoord2f(1.0f, 0.0f); 
						glVertex3f(x+width, y, z);
						glTexCoord2f(1.0f, 1.0f); 
						glVertex3f(x+width, y+height, z);
						width += 0.01f*3 + fabs(sin(sTime)/40);
						height += 0.0075f*3 + fabs(sin(sTime)/60);
						x = -(width  / 2);
						y = -(height / 2);
					}
				glEnd();
				glPopMatrix();
				//glEndList();
			break;

			default:
				break;
		}
	}
}

/* Destructor */
COverlayFX::~COverlayFX()
{
	if(p_pixels)
		delete [] p_pixels;
}
