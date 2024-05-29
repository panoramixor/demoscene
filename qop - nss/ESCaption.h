// caption class
 
#ifndef __CCAPTION_H__
#define __CCAPTION_H__

class CCaption
{
public:
	CCaption();
	~CCaption();
	void Print(char *string);
	void SetPos(float x, float y, float z);
private:
	unsigned int len;
	CVector2 size;
	CVector3 spacing;
	CVector3 position;
	CVector3 rotation;
	CVector3 color;
	CFont *cadena;
};

#endif /*__CCAPTION_H__*/