#pragma once
#include "WickedEngine.h"
#include <lib3ds.h>


class Scene3DS : public RenderPath3D_Forward
{
public:
	Lib3dsFile *c3ds_file;
	int activeCamera;
	float times;

	Scene3DS();
	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate() override;

	void UpdateCamera_3DS(char *camera);
	void UpdateAnimation_3DS();
	void Load();
	void ProcessNode(Lib3dsNode *node);
	void ProcessNodeAnim(Lib3dsNode *node);
	void ImportScene_3DS(const std::string& fileName, wiScene::Scene& scene);

};
