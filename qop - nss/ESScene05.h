// scene05

#ifndef __CSCENE05_H__
#define __CSCENE05_H__

class CScene05: public CScene
{
public:
	virtual void CScene05::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene05::PostRender(float sTime);
	virtual bool CScene05::Load(char *filename);
	virtual void CScene05::Init(float sTime, unsigned int take);
private:

	// fade in
	CInterpolator fx_fadein;
	unsigned int activecamera;

};

#endif // __CSCENE05_H__
