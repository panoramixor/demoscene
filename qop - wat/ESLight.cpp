// Light class

#include "stdafx.h"

CLight::CLight()
{
	m_light = -1;
}

/* Disable all the lights */
void CLight::DisableAllLights()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);
	glDisable(GL_LIGHT6);
	glDisable(GL_LIGHT7);
}

/* Set up the light properties and enable the light */
void CLight::SetUpLight(unsigned int light, VECTOR ambient, VECTOR diffuse, VECTOR specular, VECTOR position)
{
	m_light = light;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_position = position;
	
	glEnable(GL_LIGHTING);
	glLightfv(m_light, GL_AMBIENT, (float*)&m_ambient.c);
	glLightfv(m_light, GL_DIFFUSE, (float*)&m_diffuse.c);
	glLightfv(m_light, GL_SPECULAR, (float*)&m_specular.c);
	glLightfv(m_light, GL_POSITION, (float*)&m_position.c);
	EnableLight();
}

/* Disable the light */
void CLight::DisableLight()
{
	if(m_light!=-1)
		glDisable(m_light);
}

/* Enable the light (to be used only if it has been disabled before */
void CLight::EnableLight()
{
	if(m_light!=-1)
		glEnable(m_light);
}

/* Destructor */
CLight::~CLight()
{
}
