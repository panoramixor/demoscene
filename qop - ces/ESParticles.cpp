// Particle class

#include "stdafx.h"

/* Constructor */
CParticles::CParticles()
{
	p_particles = 0;
	blend_src = GL_DST_ALPHA;
	blend_dst = GL_ONE_MINUS_SRC_ALPHA;
//	pointsize = 0.5;

}

void CParticles::Create(unsigned int num_particles, float scale, CTexture *tex)
{
	LogLine("ESParticles->Creating: %i particles, with %s\n", num_particles, tex->texturename);
	n_particles = num_particles;
	srand(n_particles);
	texture = tex;
	p_particles = new PARTICLE[n_particles];
	for(unsigned int i=0;i<n_particles;i++) {
		p_particles[i].posx = (((float)(rand()%100)/50.0f)-1.0f)*(i*scale);
		p_particles[i].posy = (((float)(rand()%100)/50.0f)-1.0f)*(i*scale);
		p_particles[i].posz = (((float)(rand()%100)/50.0f)-1.0f)*(i*scale);
		p_particles[i].life = 100;
		p_particles[i].velocity = 0;
	}

    float maxSize = 0.0f;
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
	pointsize = maxSize;
}

void CParticles::SetBlend(GLenum src, GLenum dst)
{
	blend_src = src;
	blend_dst = dst;
}

void CParticles::SetSize(float size)
{
	pointsize = size;
}

void CParticles::Play(float sTime) 
{
	glDepthMask(FALSE);
	
	// This is how will our point sprite's size will be modified by 
    // distance from the viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

    float maxSize = 0.0f;
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );

    glPointSize(pointsize);

    // The alpha of a point is calculated to allow the fading of points 
    // instead of shrinking them past a defined threshold size. The threshold 
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
    // the minimum and maximum point sizes.
    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 30.0f );

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 0.1f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
	//glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, pointsize);

	texture->Use();
    // Specify point sprite texture coordinate replacement mode for each texture unit
    glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
	glEnable(GL_TEXTURE_2D);

	glEnable( GL_BLEND );
	glBlendFunc( blend_src, blend_dst);

	glEnable( GL_POINT_SPRITE_ARB );
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1,1,1,1);
//	glTranslatef(0,0,sTime*10);
	glRotatef(sTime*10, 1,1,1);
	for(unsigned int i=0;i<n_particles;i++) {
		p_particles[i].velocity = (float)sin((float)i);
		p_particles[i].life = (float)sin(p_particles[i].velocity)*100;
		glBegin(GL_POINTS);
			glVertex3f(p_particles[i].posx+((float)sin(sTime*2+i)*p_particles[i].velocity), p_particles[i].posy+((float)sin(sTime*2+i)*p_particles[i].velocity), p_particles[i].posz+((float)sin(sTime*2+i)*p_particles[i].velocity));
		glEnd();
	}
	glDisable( GL_POINT_SPRITE_ARB );
	glDepthMask(TRUE);
}

CParticles::~CParticles()
{

}

