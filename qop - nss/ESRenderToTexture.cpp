// class for rendering to texture

#include "stdafx.h"

/* Constructor */
CRenderToTexture::CRenderToTexture()
{
	fbo = 0;
	rb = 0;
	listid = 0;
}

/* Destructor */
CRenderToTexture::~CRenderToTexture()
{
	if(fbo)
		delete fbo;
	if(rb)
		delete rb;
}

void CRenderToTexture::Create()
{
	if(GLEW_EXT_framebuffer_object)
	{
		fbo = new FramebufferObject;
		rb = new Renderbuffer;

		glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &activetexture);
		glActiveTextureARB(activetexture);
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &text_id1); // create (reference to) a new texture
		glBindTexture(GL_TEXTURE_2D, text_id1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window_width, window_height, 0, GL_RGB, GL_FLOAT, 0);
		fbo->Bind(); // Bind framebuffer object.
		// Attach texture to framebuffer color buffer
		fbo->AttachTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, text_id1);
		rb->Set( GL_DEPTH_COMPONENT24, window_width, window_height);
		fbo->AttachRenderBuffer( GL_DEPTH_ATTACHMENT_EXT, rb->GetId() );
		fbo->IsValid();
		FramebufferObject::Disable();
	}
}

void CRenderToTexture::Enable()
{
	if(GLEW_EXT_framebuffer_object)
	{
		glGetIntegerv(GL_DRAW_BUFFER, &_currentDrawbuf); // Save the current Draw buffer
		fbo->Bind(); 									 // Render to the FBO
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);			 // Draw into the first texture 
		glGetIntegerv(GL_VIEWPORT, vp);
		glViewport(0, 0, window_width, window_height);
	}
}


void CRenderToTexture::Disable()
{
	if(GLEW_EXT_framebuffer_object)
	{
		FramebufferObject::Disable();
		glDrawBuffer(_currentDrawbuf);
	}
}

void CRenderToTexture::Draw()
{
	if(GLEW_EXT_framebuffer_object)
	{
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(vp[0], vp[1], vp[2], vp[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, window_width/window_height, 0.01f, 4000.0);
//	    gluOrtho2D(-1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		float width = 2;
		float height = 2;
		float x=-width/2.0f;
		float y=-height/2.0f;
		float z=-1.4f;
		if(listid == 0) {
			listid = glGenLists(1);
			glNewList(listid, GL_COMPILE);
			glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glColor4f(0.18f,0.18f,0.18f,0.18f);
			DisableAllTextures();

			glActiveTextureARB(activetexture);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, text_id1);//read from the second texture
			glBegin(GL_QUADS);		
			for(unsigned int i=0;i<20;i++) {
						glTexCoord2f(1.0f, 1.0f); 
						glVertex3f(x+width, y+height, z+0.001f*i);
						glTexCoord2f(1.0f, 0.0f); 
						glVertex3f(x+width, y, z+0.001f*i);
						glTexCoord2f(0.0f, 0.0f); 
						glVertex3f(x, y, z+0.001f*i);
						glTexCoord2f(0.0f, 1.0f); 
						glVertex3f(x, y+height, z+0.001f*i);
			}
			glEnd();
			glEndList();
		} else {
			glCallList(listid);
		}
	}
}