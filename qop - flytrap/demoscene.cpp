#include <fbxsdk.h>
#include "Scenefbx.h"

//#include <wiGraphicsDevice_DX11.h>

#include "FmodWrapper.h"

#include "demoscene.h"
#include "wiGraphics.h"

#include <string>
#include <sstream>
#include <fstream>

static int scenenum = 0;

wiFadeManager fm;

bool bCapture = true;

using namespace std;
using namespace wiScene;
using namespace wiECS;

CFMod fmodPlayer;
Demo* pThis;

extern HWND gHWND;

sceneManagerEntry sceneTakeData;
#define FRAME(x) (float)(x)/30.f

void onFade()
{
	SendMessage(gHWND, WM_CLOSE, 0, 0);
}

DWORD loadedScenes = 0;
DWORD totalScenes = 15;
HANDLE hTLoader = INVALID_HANDLE_VALUE;

SceneFBX* scene00 = new SceneFBX;
SceneFBX* scene01 = new SceneFBX;
SceneFBX* scene02 = new SceneFBX;
SceneFBX* scene04 = new SceneFBX;
SceneFBX* scene05 = new SceneFBX;
SceneFBX* scene05part2 = new SceneFBX;
SceneFBX* scene06 = new SceneFBX;
SceneFBX* scene07 = new SceneFBX;
SceneFBX* scene07part2 = new SceneFBX;
SceneFBX* scene08 = new SceneFBX;
SceneFBX* scene09 = new SceneFBX;
SceneFBX* scene10 = new SceneFBX;
SceneFBX* scene11 = new SceneFBX;
SceneFBX* scene11part2 = new SceneFBX;
SceneFBX* scene12 = new SceneFBX;

DWORD WINAPI LoaderThread(LPVOID  thisptr)
{
	
		// intro (comms room)
		scene00->Initialize();
		{
			auto& wc = scene00->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
			wc.fogStart = 100;
			wc.fogEnd = 200;
			wc.fogHeight = 0;

		}

		scene00->ImportFBXFile("data/scene00.fbx");
		sceneTakeData.addTake(scene00, 0, (float)23.80f, FRAME(0), 1);
		sceneTakeData.addTake(scene00, FRAME(292), (float)23.80f, FRAME(292), 0);
		loadedScenes++;

		
		// mothership arrival / scouts released
		scene01->Initialize();
		scene01->ImportFBXFile("data/scene01.fbx");
		sceneTakeData.addTake(scene01, 23.80f, (float)71.31f, 0, 0);
		sceneTakeData.addTake(scene01, 54.99f, (float)71.31f, 31.19f, 1);
		loadedScenes++;

		// scouts in flight / approach
		scene02->Initialize();
		scene02->ImportFBXFile("data/scene02-alternate.fbx");
		sceneTakeData.addTake(scene02, 71.31f, (float)87.77f, 0, 4);
		sceneTakeData.addTake(scene02, 74.11f, (float)87.77f, 0, 0);
		sceneTakeData.addTake(scene02, 76.81f, (float)87.77f, FRAME(228), 1);
		sceneTakeData.addTake(scene02, 79.55f, (float)87.77f, FRAME(100), 3);
		sceneTakeData.addTake(scene02, 82.29f, (float)87.77f, FRAME(250), 2);
		sceneTakeData.addTake(scene02, 83.65f, (float)87.77f, FRAME(350), 2);
		sceneTakeData.addTake(scene02, 85.02f, (float)87.77f, FRAME(450), 2);
		sceneTakeData.addTake(scene02, 86.39f, (float)87.77f, FRAME(550), 2);
		loadedScenes++;

		// scouts landing site
		scene04->Initialize();
		scene04->ImportFBXFile("data/scene04.fbx");
		sceneTakeData.addTake(scene04, 87.77f, (float)104.22f, 0.f, 3);
		sceneTakeData.addTake(scene04, 89.13f, (float)104.22f, 100.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 90.51f, (float)104.22f, 200.f / 30.f, 3);

		sceneTakeData.addTake(scene04, 91.88f, (float)104.22f, 300.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 92.14f, (float)104.22f, 350.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 92.40f, (float)104.22f, 400.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 92.57f, (float)104.22f, 450.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 92.74f, (float)104.22f, 350.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 92.91f, (float)104.22f, 400.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 93.00f, (float)104.22f, 350.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 93.09f, (float)104.22f, 400.f / 30.f, 3);
		sceneTakeData.addTake(scene04, 93.17f, (float)104.22f, 350.f / 30.f, 3);

		sceneTakeData.addTake(scene04, 93.26f, (float)104.22f, 100.f / 30.f, 0);
		sceneTakeData.addTake(scene04, 95.99f, (float)104.22f, 100.f / 30.f, 1);
		sceneTakeData.addTake(scene04, 98.74f, (float)104.22f, FRAME(200), 2);
		sceneTakeData.addTake(scene04, 101.49f, (float)104.22f, 280.f / 30.f, 1);

		sceneTakeData.addTake(scene04, 103.71f, (float)104.22f, FRAME(500), 2);
		sceneTakeData.addTake(scene04, 103.88f, (float)104.22f, FRAME(530), 2);
		sceneTakeData.addTake(scene04, 103.97f, (float)104.22f, FRAME(500), 2);
		sceneTakeData.addTake(scene04, 104.06f, (float)104.22f, FRAME(530), 2);
		sceneTakeData.addTake(scene04, 104.14f, (float)104.22f, FRAME(500), 2);
		loadedScenes++;
		
		// scouts exploring site
		scene05->Initialize();
		scene05->ImportFBXFile("data/scene05.fbx");
		sceneTakeData.addTake(scene05, 104.22f, (float)115.20f, FRAME(38), 1);
		sceneTakeData.addTake(scene05, 106.45f, (float)115.20f, FRAME(350), 0);
		sceneTakeData.addTake(scene05, 106.63f, (float)115.20f, FRAME(300), 0);
		sceneTakeData.addTake(scene05, 106.97f, (float)115.20f, FRAME(400), 0);
		sceneTakeData.addTake(scene05, 107.31f, (float)115.20f, FRAME(350), 0);
		sceneTakeData.addTake(scene05, 107.83f, (float)115.20f, FRAME(500), 0);
		sceneTakeData.addTake(scene05, 108.0f, (float)115.20f, FRAME(600), 0);
		sceneTakeData.addTake(scene05, 108.35f, (float)115.20f, FRAME(700), 0);
		//	sceneTakeData.addTake(scene05, 108.68f, (float)115.20f, FRAME(650), 0);
		sceneTakeData.addTake(scene05, 108.85f, (float)115.20f, FRAME(800), 0);
		//	sceneTakeData.addTake(scene05, 109.20f, (float)115.20f, FRAME(650), 0);
		sceneTakeData.addTake(scene05, 109.72f, (float)115.20f, FRAME(1000), 2);
		loadedScenes++;
		
		// pilot1 exploring
		scene05part2->Initialize();
		scene05part2->ImportFBXFile("data/scene05-part2.fbx");
		sceneTakeData.addTake(scene05part2, 115.20f, (float)120.68f, 0, 1);
		sceneTakeData.addTake(scene05part2, 116.20f, (float)120.68f, FRAME(40), 0);
		sceneTakeData.addTake(scene05part2, 117.94f, (float)120.68f, FRAME(80), 1);
		loadedScenes++;

		
		// pilot3 exploring / contact
		scene06->Initialize();
		{
			auto& wc = scene06->GetWeather();
			wc.ambient = XMFLOAT3(0.8f, 0.8f, 0.8f);
			wc.fogStart = 100;
			wc.fogEnd = 200;
			wc.fogHeight = 0;
		}
		scene06->SetWaterPlane();
		scene06->ImportFBXFile("data/scene06.fbx");
		sceneTakeData.addTake(scene06, 120.68f, (float)128.91, 0, 0);
		sceneTakeData.addTake(scene06, 122.05f, (float)128.91, FRAME(70), 0);
		sceneTakeData.addTake(scene06, 123.43f, (float)128.91, FRAME(120), 0);
		sceneTakeData.addTake(scene06, 124.80f, (float)128.91, FRAME(170), 0);
		sceneTakeData.addTake(scene06, 125.49f, (float)128.91, FRAME(230), 0);
		sceneTakeData.addTake(scene06, 126.17f, (float)128.91, FRAME(254), 0);
		sceneTakeData.addTake(scene06, 127.53f, (float)128.91, FRAME(282), 1);
		loadedScenes++;

		// pilot1 contact
		scene07->Initialize();
		scene07->ImportFBXFile("data/scene07.fbx");
		sceneTakeData.addTake(scene07, 128.91f, (float)139.89f, FRAME(0), 0);
		sceneTakeData.addTake(scene07, 130.28f, (float)139.89f, FRAME(80), 0);
		sceneTakeData.addTake(scene07, 131.41f, (float)139.89f, FRAME(222), 1);
		sceneTakeData.addTake(scene07, 138.51f, (float)139.89f, FRAME(500), 2);

		loadedScenes++;
		
		// pilot1 death
		scene07part2->Initialize();
		scene07part2->ImportFBXFile("data/scene07-part2.fbx");
		sceneTakeData.addTake(scene07part2, 139.89f, (float)156.34f, FRAME(0), 0);
		sceneTakeData.addTake(scene07part2, 145.37f, (float)156.34f, FRAME(130), 2);
		sceneTakeData.addTake(scene07part2, 148.11f, (float)156.34f, FRAME(230), 1);
		sceneTakeData.addTake(scene07part2, 149.49f, (float)156.34f, FRAME(266), 2);
		sceneTakeData.addTake(scene07part2, 150.86f, (float)156.34f, FRAME(321), 1);
		loadedScenes++;
	
		// pilot2 in dark
		scene08->Initialize();
		{
			auto& wc = scene08->GetWeather();
			wc.ambient = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		scene08->ImportFBXFile("data/scene08.fbx");
		sceneTakeData.addTake(scene08, 156.34f, 156.34f + FRAME(165), FRAME(0), 0);
		sceneTakeData.addTake(scene08, 156.34f + FRAME(40), 156.34f + FRAME(165), FRAME(66), 1);
		sceneTakeData.addTake(scene08, 156.34f + FRAME(81), 156.34f + FRAME(165), FRAME(81), 0);
		loadedScenes++;

		// pilot3 fight
		scene09->Initialize();
		{
			auto& wc = scene09->GetWeather();
			wc.ambient = XMFLOAT3(0.6f, 0.6f, 0.6f);
	//		wc.fogStart = 100;
	//		wc.fogEnd = 200;
	//		wc.fogHeight = 0;
		}
		scene09->SetWaterPlane();
		scene09->ImportFBXFile("data/scene09.fbx");
		sceneTakeData.addTake(scene09, 161.83f, 183.67f, FRAME(0), 2);
		sceneTakeData.addTake(scene09, 163.20f, 183.67f, FRAME(64), 0);
		sceneTakeData.addTake(scene09, 164.57f, 183.67f, FRAME(120), 2);
		sceneTakeData.addTake(scene09, 165.94f, 183.67f, FRAME(140), 0);
		sceneTakeData.addTake(scene09, 168.68f, 183.67f, FRAME(200), 2);
		sceneTakeData.addTake(scene09, 171.43f, 183.67f, FRAME(254), 0);
		sceneTakeData.addTake(scene09, 172.80f, 183.67f, FRAME(294), 2);
		sceneTakeData.addTake(scene09, 174.17f, 183.67f, FRAME(318), 0);
		sceneTakeData.addTake(scene09, 176.91f, 183.67f, FRAME(415), 0);
		//	sceneTakeData.addTake(scene09, 177.17f, 161.83f, FRAME(415), 0);
		//	sceneTakeData.addTake(scene09, 177.43f, 161.83f, FRAME(415), 0);
		//	sceneTakeData.addTake(scene09, 177.60f, 161.83f, FRAME(415), 0);
		//	sceneTakeData.addTake(scene09, 177.77f, 161.83f, FRAME(415), 0);
		sceneTakeData.addTake(scene09, 178.28f, 183.67f, FRAME(493), 2);
		//	sceneTakeData.addTake(scene09, 179.66f, 161.83f, FRAME(493), 1);
		sceneTakeData.addTake(scene09, 179.88f, 183.67f, FRAME(750), 1);
		//	sceneTakeData.addTake(scene09, 180.09f, 161.83f, FRAME(850), 1);
		sceneTakeData.addTake(scene09, 180.28f, 183.67f, FRAME(850), 1);
		sceneTakeData.addTake(scene09, 181.03f, 183.67f, FRAME(1081), 2);
		//	sceneTakeData.addTake(scene09, 180.48f, 161.83f, FRAME(950), 1);
		//	sceneTakeData.addTake(scene09, 180.63f, 161.83f, FRAME(1050), 1);
		//	sceneTakeData.addTake(scene09, 180.85f, 161.83f, FRAME(1100), 1);
		sceneTakeData.addTake(scene09, 182.39f, 183.67f, FRAME(1170), 1);
		loadedScenes++;
	
		scene10->Initialize();
		{
			auto& wc = scene10->GetWeather();
			wc.ambient = XMFLOAT3(0.2f, 0.2f, 0.2f);
			wc.fogStart = 100;
			wc.fogEnd = 200;
			wc.fogHeight = 0;
		}
		scene10->ImportFBXFile("data/scene10.fbx");
		sceneTakeData.addTake(scene10, 183.67f, 205.71f, FRAME(0), 0);
		//	sceneTakeData.addTake(scene10, 189.25f, 205.71f, FRAME(188), 0);
		sceneTakeData.addTake(scene10, 194.74f, 205.71f, FRAME(296), 1);
		sceneTakeData.addTake(scene10, 200.22f, 205.71f, FRAME(630), 2);
		loadedScenes++;
	
		// pilot2 liftoff
		scene11->Initialize();
		scene11->ImportFBXFile("data/scene11.fbx");
		sceneTakeData.addTake(scene11, 205.71f, 211.20f, FRAME(0), 2);
		sceneTakeData.addTake(scene11, 206.06f, 211.20f, FRAME(100), 2);
		sceneTakeData.addTake(scene11, 206.57f, 211.20f, FRAME(150), 2);
		sceneTakeData.addTake(scene11, 206.72f, 211.20f, FRAME(200), 2);
		sceneTakeData.addTake(scene11, 207.09f, 211.20f, FRAME(250), 2);
		sceneTakeData.addTake(scene11, 207.43f, 211.20f, FRAME(300), 2);
		sceneTakeData.addTake(scene11, 207.69f, 211.20f, FRAME(350), 2);
		sceneTakeData.addTake(scene11, 207.94f, 211.20f, FRAME(400), 0);
		sceneTakeData.addTake(scene11, 208.12f, 211.20f, FRAME(450), 0);
		sceneTakeData.addTake(scene11, 208.45f, 211.20f, FRAME(500), 0);
		sceneTakeData.addTake(scene11, 208.63f, 211.20f, FRAME(550), 1);
		sceneTakeData.addTake(scene11, 208.80f, 211.20f, FRAME(600), 1);
		sceneTakeData.addTake(scene11, 209.31f, 211.20f, FRAME(650), 1);
		sceneTakeData.addTake(scene11, 209.49f, 211.20f, FRAME(690), 1);
		sceneTakeData.addTake(scene11, 209.83f, 211.20f, FRAME(700), 0);
		sceneTakeData.addTake(scene11, 210.09f, 211.20f, FRAME(750), 0);
		sceneTakeData.addTake(scene11, 210.34f, 211.20f, FRAME(780), 0);
		sceneTakeData.addTake(scene11, 210.51f, 211.20f, FRAME(810), 0);
		sceneTakeData.addTake(scene11, 210.69f, 211.20f, FRAME(840), 0);
		sceneTakeData.addTake(scene11, 210.86f, 211.20f, FRAME(850), 0);
		sceneTakeData.addTake(scene11, 211.03f, 211.20f, FRAME(860), 0);
		sceneTakeData.addTake(scene11, 211.11f, 211.20f, FRAME(870), 0);
		loadedScenes++;

		// pilot2 locked in by turrets
		scene11part2->Initialize();
		scene11part2->ImportFBXFile("data/scene11-part2.fbx");
		sceneTakeData.addTake(scene11part2, 211.20f, 227.66f, FRAME(0), 0);
		sceneTakeData.addTake(scene11part2, 216.69f, 227.66f, FRAME(170), 3);
		sceneTakeData.addTake(scene11part2, 219.42f, 227.66f, FRAME(239), 2);
		sceneTakeData.addTake(scene11part2, 222.17f, 227.66f, FRAME(246), 1);
		loadedScenes++;
	
		// credits / fin
		scene12->Initialize();
		scene12->ImportFBXFile("data/scene12.fbx");
		sceneTakeData.addTake(scene12, 227.66f, 276.74f, FRAME(0), 0);
		loadedScenes++;

		return 0;
}



void Demo::Initialize()
{
	string fontpath = wiHelper::GetOriginalWorkingDirectory() + "fonts/";
	string shaderpath = wiHelper::GetOriginalWorkingDirectory() + "shaders/";
	wiRenderer::SetShaderPath(shaderpath);
	wiFont::SetFontPath(fontpath);

	__super::Initialize();

	pThis = this;

#ifdef _DEBUG
	infoDisplay.active = true;
	infoDisplay.watermark = true;
	infoDisplay.fpsinfo = true;
	infoDisplay.resolution = true;
#endif

	isfading = false;
	scenenum = 0;
	currentSceneStartTime = 0;

	DWORD tid = 0;
	hTLoader = CreateThread(0, 0, LoaderThread, this, 0, &tid);

	loader = new SceneLoader;
	loader->Initialize();
	ActivatePath(loader, 1, wiColor::Black());
}

static int lasttakeidx=-1;
SceneFBX* lastfbxscene = (SceneFBX * )-1;
static int lastCameraIndex=-1;

bool isSoundTrackLoaded = false;

void Demo::Update(float dt)
{
	if (!loader->isLoadFinished) {
		loader->Update(dt);
		return;
	}
	if (!isSoundTrackLoaded) {
		fmodPlayer.Load("data/soundtrack.mp3", 0);
		fmodPlayer.Play();
		fmodPlayer.SetTime(sceneTakeData.takes[0].startSceneTime * 1000);
		isSoundTrackLoaded = true;
	}

	if (fmodPlayer.GetTime() > sceneTakeData.takes.back().endSceneTime-3.f) {
		if (!isfading) {
			fadeManager = fm;
			fadeManager.Start(3, wiColor::Black(), onFade);
			isfading = true;
		}
	}

	/*
	if (isSoundTrackLoaded && !fmodPlayer.isPlaying()) {
		onFade();
	}*/

	/*
	if (sceneTakeData.takes.back().endSceneTime < fmodPlayer.GetTime()) {
		fmodPlayer.SetTime(sceneTakeData.takes[0].startSceneTime * 1000);
	}*/

	int takeidx = sceneTakeData.getTakeIndex(fmodPlayer.GetTime());
	if (takeidx != lasttakeidx) {
		sceneTakeData.takes[takeidx].FBXScene->FBXSceneStartTime = fmodPlayer.GetTime();
		sceneTakeData.takes[takeidx].FBXScene->FBXTakeStartTime = sceneTakeData.takes[takeidx].startTakeTime;
//		if (sceneTakeData.takes[takeidx].FbxCameraCameraIndex != lastCameraIndex) {
			sceneTakeData.takes[takeidx].FBXScene->ActivateCamera(sceneTakeData.takes[takeidx].FbxCameraCameraIndex);
			lastCameraIndex = sceneTakeData.takes[takeidx].FbxCameraCameraIndex;
//		}
		if (sceneTakeData.takes[takeidx].FBXScene != lastfbxscene) {
			if (lastfbxscene == (SceneFBX*)-1) {
				ActivatePath(sceneTakeData.takes[takeidx].FBXScene, 1, wiColor::Black());
			} else
			ActivatePath(sceneTakeData.takes[takeidx].FBXScene);
			lastfbxscene = sceneTakeData.takes[takeidx].FBXScene;
		}
		lasttakeidx = takeidx;
	}
	FMOD_System_Update(fmodPlayer.hFmod);
	sceneTakeData.takes[takeidx].FBXScene->Update(dt);
//	MainComponent::Update(dt);
}


void Demo::FixedUpdate()
{
	__super::FixedUpdate();
//	MainComponent::FixedUpdate();
}
