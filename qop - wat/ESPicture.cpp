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
	mask.ID = mymask.ID;
}

void CPicture::Create(float x, float y, float z, float width, float height, char *face_filepath, char *mask_filepath)
{
	x = x - width  / 2;
	y = y - height / 2;

	face.Load(face_filepath);
	if(lstrlen(mask_filepath) != 0)	{
		LogLine("face: %s\n", face_filepath);
		LogLine("mask: %s\n", mask_filepath);
		b_Masked = true;
		mask.Load(mask_filepath);
	}
	m_picture = glGenLists(1);
	glNewList(m_picture, GL_COMPILE);	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
//	glLoadIdentity();
//	glRotatef(180,1,0,0);

  	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y,			z);
	glEnd();
	glEndList();
}

void CPicture::Draw()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(posx,posy,posz);
	if(b_Masked) {
//		glBlendFunc(GL_DST_COLOR,GL_ONE);
		glBlendFunc(GL_DST_COLOR,GL_ZERO);
		glEnable(GL_BLEND);
		glColor4f(a,a,a,1);
		mask.Use();
		glCallList(m_picture);
	}
	glBlendFunc(GL_SRC_COLOR,GL_ONE);
	glEnable(GL_BLEND);
	glColor4f(r,g,b,a);
	face.Use();
	glCallList(m_picture);
	glPopMatrix();
}

void CPicture::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float myscenetime = (fTime-scenetime)*1000-lasttime;
	if(keyframer_index < MAX_KEYFRAMER_INDEX)
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
