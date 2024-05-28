// scene03

#ifndef __CSCENE03_H__
#define __CSCENE03_H__

class CScene03: public CScene
{
public:
	virtual void CScene03::Play(float sTime);
	virtual bool CScene03::Load(char *filename);
	virtual void CScene03::Init(float sTime, unsigned int take);
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
	CPicture photo;

};

#endif // __CSCENE03_H__
