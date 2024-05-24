#include "sceneloading.h"
#include "FmodWrapper.h"
#include "demoscene.h"

using namespace std;
using namespace wiScene;
using namespace wiECS;

extern DWORD loadedScenes;
extern DWORD totalScenes;
extern HANDLE hTLoader;

extern CFMod fmodPlayer;

extern sceneManagerEntry sceneTakeData;

SceneLoader* pThis;

extern string getFromTexturePath(string str);



void onLoaderHasFinished()
{
	pThis->isLoadFinished = true;
}

SceneLoader::SceneLoader()
{
	isLoadFinished = false;
    loadedPercent = 0;
	isfading = false;
}

SceneLoader::~SceneLoader()
{

}

void SceneLoader::Initialize()
{
    __super::Initialize();

	pThis = this;

    // Reset all state that tests might have modified:
//	wiRenderer::SetToDrawGridHelper(true);
//    wiRenderer::SetTemporalAAEnabled(true);

//    wiRenderer::ClearWorld();

    wScene.Clear();

    wScene.cameras.Clear();

    wScene.materials.Clear();
    wScene.lights.Clear();
    wScene.weathers.Clear();

	ClearSprites();

	const float screenW = wiRenderer::GetDevice()->GetScreenWidth();
	const float screenH = wiRenderer::GetDevice()->GetScreenHeight();

	{
		const XMFLOAT3 startPos = XMFLOAT3(screenW * 0.5f, screenH * 0.5f, 0);
		wiImageParams params;
		params.pos = startPos;
		params.siz = XMFLOAT2(1024, 512);
		params.pivot = XMFLOAT2(0.5f, 0.5f);
		params.quality = QUALITY_LINEAR;
		params.sampleFlag = SAMPLEMODE_CLAMP;
		params.blendFlag = BLENDMODE_ALPHA;

		sprite = wiSprite(getFromTexturePath("data/loader-back.png"));
		sprite.params = params;
		AddSprite(&sprite);
	}

	{
		if (!isfading) {
			if (WaitForSingleObject(hTLoader, 5) != WAIT_TIMEOUT) {
				loadedPercent = 100.f;
				isfading = true;
			}
		}

		const XMFLOAT3 startPos = XMFLOAT3(screenW * 0.5002f, screenH * 0.446f, 0);
		wiImageParams params;
		params.pos = startPos;
		params.siz = XMFLOAT2(loadedPercent * 804.f / 100.f, 14);
		params.pivot = XMFLOAT2(0.5f, 0.5f);
		params.quality = QUALITY_LINEAR;
		params.sampleFlag = SAMPLEMODE_CLAMP;
		params.blendFlag = BLENDMODE_ALPHA;

		sprite2 = wiSprite(getFromTexturePath("data/fullwhite.dds"));
		sprite2.params = params;
		AddSprite(&sprite2);
	}

	{
		const XMFLOAT3 startPos = XMFLOAT3(screenW * 0.5f, screenH * 0.5f, 0);
		wiImageParams params;
		params.pos = startPos;
		params.siz = XMFLOAT2(1024, 512);
		params.pivot = XMFLOAT2(0.5f, 0.5f);
		params.quality = QUALITY_LINEAR;
		params.sampleFlag = SAMPLEMODE_CLAMP;
		params.blendFlag = BLENDMODE_ALPHA;

		sprite3 = wiSprite(getFromTexturePath("data/loader-front.png"));
		sprite3.params = params;
		AddSprite(&sprite3);
	}


}

void SceneLoader::Update(float dt)
{
	const float screenW = wiRenderer::GetDevice()->GetScreenWidth();
	const float screenH = wiRenderer::GetDevice()->GetScreenHeight();

	{
		if (!isfading) {
			if (WaitForSingleObject(hTLoader, 5) != WAIT_TIMEOUT) {
				loadedPercent = 100.f;
				isfading = true;
			}
		}

		const XMFLOAT3 startPos = XMFLOAT3(screenW * 0.5002f, screenH * 0.446f, 0);
		wiImageParams params;
		params.pos = startPos;
		params.siz = XMFLOAT2(loadedPercent * 804.f / 100.f, 14);
		params.pivot = XMFLOAT2(0.5f, 0.5f);
		params.quality = QUALITY_LINEAR;
		params.sampleFlag = SAMPLEMODE_CLAMP;
		params.blendFlag = BLENDMODE_ALPHA;

		sprite2.params = params;
	}

    __super::Update(dt);
}

void SceneLoader::FixedUpdate()
{

	if (loadedPercent >= 100.f) {
		loadedPercent = 100.f;
		isLoadFinished = true;
	}
	else {
		loadedPercent = loadedScenes * 100.f / totalScenes-1;
	}

    __super::FixedUpdate();
}

