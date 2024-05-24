#pragma once

#include <WickedEngine.h>

using namespace wiECS;
using namespace wiScene;

class SceneLoader : public RenderPath3D_TiledDeferred
{
public:
	wiFadeManager fm;
	wiScene::Scene wScene;
	bool isLoadFinished;
	float loadedPercent;
	bool isfading;

	wiSprite sprite;
	wiSprite sprite2;
	wiSprite sprite3;

	SceneLoader();
	~SceneLoader();

	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate() override;
};
