#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
//#include "VectorMatrix.h"
#include "glew/include/glew.h"

// avoids nasty linker errors in modern VisualStudio versions
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef __cplusplus
FILE iob[] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE* __cdecl _iob(void) { return iob; }
}
#endif

#include <mmsystem.h>
//#include "ESLight.h"
#include "ESVectorMatrix.h"
#include "ESTexture.h"
//#include "ESVector2d.h"
#include "ESUtility.h"

#include "lib3ds/include/file.h"
#include "lib3ds/include/camera.h"
#include "lib3ds/include/mesh.h"
#include "lib3ds/include/node.h"
#include "lib3ds/include/material.h"
#include "lib3ds/include/matrix.h"
#include "lib3ds/include/vector.h"
#include "lib3ds/include/light.h"
#include <string.h>
//#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <gl/gl.h>

#include "fmod/FMOD.H"
//#include "VectorMatrix.h"
#include "ESConstants.h"
#include "ESKeyFrame.h"
#include "ESInterpolation.h"

#include "ESModel3DS.h"
#include "ESFMod.h"
#include "ESPicture.h"
#include "ESScene.h"
#include "ESScenePlayer.h"

#include "ESOverlayEffects.h"
#include "bfl/BFL.h"
#include "ESParticles.h"
#include "ESCaption.h"
#include "nvtristrip/NvTriStrip.h"


#include "ESScene01.h"
#include "ESScene02.h"
#include "ESScene03.h"
#include "ESScene04.h"
#include "ESScene05.h"
#include "ESScene06.h"
#include "ESScene07.h"
#include "ESScene08.h"
#include "ESScene09.h"
#include "ESScene10.h"
#include "ESScene11.h"
#include "ESScene12.h"
#include "ESScene13.h"
#include "ESScene14.h"

extern unsigned int window_width;
extern unsigned int window_height;

#define bitsPerPixel  32
#define bitsPerDepth  24

#define render_to_texture_width 512
#define render_to_texture_height 512

#define MAX_SCENEKEYFRAMES 256

