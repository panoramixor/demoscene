// scene14

#ifndef __CSCENE14_H__
#define __CSCENE14_H__

class CScene14: public CScene
{
public:
	virtual void CScene14::Play(float sTime);
	virtual bool CScene14::Load(char *filename);
	virtual void CScene14::Init(float sTime, unsigned int take);
	~CScene14();
private:
	CModel3DS model;
	CModel3DS model_box;
	CPicture fader;
	float scenetime;
	int fxswitch;
	KeyFrame fader_out[4];
	KeyFrame fader_in[2];
	COverlayFX efectito;
	COverlayFX efectito2;
};

#endif // __CSCENE14_H__
