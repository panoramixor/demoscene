// scene04

#ifndef __CSCENE04_H__
#define __CSCENE04_H__

class CScene04: public CScene
{
public:
	virtual void CScene04::Play(float sTime);
	virtual bool CScene04::Load(char *filename);
	virtual void CScene04::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
	CPicture fader;
	float scenetime;
	KeyFrame fader_out[4];
	KeyFrame scroller[4];
//	bool b_init;
	int fxswitch;
	COverlayFX efectito;

};

#endif // __CSCENE04_H__
