// fp05invtro  - an invitation intro to Flashparty 2005
// qop + 779

#include "stdafx.h"
#include "fp05invtro.h"

CScenePlayer sp;

// this is awefull but this is the only way i can pass a callback to fmod and
// get access to ScenePlayer members from it at the same time.
signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata)
{
	if(!_strcmpi((char *)buff, "finish")) {
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
	
	// leete los docs de glfw... buena lib aunque por alguna razon hoy no me anda en fullscreen, debe ser el puto driver :)
#ifdef _DEBUG
	if(!glfwOpenWindow(640, 480, 0,0,0,32, 24,0, GLFW_WINDOW) )
#else
	if(!glfwOpenWindow(window_width, window_height, 0,0,0,32, 24,0, GLFW_FULLSCREEN) )
#endif
	{
		glfwTerminate();
		return 0;
	}
	glfwSetWindowTitle("flashparty 05 invtro - QOP+779");
	glfwDisable(GLFW_STICKY_KEYS);
	glfwSwapInterval(1);			// esperar o no el refresh
	glfwSetTime(0.0);

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

	glClearColor(1, 1, 1, 1);		// blanco como para sacarte la duda cuando algo no se ve ;]
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);


	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glDepthRange(0, 2000);
	char s_fps[100] = { 0 };
	
	// load syncpoint file
	sp.Load("./data/syncpoints.ess"); 
	glfwSetTime(0.1);

	// main loop 
	do
	{
		// timing
		t = glfwGetTime();
		fTime = (float)t;
		dt = t - t_old;
		t_old = t;

#ifdef _DEBUG
		// imprimo los fps en el titlebar
		sprintf(s_fps, "fps: %f | time: %f", esGetFps((float)dt), fTime);
		glfwSetWindowTitle(s_fps);
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		sp.Play();

		glfwSwapBuffers();

		running = !(glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED ));
	}
	while( running );

	glfwTerminate(); 
	return 0;
}
