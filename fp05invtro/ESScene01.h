#ifndef __CSCENE01_H__
#define __CSCENE01_H__

class CScene01: public CScene
{
public:
	virtual void CScene01::Play(float sTime);	
	virtual bool CScene01::Load(char *filename);
	virtual void CScene01::Init(float sTime, unsigned int take);
private:
	float scenetime;
};

#endif // __CSCENE01_H__
