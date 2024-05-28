// Title scene

#ifndef __CSCENE01_H__
#define __CSCENE01_H__

class CScene01: public CScene
{
public:
	virtual void CScene01::Play(float sTime);
	virtual bool CScene01::Load(char *filename);
	virtual void CScene01::Init(float sTime, unsigned int take);
private:
	CModel3DS model;
//	CTexture title;
	CPicture title;
	CPicture isbackto;
	CPicture qoplogo;
	CPicture fader;
	float scenetime;
	KeyFrame fadein[4];
	KeyFrame fadein2[4];
	KeyFrame fadein3[5];
	KeyFrame fader_out[4];
	KeyFrame fader_in[4];
	COverlayFX efectito;
};

#endif // __CSCENE01_H__
