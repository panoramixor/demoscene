// scene12

#ifndef __CSCENE12_H__
#define __CSCENE12_H__

class CScene12: public CScene
{
public:
	virtual void CScene12::Play(float sTime);
	virtual bool CScene12::Load(char *filename);
	virtual void CScene12::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
	CPicture fader;
	float scenetime;
	int fxswitch;
	KeyFrame fader_out[4];
	KeyFrame fader_in[2];
	COverlayFX efectito;
	COverlayFX efectito2;
};

#endif // __CSCENE12_H__
