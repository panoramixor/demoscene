// simple 2D effects implementation

#include "stdafx.h"

extern float fDeltaTime;
extern CTexture framebuffer;

GLfloat step = 0;

float r_val, g_val, b_val, a_val, velocity;
bool r_flag, b_flag, g_flag, a_flag, fader_flag, flipper;


// fade to color
bool fade_to_color(GLfloat i_r, GLfloat i_g, GLfloat i_b, GLfloat i_a, GLfloat e_r, GLfloat e_g, GLfloat e_b, GLfloat e_a)
{
	velocity += fDeltaTime/10;
	if (!fader_flag)
	{
		r_val = i_r;
		g_val = i_g;
		b_val = i_b;
		a_val = i_a;
		fader_flag = true;
	}

	// handle red component fading
	
	if ((r_val >= e_r-velocity) && (r_val <= e_r+velocity))
	{
		r_flag = true;
	}
	
	if (r_flag == false)
	{
		if (e_r > r_val)
		{
			r_val += velocity; 
		}

		if (e_r < r_val)
		{
			r_val -= velocity;
		}
	}

	// handle green component fading
	
	if ((g_val > e_g-velocity) && (g_val < e_g+velocity))
	{ 
		g_flag = true;
	} 
	
	if (g_flag == false)
	{ 
		if (e_g > g_val) 
		{
			g_val += velocity; 
		}

		if (e_g < g_val) 
		{
			g_val -= velocity;
		}
	}

	// handle blue component fading

	if ((b_val > e_b-velocity) && (b_val < e_b+velocity))
	{ 
		i_b = e_b; 
		b_flag = true;
	} 
	
	if (b_flag == false)
	{ 
		if (e_b > b_val)
		{
			b_val += velocity; 
		}
		if (e_b < b_val)
		{
			b_val -= velocity;
		}
	}
	
	// handle alpha component fading
	
	if ((a_val >= e_a-velocity) && (a_val <= e_a+velocity))
	{ 
		a_flag = true;
	} 
	
	if (a_flag == false)
	{ 
		if (e_a > a_val)
		{
			a_val += velocity; 
		}
		if (e_a < a_val)
		{
			a_val -= velocity;
		}
	}

	glLoadIdentity();
//	glTranslatef(0,0,-1);
//	glScalef(2,1,1);
	glDisable(GL_TEXTURE_2D);
	glColor4f(r_flag ? e_r : r_val, g_flag ? e_g : g_val, b_flag ? e_b : b_val, a_flag ? e_a : a_val);
		glBegin(GL_TRIANGLES);
			glVertex3f(-4.0f, 4.0f, -1.5);
			glVertex3f(4.0f, 4.0f, -1.5);
			glVertex3f(-4.0f, -4.0f, -1.5);
			glVertex3f(4.0f, -4.0f, -1.5);
			glVertex3f(-4.0f, -4.0f, -1.5);
			glVertex3f(4.0f, 4.0f, -1.5);
		glEnd();
	glEnable(GL_TEXTURE_2D);

	return a_flag;
}


// lineal fade in from white
bool fade_in()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	return fade_to_color(1,1,1,1,1,1,1,0);
}


// lineal fade out to white
bool fade_out()
{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		return fade_to_color(1,1,1,0,1,1,1,1);
}


// fade to black
bool fade_to_black()
{
		glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
//		velocity += 0.001;
		return fade_to_color(0,0,0,0,0,0,0,1);
}


// lineal fade in from black
bool fade_from_black()
{
		glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		return fade_to_color(0,0,0,1,0,0,0,0);
}


// flashing effect
bool flash_me()
{
		if (!flipper)
		{
			velocity+=0.5;
		}

		return fade_to_color(0.2f,0.2f,0.2f,1.0f,0.2f+(GLfloat)cosf(velocity)/20,0.2f+ (GLfloat)sinf(velocity)/20.f,0.2f+ (GLfloat)asin(-velocity)/20,1.0f);
}

void crossfade(CPicture picture, CTexture mask, KeyFrame ColorFadeIn[], KeyFrame ColorFadeOut[], float starttime)
{
	picture.SetMask(mask);
	picture.Sequence(ColorFadeIn, 2, 0, starttime);
}

void postfiler(bool bCapture)
{
		float x,y,z,width,height;

		width = 5.34f;
		height = 4.0f;

		x = -(width/2);
		y = -(height/2);
		z = -2;

//		fb.LoadFrameBuffer();
		if(!bCapture)
			framebuffer.RenderToTexture();
		else {
			framebuffer.UseRenderToTexture();
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
			glEnd();
		}
}
