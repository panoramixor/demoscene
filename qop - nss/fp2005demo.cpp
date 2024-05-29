// qop - not so soon
// a demo for flashparty 2005
//

#include "stdafx.h"
#include "fp2005demo.h"

extern void CaptureScreen();
CFMod music;
CScenePlayer sp;

// avoids nasty linker errors in modern VisualStudio versions
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef __cplusplus
FILE iob[] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE* __cdecl _iob(void) { return iob; }
}
#endif


// this is awefull but this is the only way i can pass a callback to fmod and
// get access to ScenePlayer members from it at the same time.
signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata)
{
	sp.take++;

	if(!_strcmpi((char *)buff, "finish")) {
		glfwTerminate();
		return FALSE;
	}
	return TRUE;
}

float fTime;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	glfwInit();

	// a la mierda el .log
	DeleteFile("demo.log");
	glfwOpenWindowHint(GLFW_ICONIFIED, GL_TRUE);

#ifdef _DEBUG
	if(!glfwOpenWindow(window_width, window_height, 0,0,0,32, 32,0, GLFW_WINDOW) )
#else
	if(!glfwOpenWindow(window_width, window_height, 0,0,0,32, 32,0, GLFW_FULLSCREEN) )
#endif
	{
		glfwTerminate();
		return 0;
	}
	glfwIconifyWindow();
	char title[] = "not so soon!";
	glfwSetWindowTitle(title);
	glfwDisable(GLFW_STICKY_KEYS);
	glfwSwapInterval(0);			// esperar o no el refresh

	glDisable(GL_LIGHTING);

	double t;
	double dt;
	double t_old=0;
	int running;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* problem: glewInit failed, something is seriously wrong */
		FatalError("Error: %s\n", (char *)glewGetErrorString(err));
	}

	if (!GLEW_ARB_multitexture)
	{
		MessageBox(NULL, "GL_ARB_multitexture is not present\n", "ERROR!", MB_OK);
		exit(0);
	}
	
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);

	glClearColor(0, 0, 0, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glDepthRange(0, 2000);
	char s_fps[100] = { 0 };

	// load syncpoint file
	glfwSetTime(0.0f);
	sp.Load("./data/syncpoints.ess"); 
	glfwRestoreWindow();

	glfwSetTime(0.0f);
	float sTime = (float)glfwGetTime();
	float starttime = sTime;
	music.Play();
	
	// main loop 
	do
	{
		// timing

		t = glfwGetTime()-starttime;
		fTime = (float)t;
		dt = fTime - t_old;
		t_old = fTime;

#ifdef _DEBUG
		// imprimo los fps en el titlebar
		sprintf(s_fps, "fps: %f | time: %f | scene%d", esGetFps((float)dt), fTime, sp.sceneindex);
		glfwSetWindowTitle(s_fps);
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		sp.Play();

		glfwSwapBuffers();

		if(glfwGetKey(GLFW_KEY_F1))
			CaptureScreen();

		running = !(glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED ));
	}
	while( running );

	glfwTerminate(); 
	return 0;
}
