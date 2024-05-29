// scene07

#ifndef __CSCENE07_H__
#define __CSCENE07_H__

class CScene07: public CScene
{
public:
	virtual void CScene07::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene07::PostRender(float sTime);
	virtual bool CScene07::Load(char *filename);
	virtual void CScene07::Init(float sTime, unsigned int take);
private:

	int activecamera;
	// fade in
	CInterpolator fx_fadein;
};

#endif // __CSCENE07_H__
