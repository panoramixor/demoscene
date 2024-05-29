// scene06

#ifndef __CSCENE06_H__
#define __CSCENE06_H__

class CScene06: public CScene
{
public:
	virtual void CScene06::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene06::PostRender(float sTime);
	virtual bool CScene06::Load(char *filename);
	virtual void CScene06::Init(float sTime, unsigned int take);
private:

	int activecamera;
	// fade in
	CInterpolator fx_fadein;
};

#endif // __CSCENE06_H__
