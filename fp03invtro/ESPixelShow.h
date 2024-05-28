/**********************************************************

	Project: EngineSuffer
	Desc:    class for displaying 2d bitmaps on screen
	Author: panoramix
**********************************************************/

#ifndef __CPIXELSHOW_H___
#define __CPIXELSHOW_H___

class CPixelShow
{
public:
	CPixelShow();
	void Create(int width, int height, char *filename);
	void Create(int width, int height, int resource);
	~CPixelShow();
	HGLOBAL LoadRaw(char *filename);
	void Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime);
	void Draw();

private:
	int sizex;
	int sizey;
	float posx;
	float posy;
	float posz;
	float lasttime;
	int keyframer_index;
	HGLOBAL pixelart;
};


#endif /* __CPIXELSHOW_H___ */