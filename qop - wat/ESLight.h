// Light class

#ifndef __CLIGHT_H__
#define __CLIGHT_H__

class CLight
{
public:
	CLight();
	~CLight();
	void SetUpLight(unsigned int light, VECTOR ambient, VECTOR diffuse, VECTOR specular, VECTOR position);
	void DisableLight();
	void EnableLight();

private:
	static void DisableAllLights();

	unsigned int m_light;	/* OpenGL name of the light */
	VECTOR m_ambient, m_diffuse, m_specular, m_position;
};

#endif /*__CLIGHT_H__*/