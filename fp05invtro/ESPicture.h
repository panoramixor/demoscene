
#ifndef __CPICTURE_H___
#define __CPICTURE_H___

class CPicture
{
public:
	CPicture();
	~CPicture();
	void Create(float x, float y, float z, float width, float height, char *name, unsigned char r, unsigned char g, unsigned char b);
	void Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime);
	void SetMask(CTexture mask);
	void Draw();
	bool IsLoaded();

private:
	int keyframer_index;
	int m_picture;
	int m_mask;
	float r, g, b, a;
	float posx, posy, posz;
	float lasttime;
	bool b_Masked;
	CTexture face;
	CTexture mask;

};


#endif /* __CPICTURE_H___ */