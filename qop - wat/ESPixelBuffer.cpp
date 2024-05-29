// Pixel Puffer (pbuffers) class 

#include "stdafx.h"

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;

PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

bool isExtensionSupported(const GLubyte *extensions, const char *extName)
{
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extName, ' ');
  if (where || *extName == '\0')
    return 0;
  
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *) start, extName);
    if (!where)
      break;
    terminator = where + strlen(extName);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return true;
    start = terminator;
  }
  return false;
}

bool InitPBuffers(void)
{

  wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
  if (!wglGetExtensionsStringARB)
    return false;
  
  const char *wglExt = wglGetExtensionsStringARB(wglGetCurrentDC());
  
  if (!isExtensionSupported((const GLubyte*)wglExt, "WGL_ARB_pbuffer") ||
      !isExtensionSupported((const GLubyte*)wglExt, "WGL_ARB_pixel_format"))
    return false;
  
  wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribivARB");
  wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribfvARB");
  wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
  
  wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC) wglGetProcAddress("wglCreatePbufferARB");
  wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC) wglGetProcAddress("wglGetPbufferDCARB");
  wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC) wglGetProcAddress("wglReleasePbufferDCARB");
  wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC) wglGetProcAddress("wglDestroyPbufferARB");
  wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC) wglGetProcAddress("wglQueryPbufferARB");
  
  if (wglGetPixelFormatAttribivARB &&
    wglGetPixelFormatAttribfvARB &&
    wglChoosePixelFormatARB &&
    wglCreatePbufferARB &&
    wglGetPbufferDCARB &&
    wglReleasePbufferDCARB &&
    wglDestroyPbufferARB &&
    wglQueryPbufferARB)
    return true;
  else
    return false;
}

CPixelBuffer::CPixelBuffer()
{
  width = 0;
  height = 0;
  m_cBits = 0;
  m_dBits = 0;
  m_share = false;

  m_pbuf = 0;
  m_RC = 0;
  m_DC = 0;
  m_isValid = false;

  m_oldDC = 0;
  m_oldRC = 0;
}

void CPixelBuffer::Init(int w, int h, int colorBits, int depthBits, bool share)
{
  int iAttributes[MAX_ATTRIBS*2] = {WGL_DRAW_TO_PBUFFER_ARB, true,
                                    WGL_COLOR_BITS_ARB, colorBits,
                                    WGL_DEPTH_BITS_ARB, depthBits, 
                                    0};

  float fAttributes[MAX_ATTRIBS*2] = {0};
  int pFormats[MAX_PFORMATS];
  int format;
  unsigned int nFormats = 0;
  HDC hDC;

  // Save pbuffer settings
  width = w;
  height = h;
  m_cBits = colorBits;
  m_dBits = depthBits;
  m_share = share;

  // Retrieve device context of current window
  hDC = wglGetCurrentDC();
  if (!hDC)
    return;
  
  // Select a pixel format based on the values in iAttributes
  if (!wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, MAX_PFORMATS, pFormats, &nFormats))
    return;
  
  // Select first format
  format = pFormats[0];

  // Terminate attributes list at the beginning
  iAttributes[0] = 0;
  
  // Create the pbuffer
  m_pbuf = wglCreatePbufferARB(hDC, format, width, height, iAttributes);
  if (!m_pbuf)
    return;

  // Retrieve the device context of the pbuffer
  m_DC = wglGetPbufferDCARB(m_pbuf);
  if (!m_DC)
  {
    this->~CPixelBuffer();
    return;
  }

  // Create a rendering context for the pbuffer
  m_RC = wglCreateContext(m_DC);
  if (!m_RC)
  {
    this->~CPixelBuffer();
    return;
  }

  if (share)
    wglShareLists(wglGetCurrentContext(), m_RC);

  // Success
  m_isValid = true;
}

void CPixelBuffer::Activate()
{
  if (m_isValid)
  {
    // Ensure pbuffer isn't already active (is active only if we have a saved DC or RC)
    if (m_oldDC || m_oldRC)
      this->Deactivate();

    // Save current device context and rendering context
    m_oldDC = wglGetCurrentDC();
    m_oldRC = wglGetCurrentContext();

    // Make pbuffer device context and rendering context active
    wglMakeCurrent(m_DC, m_RC);
  }
}

void CPixelBuffer::Restore()
{
  int lost = 0;

  if (m_isValid)
  {
    // Check to see if pbuffer memory was lost due to a display mode change
    wglQueryPbufferARB(m_pbuf, WGL_PBUFFER_LOST_ARB, &lost);
    if (lost)
    {
      // If so, destroy the pbuffer and create it again using the same settings
      this->Release();
      this->Init(width, height, m_cBits, m_dBits, m_share);      
    }
  }
}

void CPixelBuffer::Deactivate()
{
  if (m_oldDC && m_oldRC) 
  {
    // Restore previous device context and rendering context
    wglMakeCurrent(m_oldDC, m_oldRC);

    m_oldDC = 0;
    m_oldRC = 0;
  }
} 

void CPixelBuffer::Release()
{
  // Restore previous rendering context
  this->Deactivate();

  // Delete pbuffer rendering context
  wglDeleteContext(m_RC);

  // Release the pbuffer device context
  wglReleasePbufferDCARB(m_pbuf, m_DC);

  // Destroy the pbuffer
  wglDestroyPbufferARB(m_pbuf);

  // pbuffer is no longer valid
  m_isValid = false;
}

void CPixelBuffer::ScreenShot()
{
	unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	unsigned int imagetexture;
	
	glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image );

	glGenTextures(1, &imagetexture);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, imagetexture);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

	Deactivate();

	glBindTexture(GL_TEXTURE_2D, imagetexture);

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
		glBegin(GL_TRIANGLES);
			glVertex3f(-4.0f, 4.0f, -1.5);
			glVertex3f(4.0f, 4.0f, -1.5);
			glVertex3f(-4.0f, -4.0f, -1.5);
			glVertex3f(4.0f, -4.0f, -1.5);
			glVertex3f(-4.0f, -4.0f, -1.5);
			glVertex3f(4.0f, 4.0f, -1.5);
		glEnd();

	delete image;
}

CPixelBuffer::~CPixelBuffer()
{
  this->Release();

  // Reset member variables to 0
  width = 0;
  height = 0;
  m_cBits = 0;
  m_dBits = 0;
  m_RC = 0;
  m_DC = 0;
  m_pbuf = 0;
  m_share = false;
  m_isValid = false;
}