#pragma once

#include <fbxsdk.h>
#include <WickedEngine.h>

using namespace wiECS;
using namespace wiScene;

typedef struct _vd {
	std::vector<uint> bones;
	std::vector<float> weights;
} _vd;

typedef struct _pc {
	std::vector<XMFLOAT3> vertex_positions;
} _pc;

typedef struct _matCacheEnrty
{
	Entity materialEntry;
	FbxSurfaceMaterial* ptr;
}matCacheEnrty;


typedef struct _meshNodeCacheEntry {
	Entity objEntity;
	Entity meshEntity;
	Entity emitterEntity;
	FbxMesh* ptr;
	std::vector<_vd> bonedata;
} meshNodeCacheEntry;

class SceneFBX : public RenderPath3D_TiledDeferred
{
public:
	std::vector<matCacheEnrty> matCache;
	std::vector<meshNodeCacheEntry> meshNodeCache;
	std::vector<_pc> pointCacheData;

	meshNodeCacheEntry* cacheFindByFbxMesh(FbxMesh*);
	meshNodeCacheEntry* cacheFindByObjentity(Entity);

	bool isfading = false;
	double frameTime;
	wiFadeManager fm;

	FbxTime FBXSceneTime;
	float FBXSceneStartTime;
	float FBXTakeStartTime;

	wiScene::Scene wScene;

	std::vector<FbxCamera *> FbxCameraList;
	FbxCamera* lActiveCamera;

	double SceneTimeDuration;

	FbxScene* lScene;
	FbxAnimEvaluator* lSceneEvaluator;
	FbxArray< FbxSurfaceMaterial*> FBXMaterialArray;

	FbxAnimLayer* lAnimLayer;

	// If one node is selected, draw it and its children.
	FbxAMatrix lDummyGlobalPosition;

	std::vector<Entity> SceneMaterials;

	FbxManager* lSdkManager;

	SceneFBX();
	~SceneFBX();

	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate() override;

	void ImportFBXFile(char* filename);

	void ImportFBXNode(FbxNode* node);
	void ImportMesh(FbxNode* node);
	void ImportCamera(FbxNode* node);
	void ImportLight(FbxNode* node);
	void ImportLine(FbxNode* node);


	void ImportSkeleton(FbxNode* node);
	void ImportSkin(FbxNode* node);
	void ImportBlendShape(FbxNode* node);
	void ImportPointCache(FbxNode* node);

	void ComputeShapeDeformation(FbxNode* node);

	void SceneFBX::SetWaterPlane();

//	Entity ImportStingrayMaterial(FbxSurfaceMaterial* pSurfaceMaterial, const FbxImplementation* pImplementation);
//	Entity ImportPhysicalMaterial(FbxSurfaceMaterial* pSurfaceMaterial, const FbxImplementation* pImplementation);

	void ImportFBXMeshMaterials(FbxNode* node, std::vector<MeshComponent::MeshSubset> &matList);
	void SceneFBX::ImportFBXDummy(FbxNode* node);
	void SceneFBX::UpdateFBXDummy(FbxNode* node);

	float SceneFBX::getCustomFloatAttribute(FbxNode* node, char* attrname);
	int SceneFBX::getCustomIntAttribute(FbxNode* node, char* attrname);
	FbxString SceneFBX::getCustomStrAttribute(FbxNode* node, char* attrname);

//	void UpdatetStingrayMaterial(FbxSurfaceMaterial* pSurfaceMaterial, const FbxImplementation* pImplementation);
	wiECS::Entity SceneFBX::Import3DSPBRMaterial(FbxSurfaceMaterial* pSurfaceMaterial);

//	void ImportFBXMaterials();

	void UpdateFBXScene();
	void UpdateFBXNode(FbxNode* node);

	void UpdateCamera(FbxNode* node);
//	void UpdateLight(FbxNode* node);
	void UpdateLight(FbxNode* node, bool bInitialization);
	void UpdateMesh(FbxNode* node);
	void UpdateLine(FbxNode* node);

	void UpdateSkeleton(FbxNode* node);
	void UpdateSkin(FbxNode* node);
	void UpdateBlendShape(FbxNode* node);
	void UpdatePointCache(FbxNode* node);

//	void UpdateSkeleton(FbxNode* node, FbxNode*);

	void LoadUVInformation(FbxNode* node, Entity meshEntity);
	void LoadNormals(FbxNode* pNode, Entity meshEntity);

	void Load();
	void Start();


	void SceneFBX::ActivateNextCamera();
	void SceneFBX::ActivateCamera(int index);


	WeatherComponent& GetWeather();

	bool SetCurrentAnimStack(int pIndex);



};
