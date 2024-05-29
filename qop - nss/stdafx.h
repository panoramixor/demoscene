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
#include "lib/lib3ds/include/file.h"
#include "lib/lib3ds/include/camera.h"
#include "lib/lib3ds/include/mesh.h"
#include "lib/lib3ds/include/node.h"
#include "lib/lib3ds/include/material.h"
#include "lib/lib3ds/include/matrix.h"
#include "lib/lib3ds/include/vector.h"
#include "lib/lib3ds/include/light.h"
#include <string.h>
#include <stdlib.h>
#include <math.h> 
#include "lib/glew/include/GL/glew.h"
#include "lib/glew/include/GL/wglew.h"
#include "lib/glfw/include/GL/glfw.h" 
#include "VectorMatrix.h"
#include "ESVectorMatrix.h"
#include "ESUtility.h"

#include "framebufferObject.h"
#include "renderbuffer.h"
#include "ESRenderToTexture.h"
#include "ESLight.h"

#include "lib/devil/include/IL/config.h"
#include "lib/devil/include/IL/il.h"
#include "lib/devil/include/IL/ilu.h"
#include "lib/devil/include/IL/ilut.h"
#include "lib/devil/include/IL/devil_internal_exports.h"
#include "lib/fmod/fmod.h"

#include "ESInterpolator.h"
#include "ESDevilTexture.h"
#include "RenderTexture.h"
#include "ESConstants.h"
#include "ESKeyFrame.h"

#include "ESFont.h"
#include "ESCaption.h"

typedef struct _quad {
	int idx[4];
} quad;

#include "ESCube.h"
#include "ESModel3DS.h"
#include "ESFmod.h"
#include "ESScene.h"
#include "ESScenePlayer.h"
#include "ESScene00.h"
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

#include "lib/nvtristrip/NvTriStrip.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "lib/glew/lib/glew32.lib")
#pragma comment (lib, "lib/glfw/lib/glfw.lib")
#pragma comment (lib, "lib/devil/lib/devil.lib")
#pragma comment (lib, "lib/devil/lib/ilu.lib")
#pragma comment (lib, "lib/devil/lib/ilut.lib")
#pragma comment (lib, "lib/fmod/fmodvc.lib")

#ifdef _DEBUG
	#pragma comment (lib, "lib/lib3ds/lib/lib3ds-130sd.lib")
#else
	#pragma comment (lib, "lib/lib3ds/lib/lib3ds-130s.lib")
#endif

#pragma comment(lib, "lib/nvtristrip/nvtristrip.lib")

#ifdef _DEBUG
	#define window_width 512
	#define window_height 384
#else
	#define window_width 1024
	#define window_height 768
#endif

#define render_to_texture_width 512
#define render_to_texture_height 512

#define ES_OPACITYMAP		0x00000001
#define ES_DIFUSECOLORMAP	0x00000010
#define ES_REFLECTIONMAP	0x00000100
#define ES_NONE				0x00000000


// TODO: reference additional headers your program requires here
