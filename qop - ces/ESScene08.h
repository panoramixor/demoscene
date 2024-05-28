// scene08

#ifndef __CSCENE08_H__
#define __CSCENE08_H__

class CScene08: public CScene
{
public:
	virtual void CScene08::Play(float sTime);
	virtual bool CScene08::Load(char *filename);
	virtual void CScene08::Init(float sTime, unsigned int take);
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

};

#endif // __CSCENE08_H__
