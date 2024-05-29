// simple texture class

#ifndef __Texture_h_
#define __Texture_h_

#define MAX_TEXTURE_NAME_LENGTH 64

//#include <gl\glaux.h>		// Header File for the Glaux Library
#include <vfw.h>			// Header File for AVI playback
#include "ESVector2d.h"


// These are the new defines used for Multi-texturing.
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

// The new functions for multi-texturing.
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);

class CTexture
{
public:
	CTexture();
	~CTexture();

public:
	AVISTREAMINFO	 pSi;
	PAVISTREAM		 pAvi;
	PGETFRAME		 pGf;
	BITMAPINFOHEADER bmih;
	HDRAWDIB		 hDd;
	HBITMAP			 hBitmap;
	HDC				 hDc;

	unsigned char* data;
	unsigned int ID;

	float currentFrame;
	char* pData;
		
	int	lastFrame;
	VECTOR2D_INT center;
	
	char *texturename;								// The textures name
	unsigned int texture[1];						// OpenGL's number for the texture
	int width;										// Texture's width
	int height;										// Texture's height
	void Use();										// Binds the texture for use
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);	// Sometimes we want a texture of uniform color
	void LoadTGA(char *name);						// Loads a targa file
	void LoadBMP(char *name);						// Loads a bitmap file
	void LoadFrameBuffer();							// Loads a raw capture from memory
	void LoadJPGClamped(char *name);				// Loads a jpg file and maps it clamped
	void LoadJPG(char *name);
	void LoadSkyboxTex(char *name);					// Loads a bitmap using GL_CLAMP
	void RenderToTexture();
	void UseRenderToTexture();
	void LoadEnvMapJPG(char *name);
	void Load(char *name);							// Load the texture
	void MultiUse(CTexture g_texture1,  CTexture g_texture2);
	void LoadJPGAndFilter(char *name, GLint minfilter, GLint magfilter);
};

#endif // __Texture_h_
