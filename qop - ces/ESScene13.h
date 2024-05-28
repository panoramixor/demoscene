// scene13

#ifndef __CSCENE13_H__
#define __CSCENE13_H__

class CScene13: public CScene
{
public:
	virtual void CScene13::Play(float sTime);
	virtual bool CScene13::Load(char *filename);
	virtual void CScene13::Init(float sTime, unsigned int take);
	~CScene13();
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

#endif // __CSCENE13_H__
