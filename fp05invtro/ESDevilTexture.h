/**********************************************************

	Project: EngineSuffer

**********************************************************/

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
	CDevilTexture *CDevilTexture::CreateMask(int r, int g, int b);
	void Bind();
	void Draw();

private:
	ILuint images;
	ILuint id;
	ILuint ILid;
	ILinfo ILtexture;
	ILubyte *data;
	CDevilTexture *mask;
	char sz_filename[MAX_PATH];
	bool isenvmap;
	
};

#endif /*__CDEVIL_H__*/