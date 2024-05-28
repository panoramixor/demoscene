// scene11

#ifndef __CSCENE11_H__
#define __CSCENE11_H__

class CScene11: public CScene
{
public:
	virtual void CScene11::Play(float sTime);
	virtual bool CScene11::Load(char *filename);
	virtual void CScene11::Init(float sTime, unsigned int take);
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

#endif // __CSCENE11_H__
