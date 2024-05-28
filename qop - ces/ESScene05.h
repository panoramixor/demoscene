// scene05

#ifndef __CSCENE05_H__
#define __CSCENE05_H__

class CScene05: public CScene
{
public:
	virtual void CScene05::Play(float sTime);
	virtual bool CScene05::Load(char *filename);
	virtual void CScene05::Init(float sTime, unsigned int take);
private:
	CTexture center;
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
	int fxlist;
};

#endif // __CSCENE05_H__
