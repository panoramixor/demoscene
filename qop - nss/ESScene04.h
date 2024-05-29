// scene04

#ifndef __CSCENE04_H__
#define __CSCENE04_H__

class CScene04: public CScene
{
public:
	virtual void CScene04::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene04::PostRender(float sTime);
	virtual bool CScene04::Load(char *filename);
	virtual void CScene04::Init(float sTime, unsigned int take);
private:
	unsigned int activecamera;
	CInterpolator txtnotanim;
	CInterpolator txtsoanim;
	CInterpolator txtfondoanim;
	CInterpolator txtletrasanim;

	CCaption title_not;
	CCaption title_so;
	CCaption title_soon;

	// fade in
	CInterpolator fx_fadein;

	CDevilTexture qoplogo;

	CDevilTexture alphatest;
	CDevilTexture *alphamask;

};

#endif // __CSCENE04_H__
