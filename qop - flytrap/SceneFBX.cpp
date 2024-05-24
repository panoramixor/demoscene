#include "Scenefbx.h"
#include "fmodwrapper.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;
using namespace wiScene;
using namespace wiECS;

extern CFMod fmodPlayer;

string getFromTexturePath(string str)
{
    str = "data/textures/" + str.substr(str.find_last_of("\\/") + 1);
    return str;
}


FbxFileTexture* GetTextureFromProperty(FbxProperty* pProp) {
    return (FbxFileTexture*)pProp->GetSrcObject(FbxCriteria::ObjectType(FbxFileTexture::ClassId));
}

XMFLOAT3 ConvertVertex(FbxVector4 v)
{
    XMFLOAT3 __pos = XMFLOAT3((float)v.mData[0], (float)v.mData[1], (float)v.mData[2]);
    return __pos;
}

XMFLOAT4 ConvertQuat(FbxVector4 v)
{
    XMFLOAT4 __pos = XMFLOAT4((float)v.mData[0], (float)v.mData[1], (float)v.mData[2], (float)v.mData[3]);
    return __pos;
}


XMFLOAT4 ConvertRGBA(FbxColor v)
{
    XMFLOAT4 __pos = XMFLOAT4((float)v.mRed, (float)v.mGreen, (float)v.mBlue, (float)v.mAlpha);
    return __pos;
}

XMFLOAT3 ConvertRGB(FbxColor v)
{
    XMFLOAT3 __pos = XMFLOAT3((float)v.mRed, (float)v.mGreen, (float)v.mBlue);
    return __pos;
}

XMFLOAT2 ConvertUV(FbxVector2 v)
{
    XMFLOAT2 __pos = XMFLOAT2((float)v.mData[0], -(float)v.mData[1]);
    return __pos;
}

wiColor ConvertColor(FbxColor color)
{
    wiColor col;
    col.setR((uint8_t)float(color.mRed * 255));
    col.setG((uint8_t)float(color.mGreen * 255));
    col.setB((uint8_t)float(color.mBlue * 255));
    col.setA((uint8_t)float(color.mAlpha * 255));
    return col;
}

XMFLOAT4X4 ConvertMatrix(const FbxMatrix& fbxmat)
{
    XMFLOAT4X4 mat;
    mat._11 = float(fbxmat[0][0]);    mat._12 = float(fbxmat[0][1]);    mat._13 = float(fbxmat[0][2]);    mat._14 = float(fbxmat[0][3]);
    mat._21 = float(fbxmat[1][0]);    mat._22 = float(fbxmat[1][1]);    mat._23 = float(fbxmat[1][2]);    mat._24 = float(fbxmat[1][3]);
    mat._31 = float(fbxmat[2][0]);    mat._32 = float(fbxmat[2][1]);    mat._33 = float(fbxmat[2][2]);    mat._34 = float(fbxmat[2][3]);
    mat._41 = float(fbxmat[3][0]);    mat._42 = float(fbxmat[3][1]);    mat._43 = float(fbxmat[3][2]);    mat._44 = float(fbxmat[3][3]);
    return mat;
}

void SceneFBX::UpdateCamera(FbxNode *node)
{
    wiScene::CameraComponent& camcom = wiRenderer::GetCamera();

    if (!lActiveCamera) {
        camcom.Eye = XMFLOAT3((float)0, (float)5, (float)-10);
        return;
    }
    std::string camname = lActiveCamera->GetName();
    Entity camentity = wScene.Entity_FindByName(camname);

    TransformComponent& tc = *wScene.transforms.GetComponent(camentity);
    camcom = *wScene.cameras.GetComponent(camentity);

    float aspecth = (float)lActiveCamera->AspectHeight.Get();
    float aspectw = (float)lActiveCamera->AspectWidth.Get();
    float farpln = (float)1000.f;
    float nearpln = (float)0.01f;
//    if (!lActiveCamera->AutoComputeClipPlanes.Get()) {
        farpln = (float)lActiveCamera->FarPlane;
        nearpln = (float)lActiveCamera->NearPlane;
//        nearpln = (float)FLT_MIN*100;
//    }

    float fov = (float)lActiveCamera->FieldOfView.EvaluateValue(FBXSceneTime);

    float real_fov = fov * (float)wiRenderer::GetDevice()->GetScreenHeight() / (float)wiRenderer::GetDevice()->GetScreenWidth();
    real_fov = (real_fov * (XM_PI / 180.f));

    camcom.CreatePerspective((float)wiRenderer::GetDevice()->GetScreenWidth(), (float)wiRenderer::GetDevice()->GetScreenHeight(), nearpln, farpln, real_fov);

    FbxVector4 at = lActiveCamera->EvaluateLookAtPosition(FBXSceneTime);
    FbxVector4 realpos = lActiveCamera->EvaluatePosition(FBXSceneTime);
    FbxVector4 up = FbxVector4(0, 0, 0);

    FbxNode *camNode = lActiveCamera->GetNode();
    FbxNode* targetNode = camNode ? camNode->GetTargetUp() : NULL;
    if (targetNode && targetNode->GetScene()) {
        //Evaluate target up position
        FbxVector4 lUpDir = lSceneEvaluator->GetNodeGlobalTransform(targetNode, FBXSceneTime).GetT();
        //Transform the target up position into direction from camera position
        lUpDir -= realpos;
        lUpDir.Normalize();
        up = lUpDir;
    }
    else {
        FbxVector4 lFront = at - realpos;
        if (FbxEqual(lFront, FbxVector4(0, 0, 0, 0))) {
            lFront = FbxVector4(1,0,0,0);
        }
        else {
            lFront.Normalize();
        }
        //Compute right vector
        FbxVector4 lRight = lFront.CrossProduct(FbxVector4(0, 1, 0));
        lRight.Normalize();

        //Compute up vector
        FbxVector4 lUp = lRight.CrossProduct(lFront);
        lUp.Normalize();
        up = lUp;
    }

    camcom.Eye = XMFLOAT3((float)realpos[0], (float)realpos[1], (float)realpos[2]);
    camcom.Up = XMFLOAT3((float)up[0], (float)up[1], (float)up[2]);

    XMVECTOR __tgt = XMLoadFloat3(&XMFLOAT3((float)at[0], (float)at[1], (float)at[2]));
    XMVECTOR __at = __tgt - XMLoadFloat3(&camcom.Eye);
    XMStoreFloat3(&camcom.At, __at);

    XMFLOAT4X4 mtx = ConvertMatrix(lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime));
    tc.ClearTransform();
    tc.MatrixTransform(mtx);
    tc.UpdateTransform();
    camcom.UpdateCamera();
}


void SceneFBX::UpdateMesh(FbxNode* node)
{
    FbxMesh* currMesh = node->GetMesh();
    int vertexCounter = 0;

    const char* nodeName = node->GetName();
    std::string objname = nodeName;

    Entity objectEntity = wScene.Entity_FindByName(objname);
    Entity meshEntity = wScene.Entity_FindByName(objname + "_mesh");
    
    if (!objectEntity) {
        __debugbreak();
        return;
    }
        

    int _materialcount = node->GetMaterialCount();
    bool bIsHardwareMaterial = false;

    for(auto i=0;i< _materialcount;i++) {
        FbxSurfaceMaterial* lMaterial = node->GetMaterial(i);
        std::string materialName = lMaterial->GetName();
        Entity matEntiry = wScene.Entity_FindByName(materialName);
        MaterialComponent *matComp = wScene.materials.GetComponent(matEntiry);
    }

    /*
    if (_materialcount) {
        FbxSurfaceMaterial* lMaterial = node->GetMaterial(0);

        //Get the implementation to see if it's a hardware shader.
        const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_SFX);

        // Stingray PBR shader 
        if (lImplementation)
        {
            UpdatetStingrayMaterial(lMaterial, lImplementation);
        }
    }
    */

    TransformComponent& tc = *wScene.transforms.GetComponent(objectEntity);
    bool bHasTransform = (bool)&tc;

    int hasSkin = currMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (hasSkin) {
        UpdateSkin(node);
    }

    else {
        if (bHasTransform) {
            tc.ClearTransform();

            XMFLOAT4X4 m = ConvertMatrix(lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime));
            tc.MatrixTransform(m);
        }
        else {
            // this would be weird
            //__debugbreak();
            return;
        }
    }

    tc.UpdateTransform();

    Entity ementity = wScene.Entity_FindByName(objname + "_emitter");
    bool bHasEmitter = ementity != INVALID_ENTITY;
    if (bHasEmitter) {
        wiEmittedParticle& emtComp = *wScene.emitters.GetComponent(ementity);

        emtComp.count = getCustomFloatAttribute(node, "count");
        emtComp.FIXED_TIMESTEP = getCustomFloatAttribute(node, "FIXED_TIMESTEP");
        emtComp.random_factor = getCustomFloatAttribute(node, "random_factor");
        emtComp.random_life = getCustomFloatAttribute(node, "random_life");
        emtComp.life = getCustomFloatAttribute(node, "life");
        emtComp.rotation = getCustomFloatAttribute(node, "rotation");
        emtComp.normal_factor = getCustomFloatAttribute(node, "normal_factor");
        emtComp.size = getCustomFloatAttribute(node, "size");

        XMFLOAT4X4 m = ConvertMatrix(lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime));
        TransformComponent& emtc = *wScene.transforms.GetComponent(ementity);
        emtc.ClearTransform();
        emtc.MatrixTransform(m);
        emtc.UpdateTransform();
    }

}

void SceneFBX::Update(float dt)
{
    if (!lScene) {
        return;
    }
    if (frameTime+dt >= SceneTimeDuration) {
        frameTime = 0;
    }
    //FBXSceneTime.SetSecondDouble(fmodPlayer.GetTime());
    frameTime = fmodPlayer.GetTime()-FBXSceneStartTime;
    FBXSceneTime.SetSecondDouble(frameTime + FBXTakeStartTime);
    this->setChromaticAberrationAmount(fmodPlayer.GetVolumeLevels() * 40.f);
    UpdateFBXScene();
    __super::Update(dt);
//    wScene.Update(dt);

}

void SceneFBX::Load()
{
	__super::Load();
}

WeatherComponent& SceneFBX::GetWeather()
{
    if (wScene.weathers.GetCount() == 0)
    {
        wScene.weathers.Create(CreateEntity());
    }
    return wScene.weathers[0];
}

void SceneFBX::Start()
{
    SetScene(&wScene);
    FBXSceneStartTime = fmodPlayer.GetTime();
//    __debugbreak();
}

void SceneFBX::SetWaterPlane()
{
    auto& wc = GetWeather();
    wc.SetOceanEnabled(true);
    wc.oceanParameters.patch_length = 50.f;
    wc.oceanParameters.wave_amplitude = 10.f;
    wc.oceanParameters.choppy_scale = 13.f;
    wc.oceanParameters.wind_dependency = 0.07f;
    wc.oceanParameters.time_scale = 0.3f;
    wc.oceanParameters.waterHeight = 2;
    wc.oceanParameters.surfaceDetail = 4;
    wc.oceanParameters.surfaceDisplacementTolerance = 2.f;
    wc.oceanParameters.waterColor = XMFLOAT3(0.f, 1.0f, 0.0f);
    wc.windSpeed = 1.f;
    wc.windWaveSize = 1.f;
    wc.windRandomness = 5.f;
    wiRenderer::OceanRegenerate();
}

void SceneFBX::Initialize()
{
	__super::Initialize();

    float screenW = (float)wiRenderer::GetDevice()->GetScreenWidth();
	float screenH = (float)wiRenderer::GetDevice()->GetScreenHeight();

	// Reset all state that tests might have modified:
//	wiRenderer::SetToDrawGridHelper(true);
//    wiRenderer::SetTemporalAAEnabled(true);

//    wiRenderer::ClearWorld();

    wScene.Clear();

    wScene.cameras.Clear();

    wScene.materials.Clear();
    wScene.lights.Clear();
    wScene.weathers.Clear();

//    wiRenderer::SetGamma(1.0f);

//    setExposure(2);
//    wiRenderer::SetGamma(2.2f);

    this->setSSREnabled(true);
    this->setAO(AO_MSAO);
    this->setAOPower(4);
    
    this->setReflectionsEnabled(true);
    this->setFXAAEnabled(true);

    this->setVolumeLightsEnabled(true);
    this->setLightShaftsEnabled(true);

    this->setChromaticAberrationEnabled(true);
    this->setChromaticAberrationAmount(0.0f);

    this->setMotionBlurEnabled(true);
    this->setMotionBlurStrength(6.0);

    this->setTessellationEnabled(true);
    this->setShadowsEnabled(true);
//    this->setSSSEnabled(true);
    this->setEyeAdaptionEnabled(true);

//    setColorGradingEnabled(true);
//    setEyeAdaptionEnabled(false)

    this->setColorGradingEnabled(false);
//    this->setBloomEnabled(true);
//    this->setBloomThreshold(1.1f);

    wiRenderer::SetResolutionScale(1.0f);

    /*
    wiRenderer::SetVoxelRadianceEnabled(true);
    wiRenderer::SetVoxelRadianceSecondaryBounceEnabled(true);
    wiRenderer::SetVoxelRadianceReflectionsEnabled(true);
    wiRenderer::SetVoxelRadianceVoxelSize(1.f);
    wiRenderer::SetVoxelRadianceNumCones(8);
    wiRenderer::SetVoxelRadianceRayStepSize(1);
    wiRenderer::SetVoxelRadianceMaxDistance(150);
    */

    /*
    setDepthOfFieldEnabled(true);
    setDepthOfFieldFocus(10.0);
    setDepthOfFieldStrength(1.6);
    */
    auto& wc = GetWeather();
//    wc.skyMapName = "data/skb-new2k.dds";
//    wc.skyMap = wiResourceManager::Load(wc.skyMapName);

    wc.ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);

    wc.fogStart = 1;
    wc.fogEnd = 0;
    wc.fogHeight = 0;

    /*
    wc.ambient = XMFLOAT3(0.5f, 0.5f, 0.5f);
    wc.horizon = XMFLOAT3(0.5f, 0.5f, 0.5f);
    wc.zenith = XMFLOAT3(0.5f, 0.5f, 0.5f);
    */

    Entity defaultmaterialEntity = wScene.Entity_CreateMaterial("default_material");
    MaterialComponent& material = *wScene.materials.GetComponent(defaultmaterialEntity);

    wiRenderer::SetOcclusionCullingEnabled(false);

    material.SetBaseColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0));
    material.roughness = 1.0f;
    material.metalness = 0.0f;
    material.SetCastShadow(false);
//    material.SetOpacity(1);
    material.SetUseVertexColors(false);
//    material.SetEmissiveStrength(0);
//    wiRenderer::SetDebugLightCulling(true);
//    material.SetEmissiveColor(XMFLOAT4(1,1,1,1));
    
/*
    Entity refprobe = wScene.Entity_CreateEnvironmentProbe("probe", XMFLOAT3(0, 0, 0));

//    EnvironmentProbeComponent& probe = *wScene.probes.GetComponent(refprobe);
    EnvironmentProbeComponent *probe = wScene.probes.GetComponent(refprobe);
    probe->range = 12.f;
    probe->SetRealTime(true);
    */
}

void SceneFBX::FixedUpdate()
{
    __super::FixedUpdate();
}



void SceneFBX::ActivateNextCamera()
{
    for (auto idx = 0; idx < FbxCameraList.size(); idx++)
    {
        if (lActiveCamera == FbxCameraList[idx]) {
            idx++;
            ActivateCamera(idx);
        }
    }

}

void SceneFBX::ActivateCamera(int index)
{
    if (index >= FbxCameraList.size()) {
        index = 0;
    }
    lActiveCamera = FbxCameraList[index];
}


void SceneFBX::ImportCamera(FbxNode* node)
{
    FbxCamera* currCamera = node->GetCamera();
    if (!lActiveCamera)
        lActiveCamera = currCamera;

    const char* nodeName = node->GetName();
    currCamera->SetName(nodeName);  // we'll need it in  UpdateCamera()

    std::string objname = nodeName;

    float aspecth = (float)currCamera->AspectHeight.Get();
    float aspectw = (float)currCamera->AspectWidth.Get();

    float farpln = (float)currCamera->FarPlane;
    float nearpln = (float)currCamera->NearPlane;

    float fov = (float)currCamera->FieldOfView.Get();
#define M_PI 3.14159265358979323846264338327950288f
    float real_fov = fov * (float)wiRenderer::GetDevice()->GetScreenHeight() / (float)wiRenderer::GetDevice()->GetScreenWidth();
    real_fov = (real_fov * (M_PI / 180.f));
    //    50.839

    Entity camEntity = wScene.Entity_CreateCamera(objname, aspectw, aspecth, nearpln, farpln);
    FbxCameraList.push_back(currCamera);
    UpdateCamera(node);
}


void SceneFBX::UpdateLight(FbxNode* node, bool bInitialization=false)
{
    FbxLight* currLight = node->GetLight();
    const char* nodeName = node->GetName();
    std::string objname = nodeName;

    FbxDouble3 __lightColor = currLight->Color.EvaluateValue(FBXSceneTime);

    FbxAMatrix nodetrans = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
    FbxDouble3 nodescl = nodetrans.GetS();
    float sclf = ((float)nodescl.mData[0] + (float)nodescl.mData[1] + (float)nodescl.mData[2]) / 3.f;

    XMFLOAT3 _lightcolor = { 1,1,1 };
    float _energy = 1;
    float _range = 10.f;
    
    FbxProperty _lightParams = currLight->FindProperty("3dsMax").Find("ParamBlock_0");
    if (_lightParams.IsValid()) {
        FbxProperty _color = _lightParams.Find("Color");
        FbxProperty _multiplier = _lightParams.Find("Multiplier");
        FbxProperty _decay = _lightParams.Find("Decay Falloff");
        _energy = (float)_multiplier.EvaluateValue<FbxFloat>(FBXSceneTime);
        _range = (float)_decay.EvaluateValue<FbxFloat>(FBXSceneTime);
/*        if (_range > 0) {
            __debugbreak();
        }*/
        _lightcolor = ConvertRGB(_color.EvaluateValue<FbxDouble3>(FBXSceneTime));
    }
    
    bool bIsWickedDirectional = false;
    bool bCastShadows = true;

    FbxProperty customProperties = node->FindProperty("UDP3DSMAX", false);
    if (customProperties.IsValid()) {

        FbxString fsfs = customProperties.Get<FbxString>();
        int numTokens = fsfs.GetTokenCount("\r\n");

        for (auto tkn = 0; tkn < numTokens; tkn++) {
            FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
            FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();

            if ((_strcmpi(currTokenName.Buffer(), "directional") == 0) && (_strcmpi(currTokenValue.Buffer(), "true") == 0)) {
                bIsWickedDirectional = true;
            }

            if ((_strcmpi(currTokenName.Buffer(), "castshadows") == 0) && (_strcmpi(currTokenValue.Buffer(), "false") == 0)) {
                bCastShadows = false;
            }

        }
    }

    Entity lightEntity = wScene.Entity_FindByName(nodeName);
    LightComponent* lc = wScene.lights.GetComponent(lightEntity);


    lc->color = ConvertRGB(__lightColor);
    lc->energy = (float)currLight->Intensity.EvaluateValue(FBXSceneTime)*0.01f;
    //lc->energy = _energy;

    int __lightType = currLight->LightType.Get();
    switch (__lightType) {
        case FbxLight::EType::ePoint:
            {
                if (!bIsWickedDirectional) {
                    if(bInitialization)
                        lc->SetType(wiScene::LightComponent::LightType::SPHERE);
                    //                lc->SetType(LightComponent::LightType::POINT);
                    if (currLight->EnableFarAttenuation) {
                        //                    lc->range_local = (float)currLight->FarAttenuationEnd.EvaluateValue(FBXSceneTime);
                        lc->radius = (float)currLight->FarAttenuationEnd.EvaluateValue(FBXSceneTime) ;
                    }
                    else {
                        //                    lc->range_local = (float)currLight->DecayStart.EvaluateValue(FBXSceneTime);
                        lc->radius = (float)currLight->DecayStart.EvaluateValue(FBXSceneTime);
                    }
//                    lc->energy = _energy;
                }
                else {
                    if (bInitialization)
                        lc->SetType(wiScene::LightComponent::LightType::DIRECTIONAL);
//                    lc->energy = (float)currLight->Intensity.EvaluateValue(FBXSceneTime) * 0.1f;
//                    lc->energy = _energy;
                }
                break;
            }

        case FbxLight::EType::eDirectional:
            {
                if (bInitialization)
                    lc->SetType(LightComponent::LightType::TUBE);
                lc->energy = (float)currLight->Intensity.EvaluateValue(FBXSceneTime);
                lc->radius = (float)currLight->InnerAngle.EvaluateValue(FBXSceneTime) ;
                lc->width = (float)currLight->DecayStart.EvaluateValue(FBXSceneTime) ;
                lc->height = (float)nodetrans.GetT().mData[2];
                lc->rotation = { (float)FBXSceneTime.GetSecondDouble(), 1, 0, 0};
                break;
            }

        case FbxLight::EType::eSpot:
            {
                if (bInitialization)
                    lc->SetType(LightComponent::LightType::SPOT);
                FbxDouble fov = currLight->InnerAngle.EvaluateValue(FBXSceneTime);
//                lc->energy = (float)currLight->Intensity.EvaluateValue(FBXSceneTime);
//                lc->energy = _energy;
                lc->fov = (float)fov * 0.02f;
                lc->range_local = (float)currLight->DecayStart.EvaluateValue(FBXSceneTime);

                auto& wc = GetWeather();
                wc.fogStart = 20;
                wc.fogEnd = 1;
                wc.fogHeight = 0;

                lc->SetVolumetricsEnabled(true);
                break;
            }

        case FbxLight::EType::eArea:
            {
                if (bInitialization)
                {
                    int __shape = currLight->AreaLightShape.Get();
                    if (__shape == FbxLight::EAreaLightShape::eRectangle) {
                        lc->SetType(LightComponent::LightType::RECTANGLE);
                    }
                    else {
                        lc->SetType(LightComponent::LightType::SPHERE);
                    }
                }
            }
            break;

        default:
        {
            printf("Light type not implemented\n");
        }
        break;
    }

    TransformComponent& tc = *wScene.transforms.GetComponent(lightEntity);
    lc->SetCastShadow(bCastShadows);

//    lc->SetVisualizerEnabled(true);
//    lc->SetVolumetricsEnabled(true);

    tc.ClearTransform();
    if (bIsWickedDirectional) {
        tc.Translate(ConvertVertex(nodetrans.GetT()));
        if (lc->IsVisualizerEnabled()) {
            wiRenderer::RenderablePoint pnt;
            pnt.position = ConvertVertex(nodetrans.GetT());
            pnt.color = wiColor::White();
            pnt.size = 10;
            wiRenderer::DrawPoint(pnt);
        }
    } else {
        FbxDouble3 scl;
        scl.mData[0] = 1.0;
        scl.mData[1] = 1.0;
        scl.mData[2] = 1.0;
        nodetrans.SetS(scl);
        tc.MatrixTransform(ConvertMatrix(nodetrans));
    }
    tc.UpdateTransform();
}


void SceneFBX::ImportLight(FbxNode* node)
{
    FbxLight* currLight = node->GetLight();
    const char* nodeName = node->GetName();
    std::string objname = nodeName;
    currLight->SetName(objname.c_str());

    FbxDouble3 __lightColor = currLight->Color.EvaluateValue(FBXSceneTime);
    XMFLOAT3 _lightcolor = { 1,1,1 };
    XMFLOAT3 _lightpos = { 0,0,0 };
    float _energy = 1;
    float _range = 10.f;

    Entity lightEntity = wScene.Entity_CreateLight(objname, _lightpos, _lightcolor, _energy, _range);
    UpdateLight(node, true);

    LightComponent* lc = wScene.lights.GetComponent(lightEntity);

    FbxProperty customProperties = node->FindProperty("UDP3DSMAX", false);
    if (customProperties.IsValid()) {
        FbxString fsfs = customProperties.Get<FbxString>();
        int numTokens = fsfs.GetTokenCount("\r\n");

        for (auto tkn = 0; tkn < numTokens; tkn++) {
            FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
            FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();

            if ((_strcmpi(currTokenName.Buffer(), "lensflare") == 0) && (_strcmpi(currTokenValue.Buffer(), "true") == 0)) {
                lc->SetType(LightComponent::LightType::POINT);
                printf("lensflare: %s\n", currTokenValue.Buffer());
                this->setLensFlareEnabled(true);
                char* filenames[] = { (char*)"data/textures/flare1.png", (char*)"data/textures/flare2.png", (char*)"data/textures/flare3.png", (char*)"data/textures/flare4.png" };
                for (auto flarenum = 0; flarenum < 4; flarenum++) {
                    lc->lensFlareNames.push_back(filenames[flarenum]);
                    shared_ptr<wiResource > lensFlareRimTextures = wiResourceManager::Load(getFromTexturePath(filenames[flarenum]));
                    lc->lensFlareRimTextures.push_back(lensFlareRimTextures);
                }
            }

        }
    }

}


wiECS::Entity SceneFBX::Import3DSPBRMaterial(FbxSurfaceMaterial* pSurfaceMaterial)
{
    /*
    Entity tmp_entity = CreateEntity();
    char tmp_buffer[100] = { 0 };
    sprintf(tmp_buffer, "%llx", tmp_entity);
    pSurfaceMaterial->SetName(tmp_buffer);
*/

    std::string materialName = pSurfaceMaterial->GetName();
    Entity matEntity = wScene.Entity_CreateMaterial(materialName);
    printf("material: %s\n", materialName.c_str());
    MaterialComponent& matComp = *wScene.materials.GetComponent(matEntity);

    matComp.SetCastShadow(true);

    // 3dsmax PBR material scrript
    FbxProperty _materialParams = pSurfaceMaterial->FindProperty("3dsMax");
    if (_materialParams.IsValid()) {
        FbxInt _ClassIda = _materialParams.Find("ClassIDa").Get<FbxInt>();
        FbxInt _ClassIdb = _materialParams.Find("ClassIDb").Get<FbxInt>();
        FbxInt _SuperClassID = _materialParams.Find("SuperClassID").Get<FbxInt>();
        FbxProperty _main = _materialParams.Find("main");

        if (_main.IsValid()) {
            //base color
            FbxColor _baseColor = _main.Find("basecolor").Get<FbxColor>();
            matComp.SetBaseColor(ConvertRGBA(_baseColor));
            FbxFileTexture* lTexture = (FbxFileTexture*)_main.Find("base_color_map").GetSrcObject(0);

            if (lTexture) {
                matComp.baseColorMapName = lTexture->GetFileName();
                matComp.baseColorMap = wiResourceManager::Load(getFromTexturePath(matComp.baseColorMapName));

                double tu = lTexture->GetTranslationU();
                double tv = lTexture->GetTranslationV();
                if (tu > 0) {
                    //__debugbreak();
                }

                double su = lTexture->GetScaleU();
                double sv = lTexture->GetScaleV();
                matComp.texMulAdd.x =  (float)su;
                matComp.texMulAdd.y = (float)sv;
//                matComp.texMulAdd.x =  tu;
//                matComp.texMulAdd.y =  tv;

            }

            // metallic
            FbxFloat _metalness = _main.Find("metalness").Get<FbxFloat>();
            matComp.SetMetalness(_metalness);

            lTexture = (FbxFileTexture*)_main.Find("metalness_map").GetSrcObject(0);
            if (lTexture) {
                matComp.surfaceMapName = lTexture->GetFileName();
                matComp.surfaceMap = wiResourceManager::Load(getFromTexturePath(matComp.surfaceMapName));
            }

            // roughness
            FbxFloat _roughness = _main.Find("roughness").Get<FbxFloat>();
            matComp.SetRoughness(_roughness);
            lTexture = (FbxFileTexture*)_main.Find("roughness_map").GetSrcObject(0);
            if (lTexture) {
                matComp.surfaceMapName = lTexture->GetFileName();
                matComp.surfaceMap = wiResourceManager::Load(getFromTexturePath(matComp.surfaceMapName));
            }

            // useGlossiness
//            int _useGlossiness = _main.Find("useGlossiness").Get<FbxInt>();
//            matComp.SetUseSpecularGlossinessWorkflow(_useGlossiness);
            // ambient occlusion
            lTexture = (FbxFileTexture*)_main.Find("ao_map").GetSrcObject(0);
            if (lTexture) {
                matComp.occlusionMapName = lTexture->GetFileName();
                matComp.occlusionMap = wiResourceManager::Load(getFromTexturePath(matComp.occlusionMapName));
                matComp.SetOcclusionEnabled_Secondary(true);
            }

            // bump map account
            FbxFloat _bump_map_amt = _main.Find("bump_map_amt").Get<FbxFloat>();
            matComp.SetNormalMapStrength(_bump_map_amt);
            lTexture = (FbxFileTexture*)_main.Find("norm_map").GetSrcObject(0);
            if (lTexture) {
                matComp.normalMapName = lTexture->GetFileName();
                matComp.normalMap = wiResourceManager::Load(getFromTexturePath(matComp.normalMapName));
                matComp.SetFlipNormalMap(true);
            }

            //emissive
            FbxColor _emissive_color = pSurfaceMaterial->FindProperty("EmissiveColor").Get<FbxColor>();
            FbxDouble _emissive_factor = pSurfaceMaterial->FindProperty("EmissiveFactor").Get<FbxDouble>();
            matComp.SetEmissiveColor(ConvertRGBA(_emissive_color));
            matComp.SetEmissiveStrength((float)_emissive_factor);
            lTexture = (FbxFileTexture*)_main.Find("emit_color_map").GetSrcObject(0);
            if (lTexture) {
                matComp.emissiveMapName = lTexture->GetFileName();
                matComp.emissiveMap = wiResourceManager::Load(getFromTexturePath(matComp.emissiveMapName));
                //            matComp.emissiveMap->data
            }
        }
    }
//    matComp.displacementMapName = lTex->GetFileName();
//    matComp.displacementMap = wiResourceManager::Load(matComp.displacementMapName);
    return matEntity;
}

void SceneFBX::ImportFBXMeshMaterials(FbxNode* node, std::vector<MeshComponent::MeshSubset>& matList)
{
    FbxMesh *currMesh = node->GetMesh();
    std::string nodeName = node->GetName();
    Entity default_matid = wScene.Entity_FindByName("default_material");

    // get all mesh materials
    int numMaterials = node->GetMaterialCount();

    if (!numMaterials) {
        if (currMesh->FindProperty("Color").IsValid()) {
            FbxColor objcolor = currMesh->FindProperty("Color").Get<FbxColor>();

            Entity matEntity = wScene.Entity_CreateMaterial(nodeName + "_material");
            MaterialComponent& matComp = *wScene.materials.GetComponent(matEntity);
            matComp.SetBaseColor(/*ConvertRGBA(objcolor)*/wiColor::Cyan());
            matComp.SetCastShadow(false);
            matComp.SetMetalness(0);
            matComp.SetRoughness(1.0f);
            matComp.SetReflectance(0);

            matComp.SetEmissiveStrength(0);
            matList.push_back(MeshComponent::MeshSubset());
            matList.back().indexOffset = 0;
            matList.back().materialID = default_matid;
        }
        else {
            matList.push_back(MeshComponent::MeshSubset());
            matList.back().indexOffset = 0;
            matList.back().materialID = default_matid;
        }
        return;
    }

    for (auto matidx = 0; matidx < numMaterials; matidx++) {
        FbxSurfaceMaterial* lMaterial = node->GetMaterial(matidx);

        Entity matEntity = 0;
        for (auto i = 0; i < matCache.size(); i++) {
            if (matCache[i].ptr == lMaterial) {
                matEntity = matCache[i].materialEntry;
                matList.push_back(MeshComponent::MeshSubset());
                matList.back().indexOffset = matidx;
                matList.back().materialID = matEntity;
                break;
            }
        }
        if (matEntity)
            continue;

        matEntity = default_matid;

        if (lMaterial->FindProperty("3dsMax").IsValid()) {
            matEntity = Import3DSPBRMaterial(lMaterial);
        }
/*        else {
            printf("unsupported material\n!");
            exit(0);
        }*/
        
        // texcoord properties
        if (node->FindProperty("UDP3DSMAX", false).IsValid()) {
            FbxProperty usserproperties = node->FindProperty("UDP3DSMAX", false);
            FbxString fsfs = usserproperties.Get<FbxString>();
            int numTokens = fsfs.GetTokenCount("\r\n");

            MaterialComponent* pmatComp = wScene.materials.GetComponent(matEntity);
            for (auto tkn = 0; tkn < numTokens; tkn++) {
                FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
                FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
                FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();

                if (_strcmpi(currTokenName.Buffer(), "reflectance") == 0) {
                    pmatComp->reflectance = (float)atof(currTokenValue.Buffer());
                    printf("reflectance: %f\n", pmatComp->reflectance);
                }

                if (_strcmpi(currTokenName.Buffer(), "refractionindex") == 0) {
                    pmatComp->SetRefractionIndex((float)atof(currTokenValue.Buffer()));
                    printf("refractionindex: %f\n", pmatComp->refractionIndex);
                }

                if (_strcmpi(currTokenName.Buffer(), "blendmode") == 0) {
                    printf("blendmode: %s\n", currTokenValue.Buffer());
                    if (_strcmpi(currTokenValue.Buffer(), "alpha") == 0) {
                        pmatComp->userBlendMode = BLENDMODE_ALPHA;
                        pmatComp->alphaRef = 1.0f;
                    }
                    if (_strcmpi(currTokenValue.Buffer(), "additive") == 0) {
                        pmatComp->userBlendMode = BLENDMODE_ADDITIVE;
                        pmatComp->alphaRef = 1.0f;
                    }
                }
                if (_strcmpi(currTokenName.Buffer(), "texcoordanimfps") == 0) {
                    pmatComp->texAnimFrameRate = (float)atof(currTokenValue.Buffer());
                    printf("texcoordanimfps: %f\n", pmatComp->texAnimFrameRate);
                }
                if (_strcmpi(currTokenName.Buffer(), "texcoordanimu") == 0) {
                    pmatComp->texAnimDirection.x = (float)atof(currTokenValue.Buffer());
                    printf("texcoordanimu: %f\n", pmatComp->texAnimDirection.x);
                }
                if (_strcmpi(currTokenName.Buffer(), "texcoordanimv") == 0) {
                    pmatComp->texAnimDirection.y = (float)atof(currTokenValue.Buffer());
                    printf("texcoordanimv: %f\n", pmatComp->texAnimDirection.y);
                }
                if (_strcmpi(currTokenName.Buffer(), "textilesizex") == 0) {
                    pmatComp->texMulAdd.x = (float)atof(currTokenValue.Buffer());
                    printf("textilesizex: %f\n", pmatComp->texMulAdd.x);
                }
                if (_strcmpi(currTokenName.Buffer(), "textilesizey") == 0) {
                    pmatComp->texMulAdd.y = (float)atof(currTokenValue.Buffer());
                    printf("textilesizey: %f\n", pmatComp->texMulAdd.y);
                }
            }
            pmatComp->SetDirty(true);
        }
        
        matList.push_back(MeshComponent::MeshSubset());
        matList.back().indexOffset = matidx;
        matList.back().materialID = matEntity;
        printf("matentity: %llx\n", matEntity);
        matCacheEnrty mce= { matEntity, lMaterial };
        matCache.push_back(mce);
    }
}

void SceneFBX::UpdateSkeleton(FbxNode* node)
{
    FbxSkeleton* lSkeleton = node->GetSkeleton();
    if (!lSkeleton)
        return;

    std::string boneName = node->GetName();
    // find the object name that needs this bone

    FbxAMatrix childmtx = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
//    FbxAMatrix parentmtx = lSceneEvaluator->GetNodeGlobalTransform(node->GetParent(), FBXSceneTime);

    TransformComponent& tc = *wScene.transforms.GetComponent(wScene.Entity_FindByName(boneName));
    TransformComponent *tcc = wScene.transforms.GetComponent(wScene.Entity_FindByName(boneName));
    if (tcc) {
        XMFLOAT4X4 m = ConvertMatrix(childmtx);
        tc.ClearTransform();
        tc.MatrixTransform(m);
        tc.UpdateTransform();
    }
}

void SceneFBX::ImportSkeleton(FbxNode* node)
{
    //UpdateSkeleton(node,0);


}


void SceneFBX::UpdateSkin(FbxNode* node)
{
    
    FbxMesh* lMesh = node->GetMesh();
    std::string nodeName = node->GetName();

    FbxSkin* fbxSkin = NULL;
    const int deformerCount = lMesh->GetDeformerCount();
    for (int i = 0; i < deformerCount; ++i)
    {
        FbxDeformer* deformer = lMesh->GetDeformer(i);
        if (deformer->GetDeformerType() == FbxDeformer::eSkin)
        {
            fbxSkin = FbxCast<FbxSkin>(deformer);
            int clusterCount = fbxSkin->GetClusterCount();

            FbxCluster* cluster = fbxSkin->GetCluster(0);
            assert(cluster);

            FbxNode* linked = cluster->GetLink();

            if (linked)               
            {
                // draw skeleton
                //UpdateSkeleton(linked, node);
            }

            break;
        }
    }
}


void SceneFBX::ImportSkin(FbxNode* node)
{
    FbxMesh* lMesh = node->GetMesh();

    std::string nodeName = node->GetName();
//    int matCount = node->GetMaterialCount();

//    std::string objname = nodeName;

    Entity objectEntity = wScene.Entity_FindByName(nodeName);
    Entity meshEntity = wScene.Entity_FindByName(nodeName + "_mesh");

    ObjectComponent& objectComp = *wScene.objects.GetComponent(objectEntity);
    MeshComponent& meshComp = *wScene.meshes.GetComponent(meshEntity);

    Entity armatureEntity = CreateEntity();
    wScene.names.Create(armatureEntity) = nodeName + "_skin";
    wScene.layers.Create(armatureEntity);
    wScene.transforms.Create(armatureEntity);
    ArmatureComponent& armature = wScene.armatures.Create(armatureEntity);


    meshComp.armatureID = armatureEntity;
//    wScene.Component_Attach(objectEntity, armatureEntity, true);
    
    const int lVertexCount = lMesh->GetControlPointsCount();
    meshNodeCacheEntry *ce=0;

    for (auto i = 0; i < meshNodeCache.size(); i++) {
        if (meshNodeCache[i].ptr == lMesh) {
            ce = &meshNodeCache[i];
            ce->bonedata.resize(lVertexCount);
            break;
        }
    }

    if (!ce) {
        __debugbreak();
    }

    FbxSkin* lSkinDeformer = (FbxSkin*)lMesh->GetDeformer(0, FbxDeformer::eSkin);
    FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

    int lClusterCount = lSkinDeformer->GetClusterCount();
    armature.boneCollection.resize(lClusterCount);
    armature.inverseBindMatrices.resize(lClusterCount);

    for (int lClusterIndex = 0; lClusterIndex < lClusterCount; lClusterIndex++)
    {
        FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
        std::string skn = lCluster->GetLink()->GetName();
        FbxNode* linked = lCluster->GetLink();
        if (!linked)
            continue;

        int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
        for (int k = 0; k < lVertexIndexCount; k++)
        {
            int lIndex = lCluster->GetControlPointIndices()[k];

            // Sometimes, the mesh can have less points than at the time of the skinning
            // because a smooth operator was active when skinning but has been deactivated during export.
            if (lIndex >= lVertexCount)
                continue;

            double lWeight = lCluster->GetControlPointWeights()[k];
            if (lWeight <= 0.09) {
                continue;
//                __debugbreak();
            }

            if (lCluster->GetLinkMode() == FbxCluster::eNormalize)
            {
                // In the normalized link mode, a vertex is always totally influenced by the links. 
                ce->bonedata[lIndex].weights.push_back((float)lWeight);
            }
            else if (lCluster->GetLinkMode() == FbxCluster::eTotalOne) {
                // In the total 1 link mode, a vertex can be partially influenced by the links. 
                ce->bonedata[lIndex].weights.push_back((1.0f - (float)lWeight));
            }
            ce->bonedata[lIndex].bones.push_back(lClusterIndex);
        }

        FbxAMatrix mt;
        lCluster->GetTransformLinkMatrix(mt);
        FbxAMatrix mt2;
        lCluster->GetTransformMatrix(mt2);

        //mt2.SetR(mt.GetR());
        armature.inverseBindMatrices[lClusterIndex] = ConvertMatrix(mt.Inverse() * mt2);

        std::string bonename = linked->GetName();

        Entity boneEntity = wScene.Entity_FindByName(bonename);
        if (!boneEntity) {
            bonename = nodeName + "_" + bonename;
            boneEntity = wScene.Entity_CreateObject(bonename);
            linked->SetName(bonename.c_str());
        }

        ObjectComponent& boneComp = *wScene.objects.GetComponent(boneEntity);
        TransformComponent& tc = *wScene.transforms.GetComponent(boneEntity);

        FbxAMatrix m;
            m = lSceneEvaluator->GetNodeGlobalTransform(linked, FBXSceneTime);
            tc.ClearTransform();
            tc.MatrixTransform(ConvertMatrix(m));
            tc.UpdateTransform();
            armature.boneCollection[lClusterIndex] = boneEntity;
    }
}



void SceneFBX::LoadUVInformation(FbxNode* node, Entity meshEntity)
{
    FbxMesh* pMesh = node->GetMesh();
    std::string nodeName = node->GetName();
//    Entity meshEntity = wScene.Entity_FindByName(nodeName + "_mesh");
    MeshComponent& meshComp = *wScene.meshes.GetComponent(meshEntity);

    //get all UV set names
    FbxStringList lUVSetNameList;
    pMesh->GetUVSetNames(lUVSetNameList);

    //iterating over all uv sets
    for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
    {
        //get lUVSetIndex-th uv set
        const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

        if (!lUVElement)
            continue;

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
            return;

        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

        //iterating through the data by polygon
        const int lPolyCount = pMesh->GetPolygonCount();

        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
        {
            for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
                {
                    FbxVector2 lUVValue;

                    //get the index of the current vertex in control points array
                    int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

                    //the UV index depends on the reference mode
                    int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                    meshComp.vertex_uvset_0.push_back(ConvertUV(lUVValue));
                }
            }
        }
        else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            int lPolyIndexCounter = 0;
            for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
                for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
                {
                    if (lPolyIndexCounter < lIndexCount)
                    {
                        FbxVector2 lUVValue;

                        //the UV index depends on the reference mode
                        int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
                        meshComp.vertex_uvset_0.push_back(ConvertUV(lUVValue));
                        lPolyIndexCounter++;
                    }
                }
            }
        }
    }
}


//get mesh normals info
void SceneFBX::LoadNormals(FbxNode* pNode, Entity meshEntity)
{
    if (!pNode)
        return;

    FbxMesh* pMesh = pNode->GetMesh();
    std::string nodeName = pNode->GetName();
//    Entity meshEntity = wScene.Entity_FindByName(nodeName + "_mesh");
    MeshComponent& meshComp = *wScene.meshes.GetComponent(meshEntity);

    //get mesh
    FbxMesh* lMesh = pNode->GetMesh();
    if (lMesh)
    {
        //print mesh node name
        FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

        //get the normal element
        FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
        if (lNormalElement)
        {
            //mapping mode is by control points. The mesh should be smooth and soft.
            //we can get normals by retrieving each control point
            if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
            {
                //Let's get normals of each vertex, since the mapping mode of normal element is by control point
                for (int lVertexIndex = 0; lVertexIndex < lMesh->GetControlPointsCount(); lVertexIndex++)
                {
                    int lNormalIndex = 0;
                    //reference mode is direct, the normal index is same as vertex index.
                    //get normals by the index of control vertex
                    if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                        lNormalIndex = lVertexIndex;

                    //reference mode is index-to-direct, get normals by the index-to-direct
                    if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

                    //Got normals of each vertex.
                    FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
//                    if (gVerbose) FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
                    //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
                    //. . .
                    lNormal.Normalize();
                    meshComp.vertex_normals.push_back(ConvertVertex(lNormal));
                }//end for lVertexIndex
            }//end eByControlPoint
            //mapping mode is by polygon-vertex.
            //we can get normals by retrieving polygon-vertex.
            else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
            {
                int lIndexByPolygonVertex = 0;
                //Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
                for (int lPolygonIndex = 0; lPolygonIndex < lMesh->GetPolygonCount(); lPolygonIndex++)
                {
                    //get polygon size, you know how many vertices in current polygon.
                    int lPolygonSize = lMesh->GetPolygonSize(lPolygonIndex);
                    //retrieve each vertex of current polygon.
                    for (int i = 0; i < lPolygonSize; i++)
                    {
                        int lNormalIndex = 0;
                        //reference mode is direct, the normal index is same as lIndexByPolygonVertex.
                        if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lNormalIndex = lIndexByPolygonVertex;

                        //reference mode is index-to-direct, get normals by the index-to-direct
                        if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

                        //Got normals of each polygon-vertex.
                        FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
//                        if (gVerbose) FBXSDK_printf("normals for polygon[%d]vertex[%d]: %f %f %f %f \n", lPolygonIndex, i, lNormal[0], lNormal[1], lNormal[2], lNormal[3]);
                        //add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
                        //. . .
                        lNormal.Normalize();
                        meshComp.vertex_normals.push_back(ConvertVertex(lNormal));

                        lIndexByPolygonVertex++;
                    }//end for i //lPolygonSize
                }//end for lPolygonIndex //PolygonCount
            }//end eByPolygonVertex
        }
    }
}


void SceneFBX::ImportMesh(FbxNode* node)
{
    FbxMesh* currMesh = node->GetMesh();
    /*
    Entity tmp_entity  = CreateEntity();
    char tmp_buffer[100] = { 0 };
    sprintf(tmp_buffer, "%llx", tmp_entity);
    node->SetName(tmp_buffer);
    */

    std::string objname = node->GetName();
    std::string meshname = currMesh->GetNameWithNameSpacePrefix();

    Entity objectEntity = wScene.Entity_CreateObject(objname);

    meshNodeCacheEntry* ce = 0;
    
    // seach cache
    for (auto i = 0; i < meshNodeCache.size(); i++) {
        if (meshNodeCache[i].ptr == currMesh) {
            ce = &meshNodeCache[i];
            break;
        }
    }

    if (!ce) {
        ce = new meshNodeCacheEntry;

        ce->meshEntity = wScene.Entity_CreateMesh(objname + "_mesh");
        ce->ptr = currMesh;
        ce->emitterEntity = INVALID_ENTITY;
        ce->bonedata.clear();
        meshNodeCache.push_back(*ce);
        ce = &meshNodeCache.back();
    }

    ObjectComponent& objectComp = *wScene.objects.GetComponent(objectEntity);
    MeshComponent& meshComp = *wScene.meshes.GetComponent(ce->meshEntity);
    objectComp.meshID = ce->meshEntity;
    
    FbxStringList uvsetName;
    currMesh->GetUVSetNames(uvsetName);

    int _faceCount = currMesh->GetPolygonCount();

    std::vector<MeshComponent::MeshSubset> matList;
    ImportFBXMeshMaterials(node, matList);

    int hasSkin = currMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (hasSkin) {
        ImportSkin(node);
    }

    // vertex color
    FbxGeometryElementVertexColor * vcolorElement = currMesh->GetElementVertexColor(0);

    for (int lPolyIndex = 0; lPolyIndex < currMesh->GetPolygonCount(); lPolyIndex++)
    {
        // build the max index array that we need to pass into MakePoly
        const int lPolySize = currMesh->GetPolygonSize(lPolyIndex);
        for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
        {
            FbxVector4 vertexpos;

            //get the index of the current vertex in control points array
            int lPolyVertIndex = currMesh->GetPolygonVertex(lPolyIndex, lVertIndex);
            meshComp.indices.push_back(lPolyIndex* lPolySize+ lVertIndex);
            meshComp.vertex_positions.push_back(ConvertVertex(currMesh->GetControlPoints()[lPolyVertIndex]));

            if (currMesh->GetElementVertexColorCount()) {
                FbxColor _col = vcolorElement->GetDirectArray().GetAt(lPolyVertIndex);
                meshComp.vertex_colors.push_back(ConvertColor(_col).rgba);
            }
            else {
                meshComp.vertex_colors.push_back(wiColor::White().rgba);
            }
            if (hasSkin) {
                int bd = lPolyVertIndex;
                int numBones = (int)ce->bonedata[bd].bones.size();
                XMUINT4 boneIndices = { 0,0,0,0 };
                XMFLOAT4 boneWeights = { 0,0,0,0 };

                if (numBones >= 1) {
                    boneIndices.x = ce->bonedata[bd].bones[0];
                    boneWeights.x = ce->bonedata[bd].weights[0];
                }
                if (numBones >= 2) {
                    boneIndices.y = ce->bonedata[bd].bones[1];
                    boneWeights.y = ce->bonedata[bd].weights[1];

                }
                if (numBones >= 3) {
                    boneIndices.z = ce->bonedata[bd].bones[2];
                    boneWeights.z = ce->bonedata[bd].weights[2];
                }
                if (numBones >= 4) {
                    boneIndices.w = ce->bonedata[bd].bones[3];
                    boneWeights.w = ce->bonedata[bd].weights[3];
                }
                if (numBones > 4) {
//                    __debugbreak();
                }

                meshComp.vertex_boneindices.push_back(boneIndices);
                meshComp.vertex_boneweights.push_back(boneWeights);
            }
        }
    }

    LoadUVInformation(node, ce->meshEntity);
    LoadNormals(node, ce->meshEntity);

    int numLayers = currMesh->GetLayerCount();
    int lastmaterialindex = -1;

    int matidx = 0;
    int materialCount = currMesh->GetElementMaterialCount();
    std::vector< MeshComponent::MeshSubset> subsets;

    if (!materialCount) {
        subsets.push_back(MeshComponent::MeshSubset());
        subsets.back().indexOffset = 0;
        subsets.back().indexCount = (int)meshComp.indices.size();
        subsets.back().materialID = matList.at(0).materialID;
        meshComp.vertex_uvset_0.clear();
    }
    else {
        for (auto _faceidx = 0; _faceidx < _faceCount; _faceidx++) {
            if (numLayers) {
                matidx = currMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(_faceidx);
                if (lastmaterialindex == matidx) {
                    subsets.back().indexCount = ((_faceidx + 1) * 3) - subsets.back().indexOffset;
                    subsets.back().materialID = matList.at(matidx).materialID;
                }
                else {
                    subsets.push_back(MeshComponent::MeshSubset());
                    subsets.back().indexOffset = _faceidx * 3;

                    lastmaterialindex = matidx;
                }
              }
        }
    }


    if (node->FindProperty("UDP3DSMAX", false).IsValid()) {
        FbxProperty usserproperties = node->FindProperty("UDP3DSMAX", false);
        FbxString fsfs = usserproperties.Get<FbxString>();
        int numTokens = fsfs.GetTokenCount("\r\n");

        MaterialComponent* pmatComp = wScene.materials.GetComponent(matList.at(matidx).materialID);
//        MaterialComponent& matComp = *wScene.materials.GetComponent(matList.at(matidx).materialID);
        for (auto tkn = 0; tkn < numTokens; tkn++) {
            FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
            FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();

            MaterialComponent& matComp = *wScene.materials.GetComponent(matList.at(matidx).materialID);
            if (_strcmpi(currTokenName.Buffer(), "castshadows") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "false") == 0) {
                    pmatComp->SetCastShadow(false);
                }
                else {
                    pmatComp->SetCastShadow(true);
                }
            }
            if (_strcmpi(currTokenName.Buffer(), "doublesided") == 0) {
                printf("doublesided: %s\n", currTokenValue.Buffer());
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {
                    meshComp.SetDoubleSided(true);
                }
            }

            // particles emitter params
            if (_strcmpi(currTokenName.Buffer(), "particleEmitter") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {

                    printf("particleEmitter: %s\n", currTokenValue.Buffer());

                    Entity ementity = wScene.Entity_CreateEmitter(objname + "_emitter");
                    ce->emitterEntity = ementity;
                    wiEmittedParticle& emtComp = *wScene.emitters.GetComponent(ementity);

                    TransformComponent& emtc = *wScene.transforms.GetComponent(ementity);
                    MaterialComponent* material = wScene.materials.GetComponent(ementity);
                    material->baseColorMapName = "data/flare2.png";
                    material->baseColorMap = wiResourceManager::Load(getFromTexturePath(material->baseColorMapName));
                    material->baseColor = XMFLOAT4(0.0, 1.0, 0.0, 0.5);
                    material->emissiveColor = XMFLOAT4(0.0, 1.0, 0.0, 0.5);
                    material->SetEmissiveStrength(1.f);
                    material->userBlendMode = BLENDMODE_ADDITIVE;

                    /*
                    FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
                    emtc.ClearTransform();
                    emtc.Translate(ConvertVertex(a.GetT()));
                    emtc.UpdateTransform();*/

                    int MAX_PARTICLES = getCustomIntAttribute(node, "MAX_PARTICLES");
                    emtComp.SetMaxParticleCount(MAX_PARTICLES);
                    emtComp.meshID = ce->meshEntity;

                    emtComp.count = getCustomFloatAttribute(node, "count");
                    printf("count : %f\n", emtComp.count);

                    emtComp.FIXED_TIMESTEP = getCustomFloatAttribute(node, "FIXED_TIMESTEP");
                    printf("timeStep : %f\n", emtComp.FIXED_TIMESTEP);

                    emtComp.random_factor = getCustomFloatAttribute(node, "random_factor");
                    printf("random_factor : %f\n", emtComp.random_factor);

                    emtComp.random_life = getCustomFloatAttribute(node, "random_life");
                    printf("random_life : %f\n", emtComp.random_life);

                    emtComp.life = getCustomFloatAttribute(node, "life");
                    printf("life : %f\n", emtComp.life);

                    emtComp.rotation = getCustomFloatAttribute(node, "rotation");
                    printf("rotation : %f\n", emtComp.rotation);

                    emtComp.normal_factor = getCustomFloatAttribute(node, "normal_factor");
                    printf("normal_factor : %f\n", emtComp.normal_factor);

                    emtComp.size = getCustomFloatAttribute(node, "size");
                    printf("size : %f\n", emtComp.size);

                    emtComp.SetSorted(true);
                    emtComp.shaderType = wiEmittedParticle::PARTICLESHADERTYPE::SOFT;
                }
            }

        }
    }

    /*
    fbxsdk::FbxProperty mxp = node->FindProperty("UDP3DSMAX", false);
    if (mxp.IsValid()) {
        FbxString fsfs = mxp.Get<FbxString>();
        FbxString t1 = fsfs.GetToken(0, "\r\n").Buffer();
        FbxString t2 = fsfs.GetToken(1, "\r\n").Buffer();
        FbxString currTokenName = t1.GetToken(0, "=").Buffer();
        FbxString currTokenValue = t1.GetToken(1, "=").Buffer();
        MaterialComponent& matComp = *wScene.materials.GetComponent(matList.at(matidx).materialID);
        if (_strcmpi(currTokenName.Buffer(), "castshadows") == 0) {
            if (_strcmpi(currTokenValue.Buffer(), "false") == 0) {
                matCo mp.SetCastShadow(false);
            }
            else {
                matComp.SetCastShadow(true);
            }
        }
        if (_strcmpi(currTokenName.Buffer(), "doublesided") == 0) {
            printf("doublesided: %s\n", currTokenValue.Buffer());
            if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {
                meshComp.SetDoubleSided(true);
            }
        }
    }*/

    meshComp.SetDoubleSided(true);
    meshComp.subsets = subsets;

    meshComp.CreateRenderData();

//    meshNodeCacheEntry ce = { meshEntity, currMesh, tmp_bonedata };
//    meshNodeCache.push_back(ce);

    TransformComponent& tc = *wScene.transforms.GetComponent(objectEntity);
    bool bHasTransform = (bool)&tc;

    Entity ementity = wScene.Entity_FindByName(objname + "_emitter");
    bool bHasEmitter = ementity != INVALID_ENTITY;

    if (bHasTransform && !hasSkin) {

        FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
//        FbxVector4 t = { 1,1,1,1 };
//        a.SetS(t);
        XMFLOAT4X4 m = ConvertMatrix(a);
        tc.ClearTransform();
        tc.MatrixTransform(m);
        tc.UpdateTransform();
        if (bHasEmitter) {
            TransformComponent& emtc = *wScene.transforms.GetComponent(ce->emitterEntity);
            emtc.ClearTransform();
            emtc.MatrixTransform(m);
            emtc.UpdateTransform();
        }
    }
}

void SceneFBX::ImportLine(FbxNode* node)
{
    UpdateLine(node);
}


void SceneFBX::UpdateLine(FbxNode* node)
{
    /*
    const char* nodeName = node->GetName();
    FbxLine* currLine = node->GetLine();

    int nunPoints = currLine->GetControlPointsCount();
    FbxVector4* _vertex = currLine->GetControlPoints();
    
    for (auto vidx = 0; vidx < nunPoints; vidx++) {
        FbxVector4 thisvertex = _vertex[vidx];
        FbxVector4 nextvertex;
        if (vidx + 1 < nunPoints) {
            nextvertex = _vertex[vidx + 1];
        }
        else {
            nextvertex = thisvertex;
        }
        FbxVector4 ttvertex = lSceneEvaluator->GetNodeGlobalTransform(node).MultT(thisvertex);
        FbxVector4 tnvertex = lSceneEvaluator->GetNodeGlobalTransform(node).MultT(nextvertex);

        wiRenderer::RenderableLine line;
        XMVECTOR v1 = XMLoadFloat3(&XMFLOAT3((float)ttvertex.mData[0], (float)ttvertex.mData[1], (float)ttvertex.mData[2]));
        XMVECTOR v2 = XMLoadFloat3(&XMFLOAT3((float)tnvertex.mData[0], (float)tnvertex.mData[1], (float)tnvertex.mData[2]));

        line.color_start = ConvertRGBA(currLine->Color.EvaluateValue(FBXSceneTime));
        line.color_end = ConvertRGBA(currLine->Color.EvaluateValue(FBXSceneTime));

        XMStoreFloat3(&line.start, v1);
        XMStoreFloat3(&line.end, v2);
        //        XMStoreFloat4(&line.color_start, c->vector);
        //        XMStoreFloat4(&line.color_end, c->vector);
        wiRenderer::DrawLine(line);
    }
    */
}

void SceneFBX::UpdateFBXNode(FbxNode* node)
{
    const char* nodeName = node->GetName();
    std::string objname = nodeName;
    
    // Determine # of children the node has
    int numChildren = node->GetChildCount();
    FbxNode* childNode = 0;
    for (int i = 0; i < numChildren; i++)
    {
        childNode = node->GetChild(i);

        FbxNodeAttribute* nodeAttrib = childNode->GetNodeAttribute();
        if (!nodeAttrib)
            continue;

        FbxNodeAttribute::EType ntype = nodeAttrib->GetAttributeType();

        if (!ntype)
            continue;

        switch (ntype) {
            case FbxNodeAttribute::eMesh:
            {
                UpdateMesh(childNode);
                break;
            }

            case FbxNodeAttribute::eCamera:
            {
                UpdateCamera(childNode);
                break;
            }

            case FbxNodeAttribute::EType::eLight:
            {
                UpdateLight(childNode);
            }
            break;

            case FbxNodeAttribute::eMarker:
            {
                //                ImportMarker(childNode);
                break;
            }

            case FbxNodeAttribute::eSkeleton:
            {
                UpdateSkeleton(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbs:
            {
                //                ImportNurbs(childNode);
                break;
            }

            case FbxNodeAttribute::ePatch:
            {
                //                ImportPatch(childNode);
                break;
            }

            case FbxNodeAttribute::eCameraStereo:
            {
                //                ImportCameraStereo(childNode);
                break;
            }

            case FbxNodeAttribute::eCameraSwitcher:
            {
                //                ImportCameraSwitcher(childNode);
                break;
            }

            case FbxNodeAttribute::eOpticalReference:
            {
                //                ImportOpticalReference(childNode);
                break;
            }

            case FbxNodeAttribute::eOpticalMarker:
            {
                //                ImportOpticalMarker(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbsCurve:
            {
                //                ImportNurbsCurve(childNode);
                break;
            }

            case FbxNodeAttribute::eTrimNurbsSurface:
            {
                //                ImportTrimNurbsSurface(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbsSurface:
            {
                //                ImportNurbsSurface(childNode);
                break;
            }

            case FbxNodeAttribute::eShape:
            {
                //                ImportShape(childNode);
                break;
            }
            case FbxNodeAttribute::eLODGroup:
            {
                //                ImportLODGroup(childNode);
                break;
            }
            case FbxNodeAttribute::eSubDiv:
            {
                //                ImportSubDiv(childNode);
                break;
            }
            case FbxNodeAttribute::eCachedEffect:
            {
                //                ImportCachedEffect(childNode);
                break;
            }
            case FbxNodeAttribute::eLine:
            {
                UpdateLine(childNode);
                break;
            }

            case FbxNodeAttribute::eNull:
            {
                UpdateFBXDummy(childNode);
            }

            break;

            case FbxNodeAttribute::eUnknown:
            default:
                break;

        }
        UpdateFBXNode(childNode);
    }
}


void SceneFBX::UpdateFBXScene()
{
    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode)
        UpdateFBXNode(lRootNode);
}


void SceneFBX::UpdateFBXDummy(FbxNode* node)
{
    //UDP3DSMAX
    FbxProperty userProperties = node->FindProperty("UDP3DSMAX", false);
    if (userProperties.IsValid()) {

        FbxString fsfs = userProperties.Get<FbxString>();
        int numTokens = fsfs.GetTokenCount("\r\n");

        for (auto tkn = 0; tkn < numTokens; tkn++) {
            FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
//            FbxString currTokenName = currToken.GetToken(0, "=").Buffer();
            FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            
            // particles for engine thruster
            if (_strcmpi(currTokenName.Buffer(), "particleEmitter") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {
                    Entity ementity = wScene.Entity_FindByName(node->GetName());
                    if (ementity) {
                        wiEmittedParticle& emtComp = *wScene.emitters.GetComponent(ementity);

                        TransformComponent& emtc = *wScene.transforms.GetComponent(ementity);
                        MaterialComponent& material = *wScene.materials.GetComponent(ementity);

                        FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);

                        int MAX_PARTICLES = getCustomIntAttribute(node, "MAX_PARTICLES");
                        if (MAX_PARTICLES == 0) {
                            MAX_PARTICLES = 1;
                            wScene.Entity_Remove(ementity);
                        }

//                        emtComp.SetMaxParticleCount(MAX_PARTICLES); // WARNING: changing this resets the emmiter
                        emtComp.count = getCustomFloatAttribute(node, "count");
                        emtComp.FIXED_TIMESTEP = getCustomFloatAttribute(node, "FIXED_TIMESTEP");
                        emtComp.random_factor = getCustomFloatAttribute(node, "random_factor");
                        emtComp.random_life = getCustomFloatAttribute(node, "random_life");
                        emtComp.life = getCustomFloatAttribute(node, "life");
                        emtComp.rotation = getCustomFloatAttribute(node, "rotation");
                        emtComp.normal_factor = getCustomFloatAttribute(node, "normal_factor");
                        emtComp.size = getCustomFloatAttribute(node, "size");
  
                        emtc.ClearTransform();
                        emtc.Translate(ConvertVertex(a.GetT()));
                        emtc.UpdateTransform();
                    }
                }
            }


            if (_strcmpi(currTokenName.Buffer(), "ForceField") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {

                    std::string forcefield = node->GetName();
                    FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
                    Entity ffentity = wScene.Entity_FindByName(forcefield + "_force");
                    ForceFieldComponent& ffcomp = *wScene.forces.GetComponent(ffentity);
                    TransformComponent& fftc = *wScene.transforms.GetComponent(ffentity);

                    ffcomp.gravity = getCustomFloatAttribute(node, "gravity");
                    ffcomp.range_local = getCustomFloatAttribute(node, "range_local");

                    fftc.ClearTransform();
                    fftc.Translate(ConvertVertex(a.GetT()));
                    fftc.UpdateTransform();

                    /*
                    wiRenderer::RenderablePoint pnt;
                    pnt.position = ConvertVertex(a.GetT());
                    pnt.color = wiColor::Red();
                    pnt.size = ffcomp.range_local;
                    wiRenderer::DrawPoint(pnt);*/
                }
            }

            // reflection probe
            if (_strcmpi(currTokenName.Buffer(), "probe") == 0) {
                Entity probeEntity = wScene.Entity_FindByName(node->GetName());
                EnvironmentProbeComponent* probe = wScene.probes.GetComponent(probeEntity);
                TransformComponent& emtc = *wScene.transforms.GetComponent(probeEntity);
                FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, probeEntity);
                emtc.MatrixTransform(ConvertMatrix(a));
                emtc.UpdateTransform();
            }
        }
    }

}

float SceneFBX::getCustomFloatAttribute(FbxNode *node, char *attrname)
{
    float res = 0.f;
    FbxProperty _tmpAttrib = node->FindProperty(attrname, false);
    if (_tmpAttrib.IsValid())
    {
        _tmpAttrib.EvaluateValue(FBXSceneTime).Get(&res, fbxsdk::EFbxType::eFbxFloat);
    }
    return res;
}

int SceneFBX::getCustomIntAttribute(FbxNode* node, char* attrname)
{
    int res = 0;
    FbxProperty _tmpAttrib = node->FindProperty(attrname, false);
    if (_tmpAttrib.IsValid())
    {
        _tmpAttrib.EvaluateValue(FBXSceneTime).Get(&res, fbxsdk::EFbxType::eFbxInt);
    }
    return res;
}

FbxString SceneFBX::getCustomStrAttribute(FbxNode* node, char* attrname)
{
    FbxString resStr;
    FbxProperty _tmpAttrib = node->FindProperty(attrname, false);
    if (_tmpAttrib.IsValid())
    {
        resStr = _tmpAttrib.Get<FbxString>();
    }
    return resStr;
}

void SceneFBX::ImportFBXDummy(FbxNode* node)
{
    //UDP3DSMAX
    FbxProperty userProperties = node->FindProperty("UDP3DSMAX", false);
    if (userProperties.IsValid()) {

        FbxString fsfs = userProperties.Get<FbxString>();
        int numTokens = fsfs.GetTokenCount("\r\n");

        for (auto tkn = 0; tkn < numTokens; tkn++) {
            FbxString currToken = fsfs.GetToken(tkn, "\r\n").Buffer();
            FbxString currTokenName = currToken.GetToken(0, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();
            FbxString currTokenValue = currToken.GetToken(1, "=").UnPad(FbxString::EPaddingType::eBoth, ' ').Buffer();

            // particles emitter params
            if (_strcmpi(currTokenName.Buffer(), "particleEmitter") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {

                    printf("particleEmitter: %s\n", currTokenValue.Buffer());

                    Entity ementity = wScene.Entity_CreateEmitter(node->GetName());
                    wiEmittedParticle& emtComp = *wScene.emitters.GetComponent(ementity);

                    TransformComponent& emtc = *wScene.transforms.GetComponent(ementity);
                    MaterialComponent* material = wScene.materials.GetComponent(ementity);
                    material->baseColorMapName = "data/flare2.png";
                    material->baseColorMap = wiResourceManager::Load(getFromTexturePath(material->baseColorMapName));
                    material->baseColor = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
                    material->userBlendMode = BLENDMODE_ADDITIVE;

                    FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);
                    emtc.ClearTransform();
                    emtc.Translate(ConvertVertex(a.GetT()));
                    emtc.UpdateTransform();

                    int MAX_PARTICLES = getCustomIntAttribute(node, "MAX_PARTICLES");
                    emtComp.SetMaxParticleCount(MAX_PARTICLES);

                    emtComp.count = getCustomFloatAttribute(node, "count");
                    printf("count : %f\n", emtComp.count);

                    emtComp.FIXED_TIMESTEP = getCustomFloatAttribute(node, "FIXED_TIMESTEP");
                    printf("timeStep : %f\n", emtComp.FIXED_TIMESTEP);

                    emtComp.random_factor = getCustomFloatAttribute(node, "random_factor");
                    printf("random_factor : %f\n", emtComp.random_factor);

                    emtComp.random_life = getCustomFloatAttribute(node, "random_life");
                    printf("random_life : %f\n", emtComp.random_life);

                    emtComp.life = getCustomFloatAttribute(node, "life");
                    printf("life : %f\n", emtComp.life);

                    emtComp.rotation = getCustomFloatAttribute(node, "rotation");
                    printf("rotation : %f\n", emtComp.rotation);

                    emtComp.normal_factor = getCustomFloatAttribute(node, "normal_factor");
                    printf("normal_factor : %f\n", emtComp.normal_factor);

                    emtComp.size = getCustomFloatAttribute(node, "size");
                    printf("size : %f\n", emtComp.size);

                    emtComp.SetSorted(true);
                    emtComp.shaderType = wiEmittedParticle::PARTICLESHADERTYPE::SOFT;
                }
            }

            if (_strcmpi(currTokenName.Buffer(), "ForceField") == 0) {
                if (_strcmpi(currTokenValue.Buffer(), "true") == 0) {

                    FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime);

                    std::string forcefield = node->GetName();
                    Entity ffentity = wScene.Entity_CreateForce(forcefield + "_force");
                    ForceFieldComponent &ffcomp = *wScene.forces.GetComponent(ffentity);
                    TransformComponent& fftc = *wScene.transforms.GetComponent(ffentity);

                    ffcomp.gravity = getCustomFloatAttribute(node, "gravity");
                    printf("gravity : %f\n", ffcomp.gravity);

                    ffcomp.range_local = getCustomFloatAttribute(node, "range_local");
                    printf("range_local : %f\n", ffcomp.range_local);

                    ffcomp.type = getCustomIntAttribute(node, "type");
                    if (ffcomp.type == 200) {
                        printf("type : %s\n", "ENTITY_TYPE_FORCEFIELD_POINT");
                    }
                    else {
                        printf("type : %s\n", "ENTITY_TYPE_FORCEFIELD_PLANE");
                    }
                    fftc.ClearTransform();
                    fftc.Translate(ConvertVertex(a.GetT()));
                    fftc.UpdateTransform();
                }
            }

            // environment cubemap
            if (_strcmpi(currTokenName.Buffer(), "environment") == 0) {
                auto& wc = GetWeather();
//                if (strlen(currTokenValue) == 0) {
                    wc.skyMapName = "data/skb-new2k.dds";
//                }
//                else {
//                    wc.skyMapName = currTokenValue;
//                }

                wc.skyMap = wiResourceManager::Load(getFromTexturePath(wc.skyMapName));
                printf("Loaded environment cubemap %s\n", wc.skyMapName.c_str());
            }

            // reflection probe
            if (_strcmpi(currTokenName.Buffer(), "probe") == 0) {
                printf("Reflection probe\n");
                Entity probeEntity = wScene.Entity_CreateEnvironmentProbe(node->GetName(), ConvertVertex(lSceneEvaluator->GetNodeGlobalTransform(node, FBXSceneTime).GetT()));
                EnvironmentProbeComponent *probe = wScene.probes.GetComponent(probeEntity);
                probe->range = 12.f;
                probe->SetRealTime(true);

                TransformComponent& emtc = *wScene.transforms.GetComponent(probeEntity);
                FbxAMatrix a = lSceneEvaluator->GetNodeGlobalTransform(node, probeEntity);
                emtc.MatrixTransform(ConvertMatrix(a));
                emtc.UpdateTransform();
            }
        }
    }
}

void SceneFBX::ImportFBXNode(FbxNode* node)
{
	const char* nodeName = node->GetName();
    /*
	std::string objname = nodeName;
    {

        FbxVector4 lZero(0, 0, 0);

        // Activate pivot converting
        node->SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive);
        node->SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive);

        // We want to set all these to 0 and bake them into the transforms.
        node->SetPostRotation(FbxNode::eDestinationPivot, lZero);
        node->SetPreRotation(FbxNode::eDestinationPivot, lZero);
        node->SetRotationOffset(FbxNode::eDestinationPivot, lZero);
        node->SetScalingOffset(FbxNode::eDestinationPivot, lZero);
        node->SetRotationPivot(FbxNode::eDestinationPivot, lZero);
        node->SetScalingPivot(FbxNode::eDestinationPivot, lZero);

        // This is to import in a system that supports rotation order.
        // If rotation order is not supported, do this instead:
        node->SetRotationOrder(FbxNode::eDestinationPivot, eEulerXYZ);
        //        ERotationOrder lRotationOrder;
        //        node->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);
        //        node->SetRotationOrder(FbxNode::eDestinationPivot, lRotationOrder);

                // Similarly, this is the case where geometric transforms are supported by the system.
                // If geometric transforms are not supported, set them to zero instead of
                // the source’s geometric transforms.
                // Geometric transform = local transform, not inherited by children.
        node->SetGeometricTranslation(FbxNode::eDestinationPivot, node->GetGeometricTranslation(FbxNode::eSourcePivot));
        node->SetGeometricRotation(FbxNode::eDestinationPivot, node->GetGeometricRotation(FbxNode::eSourcePivot));
        node->SetGeometricScaling(FbxNode::eDestinationPivot, node->GetGeometricScaling(FbxNode::eSourcePivot));

        // Idem for quaternions.
        node->SetQuaternionInterpolation(FbxNode::eDestinationPivot, node->GetQuaternionInterpolation(FbxNode::eSourcePivot));
    }*/

    // When the setup is done, call ConvertPivotAnimationRecursive to the wScene’s root node.
// Sampling rate e.g. 30.0.
//    lScene->GetRootNode()->ConvertPivotAnimationRecursive(0, FbxNode::eDestinationPivot, 30.0);


	// Determine # of children the node has
	int numChildren = node->GetChildCount();
	FbxNode* childNode = 0;
	for (int i = 0; i < numChildren; i++)
	{
		childNode = node->GetChild(i);
        std::string childname = childNode->GetName();

        FbxNodeAttribute* nodeAttrib = childNode->GetNodeAttribute();
        if (!nodeAttrib)
            continue;

        FbxNodeAttribute::EType ntype = nodeAttrib->GetAttributeType();

        if (!ntype)
            continue;

        switch (ntype) {
            case FbxNodeAttribute::eMesh: 
            {
                ImportMesh(childNode);
                break;
            }

            case FbxNodeAttribute::eCamera:
            {
                ImportCamera(childNode);
                break;
            }

            case FbxNodeAttribute::EType::eLight:
            {
                ImportLight(childNode);
            }
                break;

            case FbxNodeAttribute::eMarker:
            {
                //                ImportMarker(childNode);
                break;
            }

            case FbxNodeAttribute::eSkeleton:
            {
                ImportSkeleton(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbs:
            {
                //                ImportNurbs(childNode);
                break;
            }

            case FbxNodeAttribute::ePatch:
            {
                //                ImportPatch(childNode);
                break;
            }

            case FbxNodeAttribute::eCameraStereo:
            {
                //                ImportCameraStereo(childNode);
                break;
            }

            case FbxNodeAttribute::eCameraSwitcher:
            {
                //                ImportCameraSwitcher(childNode);
                break;
            }

            case FbxNodeAttribute::eOpticalReference:
            {
                //                ImportOpticalReference(childNode);
                break;
            }

            case FbxNodeAttribute::eOpticalMarker:
            {
                //                ImportOpticalMarker(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbsCurve:
            {
                //                ImportNurbsCurve(childNode);
                break;
            }

            case FbxNodeAttribute::eTrimNurbsSurface:
            {
                //                ImportTrimNurbsSurface(childNode);
                break;
            }

            case FbxNodeAttribute::eNurbsSurface:
            {
                //                ImportNurbsSurface(childNode);
                break;
            }

            case FbxNodeAttribute::eShape:
            {
                //ImportShape(childNode);
                break;
            }
            case FbxNodeAttribute::eLODGroup:
            {
                //                ImportLODGroup(childNode);
                break;
            }
            case FbxNodeAttribute::eSubDiv:
            {
                //                ImportSubDiv(childNode);
                break;
            }
            case FbxNodeAttribute::eCachedEffect:
            {
                //                ImportCachedEffect(childNode);
                break;
            }

            case FbxNodeAttribute::eLine:
            {
                ImportLine(childNode);
                break;
            }


            case FbxNodeAttribute::eNull:
            {
                ImportFBXDummy(childNode);
            }
            break;
            
            case FbxNodeAttribute::eUnknown:
            default:
                break;

        }
		ImportFBXNode(childNode);
	}

    //lScene->GetRootNode()->ConvertPivotAnimationRecursive(0, FbxNode::eDestinationPivot, 30.0);
}


void SceneFBX::ImportFBXFile(char* filename)
{
	lSdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

    lImporter->SetEmbeddingExtractionFolder("data/textures");

	lScene = FbxScene::Create(lSdkManager, "FBXScene");
	lImporter->Import(lScene);
	lImporter->Destroy();

    // Let us assume that myScene is already created
    lSceneEvaluator = lScene->GetAnimationEvaluator();
/*
    FbxGeometryConverter geometryConverter(lSdkManager);
    geometryConverter.Triangulate(lScene, true);

    
    FbxTimeSpan gTimeSpan;
    lScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(gTimeSpan);
    FbxTime::EMode gTimeMode = lScene->GetGlobalSettings().GetTimeMode();
    FbxTime::EProtocol gTimeProtocol = lScene->GetGlobalSettings().GetTimeProtocol();
    SceneTimeDuration = gTimeSpan.GetStop().GetSecondDouble();

    FbxSystemUnit SceneUnitSystem = lScene->GetGlobalSettings().GetSystemUnit();
    FbxSystemUnit OurUnitSystem = FbxSystemUnit::m;

    FbxSystemUnit::ConversionOptions lConversionOptions = {
        true, // mConvertRrsNodes 
        true, // mConvertLimits 
        true, // mConvertClusters 
        true, // mConvertLightIntensity 
        true, // mConvertPhotometricLProperties 
        true // mConvertCameraClipPlanes 
    };

    if (SceneUnitSystem != OurUnitSystem)
    {
        OurUnitSystem.ConvertScene(lScene, lConversionOptions);
    }
*/
    FbxAxisSystem SceneAxisSystem = lScene->GetGlobalSettings().GetAxisSystem();
    FbxAxisSystem OurAxisSystem = FbxAxisSystem::eDirectX;
    if (SceneAxisSystem != OurAxisSystem)
    {
        OurAxisSystem.DeepConvertScene(lScene);
    }

	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		ImportFBXNode(lRootNode);
	}
//    wiRenderer::SetShadowPropsCube(1024, -1);
//    wiRenderer::SetShadowProps2D(1024, -1, -1);
//    wiRenderer::SetTransparentShadowsEnabled(true);
}

SceneFBX::SceneFBX()
{
    FBXSceneTime.SetSecondDouble(0);   // Starting time
    FBXTakeStartTime = 0;
    lScene = 0;
}

SceneFBX::~SceneFBX()
{
    lSdkManager->Destroy();
}
