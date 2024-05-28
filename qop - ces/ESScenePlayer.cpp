// Scene sequencer/player class

#include "stdafx.h"
#include "commctrl.h"
#include "resource1.h"

extern signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata);
extern float sTime;
extern float fTime;
extern HWND  g_hWndOptions;									// This is the handle for the options dialog

CFMod music;

CScene01 scene01;
CScene02 scene02;
CScene03 scene03;
CScene04 scene04;
CScene05 scene05;
CScene06 scene06;
CScene07 scene07;
CScene08 scene08;
CScene09 scene09;
CScene10 scene10;
CScene11 scene11;
CScene12 scene12;
CScene13 scene13;
CScene14 scene14;

// 
CPicture borde;

/* Constructor */
CScenePlayer::CScenePlayer()
{
	take = -1;
	sceneindex = 0;
}


// plays scenes
void CScenePlayer::Play()
{
//	glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHTING);
//	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//sTime = music.GetTime();
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

			if(sceneindex > 13) {
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

	if(sceneindex != 13)
		borde.Draw();
}

// preloads all scenes
bool CScenePlayer::Load(char *filename)
{
	music.Load("./data/music/demotheme.ogg", FSOUND_OUTPUT_DSOUND);
	borde.Create(0,0,-1.f,2.0f,1.5f,"./data/borde3.tga", 255, 255, 255);

	FILE *f_points = fopen(filename, "rb");
	fread(&numsyncpoints, sizeof(unsigned int), 1, f_points);
	fread(syncpoints, sizeof(SyncPoint), numsyncpoints, f_points);
	fclose(f_points);

	for(unsigned int i=0;i<numsyncpoints;i++) {
		music.AddSyncPoint(syncpoints[i].offset, syncpoints[i].name);
	}
	music.SetSyncCallback(synccallback);

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 30, 0);
	scene01.Load("");
	scenearray[0].p_scene = &scene01;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene01.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 35, 0);

	scene02.Load("");
	scenearray[1].p_scene = &scene02;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene02.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 40, 0);

	scene03.Load("");
	scenearray[2].p_scene = &scene03;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene03.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 45, 0);

	scene04.Load("");
	scenearray[3].p_scene = &scene04;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene04.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 50, 0);

	scene05.Load("");
	scenearray[4].p_scene = &scene05;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene05.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 55, 0);

	scene06.Load("");
	scenearray[5].p_scene = &scene06;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene06.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 60, 0);

	scene07.Load("");
	scenearray[6].p_scene = &scene07;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene07.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 65, 0);

	scene12.Load("");
	scenearray[7].p_scene = &scene12;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene12.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 70, 0);

	scene09.Load("");
	scenearray[8].p_scene = &scene09;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene09.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 75, 0);

	scene10.Load("");
	scenearray[9].p_scene = &scene10;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene10.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 80, 0);

	scene11.Load("");
	scenearray[10].p_scene = &scene11;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene11.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 85, 0);

	scene08.Load("");
	scenearray[11].p_scene = &scene08;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene08.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 90, 0);

	scene14.Load("");
	scenearray[12].p_scene = &scene14;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene14.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 95, 0);

	scene13.Load("");
	scenearray[13].p_scene = &scene13;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	scene13.Play(0);
	glPopAttrib();

	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 100, 0);
	Sleep(30);
	SendMessage(g_hWndOptions, WM_COMMAND, 0xDEAD, 0);
	music.Play();
	return true;
}

/* Destructor */
CScenePlayer::~CScenePlayer()
{

}
