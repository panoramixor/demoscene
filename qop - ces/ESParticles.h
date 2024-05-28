// Particle class

#ifndef __CPARTICLES_H__
#define __CPARTICLES_H__

typedef struct tPARTICLE {
	float posx;
	float posy;
	float posz;
	float velocity;
	float life;
	float color_r;
	float color_g;
	float color_b;
	float color_a;
} PARTICLE;

class CParticles
{
public:
	CParticles();
	~CParticles();
	void Create(unsigned int num_particles, float scale, CTexture *tex);
	void SetBlend(GLenum src, GLenum dst);
	void SetSize(float size);
	void SetEffect(int effect);
	void Play(float sTime);

private:
	PARTICLE *p_particles;
	unsigned int n_particles;
	CTexture *texture;
	GLenum blend_src;
	GLenum blend_dst;
	float pointsize;
};

#endif /*__CPARTICLES_H__*/