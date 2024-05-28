// scene10

#ifndef __CSCENE10_H__
#define __CSCENE10_H__

class CScene10: public CScene
{
public:
	virtual void CScene10::Play(float sTime);
	virtual bool CScene10::Load(char *filename);
	virtual void CScene10::Init(float sTime, unsigned int take);
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

};

#endif // __CSCENE10_H__
