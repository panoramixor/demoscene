#include "RenderPath3D_Forward_BindLua.h"

const char RenderPath3D_Forward_BindLua::className[] = "RenderPath3D_Forward";

Luna<RenderPath3D_Forward_BindLua>::FunctionType RenderPath3D_Forward_BindLua::methods[] = {
	lunamethod(RenderPath2D_BindLua, AddSprite),
	lunamethod(RenderPath2D_BindLua, AddFont),
	lunamethod(RenderPath2D_BindLua, RemoveSprite),
	lunamethod(RenderPath2D_BindLua, RemoveFont),
	lunamethod(RenderPath2D_BindLua, ClearSprites),
	lunamethod(RenderPath2D_BindLua, ClearFonts),
	lunamethod(RenderPath2D_BindLua, GetSpriteOrder),
	lunamethod(RenderPath2D_BindLua, GetFontOrder),

	lunamethod(RenderPath2D_BindLua, AddLayer),
	lunamethod(RenderPath2D_BindLua, GetLayers),
	lunamethod(RenderPath2D_BindLua, SetLayerOrder),
	lunamethod(RenderPath2D_BindLua, SetSpriteOrder),
	lunamethod(RenderPath2D_BindLua, SetFontOrder),

	lunamethod(RenderPath_BindLua, Initialize),
	lunamethod(RenderPath_BindLua, OnStart),
	lunamethod(RenderPath_BindLua, OnStop),
	lunamethod(RenderPath_BindLua, GetLayerMask),
	lunamethod(RenderPath_BindLua, SetLayerMask),

	lunamethod(RenderPath3D_BindLua, SetAO),
	lunamethod(RenderPath3D_BindLua, SetSSREnabled),
	lunamethod(RenderPath3D_BindLua, SetShadowsEnabled),
	lunamethod(RenderPath3D_BindLua, SetReflectionsEnabled),
	lunamethod(RenderPath3D_BindLua, SetFXAAEnabled),
	lunamethod(RenderPath3D_BindLua, SetBloomEnabled),
	lunamethod(RenderPath3D_BindLua, SetColorGradingEnabled),
	lunamethod(RenderPath3D_BindLua, SetVolumeLightsEnabled),
	lunamethod(RenderPath3D_BindLua, SetLightShaftsEnabled),
	lunamethod(RenderPath3D_BindLua, SetLensFlareEnabled),
	lunamethod(RenderPath3D_BindLua, SetMotionBlurEnabled),
	lunamethod(RenderPath3D_BindLua, SetSSSEnabled),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldEnabled),
	lunamethod(RenderPath3D_BindLua, SetEyeAdaptionEnabled),
	lunamethod(RenderPath3D_BindLua, SetTessellationEnabled),
	lunamethod(RenderPath3D_BindLua, SetMSAASampleCount),
	lunamethod(RenderPath3D_BindLua, SetSharpenFilterEnabled),
	lunamethod(RenderPath3D_BindLua, SetSharpenFilterAmount),
	lunamethod(RenderPath3D_BindLua, SetMotionBlurStrength),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldFocus),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldStrength),
	lunamethod(RenderPath3D_BindLua, SetDepthOfFieldAspect),
	{ NULL, NULL }
};
Luna<RenderPath3D_Forward_BindLua>::PropertyType RenderPath3D_Forward_BindLua::properties[] = {
	{ NULL, NULL }
};

void RenderPath3D_Forward_BindLua::Bind()
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		Luna<RenderPath3D_Forward_BindLua>::Register(wiLua::GetGlobal()->GetLuaState());
	}
}
