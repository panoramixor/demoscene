// scene08

#ifndef __CSCENE08_H__
#define __CSCENE08_H__

class CScene08: public CScene
{
public:
	virtual void CScene08::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene08::PostRender(float sTime);
	virtual bool CScene08::Load(char *filename);
	virtual void CScene08::Init(float sTime, unsigned int take);
private:

	int activecamera;
	// fade in
	CInterpolator fx_fadein;
};

#endif // __CSCENE08_H__
