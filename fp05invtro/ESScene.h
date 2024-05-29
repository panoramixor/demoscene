
#ifndef __CSCENE_H__
#define __CSCENE_H__

class CScene
{
public:
	CScene();
	~CScene();

	virtual void Play(float sTime) = 0;	// pure virtual
	virtual bool Load(char *filename) = 0;	// pure virtual
	virtual void Init(float sTime, unsigned int take) = 0;
	bool b_init;
	CTexture tex_fb;
};

#endif /*__CSCENE_H__*/