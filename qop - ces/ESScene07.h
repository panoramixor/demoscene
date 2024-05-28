// scene07

#ifndef __CSCENE07_H__
#define __CSCENE07_H__

class CScene07: public CScene
{
public:
	virtual void CScene07::Play(float sTime);
	virtual bool CScene07::Load(char *filename);
	virtual void CScene07::Init(float sTime, unsigned int take);
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
