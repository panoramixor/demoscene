// scene10

#ifndef __CSCENE10_H__
#define __CSCENE10_H__

class CScene10: public CScene
{
public:
	virtual void CScene10::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene10::PostRender(float sTime);
	virtual bool CScene10::Load(char *filename);
	virtual void CScene10::Init(float sTime, unsigned int take);
private:

	int activecamera;
	// fade in
	CInterpolator fx_fadein;
};

#endif // __CSCENE09_H__
