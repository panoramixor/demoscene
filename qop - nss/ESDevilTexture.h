// texture class using DevIL

#ifndef __CDEVIL_H__
#define __CDEVIL_H__

class CDevilTexture
{
public:
	CDevilTexture();
	~CDevilTexture();

	void Load(char *filename);
	void LoadEnvMap(char *filename);
	unsigned char *CDevilTexture::GetData();
	CDevilTexture *CDevilTexture::CreateMask();
	CDevilTexture *CDevilTexture::KeyColorMask(int r, int g, int b);
	void Bind();
	void Draw();

	bool isenvmap;
	bool isdifuse;
	bool isopacity;

private:
	GLint activetexture;
	ILuint images;
	ILuint id[2];
	ILuint ILid;
	ILinfo ILtexture;
	ILubyte *data;
	CDevilTexture *mask;
	char sz_filename[MAX_PATH];

};

#endif /*__CDEVIL_H__*/