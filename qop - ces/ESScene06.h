// scene06

#ifndef __CSCENE06_H__
#define __CSCENE06_H__

class CScene06: public CScene
{
public:
	virtual void CScene06::Play(float sTime);
	virtual bool CScene06::Load(char *filename);
	virtual void CScene06::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
	CPicture fader;
	float scenetime;
	KeyFrame fader_out[4];
	KeyFrame scroller[4];
//	bool b_init;
	int fxswitch;
	COverlayFX efectito;
	COverlayFX efectito2;
	COverlayFX efectito3;
};

#endif // __CSCENE06_H__
