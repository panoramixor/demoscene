// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define MAX_SCENEKEYFRAMES 256
#define MAX_TIMES 11

#include "fmod.h"
#include "fmod_supp.h"
#include "commctrl.h"

#include "ESConstants.h"
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <assert.h>
#include "VectorMatrix.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "dependencies/glext.h"
#include "dependencies/wglext.h"
#include <mmsystem.h>
#include "ESVectorMatrix.h"
#include "ESQuaternion.h"
#include "ESKeyFrame.h"
#include "ESInterpolation.h"
#include "ESTexture.h"
#include "ESModel3DS.h"
#include "ESLight.h"
#include "ESVector2d.h"
#include "ESPicture.h"
#include "bfl.h"
#include "ESUtility.h"
#include "ESCamera.h"
#include "ESSkybox.h"
#include "ESPixelBuffer.h"
#include "ESCaption.h"
#include "ESCameraPath.h"

extern int window_width;
extern int window_height;
extern int global_depthbits;
extern int global_pixelbits;
extern bool global_nosound;
extern bool global_showfps;

extern void LogLine(char* formatstring, ...);


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
