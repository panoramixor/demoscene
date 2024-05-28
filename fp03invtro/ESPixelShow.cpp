/**********************************************************

	Project: EngineSuffer
	Desc:    class for displaying 2d bitmaps on screen
	Author:  panoramix
**********************************************************/

#include "stdafx.h"

extern float fTime;

HGLOBAL CPixelShow::LoadRaw(char *filename) {
	DWORD filesize;
	HGLOBAL pixels;
	DWORD tmp;
	HANDLE h_file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_file != INVALID_HANDLE_VALUE) {
		filesize = GetFileSize(h_file, NULL);
		pixels = GlobalAlloc(GMEM_FIXED, filesize);
		ReadFile(h_file, pixels, filesize, &tmp,NULL);
		CloseHandle(h_file);
	}
	return pixels;
}


CPixelShow::CPixelShow()
{

}


void CPixelShow::Create(int width, int height, char *filename)
{
	DWORD filesize;
	DWORD tmp;
	HANDLE h_file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_file != INVALID_HANDLE_VALUE) {
		filesize = GetFileSize(h_file, NULL);
		pixelart = GlobalAlloc(GMEM_FIXED, filesize);
		ReadFile(h_file, pixelart, filesize, &tmp,NULL);
		CloseHandle(h_file);
	}
	sizex = width;
	sizey = height;

}

void CPixelShow::Create(int width, int height, int resource)
{
	HRSRC hrsrc = FindResource(0, (const char *)resource, RT_RCDATA);
	HGLOBAL rsrc = LoadResource(0, hrsrc);
	pixelart = LockResource(rsrc);
	sizex = width;
	sizey = height;
}


CPixelShow::~CPixelShow()
{
	GlobalFree(pixelart);
}

void CPixelShow::Draw()
{
	glRasterPos3f(posx, posy, -1);
	glDrawPixels(sizex, sizey, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)pixelart);
}

void CPixelShow::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float myscenetime = (fTime-scenetime)*1000-lasttime;
	if(keyframer_index < MAX_KEYFRAMER_INDEX)
	{
		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, NULL);
		if (kd.keyframeTime < 1.0f)
		{
				posx = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
				posy = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
				posz = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);
		}
		else
		{
				keyframer_index++;//= 2;

				posx = SceneKeyFrames[keyframer_index].positionX;
				posy = SceneKeyFrames[keyframer_index].positionY;
				posz = SceneKeyFrames[keyframer_index].positionZ;
		}
	}
	else
	{
				if (looper != 0)
				{
					keyframer_index = looper;
					lasttime += SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
				}
	}
	Draw();
}
