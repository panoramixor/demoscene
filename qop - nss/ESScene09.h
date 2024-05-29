// scene09

#ifndef __CSCENE09_H__
#define __CSCENE09_H__

class CScene09: public CScene
{
public:
	virtual void CScene09::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene09::PostRender(float sTime);
	virtual bool CScene09::Load(char *filename);
	virtual void CScene09::Init(float sTime, unsigned int take);
private:

	int activecamera;
	// fade in
	CInterpolator fx_fadein;
};

#endif // __CSCENE09_H__
