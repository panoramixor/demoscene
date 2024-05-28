//-------------------------------------------------------------------
//	File:		Texture.h
//	Created:	05/26/99 2:30:AM
//	Author:		Aaron Hilton
//	Comments:	Create and manage OpenGL textures.
//-------------------------------------------------------------------
#ifndef __Texture_h_
#define __Texture_h_

#define MAX_TEXTURE_NAME_LENGTH 64

class CTexture
{
public:
	CTexture();
	~CTexture();

	// Create and load the files.
	void CTexture::LoadBMPResource(int name);
	void Toast();

	void Use();

protected:

	char m_szName[MAX_TEXTURE_NAME_LENGTH];
	unsigned int m_nID;
	int width;
	int height;
	// Status information.
//	UInt32 m_nWidth, m_nHeight;
};

#endif // __Texture_h_
//-------------------------------------------------------------------
//	History:
//	$Log:$
//-------------------------------------------------------------------
