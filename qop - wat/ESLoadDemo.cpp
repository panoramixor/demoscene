#include "stdafx.h"

extern float fTime;
extern float fDeltaTime;

extern int SequenceLoad();

static CPicture backgrnd;
static bool b3DSLoaded, bPBuffer, bPicturesLoaded, bSkyboxLoaded;
HANDLE h3DSLoaded, hPBuffer, hPicturesLoaded, hSkyboxLoaded;
static bool bLoader=false;
extern void FatalError(char *formatstring, char *datastring);
extern bool InitPBuffers(void);
extern void Stop_Music();
extern void Play_Music();
extern HDC hDC;
extern HGLRC hRC;
extern void Init_Music(char *filename, int outputtype);															   // Start the music
extern bool global_nosound;
extern HWND hOptionsDlg;

// models

Model_3DS scene01_3ds;
Model_3DS scene02_3ds;
Model_3DS scene03_walls_3ds;
Model_3DS scene03_temple_3ds;
Model_3DS scene04_3ds;
Model_3DS scene05_3ds;
Model_3DS scene06_building;
Model_3DS scene06_terrain;
Model_3DS scene07_3ds;
Model_3DS scene08_3ds;

// pictures

CPicture scene00_picture0;
CPicture scene00_picture1;
CPicture scene00_picture2;
CPicture scene00_picture3;
CPicture flux_picture;
CPicture fractalia_picture;
//CPicture scene03_picture0;
//CPicture scene05_picture0;
CPicture scene10_credits; 

// screenshots
CPicture scene01_screen;
CPicture scene02_screen;
CPicture scene03_screen;
CPicture scene04_screen;
CPicture scene05_screen;
CPicture scene06_screen;
CPicture scene07_screen;
CPicture scene08_screen;
CPicture scene09_screen; 
CPicture scene10_screen;
CPicture scene10_theend;

// textures
CTexture ESNoise;
CTexture scene01_envmap0;
CTexture water;
CTexture mask;
CTexture particles;

// skybox

CSkybox scene01_skybox;
CSkybox scene03_skybox;
CSkybox scene06_skybox;
CSkybox scene07_skybox;

CCamPath scene01_campath;
CCamPath scene02_campath;
CCamPath scene03_campath;
CCamPath scene04_campath;
CCamPath scene05_campath;
CCamPath scene06_campath;
CCamPath scene07_campath;
CCamPath scene08_campath;
CCamPath scene09_campath;
CCamPath scene10_campath;

void ProgressBar(float percent)
{
	SendDlgItemMessage(hOptionsDlg, IDC_PROGRESS1, PBM_SETPOS, (int) percent, 0);

	if(percent>100) percent = 100;
	glBegin(GL_QUADS);		
		glVertex3f(-2.5f,   0, -2);
		glVertex3f(-2.5f,   0.2f, -2);
		glVertex3f(-2.5f+((percent*5)/100), 0.2f, -2); 
		glVertex3f(-2.5f+((percent*5)/100), 0, -2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glFlush();
	return;
}

void Load3DS()
{
	scene01_3ds.Load("./data/scenes/scene01/models/scene01-2.3ds");
	scene02_3ds.Load("./data/scenes/scene02/models/scene02.3ds");
	scene03_temple_3ds.Load("./data/scenes/scene03/models/temple/temple.3ds");
	scene03_walls_3ds.Load("./data/scenes/scene03/models/walls/walls.3ds");
	scene04_3ds.Load("./data/scenes/scene04/models/scene04_column.3ds");
	scene05_3ds.Load("./data/scenes/scene05/models/scene05.3ds");
	scene06_building.Load("./data/scenes/scene06/models/scene06.3ds");
	scene06_terrain.Load("./data/scenes/scene06/models/terrain.3ds");
	scene07_3ds.Load("./data/scenes/scene07/models/scene072.3ds");
	scene08_3ds.Load("./data/scenes/scene08/models/scene082.3ds");
	b3DSLoaded = true;
	return;
}

void LoadPictures()
{
	scene00_picture3.Create(0,0,-2,5.34f,4,"./data/scenes/scene00/pictures/wat.jpg", "./data/scenes/scene00/pictures/watmask.jpg");
	scene01_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene01.jpg", NULL);
	scene02_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene02.jpg", NULL);
	scene03_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene03.jpg", NULL);
	scene04_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene04.jpg", NULL);
	scene05_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene05.jpg", NULL);
	scene06_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene06.jpg", NULL);
	scene07_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene07.jpg", NULL);
	scene08_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene08.jpg", NULL);
	scene09_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene09.jpg", NULL);
	scene10_screen.Create(0,0,-2,5.34f,4,"./data/screenshot/scene10.jpg", NULL);
	scene10_credits.Create(0,0,-2,5.34f,4,"./data/screenshot/mask.jpg", "./data/scenes/scene10/pictures/mask.jpg");
	scene10_theend.Create(0,0,-2,5.34f,4,"./data/screenshot/mask2.jpg", NULL);

	particles.Load("./data/textures/particle.bmp");

	ESNoise.LoadJPG("./data/textures/noise.jpg");
	scene01_envmap0.LoadEnvMapJPG("./data/textures/envmap0.jpg");
	water.Load("./data/textures/water.jpg");
	mask.Load("./data/screenshot/mask.jpg");

	bPicturesLoaded = true;
	return;
}

void LoadSkybox()
{
	scene01_skybox.Create(0, 0, 0, 80, 20, 80, "./data/scenes/scene01/skybox/LMmnts_2");
	scene03_skybox.Create(0, 0, 0, 80, 20, 80, "./data/scenes/scene03/skybox/inletdusk");
	scene06_skybox.Create(0, 0, 0, 80, 60, 80, "./data/scenes/scene06/skybox/arcn");
	scene07_skybox.Create(0, 0, 0, 80, 20, 80, "./data/scenes/scene03/skybox/inletdusk");
	bSkyboxLoaded = true;
	return;
}

void LoadPBuffer()
{
	if(!InitPBuffers())
		FatalError("Cant initialize %s!", "PBuffers");
	
	bPBuffer = true;
	return;
}


DWORD WINAPI LoaderThread(LPVOID lpParam) 
{
	h3DSLoaded = CreateEvent(NULL, 1, 0, "3DS");
	hPicturesLoaded = CreateEvent(NULL, 1, 0, "PICS");
	hPBuffer = CreateEvent(NULL, 1, 0, "PBUFFER");
	hSkyboxLoaded = CreateEvent(NULL, 1, 0, "SKY");

	while(1) {
		if(WaitForSingleObject(hPicturesLoaded, 10) == WAIT_OBJECT_0) 
				SendDlgItemMessage(hOptionsDlg, IDC_PROGRESS1, PBM_SETPOS, 25, 0);
		
		if(WaitForSingleObject(h3DSLoaded, 10)== WAIT_OBJECT_0) 
				SendDlgItemMessage(hOptionsDlg, IDC_PROGRESS1, PBM_SETPOS, 50, 0);

		if(WaitForSingleObject(hSkyboxLoaded, 10)== WAIT_OBJECT_0)  
				SendDlgItemMessage(hOptionsDlg, IDC_PROGRESS1, PBM_SETPOS, 75, 0);

		if(WaitForSingleObject(hPBuffer, 10)== WAIT_OBJECT_0) {
				SendDlgItemMessage(hOptionsDlg, IDC_PROGRESS1, PBM_SETPOS, 100, 0);
				break;
		}
	}
	return 0;
}

bool LoadNemo(HWND hwnd)
{
	static HANDLE thread;
	DWORD threadstatus;
	glDrawBuffer(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT);
	if(!bLoader) {
		DWORD threadid;
		h3DSLoaded = CreateEvent(NULL, 0, 0, "3DS");
		hPicturesLoaded = CreateEvent(NULL, 0, 0, "PICS");
		hPBuffer = CreateEvent(NULL, 0, 0, "PBUFFER");
		hSkyboxLoaded = CreateEvent(NULL, 0, 0, "SKY");
		thread = CreateThread(NULL, 0, LoaderThread, (LPVOID) hwnd, 0, &threadid);
		bLoader = true;
	} else {
		GetExitCodeThread(thread, &threadstatus);
		if(threadstatus == STILL_ACTIVE) {
			WaitForSingleObject(hPicturesLoaded, 10);
			if(GetLastError() == WAIT_OBJECT_0) {
				ProgressBar(25);
				WaitForSingleObject(h3DSLoaded, 10);
				if(GetLastError() == WAIT_OBJECT_0) {
					ProgressBar(50);
					WaitForSingleObject(hPBuffer, 10);
					if(GetLastError() == WAIT_OBJECT_0) {
						ProgressBar(75);
						WaitForSingleObject(hSkyboxLoaded, 10);
						if(GetLastError() == WAIT_OBJECT_0) {
							ProgressBar(100);
							return true;
						}
					}
				}
			}
			return false;
		} else {
			return true;
		}
	}
	return false;
}

bool LoadDemo(HWND hwnd)
{
	if(!bLoader) {
		b3DSLoaded = false;
		bPBuffer = false;
		bPicturesLoaded = false;
		bSkyboxLoaded = false;
		SequenceLoad();
		ProgressBar(5);	
		bLoader = true;
		return false;
	}

	if(!bPicturesLoaded)
	{
		LoadPictures();
		ProgressBar(25);
		return false;
	}	

	ProgressBar(35);	

	if(!b3DSLoaded)
	{
		Load3DS();
		ProgressBar(50);	
		return false;
	}	
	
	ProgressBar(60);

	if(!bSkyboxLoaded)
	{
		LoadSkybox();
		ProgressBar(75);
		return false;
	}

	ProgressBar(85);

	if(!bPBuffer)
	{
		LoadPBuffer();
		ProgressBar(100);
		return false;
	}
	ProgressBar(100);

	scene01_campath.Load("./data/scenes/scene01/scene01.esc");
	scene02_campath.Load("./data/scenes/scene02/scene02.esc");
	scene03_campath.Load("./data/scenes/scene03/scene03.esc");
	scene04_campath.Load("./data/scenes/scene04/scene04.esc");
	scene05_campath.Load("./data/scenes/scene05/scene05.esc");
	scene06_campath.Load("./data/scenes/scene06/scene06.esc");
	scene07_campath.Load("./data/scenes/scene07/scene07.esc");
	scene08_campath.Load("./data/scenes/scene08/scene08.esc");
	scene09_campath.Load("./data/scenes/scene09/scene09.esc");
	scene10_campath.Load("./data/scenes/scene10/scene10.esc");

	if(!global_nosound) {
		Init_Music("./data/music/tune2.ogg", FSOUND_OUTPUT_DSOUND);
	} else {
		Init_Music("./data/music/tune2.ogg", FSOUND_OUTPUT_NOSOUND);
	}
	Play_Music();
	return true;
}



void UnLoadDemo()
{

	// Stop Music
	Stop_Music();
	return;
}