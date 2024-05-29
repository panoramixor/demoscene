#ifndef __CCUBE_H__
#define __CCUBE_H__

class CCube
{
public:
	CCube();
	~CCube();
	void Create(float width, float height, float lenght);
	void Draw(float sTime);
private:
	unsigned int lid;
	quad index_array[6];
	CVector3 coord_array[9];
};

#endif /*__CCUBE_H__*/