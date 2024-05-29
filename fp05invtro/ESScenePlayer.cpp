#include "stdafx.h"
#include "commctrl.h"
//#include "resource1.h"

extern signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata);
float sTime;
extern float fTime;

CFMod music;

CScene01 scene01;
CPicture borde;

/* Constructor */
CScenePlayer::CScenePlayer()
{
	take = -1;
	sceneindex = 0;
}

void CScenePlayer::Play()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sTime = fTime;
	static int old_take = -1;

	if (take != old_take)
	{
		bInitSequence = false;
		current_take = take;
		old_take = take;

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);


		if(strstr(syncpoints[current_take].name, "<scene_")) {
			scenearray[sceneindex].p_scene->b_init = false;
			sceneindex++;

			if(sceneindex > 1) {
				sceneindex=0;
				take = 0;
				current_take = 0;
				old_take = 0;
				sTime = 0;
			}
		}
	}


	if(!bInitSequence) {
		bInitSequence = true;
		scenearray[sceneindex].p_scene->Init(sTime, take);
	}
	scenearray[sceneindex].p_scene->Play(sTime);

}


bool CScenePlayer::Load(char *filename)
{
	music.Load("./data/f0ck-invtro.ogg", FSOUND_OUTPUT_DSOUND);

	FILE *f_points = fopen(filename, "rb");
	fread(&numsyncpoints, sizeof(unsigned int), 1, f_points);
	fread(syncpoints, sizeof(SyncPoint), numsyncpoints, f_points);
	fclose(f_points);

	for(unsigned int i=0;i<numsyncpoints;i++) {
		music.AddSyncPoint(syncpoints[i].offset, syncpoints[i].name);
	}
	music.SetSyncCallback(synccallback);

	scene01.Load("");
	scenearray[0].p_scene = &scene01;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene01.Play(0);
	glPopAttrib();

	music.Play();
	return true;
}

/* Destructor */
CScenePlayer::~CScenePlayer()
{

}
