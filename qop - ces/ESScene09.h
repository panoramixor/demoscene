// scene09

#ifndef __CSCENE09_H__
#define __CSCENE09_H__

class CScene09: public CScene
{
public:
	virtual void CScene09::Play(float sTime);
	virtual bool CScene09::Load(char *filename);
	virtual void CScene09::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
	CPicture fader;
	float scenetime;
	CTexture center;
	KeyFrame fader_out[4];
	KeyFrame scroller[4];
//	bool b_init;
	int fxswitch;
	COverlayFX efectito;
	COverlayFX efectito2;
};

#endif // __CSCENE09_H__
