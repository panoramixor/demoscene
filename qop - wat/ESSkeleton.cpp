// qop  - warped and twisted
// a demo for flashparty 2003

#include "stdafx.h"

KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];
CEaseInOutInterpolateKeyframe InterpolatorArray[MAX_SCENEKEYFRAMES];
CEaseInOutInterpolateKeyframe InterpolatorArrayBackUp[MAX_SCENEKEYFRAMES];

// avoids nasty linker errors in modern VisualStudio versions
#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef __cplusplus
FILE iob[] = { *stdin, *stdout, *stderr };
extern "C" {
	FILE* __cdecl _iob(void) { return iob; }
}
#endif

int window_width = 0;
int window_height = 0;
int global_depthbits = 24;
int global_pixelbits = 32;
bool global_nosound, global_showfps;


// Globals
DEVMODE device_mode;

HDC hDC;
HGLRC hRC;

char		  szAppName[]="warped and twisted (c) qop";
char	szViewportPreview[] = "ViewPort";
//extern FILE*  INIFile;
HANDLE		  hProcess;													// Demo process handle, we use it to set it as 

//float pixels[window_width*window_height];
LARGE_INTEGER TimerFreq;		// Timer Frequency
LARGE_INTEGER TimeStart;		// Time of start
LARGE_INTEGER TimeCur;			// Current time
float		  fTime;			
float		  fDeltaTime;		// Time passed since first frame


HINSTANCE hInstance;

void Play_Music();
void Stop_Music();
void Seek_Music(int newtime);
int Music_GetTime();

void ShutDown();

extern bool Sequencer();

KeyFrame interfacedata;
KeyFrame klast;

bool bReady;
bool bSetKeyFrame;
bool bAddKeyFrame;
bool bSaveCameraPath;
bool bClickRotate;
bool bLoaded;

int nPixelFormat;

HWND hKeyFrameDlg;
HWND hLoadCameraPathDlg;
HWND hSaveCameraPathDlg;
HWND hEditDlg;
HWND hOptionsDlg;
HWND hWnd;

float scenetime;
bool bPlay = false;
bool bRew = false;
bool bForw = false;
bool bAddKf = false;
bool bUpdate = false;
bool bFromViewPort = false;
bool bLoadCameraPath = false;
bool bSavedCameraFile = false;
extern void setEditorValues(KeyFrame);
extern void getEditorValues(KeyFrame);
extern void ResetKeyFrameData(KeyFrame);
extern void InsertKeyframe(KeyFrame kd, int index);
extern void DeleteKeyFrame(int index);
extern void ClearKeyFrames();
extern void SetupInterpolators();
extern int kfindex;
extern bool LoadDemo(HWND);
extern void UnLoadDemo();
extern bool init_seq_scenetest();
extern void seq_scenetest();

extern KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];
extern CEaseInOutInterpolateKeyframe InterpolatorArray[MAX_SCENEKEYFRAMES];
extern CEaseInOutInterpolateKeyframe InterpolatorArrayBackUp[MAX_SCENEKEYFRAMES];

void CreateMainWindow();

// The function pointers for multitexturing.
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

BOOL CALLBACK OptionsDlg(HWND hwndDlg,	UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitDisplay() {

		ShowWindow(hWnd, SW_HIDE);
		ShowCursor(false);		

		// Remember our drawing context.
		hDC = GetDC( hWnd );

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |	PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;								   // RGBA Color mode
		pfd.cColorBits = global_pixelbits;
		pfd.cDepthBits = global_depthbits;

		// Choose a pixel format that best matches that described in pfd
		nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	
		// Set the pixel format for the device context
		SetPixelFormat(hDC, nPixelFormat, &pfd);

		// Yeppers, make something that OpenGL understands.
		hRC = wglCreateContext( hDC );
		wglMakeCurrent( hDC, hRC );

		const GLubyte *strext;
		strext = glGetString(GL_EXTENSIONS);
		LogLine((char *)"---- supported OpenGL extensions ----\n%s\n---- end of list ----\n", (char *)strext);

	// Reset perspective.
	glViewport(0, 0, (GLsizei) window_width, (GLsizei)window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the perspective we're going to use.
	gluPerspective(90, (GLfloat)window_width / (GLfloat)window_height, 0.01, 110.0);
	glMatrixMode(GL_MODELVIEW);
	
	// Reset the view matrix position.

	glLoadIdentity();	
	glTranslatef(0.0, 0.0, 0.0);
	
	// Select "nicest" perspective correction.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	bReady = true;
}

BOOL CALLBACK OptionsDlg(HWND hwndDlg,	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	hOptionsDlg = hwndDlg;
	static bool loader = false;
	UINT timer=0;
	switch(uMsg)
	{
			//UpdateWindow(hwndDlg);
		case WM_INITDIALOG:
			InitCommonControls();
			CheckRadioButton(hwndDlg, IDC_640x480, IDC_1024x768, IDC_800x600);
			return 0;
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			ExitProcess(NULL);
			break;

		case WM_TIMER:
			if(loader) {
				bLoaded = LoadDemo(hWnd);
				if (bLoaded) {
					KillTimer(hwndDlg, timer);
					ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN);
					ShowWindow(hwndDlg, SW_HIDE);
					ShowWindow(hWnd, SW_SHOW);
					loader = false;
				}
			} 

			break;

		case WM_COMMAND:
			switch(wParam)
			{
				case ID_OPTIONS_START:
					{
						if(IsDlgButtonChecked(hwndDlg, IDC_OPTIONS_NOSOUND) == BST_CHECKED)	{
							global_nosound = true;
						}

						if(IsDlgButtonChecked(hwndDlg, IDC_OPTIONS_SHOWFPS) == BST_CHECKED)	{
							global_showfps = true;
						}

						if(IsDlgButtonChecked(hwndDlg, IDC_640x480) == BST_CHECKED)	{
							window_width = 640;
							window_height = 480;
						} else if(IsDlgButtonChecked(hwndDlg, IDC_800x600) == BST_CHECKED)	{
							window_width = 800;
							window_height = 600;
						} else if(IsDlgButtonChecked(hwndDlg, IDC_1024x768) == BST_CHECKED)	{
							window_width = 1024;
							window_height = 768;
						}
						DeleteFile("demo.log");
						LogLine("%s\n", "---- started ----");
						LogLine("Display resolution: %ix", window_width);
						LogLine("%i ", window_height);
						LogLine("%ibits\n", global_pixelbits);

						CreateMainWindow();
						InitDisplay();
						QueryPerformanceFrequency(&TimerFreq);
						QueryPerformanceCounter(&TimeStart);
						timer = SetTimer(hwndDlg, 1, 10, NULL);
						loader = true;
					}
					break;

				case ID_OPTIONS_EXIT:
					ExitProcess(0);
					break;
			}
	}
	return FALSE;
}
//-----------------------------------------------------------------------------------------------
// SkeletonProc(): Main loop.
//-----------------------------------------------------------------------------------------------

LRESULT CALLBACK SkeletonProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_SYSCOMMAND:						// handle syscommand message
			switch (wParam)						
			{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:			// Monitor Trying To Enter Powersave?
				return 0;						// just say NO! :)
			}
			break;								// Exit

	//-------------------------------------------------------------------------------------------
	// Create device context and initialize it.
	//-------------------------------------------------------------------------------------------

    case WM_CREATE:
		{
			LogLine("created!->>\n", NULL);
		}
		break;

	//-------------------------------------------------------------------------------------------
	// Key pressed, Mouse button pressed, or just been ordered to get lost. Shutdown.
	//-------------------------------------------------------------------------------------------
	case WM_LBUTTONDOWN:

	case WM_CHAR:
		if(bLoaded) {
			LogLine("%s\n", "---- user abort ----");
		} else {
			LogLine("%s\n", "---- user abort [IGNORED WHILE LOADING] ----");
		}

	case WM_DESTROY:
		if(bLoaded) {
			UnLoadDemo();
			CloseWindow(hWnd);
			HWND startmenu = FindWindow("Shell_TrayWnd", NULL);
			ShowWindow(startmenu, SW_SHOW);
			LogLine("%s\n", "---- ended ----");
			ShutDown();
		}
		break;

	case WM_QUIT:
			ExitProcess(NULL);
			break;

	//-------------------------------------------------------------------------------------------
	// Draw a frame.
	//-------------------------------------------------------------------------------------------

	case WM_PAINT:
		{
			if(bReady == true) {

				ShowWindow(hWnd, SW_SHOW);
				// Clear screen before rendering.
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// Remember the working matrix and attributes.
				glPushMatrix();
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				QueryPerformanceCounter(&TimeCur);
				float fOldTime = fTime;
				fTime = (float)((double)(TimeCur.QuadPart-TimeStart.QuadPart)/(double)TimerFreq.QuadPart);
				fDeltaTime = fTime - fOldTime;
				if(Sequencer()) 
					SendMessage(hWnd, WM_DESTROY, 0, 0);
				glPopAttrib();
				glPopMatrix();

				// Make it all visible at once!
				SwapBuffers( hDC );
			}
		}
		break;
	//-------------------------------------------------------------------------------------------
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}

void CreateMainWindow()
{
	WNDCLASS wc;

	hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	SetThreadPriority(hProcess, THREAD_PRIORITY_HIGHEST);		// I know this is not really the highest, but I crashed win2k 3 times with THREAD_PRIORITY_TIME_CRITICAL

	// Change display mode
	memset(&device_mode, 0, sizeof(DEVMODE));
	int i = 0;

	while(EnumDisplaySettings(NULL,i,&device_mode))	{
		if((device_mode.dmPelsWidth == window_width) && (device_mode.dmPelsHeight == window_height) && (device_mode.dmBitsPerPel == global_pixelbits)) {
			break;
		}
		i++;
	}

	// Setup the window class -------------------------------------------------------------------

	wc.style	     = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)SkeletonProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon	     = NULL;
	wc.hCursor	     = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = szAppName;

	// Announce it to the Microsloth OS.

	if( !RegisterClass( &wc ) ) {
		FatalError("Cant register %s!", "window class");
		return;						  // Failed to create window, so just quit right away
	}

	// Create window
	hWnd = CreateWindow(
			szAppName,
			szAppName,
			WS_POPUP,
			NULL, NULL,
			device_mode.dmPelsWidth, device_mode.dmPelsHeight,
			NULL, NULL,
			hInstance,
			NULL );

	if( hWnd == NULL ) {
		FatalError("Cant create %s!\r\n", "Window");
		return;						  // Failed to create window, so just quit right away
	}
}


//-----------------------------------------------------------------------------------------------
// WinMain(): Application entry point. Main initialization and shutdown.
//-----------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE Instance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	hInstance = Instance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), NULL, OptionsDlg, 0);

	static BOOL bGotMsg;
	static MSG  msg;

	//-------------------------------------------------------------------------------------------
	// Loop around until told to stop.
    //-------------------------------------------------------------------------------------------

	while(msg.message != WM_QUIT)
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

		// If there is a message, translate and dispatch it.
		if( bGotMsg ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} else {
			if (bReady)
				InvalidateRect(hWnd, NULL, FALSE);
		}
    }
	return msg.wParam;
}

void ShutDown()
{
	// Not ready to render anymore.
	bReady = false;

	// Remove DC and switch display mode back to what it was..
	ChangeDisplaySettings(0, 0);
	wglMakeCurrent( hDC, NULL );
	wglDeleteContext( hRC );

	// Signal shutdown complete.
	PostQuitMessage( WM_QUIT );
}

