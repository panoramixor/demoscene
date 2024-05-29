// font class

#ifndef __CFONT_H__
#define __CFONT_H__

class CFont
{
public:
	CFont();
	~CFont();
	void Draw(char character);
private:
	CVector3 color;
	CVector2 size;
	unsigned int lid;
};

#endif /*__CFONT_H__*/