// Caption class

extern float fTime;
#include "stdafx.h"

CCaption::CCaption()
{
	keyframer_index = 0;
	bflfont = 0;
}

CCaption::~CCaption()
{
}

void CCaption::Load()
{
	char fontname[]="./data/font/expoo.bf";
	fontfile = fopen(fontname, "rb");
	if(!fontfile)
		FatalError("can't find font: %s\n", fontname);
	else
		LogLine("reading font: %s\n", fontname);
	fclose(fontfile);

	bflfont = BFL_LoadFont((FILE*)fontname);
}

void CCaption::Print(float x, float y, float z, char *string)
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(x, y, z);
		glScalef(0.1f, 0.1f, 0.1f);
		BFL_DrawString(bflfont, string, strlen(string)+1);
	glPopMatrix();
}

void CCaption::Draw(float scenetime)
{
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	Sequence(kf, 4*5, 0, scenetime);
}


void CCaption::Draw(float scenetime, int effect)
{
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	Sequence(kf, 4*5, effect, scenetime);
}

void CCaption::Create(float time1, float x1, float y1, float z1, char *line1, float time2, float x2, float y2, float z2, char *line2, float time3, float x3, float y3, float z3, char *line3, float time4, float x4, float y4, float z4, char *line4, float time5, float x5, float y5, float z5, char *line5, float timelenght)
{
	k_caption.SetKeyFrame(time1, -15+x1, 0+y1, -20+z1, 0, 0, 0);		//	fadein
//	memcpy(k_caption.name, line1, MAX_PATH);
	strncpy(k_caption.name, line1, MAX_PATH);
	kf[0] = k_caption;

	k_caption.SetKeyFrame(time1+(timelenght/4), -15+x1, 0+y1, -20+z1, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line1, MAX_PATH);
	kf[1] = k_caption;

	k_caption.SetKeyFrame(time1+((timelenght/4)*2), -15+x1, 0+y1, -20+z1, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line1, MAX_PATH);
	kf[2] = k_caption;

	k_caption.SetKeyFrame(time1+((timelenght/4)*3), -15+x1, 0+y1, -20+z1, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line1, MAX_PATH);
	kf[3] = k_caption;

	k_caption.SetKeyFrame(time2, -15+x2, 0+y2, -20+z2, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line2, MAX_PATH);
	kf[4] = k_caption;

	k_caption.SetKeyFrame(time2+(timelenght/4), -15+x2, 0+y2, -20+z2, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line2, MAX_PATH);
	kf[5] = k_caption;

	k_caption.SetKeyFrame(time2+((timelenght/4)*2), -15+x2, 0+y2, -20+z2, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line2, MAX_PATH);
	kf[6] = k_caption;

	k_caption.SetKeyFrame(time2+((timelenght/4)*3), -15+x2, 0+y2, -20+z2, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line2, MAX_PATH);
	kf[7] = k_caption;

	k_caption.SetKeyFrame(time3, -15+x3, 0+y3, -20+z3, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line3, MAX_PATH);
	kf[8] = k_caption;

	k_caption.SetKeyFrame(time3+(timelenght/4), -15+x3, 0+y3, -20+z3, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line3, MAX_PATH);
	kf[9] = k_caption;

	k_caption.SetKeyFrame(time3+((timelenght/4)*2), -15+x3, 0+y3, -20+z3, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line3, MAX_PATH);
	kf[10] = k_caption;

	k_caption.SetKeyFrame(time3+((timelenght/4)*3), -15+x3, 0+y3, -20+z3, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line3, MAX_PATH);
	kf[11] = k_caption;

	k_caption.SetKeyFrame(time4, -15+x4, 0+y4, -20+z4, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line4, MAX_PATH);
	kf[12] = k_caption;

	k_caption.SetKeyFrame(time4+(timelenght/4), -15+x4, 0+y4, -20+z4, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line4, MAX_PATH);
	kf[13] = k_caption;

	k_caption.SetKeyFrame(time4+((timelenght/4)*2), -15+x4, 0+y4, -20+z4, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line4, MAX_PATH);
	kf[14] = k_caption;

	k_caption.SetKeyFrame(time4+((timelenght/4)*3), -15+x4, 0+y4, -20+z4, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line4, MAX_PATH);
	kf[15] = k_caption;

	k_caption.SetKeyFrame(time5, -15+x5, 0+y5, -20+z5, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line5, MAX_PATH);
	kf[16] = k_caption;

	k_caption.SetKeyFrame(time5+(timelenght/4), -15+x5, 0+y5, -20+z5, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line5, MAX_PATH);
	kf[17] = k_caption;

	k_caption.SetKeyFrame(time5+((timelenght/4)*2), -15+x5, 0+y5, -20+z5, 1, 0, 0);		//	fadein
	strncpy(k_caption.name, line5, MAX_PATH);
	kf[18] = k_caption;

	k_caption.SetKeyFrame(time5+((timelenght/4)*3), -15+x5, 0+y5, -20+z5, 0, 0, 0);		//	fadein
	strncpy(k_caption.name, line5, MAX_PATH);
	kf[19] = k_caption;
	Load();
}

void CCaption::Sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int effect, float scenetime)
{
	KeyFrame kd;
	float x=0.f, y = 0.f, z = 0.f, c = 0.f;
	float myscenetime = (fTime-scenetime)*1000;
	if(keyframer_index < MAX_KEYFRAMER_INDEX)
	{
		kd.KeyFrameInterpolate(SceneKeyFrames[keyframer_index], SceneKeyFrames[keyframer_index+1], myscenetime - SceneKeyFrames[keyframer_index].keyframeTime, keyframer_index, NULL);
		if(kd.keyframeTime > 0.f) {
			if (kd.keyframeTime < 1.f)
			{
					x = -Difference(SceneKeyFrames[keyframer_index].positionX, kd.positionX);
					y = -Difference(SceneKeyFrames[keyframer_index].positionY, kd.positionY);
					z = -Difference(SceneKeyFrames[keyframer_index].positionZ, kd.positionZ);
					c = -Difference(SceneKeyFrames[keyframer_index].rotationX, kd.rotationX);
			}
			else
			{
					keyframer_index++;//= 2;

					x = SceneKeyFrames[keyframer_index].positionX;
					y = SceneKeyFrames[keyframer_index].positionY;
					z = SceneKeyFrames[keyframer_index].positionZ;
					c = SceneKeyFrames[keyframer_index].rotationX;
			}
		}
		if(c>0) {
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glLoadIdentity();
			glTranslatef(0,-0.6f, 0);
			float sinftime = (float)sin(fTime*4)/20;
			if(effect==0) {
				glColor4f(c,c,c,c);
				glDisable(GL_TEXTURE_2D);
				glLineWidth(2);
				glBegin(GL_LINES);
				
				//-------
					glVertex3f(-15+sinftime,0.15f,-1);
					glVertex3f(-1+sinftime,0.15f,-1);

				// \*
					glVertex3f(-1+sinftime,0.15f,-1);
					glVertex3f(-0.931f+sinftime,0.08f,-1);
				// \*
					glVertex3f(0.931f+sinftime,-0.051f,-1);
					glVertex3f(1+sinftime,-0.12f,-1);
				//-------
					glVertex3f(1+sinftime,-0.12f,-1);
					glVertex3f(15+sinftime,-0.12f,-1);
				//-------
					glVertex3f(-15,0.08f,-1);
					glVertex3f(15,0.08f,-1);
				//-------
					glVertex3f(-15,-0.05f,-1);
					glVertex3f(15,-0.05f,-1);
				glEnd();
				glBlendFunc(GL_DST_COLOR,GL_ZERO);
				glEnable(GL_BLEND);
				glBegin(GL_QUADS);
					glColor4f(1.5f-c,1.5f-c,1.5f-c,0.5f*c);
					glBegin(GL_QUADS);		
					glVertex3f(-15.f,	-0.05f,			-1.f);
					glVertex3f(-15.f,	0.08f, -1.f);
					glVertex3f(15.f, 0.08f, -1.f); 
					glVertex3f(15.f, -0.05f,			-1.f);
				glEnd();

				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glColor4f(c,c,c,c);
				Print(x, y-12, z, SceneKeyFrames[keyframer_index].name);
			} else {
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glColor4f(c,c,c,c);
				Print(x, y, z, SceneKeyFrames[keyframer_index].name);
			}
			glPopAttrib();
		}
	}
}