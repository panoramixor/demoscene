#include <lib3ds.h>
#include "scene3ds.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;
using namespace wiScene;
using namespace wiECS;

void Scene3DS::Update(float dt)
{
	__super::Update(dt);

	if (!c3ds_file) {
		times += dt * 100.f;
		return;
	}

	if (times > c3ds_file->frames + 1) {
		activeCamera++;
		if (activeCamera >= c3ds_file->ncameras)
		{
			activeCamera = 0;
		}
		times = 0;
	}

	lib3ds_file_eval(c3ds_file, times);

	if (c3ds_file->ncameras) {
		UpdateCamera_3DS(c3ds_file->cameras[activeCamera]->name);
	}
	UpdateAnimation_3DS();

	times += dt * 100.f;
}

void Scene3DS::Load()
{
	__super::Load();
}

void Scene3DS::Initialize()
{
	__super::Initialize();


	float screenW = (float)wiRenderer::GetDevice()->GetScreenWidth();
	float screenH = (float)wiRenderer::GetDevice()->GetScreenHeight();

	// Reset all state that tests might have modified:
	wiRenderer::GetDevice()->SetVSyncEnabled(true);
	wiRenderer::SetToDrawGridHelper(false);
	wiRenderer::SetTemporalAAEnabled(true);
	wiRenderer::ClearWorld();


	activeCamera = 0;
	times = 0;

	
	setSSREnabled(true);
	//	setSSAOEnabled(true);
	setReflectionsEnabled(true);
	setFXAAEnabled(true);

	//	wiRenderer::SetAdvancedRefractionsEnabled(true);
	setVolumeLightsEnabled(true);
	setLightShaftsEnabled(true);
	setLensFlareEnabled(true);

	setChromaticAberrationEnabled(true);
	setChromaticAberrationAmount(1.0f);

	setMotionBlurEnabled(true);
	setMotionBlurStrength(1.0);
	setTessellationEnabled(true);

	setShadowsEnabled(true);
	setSSSEnabled(true);
	setEyeAdaptionEnabled(true);
	setColorGradingEnabled(true);
	setBloomEnabled(true);

	wiRenderer::SetOcclusionCullingEnabled(true);

	wiRenderer::SetShadowPropsCube(1024, -1);
	wiRenderer::SetShadowProps2D(1024, -1, -1);

	wiRenderer::SetTransparentShadowsEnabled(true);

	setBloomThreshold(3.72175f);
	
	ImportScene_3DS("models/fp2018.3ds", wiScene::GetScene());

	Scene &scene = wiScene::GetScene();
	/*
	Entity smokeEntity = wiScene::LoadModel("models/smoke.wiscene");
	Entity smokeEntity2 = scene.Entity_FindByName("smoke_emitter");
	TransformComponent &meshtrans = *scene.transforms.GetComponent(smokeEntity2);
	meshtrans.Translate(XMFLOAT3(-2.7, -1.1, -0.7));


	MaterialComponent &ceniceromat = *scene.materials.GetComponent(scene.Entity_FindByName("cenicero"));
	ceniceromat.SetBaseColor(XMFLOAT4(1, 1, 1, 0.73f));
//	ceniceromat.SetWater(true);
//	ceniceromat.SetAlphaRef(0.5f);
//	ceniceromat.SetRoughness(0.0142);
	ceniceromat.SetMetalness(0.0896319f);
	ceniceromat.SetPlanarReflections(true);
//	ceniceromat.SetOpacity(1.0f);
	ceniceromat.userBlendMode = BLENDMODE_ALPHA;
	ceniceromat.SetReflectance(0.4121572f);
//	ceniceromat.SetRefractionIndex(0.44f);
*/
	/*
	ceniceromat.SetCastShadow(true);

	MaterialComponent &beermat = *scene.materials.GetComponent(scene.Entity_FindByName("beer"));
	//	beermat.SetRoughness(0.1);
	beermat.SetAlphaRef(0.5f);
	beermat.SetPlanarReflections(true);
	beermat.SetOpacity(0.791f);
	beermat.userBlendMode = BLENDMODE_ALPHA;
	beermat.SetRefractionIndex(0.4f);
	beermat.SetReflectance(0.22f);
	//	beermat.SetSubsurfaceScattering(0.40f);
	*/


}


void Scene3DS::FixedUpdate()
{
	__super::FixedUpdate();
}

Scene3DS::Scene3DS()
{


//	ImportScene_3DS("models/furilete2.3ds", wiScene::GetScene());
	//	ImportScene_3DS("models/cubecam.3ds", wiScene::GetScene());
	//	ImportScene_3DS("models/firulete2.3ds", wiScene::GetScene());

	//	ImportScene_3DS("models/fp2018.3ds", wiScene::GetScene());
	//	ImportScene_3DS("models/scene_02.3ds", wiScene::GetScene());
	//	ImportScene_3DS("models/scene07.3ds", wiScene::GetScene());



}

void Scene3DS::ProcessNode(Lib3dsNode *node)
{
	//			lib3ds_node_eval(node, 0);
	Lib3dsNode *p;
	for (p = node->childs; p != 0; p = p->next) {
		this->ProcessNode(p);
	}

	if (strcmp(node->name, "$$$DUMMY") == 0) {
		return;
	}

	if (node->type == Lib3dsNodeType::LIB3DS_NODE_MESH_INSTANCE) {


		int index = -1;
		Lib3dsMeshInstanceNode *n = (Lib3dsMeshInstanceNode*)node;
		Lib3dsMesh *dsmesh = 0;

		wiScene::Scene& scene = wiScene::GetScene();

		std::string objname = n->instance_name;
		if (objname.size() == 0) {
			objname = node->name;
		}

		Entity objectEntity = scene.Entity_CreateObject(objname);
		node->user_id = objectEntity;
		Entity meshEntity = scene.Entity_CreateMesh(objname + "_mesh");

		TransformComponent &tc = *scene.transforms.GetComponent(objectEntity);
		//		tc.ClearTransform();

		ObjectComponent& object = *scene.objects.GetComponent(objectEntity);
		MeshComponent& mesh = *scene.meshes.GetComponent(meshEntity);

		object.meshID = meshEntity;

		index = lib3ds_file_mesh_by_name(c3ds_file, n->instance_name);

		if (index < 0)
			index = lib3ds_file_mesh_by_name(c3ds_file, node->name);
		if (index < 0) {
			return;
		}

		dsmesh = c3ds_file->meshes[index];
		tc.ClearTransform();

		/// get stuff showing right

		XMMATRIX bmtx = XMLoadFloat4x4((XMFLOAT4X4 *)node->matrix);
		bmtx = XMLoadFloat4x4((XMFLOAT4X4 *)n->base.matrix);
		//		XMMATRIX invbmtx = XMMatrixInverse(nullptr, bmtx);
		XMMATRIX meshmtx = XMLoadFloat4x4((XMFLOAT4X4 *)dsmesh->matrix);
		//		meshmtx = XMMatrixTranspose(meshmtx);
		XMMATRIX scalemtx = XMMatrixScaling(0.1f, 0.1f, 0.1f);
		XMMATRIX invmeshmtx = XMMatrixInverse(nullptr, meshmtx);
		XMMATRIX pivotmtx = XMMatrixTranslation(n->pivot[0], n->pivot[1], n->pivot[2]);

		XMMATRIX invparentmtx = XMMatrixIdentity();
		XMMATRIX parentmtx = XMMatrixIdentity();

		
		if (node->parent) {
			XMMATRIX parentmtx = XMLoadFloat4x4((XMFLOAT4X4 *)node->parent->matrix);
			invparentmtx = XMMatrixInverse(nullptr, parentmtx);
		}

		XMMATRIX finalmtx = XMMatrixIdentity();
		finalmtx = parentmtx;
		finalmtx *= bmtx;
		finalmtx *= pivotmtx;
		finalmtx *= invmeshmtx;
		finalmtx *= scalemtx;
		tc.MatrixTransform(finalmtx);

		float(*l_normals)[3] = (float(*)[3])malloc(3 * 3 * sizeof(float)* dsmesh->nfaces);

		lib3ds_mesh_calculate_vertex_normals(dsmesh, l_normals);

		Entity matid = scene.Entity_FindByName("default_material");

		// faces (indices)
		for (auto f = 0; f < dsmesh->nfaces; f++) {

			std::vector<std::pair<uint32_t, uint32_t>> vertidx_reorder;

			vertidx_reorder.push_back(std::pair < uint32_t, uint32_t>(0, dsmesh->faces[f].index[0]));
			vertidx_reorder.push_back(std::pair < uint32_t, uint32_t>(1, dsmesh->faces[f].index[1]));
			vertidx_reorder.push_back(std::pair < uint32_t, uint32_t>(2, dsmesh->faces[f].index[2]));

			for (auto vidx = vertidx_reorder.begin(); vidx != vertidx_reorder.end(); vidx++)
			{
				uint32_t idxofs = (uint32_t)mesh.vertex_positions.size();
				uint32_t aidx = (*vidx).second;

				//				lib3ds_vector_transform(dsmesh->vertices[aidx], M, dsmesh->vertices[aidx]);

								// for every vertex in the face
				XMFLOAT3 pos;
				XMFLOAT3 nor;

				//				XMFLOAT4 col;
				XMFLOAT2 uv;

				if (dsmesh->texcos) {
					uv.x = dsmesh->texcos[aidx][0];
					uv.y = -dsmesh->texcos[aidx][1];
				}

				pos.x = dsmesh->vertices[aidx][0];
				pos.y = dsmesh->vertices[aidx][1];
				pos.z = -dsmesh->vertices[aidx][2];


				nor.x = l_normals[3 * f + (*vidx).first][0];
				nor.y = l_normals[3 * f + (*vidx).first][1];
				nor.z = -l_normals[3 * f + (*vidx).first][2];

				mesh.vertex_positions.push_back(pos);
				mesh.vertex_normals.push_back(nor);

				mesh.vertex_colors.push_back(wiColor::White().rgba);

				mesh.vertex_uvset_0.push_back(uv);
				mesh.vertex_uvset_1.push_back(uv);
				mesh.indices.push_back(idxofs);

			}
			vertidx_reorder.clear();

			if (!c3ds_file->nmaterials) {
				mesh.subsets.push_back(MeshComponent::MeshSubset());
				mesh.subsets.back().materialID = matid;
				mesh.subsets.back().indexOffset = 0;
				mesh.subsets.back().indexCount = (uint32_t)mesh.indices.size();
				matid = matid;
			}

			if (c3ds_file->nmaterials) {
				Entity newid = 0;
				if (dsmesh->faces[f].material != -1) {
					newid = scene.Entity_FindByName(c3ds_file->materials[dsmesh->faces[f].material]->name);
				}

				if (!newid) {
					mesh.subsets.push_back(MeshComponent::MeshSubset());
					mesh.subsets.back().materialID = matid;
					mesh.subsets.back().indexOffset = 0;
					mesh.subsets.back().indexCount = (uint32_t)mesh.indices.size();
					matid = matid;
				}

				if (matid != newid) {
					mesh.subsets.push_back(MeshComponent::MeshSubset());
					mesh.subsets.back().materialID = newid;
					mesh.subsets.back().indexOffset = 0;
					mesh.subsets.back().indexCount = (uint32_t)mesh.indices.size();
					matid = newid;
				}
				else {
					mesh.subsets.back().indexCount = (uint32_t)mesh.indices.size();
				}
			}
		}
		mesh.CreateRenderData();
	}
}

void Scene3DS::ProcessNodeAnim(Lib3dsNode *node)
{
	Scene &scene = wiScene::GetScene();
	Lib3dsNode *p;
	for (p = node->childs; p != 0; p = p->next) {
		ProcessNodeAnim(p);
	}

	if (strcmp(node->name, "$$$DUMMY") == 0)
	{
		return;
	}

	if (node->type == Lib3dsNodeType::LIB3DS_NODE_MESH_INSTANCE) {

		Lib3dsMeshInstanceNode *n = (Lib3dsMeshInstanceNode*)node;
		TransformComponent &tc = *scene.transforms.GetComponent(node->user_id);
		ObjectComponent& object = *scene.objects.GetComponent(node->user_id);

		int index = lib3ds_file_mesh_by_name(c3ds_file, n->instance_name);
		if (index < 0)
			index = lib3ds_file_mesh_by_name(c3ds_file, node->name);
		if (index < 0) {
			return;
		}
		Lib3dsMesh *dsmesh = c3ds_file->meshes[index];


		tc.ClearTransform();

		/// get stuff showing right

		XMMATRIX bmtx = XMLoadFloat4x4((XMFLOAT4X4 *)node->matrix);
		bmtx = XMLoadFloat4x4((XMFLOAT4X4 *)n->base.matrix);
		//		XMMATRIX invbmtx = XMMatrixInverse(nullptr, bmtx);
		XMMATRIX meshmtx = XMLoadFloat4x4((XMFLOAT4X4 *)dsmesh->matrix);
		//		meshmtx = XMMatrixTranspose(meshmtx);
//		XMMATRIX scalemtx = XMMatrixScaling(0.1f, 0.1f, 0.1f);
		XMMATRIX scalemtx = XMMatrixScaling(0.1f, 0.1f, 0.1f);
		XMMATRIX invmeshmtx = XMMatrixInverse(nullptr, meshmtx);
		XMMATRIX pivotmtx = XMMatrixTranslation(n->pivot[0], n->pivot[1], n->pivot[2]);

		XMMATRIX invparentmtx = XMMatrixIdentity();
		XMMATRIX parentmtx = XMMatrixIdentity();

		if (node->parent) {
			XMMATRIX parentmtx = XMLoadFloat4x4((XMFLOAT4X4 *)node->parent->matrix);
			invparentmtx = XMMatrixInverse(nullptr, parentmtx);
		}

		XMMATRIX finalmtx = XMMatrixIdentity();
		finalmtx = parentmtx;
		finalmtx *= bmtx;
		finalmtx *= pivotmtx;
		finalmtx *= invmeshmtx;
		finalmtx *= scalemtx;
		tc.MatrixTransform(finalmtx);

	}
}



void Scene3DS::UpdateAnimation_3DS()
{
	auto p = c3ds_file->nodes;
	for (p; p != 0; p = p->next) {
		ProcessNodeAnim(p);
	}
}

void Scene3DS::UpdateCamera_3DS(char *camera)
{
	Lib3dsTargetNode *t;
	Lib3dsCameraNode *c;
	float fov, roll;
	float vnear = 0, vfar = 0;
	float *campos = 0;
	float *tgt = 0;

	int camidx;
	Lib3dsCamera *cam;

	c = (Lib3dsCameraNode*)lib3ds_file_node_by_name(c3ds_file, camera, LIB3DS_NODE_CAMERA);
	t = (Lib3dsTargetNode*)lib3ds_file_node_by_name(c3ds_file, camera, LIB3DS_NODE_CAMERA_TARGET);

	if (t != NULL) {
		tgt = t->pos;
	}

	if (c != NULL) {
		fov = c->fov;
		roll = c->roll;
		campos = c->pos;
	}

	if ((camidx = lib3ds_file_camera_by_name(c3ds_file, camera)) == -1)
		return;

	cam = c3ds_file->cameras[camidx];

	vnear = cam->near_range;
	vfar = cam->far_range;

	if (c == NULL || t == NULL) {
		if (c == NULL) {
			fov = cam->fov;
			roll = cam->roll;
			campos = cam->position;
		}
		if (t == NULL)
			tgt = cam->target;
	}

#define M_PI 3.14159265358979323846264338327950288f
	float real_fov = fov * (float)wiRenderer::GetDevice()->GetScreenHeight() / (float)wiRenderer::GetDevice()->GetScreenWidth();
	real_fov = (real_fov *(M_PI / 180.f));

	wiScene::CameraComponent &camcom = wiRenderer::GetCamera();
	camcom.CreatePerspective((float)wiRenderer::GetDevice()->GetScreenWidth(), (float)wiRenderer::GetDevice()->GetScreenHeight(), 0.1f, 1000.f, real_fov);

	// TODO: recode matrix stuff with directx's math library
	XMMATRIX cameraProj = DirectX::XMMatrixIdentity();

	XMFLOAT3 _xmEye = XMFLOAT3(campos[0] * 0.1f, campos[1] * 0.1f, -campos[2] * 0.1f);

	XMVECTOR __eye = XMLoadFloat3(&_xmEye);
	XMVECTOR __tgt = XMLoadFloat3(&XMFLOAT3(tgt[0] * 0.1f, tgt[1] * 0.1f, -tgt[2] * 0.1f));

	XMVECTOR __at = __tgt - __eye;
	XMVECTOR __up = XMLoadFloat3(&XMFLOAT3(0, -roll, -1.0f));

	__at = XMVector3Normalize(__at);
	__up = XMVector3Normalize(__up);

	XMStoreFloat3(&camcom.At, __at);
	XMStoreFloat3(&camcom.Eye, __eye);

	XMStoreFloat3(&camcom.Up, __up);

	camcom.UpdateCamera();

	Scene &scene = wiScene::GetScene();


	EnvironmentProbeComponent &probe = *scene.probes.GetComponent(scene.Entity_FindByName("probe"));
//	probe.position = _xmEye;
//	probe.SetDirty(true);

	TransformComponent &trans = *scene.transforms.GetComponent(scene.Entity_FindByName("probe"));
	trans.Translate(_xmEye);
	trans.UpdateTransform();


//	trans = *scene.transforms.GetComponent(scene.Entity_FindByName("DefaultCameraLight"));
//	trans.Translate(_xmEye);
//	trans.UpdateTransform();
}

void Scene3DS::ImportScene_3DS(const std::string& fileName, wiScene::Scene& scene)
{
	std::string error;
	std::string warning;

	string directory, name;
	wiHelper::SplitPath(fileName, directory, name);

	//			bool fileLoaded = false;
	c3ds_file = lib3ds_file_open(fileName.c_str());
	if (!c3ds_file)
	{
		return;
	}

	if (!warning.empty())
	{
		return;
	}

	Lib3dsNode *lastnode = c3ds_file->nodes;
	if (!lastnode) {
		throw("shit!");
	}

	scene.cameras.Clear();
	scene.Clear();

	Entity defaultcam = scene.Entity_CreateCamera("default_camera", (float)wiRenderer::GetInternalResolution().x, (float)wiRenderer::GetInternalResolution().y);

//	scene.lights.Clear();

	if (!c3ds_file->nlights) {
		// light
		Entity entity = scene.Entity_CreateLight("DefaultCameraLight", XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), 1, 300);
		LightComponent* light = scene.lights.GetComponent(entity);

		light->SetCastShadow(true);
		light->SetType(LightComponent::LightType::SPHERE);
		light->radius = 300.f;

//				Entity cameraEntity = scene.cameras.GetEntity(0);

//				wiScene::GetScene().Component_Attach(entity, defaultcam);
	}

	else {
		for (auto lt = 0; lt < c3ds_file->nlights; lt++) {
			Entity lightEntity = scene.Entity_CreateLight(c3ds_file->lights[lt]->name, XMFLOAT3(c3ds_file->lights[lt]->position[0] * 0.1f, c3ds_file->lights[lt]->position[1] * 0.1f, -c3ds_file->lights[lt]->position[2] * 0.1f),
				XMFLOAT3(c3ds_file->lights[lt]->color[0], c3ds_file->lights[lt]->color[1], c3ds_file->lights[lt]->color[2]), c3ds_file->lights[lt]->multiplier, c3ds_file->lights[lt]->outer_range);

			LightComponent &lc = *scene.lights.GetComponent(lightEntity);
			lc.SetType(LightComponent::LightType::POINT);
			lc.SetCastShadow(true);

			lc.radius = c3ds_file->lights[lt]->inner_range;
			lc.SetVisualizerEnabled(true);

		}
	}

	// atmosphere
	Entity refprobe = wiScene::GetScene().Entity_CreateEnvironmentProbe("probe", XMFLOAT3(0, 0, 0));

	EnvironmentProbeComponent &probe = *scene.probes.GetComponent(refprobe);
	probe.range = 12.f;
	probe.SetRealTime(true);

	//	scene.Component_Attach(refprobe, scene.Entity_FindByName("default_camera"));

		/*
		wiScene::WeatherComponent &wc = scene.weather;
		wc.ambient = XMFLOAT3(c3ds_file->ambient[0], c3ds_file->ambient[1], c3ds_file->ambient[2]);
		if ((c3ds_file->ambient[0] + c3ds_file->ambient[1] + c3ds_file->ambient[2]) == 0.f) {
			wc.ambient = XMFLOAT3(0.2f, 0.2f, 0.2f);
		}*/

		// materials
	Entity defaultmaterialEntity = scene.Entity_CreateMaterial("default_material");
	MaterialComponent& material = *scene.materials.GetComponent(defaultmaterialEntity);

	material.SetBaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1));
	material.roughness = 1.0f;
	material.metalness = 0.0f;

	material.SetCastShadow(true);

	for (auto nmats = 0; nmats < c3ds_file->nmaterials; nmats++) {

		Entity materialEntity = scene.Entity_CreateMaterial(c3ds_file->materials[nmats]->name);
		MaterialComponent& material = *scene.materials.GetComponent(materialEntity);

		if (c3ds_file->materials[nmats]->self_illum_flag) {
			c3ds_file->materials[nmats]->diffuse;
			material.SetEmissiveColor(XMFLOAT4(c3ds_file->materials[nmats]->diffuse[0], c3ds_file->materials[nmats]->diffuse[1], c3ds_file->materials[nmats]->diffuse[2], 1.f));
			material.SetEmissiveStrength(c3ds_file->materials[nmats]->self_illum);
		}

		material.SetBaseColor(XMFLOAT4(c3ds_file->materials[nmats]->diffuse[0], c3ds_file->materials[nmats]->diffuse[1], c3ds_file->materials[nmats]->diffuse[2], 1));
//		material.baseColor = XMFLOAT4(1, 1, 1, 1);
		material.baseColorMapName = c3ds_file->materials[nmats]->texture1_map.name;
		material.displacementMapName = c3ds_file->materials[nmats]->bump_map.name;
		/*
		material.emissiveColor.x = c3ds_file->materials[nmats]->self_illum;
		material.emissiveColor.y = obj_material.emission[1];
		material.emissiveColor.z = obj_material.emission[2];
		material.emissiveColor.w = max(obj_material.emission[0], max(obj_material.emission[1], obj_material.emission[2]));
		material.refractionIndex = obj_material.ior;
		*/
		material.occlusionMapName = c3ds_file->materials[nmats]->opacity_map.name;
		material.surfaceMapName = c3ds_file->materials[nmats]->specular_map.name;
		material.normalMapName = c3ds_file->materials[nmats]->bump_map.name;

		material.emissiveMapName = c3ds_file->materials[nmats]->self_illum_map.name;

		material.roughness = 1.0f;
		material.SetOpacity(1.0f - c3ds_file->materials[nmats]->transparency);
		material.SetCastShadow(true);

		material.metalness = 0;
		if (material.metalness == 0) {
			material.roughness = 1.f - c3ds_file->materials[nmats]->shininess;
			//			material.reflectance = 0.1f;
		}
//		material.SetUseSpecularGlossinessWorkflow(true);
//		material.SetUseVertexColors(true);
		material.SetOcclusionEnabled_Primary(false);

		if (!material.surfaceMapName.empty())
		{
			std::string texturefilename = directory + material.surfaceMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".jpg";
			material.surfaceMap = wiResourceManager::Load(texturefilename);
		}

		if (!material.occlusionMapName.empty())
		{
			std::string texturefilename = directory + material.occlusionMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".jpg";
			material.occlusionMap = wiResourceManager::Load(texturefilename);
		}

		if (!material.baseColorMapName.empty())
		{
			std::string texturefilename = directory + material.baseColorMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".jpg";

			material.baseColorMap = wiResourceManager::Load(texturefilename);
		}
		if (!material.normalMapName.empty())
		{
			std::string texturefilename = directory + material.normalMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".jpg";
			material.normalMap = wiResourceManager::Load(texturefilename);
			material.SetNormalMapStrength(0.5f);
		}
		if (!material.displacementMapName.empty())
		{
			std::string texturefilename = directory + material.displacementMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".jpg";
			material.uvset_displacementMap = 0;
			material.displacementMap = wiResourceManager::Load(texturefilename);
			material.SetDisplacementMapping(50.40f);
		}

		if (!material.emissiveMapName.empty())
		{
			std::string texturefilename = directory + material.emissiveMapName;
			wiHelper::RemoveExtensionFromFileName(texturefilename);
			texturefilename += ".dds";

			material.SetEmissiveColor(XMFLOAT4(c3ds_file->materials[nmats]->diffuse[0], c3ds_file->materials[nmats]->diffuse[1], c3ds_file->materials[nmats]->diffuse[2], 1.f));

			material.uvset_emissiveMap = 0;
			material.emissiveMap = wiResourceManager::Load(texturefilename);
			material.SetEmissiveStrength(100.f);

		}

	}

	lib3ds_file_eval(c3ds_file, 0);

	auto p = c3ds_file->nodes;

	if (strlen(c3ds_file->name) == 0) {
		strcpy(c3ds_file->name, "root3ds");
	}

	Entity objectEntity = scene.Entity_CreateObject(c3ds_file->name);

	ObjectComponent& object = *scene.objects.GetComponent(objectEntity);

	for (p; p != 0; p = p->next) {
		ProcessNode(p);
	}

	scene.Update(0);
}


