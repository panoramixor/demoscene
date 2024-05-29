// intro scene

#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern void postfiler(bool);

extern float pixels[];

extern Model_3DS scene08_3ds;

extern CPicture scene00_picture0;
extern CPicture scene00_picture1;
extern CPicture scene00_picture2;
extern CPicture scene00_picture3;

extern CPicture scene01_screen;
extern CTexture mask;

static KeyFrame k0, k1, k2, k3;

float scenetime0;

extern CTexture framebuffer;

static KeyFrame ColorFadeIn[4];
static KeyFrame ColorFadeOut[4];
static KeyFrame ColorFadeIn2[4];
static KeyFrame ColorFadeOut2[4];

KeyFrame ColorFadeLogo[4];
KeyFrame ColorFadePresents[4];
KeyFrame ColorFadeTitle[4];

CCaption scene00_caption0;

void seq_scene00()
{

//		postfiler(false);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
			glColor4f((fTime-scenetime0)/30, (fTime-scenetime0)/30, (fTime-scenetime0)/30, (fTime-scenetime0)/50);
			glTranslatef(0,0,-(fTime-scenetime0)/500);
			glRotatef(90, 1, 0, 0);
			glRotatef(fTime*5, 0, 1, 0);
			glDisable(GL_BLEND);
			scene08_3ds.Draw();
		glPopMatrix();

		glPushMatrix();
			scene00_picture3.Sequence(ColorFadeTitle, 3, 0, scenetime0);
			scene00_caption0.Draw(scenetime0, 1);
			noise_sequence(ColorFadeTitle, 3, 0, scenetime0);

		glPopMatrix();
		crossfade(scene01_screen, mask, ColorFadeIn, ColorFadeIn2, scenetime0);
}

bool init_seq_scene00()
{
	k0.SetKeyFrame(1500, 0, -1, -1, 0, 0, 0);		//	fadein
	k1.SetKeyFrame(2999, 0, -1, -1, 1, 1, 1);		//
	k2.SetKeyFrame(10875, 0, -1, -1, 1, 1, 1);	//	fadeout
	k3.SetKeyFrame(12001, 0, -1, -1, 0, 0, 0);	//

	ColorFadeLogo[0] = k0;
	ColorFadeLogo[1] = k1;
	ColorFadeLogo[2] = k2;
	ColorFadeLogo[3] = k3;

	k0.SetKeyFrame(0, 0, 0, 0, 0, 0, 0);		//	fadein
	k1.SetKeyFrame(2999, 0, 0, 0, 0, 0, 0);		//
	k2.SetKeyFrame(10875, 0, 0, 0, 1, 1, 1);		//	fadeout
	k3.SetKeyFrame(12001, 0, 0, 1, 0, 0, 0);	//

	ColorFadePresents[0] = k0;
	ColorFadePresents[1] = k1;
	ColorFadePresents[2] = k2;
	ColorFadePresents[3] = k3;

	k0.SetKeyFrame(10875, -0.05f, 0, 0.5f, 0, 0, 0);		//	fadein
	k1.SetKeyFrame(12001, -0.05f, 0, 0.5f, 1, 1, 1);		//
	k2.SetKeyFrame(18004, -0.05f, 0, 0.5f, 1, 1, 1);		//	fadeout
	k3.SetKeyFrame(27006, -0.05f, 0, 0.5f, 0, 0, 0);	//

	ColorFadeTitle[0] = k0;
	ColorFadeTitle[1] = k1;
	ColorFadeTitle[2] = k2;
	ColorFadeTitle[3] = k3;

	// transition

	k0.SetKeyFrame(24358.00000f-1000, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24358.00000f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn[0] = k0;
	ColorFadeIn[1] = k1;

	k0.SetKeyFrame(24358.00000f-1000, 0, 0, 0, 0, 0, 0);		//	full transparent
	k1.SetKeyFrame(24358.00000f, 0, 0, 0, 1, 1, 1);		//  full color

	ColorFadeIn2[0] = k0;
	ColorFadeIn2[1] = k1;

	scene00_caption0.Create( 5000, 10, 0, -3, " please...",
						     6000, 10, 0, -3, "feel free",
						     7000, 10, 0, -3, "to enjoy...",
					         8000, 10, 0, -3, "",
					         9000, 10, 0, -3, ""
							,1000);	

	scenetime0 = fTime;
	return true;
}