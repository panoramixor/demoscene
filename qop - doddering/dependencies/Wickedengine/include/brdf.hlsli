#ifndef WI_BRDF_HF
#define WI_BRDF_HF
#include "globals.hlsli"

float3 F_Schlick(in float3 f0, in float f90, in float u)
{
	return f0 + (f90 - f0) * pow(1.f - u, 5.f);
}
float3 F_Fresnel(float3 SpecularColor, float VoH)
{
	float3 SpecularColorSqrt = sqrt(min(SpecularColor, float3(0.99, 0.99, 0.99)));
	float3 n = (1 + SpecularColorSqrt) / (1 - SpecularColorSqrt);
	float3 g = sqrt(n * n + VoH * VoH - 1);
	return 0.5 * sqr((g - VoH) / (g + VoH)) * (1 + sqr(((g + VoH) * VoH - 1) / ((g - VoH) * VoH + 1)));
}

float3 ComputeAlbedo(in float4 baseColor, in float reflectance, in float metalness)
{
	return lerp(lerp(baseColor.rgb, float3(0, 0, 0), reflectance), float3(0, 0, 0), metalness);
}
float3 ComputeF0(in float4 baseColor, in float reflectance, in float metalness)
{
	return lerp(lerp(float3(0, 0, 0), float3(1, 1, 1), reflectance), baseColor.rgb, metalness);
}



struct Surface
{
	float3 P;				// world space position
	float3 N;				// world space normal
	float3 V;				// world space view vector
	float4 baseColor;		// base color [0 -> 1] (rgba)
	float roughness;		// roughness: [0:smooth -> 1:rough] (perceptual)
	float occlusion;		// occlusion [0 -> 1]
	float metalness;		// metalness [0:dielectric -> 1:metal]
	float reflectance;		// reflectivity [0:diffuse -> 1:specular]
	float4 emissiveColor;	// light emission [0 -> 1]
	float sss;				// subsurface scattering [0 -> 1]
	float4 refraction;		// refraction color (rgb), refraction amount (a)

	float alphaRoughness;	// roughness remapped from perceptual to a "more linear change in roughness"
	float alphaRoughnessSq;	// roughness input to brdf functions
	float NdotV;			// cos(angle between normal and view vector)
	float3 f0;				// fresnel value (rgb) (reflectance at incidence angle)
	float f90;				// reflectance at grazing angle
	float3 albedo;			// diffuse light absorbtion value (rgb)
	float3 R;				// reflection vector
	float3 F;				// fresnel term computed from NdotV

	inline void Update()
	{
		alphaRoughness = roughness * roughness;
		alphaRoughnessSq = alphaRoughness * alphaRoughness;

		NdotV = abs(dot(N, V)) + 1e-5f;

		albedo = ComputeAlbedo(baseColor, reflectance, metalness);
		f0 = ComputeF0(baseColor, reflectance, metalness);

		R = -reflect(V, N);
		f90 = saturate(50.0 * dot(f0, 0.33));
		F = F_Schlick(f0, f90, NdotV);
	}
};
inline Surface CreateSurface(
	in float3 P, 
	in float3 N, 
	in float3 V, 
	in float4 baseColor,
	in float roughness,
	in float occlusion,
	in float metalness,
	in float reflectance, 
	in float4 emissiveColor = 0, 
	in float sss = 0)
{
	Surface surface;

	surface.P = P;
	surface.N = N;
	surface.V = V;
	surface.baseColor = baseColor;
	surface.roughness = roughness;
	surface.occlusion = occlusion;
	surface.metalness = metalness;
	surface.reflectance = reflectance;
	surface.emissiveColor = emissiveColor;
	surface.sss = sss;
	surface.refraction = 0;

	surface.Update();

	return surface;
}

struct SurfaceToLight
{
	float3 L;		// surface to light vector (normalized)
	float3 H;		// half-vector between view vector and light vector
	float NdotL;	// cos angle between normal and light direction
	float NdotV;	// cos angle between normal and view direction
	float NdotH;	// cos angle between normal and half vector
	float LdotH;	// cos angle between light direction and half vector
	float VdotH;	// cos angle between view direction and half vector
	float3 F;		// fresnel term computed from VdotH
};
inline SurfaceToLight CreateSurfaceToLight(in Surface surface, in float3 L)
{
	SurfaceToLight surfaceToLight;

	surfaceToLight.L = L;
	surfaceToLight.H = normalize(L + surface.V);

	surfaceToLight.NdotL = saturate(dot(surfaceToLight.L, surface.N));
	surfaceToLight.NdotV = saturate(dot(surface.N, surface.V));
	surfaceToLight.NdotH = saturate(dot(surface.N, surfaceToLight.H));
	surfaceToLight.LdotH = saturate(dot(surfaceToLight.L, surfaceToLight.H));
	surfaceToLight.VdotH = saturate(dot(surface.V, surfaceToLight.H));

	surfaceToLight.F = F_Schlick(surface.f0, surface.f90, surfaceToLight.VdotH);

	return surfaceToLight;
}

// Smith Joint GGX
// Note: Vis = G / (4 * NdotL * NdotV)
// see Eric Heitz. 2014. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs. Journal of Computer Graphics Techniques, 3
// see Real-Time Rendering. Page 331 to 336.
// see https://google.github.io/filament/Filament.md.html#materialsystem/specularbrdf/geometricshadowing(specularg)
float visibilityOcclusion(in Surface surface, in SurfaceToLight surfaceToLight)
{
	float GGXV = surfaceToLight.NdotL * sqrt(surfaceToLight.NdotV * surfaceToLight.NdotV * (1.0 - surface.alphaRoughnessSq) + surface.alphaRoughnessSq);
	float GGXL = surfaceToLight.NdotV * sqrt(surfaceToLight.NdotL * surfaceToLight.NdotL * (1.0 - surface.alphaRoughnessSq) + surface.alphaRoughnessSq);

	float GGX = GGXV + GGXL;
	if (GGX > 0.0)
	{
		return 0.5 / GGX;
	}
	return 0.0;
}

// The following equation(s) model the distribution of microfacet normals across the area being drawn (aka D())
// Implementation from "Average Irregularity Representation of a Roughened Surface for Ray Reflection" by T. S. Trowbridge, and K. P. Reitz
// Follows the distribution function recommended in the SIGGRAPH 2013 course notes from EPIC Games [1], Equation 3.
float microfacetDistribution(in Surface surface, in SurfaceToLight surfaceToLight)
{
	float f = (surfaceToLight.NdotH * surface.alphaRoughnessSq - surfaceToLight.NdotH) * surfaceToLight.NdotH + 1.0;
	return surface.alphaRoughnessSq / (PI * f * f);
}

float3 BRDF_GetSpecular(in Surface surface, in SurfaceToLight surfaceToLight)
{
	float Vis = visibilityOcclusion(surface, surfaceToLight);
	float D = microfacetDistribution(surface, surfaceToLight);
	return surfaceToLight.F * Vis * D * surfaceToLight.NdotL;
}
float3 BRDF_GetDiffuse(in Surface surface, in SurfaceToLight surfaceToLight)
{
	return (1.0 - surfaceToLight.F) / PI * surfaceToLight.NdotL;
}

#endif // WI_BRDF_HF