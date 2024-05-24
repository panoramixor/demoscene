#pragma once
#include "CommonInclude.h"
#include "wiGraphicsDevice.h"
#include "wiFFTGenerator.h"
#include "wiScene_Decl.h"

#include <vector>

class wiOcean
{
public:
	wiOcean(const wiScene::WeatherComponent& weather);

	void UpdateDisplacementMap(const wiScene::WeatherComponent& weather, float time, wiGraphics::CommandList cmd) const;
	void Render(const wiScene::CameraComponent& camera, const wiScene::WeatherComponent& weather, float time, wiGraphics::CommandList cmd) const;

	const wiGraphics::Texture* getDisplacementMap() const;
	const wiGraphics::Texture* getGradientMap() const;

	static void Initialize();
	static void LoadShaders();

protected:
	wiGraphics::Texture displacementMap;		// (RGBA32F)
	wiGraphics::Texture gradientMap;			// (RGBA16F)


	void initHeightMap(const wiScene::WeatherComponent& weather, XMFLOAT2* out_h0, float* out_omega);


	// Initial height field H(0) generated by Phillips spectrum & Gauss distribution.
	wiGraphics::GPUBuffer buffer_Float2_H0;

	// Angular frequency
	wiGraphics::GPUBuffer buffer_Float_Omega;

	// Height field H(t), choppy field Dx(t) and Dy(t) in frequency domain, updated each frame.
	wiGraphics::GPUBuffer buffer_Float2_Ht;

	// Height & choppy buffer in the space domain, corresponding to H(t), Dx(t) and Dy(t)
	wiGraphics::GPUBuffer buffer_Float_Dxyz;


	wiGraphics::GPUBuffer immutableCB;
	wiGraphics::GPUBuffer perFrameCB;
};