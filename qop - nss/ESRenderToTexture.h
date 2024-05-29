// render to texture class

#ifndef __CRENDERTEXTURE_H__
#define __CRENDERTEXTURE_H__

class CRenderToTexture
{
public:
	CRenderToTexture();
	~CRenderToTexture();
	void Create();
	void Enable();
	void Disable();
	void Draw();

private:
	GLint _currentDrawbuf;
	int vp[4];
	unsigned int text_id1;
	FramebufferObject *fbo;
	Renderbuffer *rb;
	GLint activetexture;
	GLuint listid;

};

#endif /*__CRENDERTEXTURE_H__*/