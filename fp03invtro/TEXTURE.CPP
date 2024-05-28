// texture class

#include "stdafx.h"

CTexture::CTexture()
{
	m_nID=0;
}

CTexture::~CTexture()
{
	Toast();
}


void CTexture::Toast()
{
	if( m_nID )
	{
		m_nID--;
		glDeleteTextures(1, &m_nID);
		m_nID=0;
	}
}

void CTexture::Use()
{
	glBindTexture(GL_TEXTURE_2D, m_nID);
}


void CTexture::LoadBMPResource(int name)
{
	// Find the bitmap in the bitmap resources
	HRSRC hrsrc = FindResource(0, (const char *)name, RT_BITMAP);

	// If you can't find it then return
	if (hrsrc==0)
		return;

	// Load the bitmap
	HGLOBAL resource = LoadResource(0, hrsrc);
	
	// If you can't load it then return
	if (resource==0)
		return;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into a bitmap
	BITMAP *bmp = (BITMAP*)buffer;

	// Get the height and width for future use
	width = bmp->bmWidth;
	height = bmp->bmHeight;

	// Reverse the blue colot bit and the red color bit

	glGenTextures(1, &m_nID);
	
	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, m_nID);

	// Use mipmapping filter

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp->bmWidth, bmp->bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (unsigned char *)buffer+sizeof(BITMAPINFO)-4);
}
