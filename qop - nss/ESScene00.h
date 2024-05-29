// scene00

#ifndef __CSCENE00_H__
#define __CSCENE00_H__

class CScene00: public CScene
{
public:
	CScene00();
	~CScene00();
	virtual void CScene00::Play(float sTime, CRenderToTexture *framebuffer);
	virtual void CScene00::PostRender(float sTime);
	virtual bool CScene00::Load(char *filename);
	virtual void CScene00::Init(float sTime, unsigned int take);
private:
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
/*
CDevilTexture alphatest;
CDevilTexture *alphamask;
*/
};

#endif // __CSCENE00_H__
