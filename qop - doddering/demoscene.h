#pragma once
#include "WickedEngine.h"
#include <fbxsdk.h>
#include "Scenefbx.h"
#include "Sceneloading.h"

typedef struct _sceneTake
{
	SceneFBX* FBXScene;
	float startSceneTime;
	float endSceneTime;
	float startTakeTime;
	int FbxCameraCameraIndex;
	bool operator<(const _sceneTake& rhs) const { return startSceneTime < rhs.startSceneTime; };
} sceneTake;


class sceneManagerEntry
{
	public:
		std::vector<sceneTake> takes;
		sceneManagerEntry() {};
		~sceneManagerEntry() {};

		void addTake(SceneFBX* scene, float startSceneTime, float endSceneTime, float startTakeTime, int FbxCameraIndex)
		{
			sceneTake tk = { 0 };
			tk.FBXScene = scene;
			tk.startSceneTime = startSceneTime;
			tk.endSceneTime = endSceneTime;
			tk.startTakeTime = startTakeTime;
			tk.FbxCameraCameraIndex = FbxCameraIndex;
			takes.push_back(tk);
			// keep it sorted
			std::sort(takes.begin(), takes.end());
		};

		int getTakeIndex(float currentTime)
		{
			for (auto idx = 0; idx < takes.size(); idx++) {
				if(currentTime-takes[idx].startSceneTime < takes[idx].endSceneTime) 	{
					if (idx >= takes.size() - 1) {
						return idx;
					}
					if ((currentTime >= takes[idx].startSceneTime) && (currentTime < takes[idx+1].startSceneTime))	{
						return idx;
					}
				}
			}
			return 0;
		};
};

class Demo : public MainComponent
{

public:

	bool isfading;
	int framenumber;
	float currentSceneStartTime;

	SceneLoader* loader;

	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate() override;
};

