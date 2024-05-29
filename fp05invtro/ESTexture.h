#ifndef __Texture_h_
#define __Texture_h_

#define MAX_TEXTURE_NAME_LENGTH 64

//  little-endian, of course
#define DDS_MAGIC 0x20534444


//  DDS_header.dwFlags
#define DDSD_CAPS                   0x00000001 
#define DDSD_HEIGHT                 0x00000002 
#define DDSD_WIDTH                  0x00000004 
#define DDSD_PITCH                  0x00000008 
#define DDSD_PIXELFORMAT            0x00001000 
#define DDSD_MIPMAPCOUNT            0x00020000 
#define DDSD_LINEARSIZE             0x00080000 
#define DDSD_DEPTH                  0x00800000 

//  DDS_header.sPixelFormat.dwFlags
#define DDPF_ALPHAPIXELS            0x00000001 
#define DDPF_FOURCC                 0x00000004 
#define DDPF_INDEXED                0x00000020 
#define DDPF_RGB                    0x00000040 

//  DDS_header.sCaps.dwCaps1
#define DDSCAPS_COMPLEX             0x00000008 
#define DDSCAPS_TEXTURE             0x00001000 
#define DDSCAPS_MIPMAP              0x00400000 

//  DDS_header.sCaps.dwCaps2
#define DDSCAPS2_CUBEMAP            0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX  0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX  0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY  0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY  0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ  0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ  0x00008000 
#define DDSCAPS2_VOLUME             0x00200000 

#define D3DFMT_DXT1     '1TXD'    //  DXT1 compression texture format 
#define D3DFMT_DXT2     '2TXD'    //  DXT2 compression texture format 
#define D3DFMT_DXT3     '3TXD'    //  DXT3 compression texture format 
#define D3DFMT_DXT4     '4TXD'    //  DXT4 compression texture format 
#define D3DFMT_DXT5     '5TXD'    //  DXT5 compression texture format 

#define PF_IS_DXT1(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == D3DFMT_DXT1))

#define PF_IS_DXT3(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == D3DFMT_DXT3))

#define PF_IS_DXT5(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == D3DFMT_DXT5))

#define PF_IS_BGRA8(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
   (pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 32) && \
   (pf.dwRBitMask == 0xff0000) && \
   (pf.dwGBitMask == 0xff00) && \
   (pf.dwBBitMask == 0xff) && \
   (pf.dwAlphaBitMask == 0xff000000U))

#define PF_IS_BGR8(pf) \
  ((pf.dwFlags & DDPF_ALPHAPIXELS) && \
  !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 24) && \
   (pf.dwRBitMask == 0xff0000) && \
   (pf.dwGBitMask == 0xff00) && \
   (pf.dwBBitMask == 0xff))

#define PF_IS_BGR5A1(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
   (pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 16) && \
   (pf.dwRBitMask == 0x00007c00) && \
   (pf.dwGBitMask == 0x000003e0) && \
   (pf.dwBBitMask == 0x0000001f) && \
   (pf.dwAlphaBitMask == 0x00008000))

#define PF_IS_BGR565(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
  !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 16) && \
   (pf.dwRBitMask == 0x0000f800) && \
   (pf.dwGBitMask == 0x000007e0) && \
   (pf.dwBBitMask == 0x0000001f))

#define PF_IS_INDEX8(pf) \
  ((pf.dwFlags & DDPF_INDEXED) && \
   (pf.dwRGBBitCount == 8))


union DDS_header {
  struct {
    unsigned int    dwMagic;
    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwHeight;
    unsigned int    dwWidth;
    unsigned int    dwPitchOrLinearSize;
    unsigned int    dwDepth;
    unsigned int    dwMipMapCount;
    unsigned int    dwReserved1[ 11 ];

    //  DDPIXELFORMAT
    struct {
      unsigned int    dwSize;
      unsigned int    dwFlags;
      unsigned int    dwFourCC;
      unsigned int    dwRGBBitCount;
      unsigned int    dwRBitMask;
      unsigned int    dwGBitMask;
      unsigned int    dwBBitMask;
      unsigned int    dwAlphaBitMask;
    }               sPixelFormat;

    //  DDCAPS2
    struct {
      unsigned int    dwCaps1;
      unsigned int    dwCaps2;
      unsigned int    dwDDSX;
      unsigned int    dwReserved;
    }               sCaps;
    unsigned int    dwReserved2;
  };
  char data[ 128 ];
};


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
	bool envmap;
	bool b_init;

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
		
//	int	width;
//	int	height;
	int	lastFrame;
	VECTOR2D_INT center;
	
	char *texturename;								// The textures name
	unsigned int texture[1];						// OpenGL's number for the texture
	int width;										// Texture's width
	int height;										// Texture's height

	bool ismask;									// texmask flag

	bool SetMask(bool value);
	bool GetMask();

	void Use();										// Binds the texture for use
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);	// Sometimes we want a texture of uniform color
	void LoadTGAResource(char *name);				// Load a targa from the resources
	void LoadBMPResource(char *name);				// Load a bitmap from the resources
	void LoadFromResource(char *name);				// Load the texture from a resource
	void LoadTGA(char *name);						// Loads a targa file
	void LoadBMP(char *name);						// Loads a bitmap file
//	void LoadFrameBuffer();							// Loads a raw capture from memory
	void LoadFrameBuffer(int width, int height);
	void LoadBMP(void *buffer);						// Loads a bmp stored in memory
	void LoadBMPKeyColor(char *name, unsigned char r, unsigned char g, unsigned char b); // takes color key transparency as parameter
	void LoadJPGClamped(char *name);				// Loads a jpg file and maps it clamped
	void LoadJPG(char *name);
	void LoadRAW(char *filename);
	void LoadSkyboxTex(char *name);					// Loads a bitmap using GL_CLAMP
	void RenderToTexture();
	void UseRenderToTexture();
	void LoadEnvMapJPG(char *name);
	void CTexture::LoadEnvMap(char *name);
	void Load(char *name);							// Load the texture
	void MultiUse(CTexture g_texture1,  CTexture g_texture2);
	void LoadJPGAndFilter(char *name, GLint minfilter, GLint magfilter);
//	void LoadDDS(char *name);
	void WriteTGA(char *name, unsigned char *pixels, int type, int width, int height) ;
};

#endif // __Texture_h_
//-------------------------------------------------------------------
//	History:
//	$Log:$
//-------------------------------------------------------------------
