// Overlay FX class

#ifndef __COVRFX_H__
#define __COVRFX_H__

class COverlayFX
{
public:
	COverlayFX();
	~COverlayFX();
	void Disable();
	void SetType(int type);
	void Enable();
	void Draw(float sTime);

private:
	void DisableAllFX();
	int dlist;
	bool b_enabled;
	unsigned int texture[1];
	float *p_pixels;
	unsigned char *p_pixels2;
	int fxtype;
	CTexture current_framebuffer;
};

#endif /*__COVRFX_H__*/