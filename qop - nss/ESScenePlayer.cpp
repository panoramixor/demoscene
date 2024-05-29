// scene player/sequencer


#include "stdafx.h"
#include "commctrl.h"
//#include "resource1.h"

extern signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata);
float sTime;
extern float fTime;
//extern HWND  g_hWndOptions;									// This is the handle for the options dialog

extern CFMod music;

CScene00 scene00;
CScene01 scene01;
CScene02 scene02;
CScene03 scene03;
CScene10 scene04;
CScene05 scene05;
CScene06 scene06;
CScene07 scene07;
CScene08 scene08;
CScene09 scene09;
CScene04 scene10;

//CPicture borde;

/* Constructor */
CScenePlayer::CScenePlayer()
{
	take = -1;
	sceneindex = 0;
	localtake = 0;
	framebuffer = 0;
//	srand(timeGetTime());
}

void CScenePlayer::Play()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	sTime = (float)glfwGetTime();
	static int old_take = -1;

	if (take != old_take)
	{
		bInitSequence = false;
		current_take = take;
		old_take = take;
		localtake++;

		if(strstr(syncpoints[current_take].name, "finish")) {
			glfwTerminate();
		}

		if(strstr(syncpoints[current_take].name, "<scene")) {
			scenearray[sceneindex].p_scene->b_init = false;
			sceneindex++;
#ifdef _DEBUG
			LogLine("DEBUG-> scene change: %d, %f\n", sceneindex, sTime);
#endif
			localtake = 0;
		}
	}

	if(!bInitSequence) {
		bInitSequence = true;
		sTime = (float)glfwGetTime();
		scenearray[sceneindex].p_scene->Init(sTime, localtake);
		DisableAllTextures();
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	sTime = (float)glfwGetTime();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	framebuffer->Enable();
	scenearray[sceneindex].p_scene->Play(sTime, framebuffer);
	framebuffer->Disable();
	glPopAttrib();
	framebuffer->Draw();
	scenearray[sceneindex].p_scene->PostRender(sTime);
	glPopAttrib();
}


bool CScenePlayer::Load(char *filename)
{
	music.Load("./data/tune.ogg", FSOUND_OUTPUT_DSOUND);
	music.SetTime(0);
	if(GLEW_EXT_framebuffer_object) {
		framebuffer = new CRenderToTexture;
        framebuffer->Create();
	}

	FILE *f_points = fopen(filename, "rb");
	fread(&numsyncpoints, sizeof(unsigned int), 1, f_points);
	fread(syncpoints, sizeof(SyncPoint), numsyncpoints, f_points);
	fclose(f_points);

	for(unsigned int i=0;i<numsyncpoints;i++) {
//		syncpoints[i].offset+= 80;
		music.AddSyncPoint(syncpoints[i].offset, syncpoints[i].name);
	}
	music.SetSyncCallback(synccallback);

	scene00.Load("");
	scenearray[0].p_scene = &scene00;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene00.Play(0, framebuffer);
	glPopAttrib();

	scene01.Load("");
	scenearray[1].p_scene = &scene01;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene01.Play(0, framebuffer);
	glPopAttrib();

	scene02.Load("");
	scenearray[2].p_scene = &scene02;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene02.Play(0, framebuffer);
	glPopAttrib();

	scene03.Load("");
	scenearray[3].p_scene = &scene03;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene03.Play(0, framebuffer);
	glPopAttrib();

	scene04.Load("");
	scenearray[4].p_scene = &scene04;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene04.Play(0, framebuffer);
	glPopAttrib();

	scene05.Load("");
	scenearray[5].p_scene = &scene05;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene05.Play(0, framebuffer);
	glPopAttrib();

	scene06.Load("");
	scenearray[6].p_scene = &scene06;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene06.Play(0, framebuffer);
	glPopAttrib();

	scene07.Load("");
	scenearray[7].p_scene = &scene07;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene07.Play(0, framebuffer);
	glPopAttrib();

	scene08.Load("");
	scenearray[8].p_scene = &scene08;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene08.Play(0, framebuffer);
	glPopAttrib();
	
	scene09.Load("");
	scenearray[9].p_scene = &scene09;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene09.Play(0, framebuffer);
	glPopAttrib();

	scene10.Load("");
	scenearray[10].p_scene = &scene10;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene10.Play(0, framebuffer);
	glPopAttrib();

	return true;
}

/* Destructor */
CScenePlayer::~CScenePlayer()
{
	if(framebuffer)
		delete framebuffer;
}
