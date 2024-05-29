// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <iostream>
#include <tchar.h>
#include <string.h>
//#include "config.h"
#include <stdlib.h>
#include <math.h> 
#include "lib/glew/include/GL/glew.h"
#include "lib/glew/include/GL/wglew.h"
#include "lib/glfw/include/GL/glfw.h" 
#include "VectorMatrix.h"
#include "ESVectorMatrix.h"
#include "ESUtility.h"

#include "ESTexture.h"

#include "lib/devil/include/IL/config.h"
#include "lib/devil/include/IL/il.h"
#include "lib/devil/include/IL/ilu.h"
#include "lib/devil/include/IL/ilut.h"
#include "lib/devil/include/IL/devil_internal_exports.h"
#include "lib/fmod/fmod.h"

#include "ESDevilTexture.h"
#include "RenderTexture.h"
#include "ESConstants.h"
#include "ESKeyFrame.h"
#include "ESInterpolation.h"


typedef struct _quad {
	int idx[4];
} quad;

#include "ESCube.h"

#include "ESPicture.h"
//#include "ESModel3DS.h"

#include "ESFmod.h"
#include "ESOverlayEffects.h"
#include "ESScene.h"
#include "ESScenePlayer.h"
#include "ESScene01.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "lib/glew/lib/glew32.lib")
#pragma comment (lib, "lib/glfw/lib/glfw.lib")
#pragma comment (lib, "lib/devil/lib/devil.lib")
#pragma comment (lib, "lib/devil/lib/ilu.lib")
#pragma comment (lib, "lib/devil/lib/ilut.lib")
#pragma comment (lib, "lib/fmod/fmodvc.lib")

#define window_width 800
#define window_height 600

#define render_to_texture_width 1024
#define render_to_texture_height 1024

// TODO: reference additional headers your program requires here
