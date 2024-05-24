// main scene secuencer 

#include <fbxsdk.h>
#include "Scenefbx.h"

#include "FmodWrapper.h"

#include "demoscene.h"
#include "wiGraphics.h"

#include <string>
#include <sstream>
#include <fstream>

static int scenenum = 0;

wiFadeManager fm;

//bool bCapture = true;

using namespace std;
using namespace wiScene;
using namespace wiECS;

CFMod fmodPlayer;
Demo* pThis;

extern HWND gHWND;

sceneManagerEntry sceneTakeData;
#define FRAME(x) (float)(x)/30.f

void lOnFade()
{
	Sleep(1);	// yield just to be sure
	SendMessage(gHWND, WM_CLOSE, 0, 0);
}

SceneFBX* scene00 = new SceneFBX;
SceneFBX* scene00b = new SceneFBX;
SceneFBX* scene01 = new SceneFBX;
SceneFBX* scene02 = new SceneFBX;
SceneFBX* scene03 = new SceneFBX;
SceneFBX* scene03b = new SceneFBX;
SceneFBX* scene04 = new SceneFBX;

// loader/progress related
DWORD loadedScenes = 0;			// current number of loaded scenes
DWORD totalScenes = 8;			// total scenes to load
HANDLE hTLoader = INVALID_HANDLE_VALUE;



// soundtrack timing
/*
18.109781	bassdrop
20.925099	bassdrop2
23.713109	bassdrop3
26.509312	kick1
29.294592	kick2
34.843307	kick2
37.639509	roll
40.435712	action
43.210069	oh
46.028117	action2
48.802475	oh2
51.598677	action3
54.373035	oh3
57.191083	action4
59.965440	roll2
62.761643	calmness
64.181589	doddering
66.966869	d2
69.757611	d3
72.553813	d3
75.339093	d5
78.129835	d6
80.926037	d7
83.722240	d8
85.295104	roller

86.502059	finish
89.298261	tomtom1
92.083541	finish2
94.874283	tomtom2
97.659563	finish3
100.455765	tomtom3
103.246507	tomtomtom
106.037248	ultimo
108.822528	finakick
122.790688	fin

		*/


DWORD WINAPI LoaderThread(LPVOID  thisptr)
{
	
		// intro 
		scene00->Initialize();
		{
			auto& wc = scene00->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}

		scene00->ImportFBXFile("data/scene00.fbx");
		{
			scene00->setDepthOfFieldEnabled(true);
			scene00->setDepthOfFieldStrength(2);
//			scene00->setDepthOfFieldFocus(4000);
			scene00->setBloomEnabled(true);
			scene00->setBloomThreshold(0.2f);
			scene00->setMotionBlurEnabled(true);
			scene00->setMotionBlurStrength(10);
		}
		// 18.109781
		sceneTakeData.addTake(scene00, 0, 18.109781f, FRAME(0), 0);
		loadedScenes++;

		// bass drop (FP 2023 THING)
		scene00b->Initialize();
		{
			auto& wc = scene00b->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}

		scene00b->ImportFBXFile("data/scene00b.fbx");
		{
			scene00b->setDepthOfFieldEnabled(true);
			scene00b->setDepthOfFieldStrength(2);
			scene00b->setBloomEnabled(true);
			scene00b->setBloomThreshold(0.2f);
			scene00b->setMotionBlurEnabled(true);
			scene00b->setMotionBlurStrength(10);
		}
		sceneTakeData.addTake(scene00b, 18.109781f, 26.509312f, 0, 0);
		loadedScenes++;

		// illustrious occasion
		scene01->Initialize();
		{
			auto& wc = scene01->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}
		scene01->ImportFBXFile("data/scene01.fbx");
		{
			scene01->setDepthOfFieldEnabled(true);
			scene01->setDepthOfFieldStrength(2);
			scene01->setBloomEnabled(true);
			scene01->setBloomThreshold(0.6f);
			scene01->setMotionBlurEnabled(true);
			scene01->setMotionBlurStrength(10);
		}
		sceneTakeData.addTake(scene01, 26.509312f, 26.509312f+FRAME(418), 0, 0);
		loadedScenes++;
		
		scene02->Initialize();
		{
			auto& wc = scene02->GetWeather();
			wc.ambient = XMFLOAT3(0.5f, 0.5f, 0.5f);
		}
		scene02->ImportFBXFile("data/scene02.fbx");
		{
				scene02->setBloomEnabled(true);
				scene02->setBloomThreshold(0.2f);
				scene02->setDepthOfFieldEnabled(true);
				scene02->setDepthOfFieldStrength(2);
				scene02->setLightShaftsEnabled(true);
				scene02->setVolumeLightsEnabled(true);
		}
		sceneTakeData.addTake(scene02, 40.435712f, 40.435712f +FRAME(168), FRAME(0), 0);
		sceneTakeData.addTake(scene02, 40.435712f +FRAME(168), 40.435712f +FRAME(336), FRAME(168), 1);
		sceneTakeData.addTake(scene02, 40.435712f +FRAME(336), 40.435712f +FRAME(336+168), FRAME(336), 0);
		sceneTakeData.addTake(scene02, 40.435712f +FRAME(336+168), 40.435712f +FRAME(670), FRAME(504), 1);
		loadedScenes++;
		
		// ----------------------
		scene03->Initialize();
		{
			auto& wc = scene03->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}
		scene03->ImportFBXFile("data/scene03.fbx");
		{
			scene03->setDepthOfFieldEnabled(true);
			scene03->setDepthOfFieldStrength(2);
			//		scene00->setDepthOfFieldFocus(4000);
			scene03->setBloomEnabled(true);
			scene03->setBloomThreshold(0.13f);
			scene03->setMotionBlurEnabled(true);
			scene03->setMotionBlurStrength(10);
		}
		sceneTakeData.addTake(scene03, 62.761643f, 64.181589f, FRAME(336), 0);
		sceneTakeData.addTake(scene03, 64.181589f, 64.181589f +FRAME(418), FRAME(0), 1);
		sceneTakeData.addTake(scene03, 64.181589f +FRAME(418), 64.181589f +FRAME(586), FRAME(335), 0);
		sceneTakeData.addTake(scene03, 64.181589f + FRAME(586), 64.181589f + FRAME(670), FRAME(418), 1);
		loadedScenes++;
		
		scene04->Initialize();
		scene04->ImportFBXFile("data/scene05.fbx");
		{

				auto& wc = scene04->GetWeather();
				wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
				scene04->setDepthOfFieldEnabled(true);
				scene04->setDepthOfFieldStrength(2);
				//		scene00->setDepthOfFieldFocus(4000);
				scene04->setBloomEnabled(true);
				scene04->setBloomThreshold(0.419f);
				scene04->setMotionBlurEnabled(true);
				scene04->setMotionBlurStrength(10);
				scene04->setEyeAdaptionEnabled(true);
		}
		sceneTakeData.addTake(scene04, 86.502059f, 86.502059f + FRAME(670), FRAME(0), 0);
		loadedScenes++;

		
		scene03b->Initialize();
		scene03b->ImportFBXFile("data/scene04.fbx");
		{

			auto& wc = scene03b->GetWeather();
			wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
			scene03b->setDepthOfFieldEnabled(false);
			scene03b->setDepthOfFieldStrength(0.2f);
			//		scene00->setDepthOfFieldFocus(4000);

			scene03b->setBloomEnabled(true);
			scene03b->setBloomThreshold(0.2f);
			scene03b->setMotionBlurEnabled(true);
			scene03b->setMotionBlurStrength(10);
			scene03b->setEyeAdaptionEnabled(true);

		}
		sceneTakeData.addTake(scene03b, 86.502059f + FRAME(670), 86.502059f + FRAME(670 + 418), FRAME(0), 0);
		loadedScenes++;
		return 0;
}


void Demo::Initialize()
{
	string fontpath = wiHelper::GetOriginalWorkingDirectory() + "data/fonts/";
	string shaderpath = wiHelper::GetOriginalWorkingDirectory() + "data/shaders/";
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

	// lauch scene loading thread
	DWORD tid = 0;
	hTLoader = CreateThread(0, 0, LoaderThread, this, 0, &tid);

	// activate loader bar renderpath
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
		fmodPlayer.Load("data/phienex - doddering.ogg", 0);
		fmodPlayer.Play();
		fmodPlayer.SetTime(sceneTakeData.takes[0].startSceneTime * 1000);
		isSoundTrackLoaded = true;
	}

	if (fmodPlayer.GetTime() > sceneTakeData.takes.back().endSceneTime-3.f) {
		if (!isfading) {
			fadeManager = fm;
			fadeManager.Start(3, wiColor::Black(), lOnFade);
			isfading = true;
		}
	}
	
	// ---------------------
	if (isSoundTrackLoaded && !fmodPlayer.isPlaying()) {
		Sleep(1);	// yield just to be sure
		SendMessage(gHWND, WM_CLOSE, 0, 0);
		Sleep(25);	// yield just to be sure
		ExitProcess(0);
	}

	int takeidx = sceneTakeData.getTakeIndex(fmodPlayer.GetTime());
	if (takeidx != lasttakeidx) {
		sceneTakeData.takes[takeidx].FBXScene->FBXSceneStartTime = fmodPlayer.GetTime();
		sceneTakeData.takes[takeidx].FBXScene->FBXTakeStartTime = sceneTakeData.takes[takeidx].startTakeTime;
		sceneTakeData.takes[takeidx].FBXScene->ActivateCamera(sceneTakeData.takes[takeidx].FbxCameraCameraIndex);
		lastCameraIndex = sceneTakeData.takes[takeidx].FbxCameraCameraIndex;
		if (sceneTakeData.takes[takeidx].FBXScene != lastfbxscene) {
			if (lastfbxscene == (SceneFBX*)-1) {
				ActivatePath(sceneTakeData.takes[takeidx].FBXScene, 1, wiColor::Black());
			} else {
				ActivatePath(sceneTakeData.takes[takeidx].FBXScene);
			}
			lastfbxscene = sceneTakeData.takes[takeidx].FBXScene;
		}
		lasttakeidx = takeidx;
	}
	FMOD_System_Update(fmodPlayer.hFmod);
	sceneTakeData.takes[takeidx].FBXScene->Update(dt);
}


void Demo::FixedUpdate()
{
	__super::FixedUpdate();
}
