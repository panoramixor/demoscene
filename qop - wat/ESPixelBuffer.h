// Pixel Puffer (pbuffers) Class 

#ifndef __PBUFFER_H__
#define __PBUFFER_H__


const int MAX_PFORMATS = 256;
const int MAX_ATTRIBS = 32;

class CPixelBuffer
{
  public: 
    int width;
    int height;

    CPixelBuffer();
    ~CPixelBuffer();
    
    void Init(int w, int h, int colorBits, int depthBits, bool share);
    void Release();

    void Restore();

	void ScreenShot();

    void Activate();
    void Deactivate();

  private:
    int m_cBits;
    int m_dBits;
    bool m_share;

    HPBUFFERARB m_pbuf;
    HDC m_DC;
    HGLRC m_RC;
    bool m_isValid;
 
    
    HDC m_oldDC;
    HGLRC m_oldRC;
};

bool InitPBuffers(void);
bool isExtensionSupported(const GLubyte *extensions, const char *extName);

#endif