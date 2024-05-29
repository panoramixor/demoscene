// Skybox class

#ifndef __CSKYBOX_H__
#define __CSKYBOX_H__

class CSkybox
{
public:
	CSkybox();
	~CSkybox();
	void Create(float x, float y, float z, float width, float height, float length, char *tex_filepath);
	void Draw();

private:
	int m_skybox;
	CTexture tex_back;
	CTexture tex_front;
	CTexture tex_bottom;
	CTexture tex_top;
	CTexture tex_left;
	CTexture tex_right;
};

#endif /*__CSKYBOX_H__*/