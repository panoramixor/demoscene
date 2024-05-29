// scene03

#ifndef __CSCENE03_H__
#define __CSCENE03_H__

class CScene03: public CScene
{
public:
	virtual void CScene03::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene03::PostRender(float sTime);
	virtual bool CScene03::Load(char *filename);
	virtual void CScene03::Init(float sTime, unsigned int take);
private:
// fade in
CInterpolator fx_fadein;

};

#endif // __CSCENE03_H__
