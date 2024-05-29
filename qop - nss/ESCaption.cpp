// caption class

#include "stdafx.h"

/* Constructor */
CCaption::CCaption()
{
	size.Set(0.05f, 0.05f);
	len = 0;
	cadena = 0;
}

/* Destructor */
CCaption::~CCaption()
{

	if(cadena)
		delete [] cadena;
}

void CCaption::SetPos(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void CCaption::Print(char *string)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	if(len == 0) {
		len = (unsigned int)strlen((char *)string);
		cadena = new CFont[len];
	}

	for(unsigned int i=0;i<len;i++) {
		cadena[i].Draw(string[i]);
		if((string[i] == 'i') || (string[i] == 'l') || (string[i] == 0x20)) {
			glTranslatef(size.x/2, 0, 0);
		} else
			glTranslatef(size.x, 0, 0);
	}
}