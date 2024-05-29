// class for displaying 2d bitmaps on screen

#include "stdafx.h"

extern float fTime;

CPicture::CPicture()
{
	keyframer_index = 0;
	b_Masked = false;
	r = 1;
	g = 1;
	b = 1;
	a = 1;
	lasttime = 0;
}

CPicture::~CPicture()
{

}

bool CPicture::IsLoaded()
{
	if(m_picture) return true;
	return false;
}

void CPicture::SetMask(CTexture mymask)
{
	mask = mymask;
	b_Masked = true;
}

void CPicture::Create(float x, float y, float z, float width, float height, char *name, unsigned char r, unsigned char g, unsigned char b)
{
	x = x - width  / 2;
	y = y - height / 2;
	if (strstr(name,".bmp")) 
		face.LoadBMPKeyColor(name, r, g, b);
	else
		face.Load(name);
	m_picture = glGenLists(1);
	glNewList(m_picture, GL_COMPILE);	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
//	glLoadIdentity();
//	glRotatef(180,1,0,0);

	glBegin(GL_TRIANGLES);		
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(x+width, y+height, z);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(x, y+height, z);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(x+width, y, z);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(x+width, y+height, z);
/*
		// quads
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(x, y + height, z);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(x + width, y + height, z); 
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(x + width, y, z);*/
	glEnd();
	glEndList();
}

void CPicture::Draw()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(posx,posy,posz);

	face.Use();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(r,g,b,a);

	glCallList(m_picture);

	glPopMatrix();
}

/*
void CPicture::Sequence2(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float myscenetime = (sTime-scenetime)*1000-lasttime;
	if(keyframer_index < MAX_KEYFRAMER_INDEX-1)
	{
//		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, NULL);
		kd.kinterpolationmode = ES_ANIMATE_LINEAR;
		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, NULL);
		if (kd.keyframeTime < 1.0f)
		{
				r = -Difference(SceneKeyFrames[keyframer_index].rotationX, kd.rotationX);
				g = -Difference(SceneKeyFrames[keyframer_index].rotationY, kd.rotationY);
				b = -Difference(SceneKeyFrames[keyframer_index].rotationZ, kd.rotationZ);
				a = (r+g+b)/3;
				posx = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
				posy = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
				posz = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);

		}
		else
		{
				keyframer_index++;//= 2;

				r = SceneKeyFrames[keyframer_index].rotationX;
				g = SceneKeyFrames[keyframer_index].rotationY;
				b = SceneKeyFrames[keyframer_index].rotationZ;
				a = (r+g+b)/3;
				posx = SceneKeyFrames[keyframer_index].positionX;
				posy = SceneKeyFrames[keyframer_index].positionY;
				posz = SceneKeyFrames[keyframer_index].positionZ;
		}
	} else if (keyframer_index == MAX_KEYFRAMER_INDEX-1) {
		if (looper != 0)
		{
			for (int i=0;i<MAX_KEYFRAMER_INDEX;i++) {
				SceneKeyFrames[i].keyframeTime +=SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
			}
			SceneKeyFrames[keyframer_index+1] = SceneKeyFrames[looper];
			SceneKeyFrames[keyframer_index+1].keyframeTime = SceneKeyFrames[looper].keyframeTime -SceneKeyFrames[0].keyframeTime + myscenetime;
			lasttime += SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
			keyframer_index = looper-1;
		}
	}
	Draw();
}
*/
//
void CPicture::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float myscenetime = (fTime-scenetime)*1000-lasttime;

	if(keyframer_index < MAX_KEYFRAMER_INDEX-1)
	{
		kd.kinterpolationmode = ES_ANIMATE_LINEAR;
		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, NULL);
		if (kd.keyframeTime < 1.0f)
		{
				r = -Difference(SceneKeyFrames[keyframer_index].rotationX, kd.rotationX);
				g = -Difference(SceneKeyFrames[keyframer_index].rotationY, kd.rotationY);
				b = -Difference(SceneKeyFrames[keyframer_index].rotationZ, kd.rotationZ);
				a = (r+g+b)/3;
				posx = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
				posy = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
				posz = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);

		}
		else
		{
				keyframer_index++;//= 2;

				r = SceneKeyFrames[keyframer_index].rotationX;
				g = SceneKeyFrames[keyframer_index].rotationY;
				b = SceneKeyFrames[keyframer_index].rotationZ;
				a = (r+g+b)/3;
				posx = SceneKeyFrames[keyframer_index].positionX;
				posy = SceneKeyFrames[keyframer_index].positionY;
				posz = SceneKeyFrames[keyframer_index].positionZ;
		}
	} else {
		if (looper != 0)
		{
			lasttime += SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
			keyframer_index = looper-1;
		}
	}
	Draw();
}
