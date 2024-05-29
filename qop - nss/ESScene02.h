// scene02

#ifndef __CSCENE02_H__
#define __CSCENE02_H__

class CScene02: public CScene
{
public:
	virtual void CScene02::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene02::PostRender(float sTime);
	virtual bool CScene02::Load(char *filename);
	virtual void CScene02::Init(float sTime, unsigned int take);
private:
// fade in
CInterpolator fx_fadein;

};

#endif // __CSCENE02_H__
