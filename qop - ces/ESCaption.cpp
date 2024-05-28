// caption class

#include "stdafx.h"

extern float sTime;

CCaption::CCaption()
{
	keyframer_index = 0;
	lasttime = 0;
	strsize = 0;
}

CCaption::~CCaption()
{
	if(bflfont) BFL_DestroyFont(bflfont);
}

void CCaption::Create(float start, float lenght, float posx, float posy, float posz, char *text)
{
	Load();
	float fadetime = 1000.0f;
	this->posx = posx;
	this->posy = posy;
	this->posz = posz;
	this->scale = 0.003f;
	fadein_out[0].SetKeyFrame(start, posx, posy, posz, 0, 0,00);
	fadein_out[1].SetKeyFrame(start+fadetime, posx, posy, posz, 1, 1, 1);
	fadein_out[2].SetKeyFrame(start+lenght-fadetime, posx, posy, posz, 1, 1, 1);
	fadein_out[3].SetKeyFrame(start+lenght, posx, posy, posz, 0, 0, 0);
	string = text;
}
void CCaption::Create(float start, float lenght, float posx, float posy, float posz, float scale, char *text)
{
	Load();
	float fadetime = 1000.0f;
	this->posx = posx;
	this->posy = posy;
	this->posz = posz;
	this->scale = scale;
	fadein_out[0].SetKeyFrame(start, posx, posy, posz, 0, 0,00);
	fadein_out[1].SetKeyFrame(start+fadetime, posx, posy, posz, 1, 1, 1);
	fadein_out[2].SetKeyFrame(start+lenght-fadetime, posx, posy, posz, 1, 1, 1);
	fadein_out[3].SetKeyFrame(start+lenght, posx, posy, posz, 0, 0, 0);
	string = text;
}
void CCaption::Load()
{
	char fontname[]="./data/fonts/font3.bf";
/*
	fontfile = fopen(fontname, "rb");
	if(!fontfile)
		FatalError("can't find font: %s\n", fontname);
	else*/
	LogLine("reading font: %s\n", fontname);
/*	__asm int 3; */
//	bflfont = BFL_LoadFont(fontname);
	bflfont = BFL_LoadFont((FILE *)fontname);
//	fclose(fontfile);
}

void CCaption::Print(float x, float y, float z)
{

	glPushMatrix();
		glLoadIdentity();

		glTranslatef(x, y, z);
		glScalef(this->scale, this->scale, this->scale);
		BFL_DrawString(bflfont, string, (int)strlen(string));
	glPopMatrix();
}

void CCaption::Draw(float scenetime)
{
	glDisable(GL_DEPTH_TEST);
//	glBlendFunc(GL_ONE, GL_ONE);
//	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING); 
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	Sequence(fadein_out, 4, 0, scenetime);
}

/*
void CCaption::Draw(float scenetime, int effect)
{
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	Sequence(kf, 4*5, effect, scenetime);
}
*/
void CCaption::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float myscenetime = (sTime-scenetime)*1000-lasttime;
	if(keyframer_index < MAX_KEYFRAMER_INDEX)
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
	}
	else
	{
				if (looper != 0)
				{
					keyframer_index = looper;
					lasttime += SceneKeyFrames[MAX_KEYFRAMER_INDEX-1].keyframeTime;
				}
	}

//	glBlendFunc(GL_ONE, GL_ONE_MINUS_DST_COLOR);
//	glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4f(r,g,b,a);
	Print(this->posx, this->posy, this->posz);
}