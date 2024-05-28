// scene02

#ifndef __CSCENE02_H__
#define __CSCENE02_H__

class CScene02: public CScene
{
public:
	virtual void CScene02::Play(float sTime);
	virtual bool CScene02::Load(char *filename);
	virtual void CScene02::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
	CPicture fader;
	float scenetime;
	int fxswitch;
	KeyFrame fader_out[4];
	KeyFrame fader_in[2];
	COverlayFX fx_lines;
	COverlayFX fx_radial_blur;
};

#endif // __CSCENE02_H__
