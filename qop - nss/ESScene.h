// scene base class

#ifndef __CSCENE_H__
#define __CSCENE_H__

class CScene
{
public:
	CScene();
	~CScene();

	virtual void Play(float sTime, CRenderToTexture *framebuffer) = 0;	// pure virtual
	virtual void PostRender(float sTime) = 0;
	virtual bool Load(char *filename) = 0;	// pure virtual
	virtual void Init(float sTime, unsigned int take) = 0;
	bool b_init;
	CDevilTexture tex_fb;
	CModel3DS model;
	float scenetime;
};

#endif /*__CSCENE_H__*/