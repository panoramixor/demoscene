#ifndef __PBUFFER_H__
#define __PBUFFER_H__

class CPBuffer
{
	public:
		CPBuffer();
		virtual ~CPBuffer();
		void Create(int width, int height);
		void Activate();
		void Use();
		void Release();
	private:
		HPBUFFERARB hPBuffer;
		HDC         hDC;
		HGLRC       hRC;
		int         nWidth;
		int         nHeight;
		GLuint g_dynamicTextureID;
};

#endif /*__PBUFFER_H__*/