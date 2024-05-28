#include "stdafx.h"

extern HDC   g_hDC;
extern HGLRC g_hRC;

/* Constructor */
CPBuffer::CPBuffer()
{
	hRC = NULL;
}

/* Destructor */
CPBuffer::~CPBuffer()
{
	glDeleteTextures( 1, &g_dynamicTextureID );
	if(hRC != NULL)
	{
		wglMakeCurrent(hDC, hRC);
		wglDeleteContext(hRC);
		wglReleasePbufferDCARB(hPBuffer, hDC);
		wglDestroyPbufferARB(hPBuffer);
		hRC = NULL;
	}
}

void CPBuffer::Create(int width, int height)
{
	nWidth = width;
	nHeight = height;

	int pf_attr[] =
	{
		WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // P-buffer will be used as a texture
		WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
		WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
		WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
		WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
		WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
		WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
		0                                   // Zero terminates the list
	};

	unsigned int count = 0;
	int pixelFormat;
	wglChoosePixelFormatARB( g_hDC,(const int*)pf_attr, NULL, 1, &pixelFormat, &count);

	int pb_attr[] =
	{
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, // Our p-buffer will have a texture format of RGBA
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,   // Of texture target will be GL_TEXTURE_2D
		0                                             // Zero terminates the list
	};
	
	hPBuffer = wglCreatePbufferARB(g_hDC, pixelFormat, render_to_texture_width, render_to_texture_height, pb_attr );
	hDC      = wglGetPbufferDCARB(hPBuffer);
	hRC      = wglCreateContext(hDC);

	glGenTextures( 1, &g_dynamicTextureID );
	glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );

	wglMakeCurrent(hDC, hRC);

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)((float)window_width/(float)window_height);
	gluPerspective( 70, ratio, 0.5f, 2000.0);

	wglMakeCurrent( g_hDC, g_hRC );
}

void CPBuffer::Activate()
{
	wglMakeCurrent(hDC, hRC);
//	glViewport(0,0,render_to_texture_width,render_to_texture_height);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CPBuffer::Use()
{
	glBindTexture(GL_TEXTURE_2D, g_dynamicTextureID); 
	wglBindTexImageARB(hPBuffer, WGL_FRONT_LEFT_ARB);
}

void CPBuffer::Release()
{
//	glViewport(0,0,window_width,window_height);
	wglMakeCurrent(g_hDC, g_hRC);
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	wglReleaseTexImageARB(hPBuffer, WGL_FRONT_LEFT_ARB);
}