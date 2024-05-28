// qop - continuously eluding subject
// flashparty 2004 demo
// 
// - panoramix 
// - valerialaura

#include "stdafx.h"
#include <commctrl.h>
#include "resource1.h"
#include "3dsdemo.h"

unsigned int window_width = 1440;
unsigned int window_height = 1080;

extern CFMod music;

KeyFrame KeyFrameArray[MAX_SCENEKEYFRAMES];
float sTime = 0;

LARGE_INTEGER TimerFreq;									  // Timer Frequency
LARGE_INTEGER TimeStart;									  // Time of start
LARGE_INTEGER TimeCur;										  // Current time
float		  fTime;										  // Time passed since first frame
float		  fDeltaTime;	

bool  g_bFullScreen = TRUE;								// Set full screen as default
bool  g_bFPS = FALSE;
bool  g_bHighQuality = TRUE;
HWND  g_hWnd;											// This is the handle for the window
HWND  g_hWndOptions;									// This is the handle for the options dialog
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;									// This holds the global hInstance for UnregisterClass() in DeInit()

HANDLE g_exit_event;

#define SCREEN_DEPTH 32

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

// demo flow manager
CScenePlayer sp;

//	This function Handles the main loop
WPARAM MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			QueryPerformanceCounter(&TimeCur);
			float fOldTime = fTime;

			fTime = (float)((double)(TimeCur.QuadPart-TimeStart.QuadPart)/(double)TimerFreq.QuadPart);
			fDeltaTime = fTime - fOldTime;

			// Pass control to the scene player
			sp.Play();
			if(g_bFPS)
				esDrawFps(fDeltaTime);

			SwapBuffers(g_hDC);
		} 
	}

	return(msg.wParam);									// Return from the program
}

//	This function sets the pixel format for OpenGL.
bool bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = bitsPerPixel;						// Here we use our #define for the color bits
    pfd.cDepthBits = bitsPerDepth;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;										// Return a success!
}


void ChangeToFullScreen(int width, int height)
{
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	dmSettings.dmPelsWidth	= width;				// Selected Screen Width
	dmSettings.dmPelsHeight	= height;			// Selected Screen Height
	
	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	// Check if we didn't recieved a good return message From the function
	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

//	This function creates a window, but doesn't have a message loop
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASS));				// Init the size of the class
	wndclass.style = CS_HREDRAW | CS_VREDRAW;			// Regular drawing capabilities
	wndclass.lpfnWndProc = WinProc;						// Pass our function pointer as the window procedure
	wndclass.hInstance = hInstance;						// Assign our hInstance
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// General icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// An arrow for the cursor
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	// A white window
	wndclass.lpszClassName = "qopdemo";			// Assign the class name

	RegisterClass(&wndclass);							// Register the class
	
	if(bFullScreen && !dwStyle) 						// Check if we wanted full screen mode
	{													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor(FALSE);								// Hide the cursor
	}
	else if(!dwStyle)									// Assign styles to the window depending on the choice
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;							// Assign our global hInstance to the window's hInstance

	// Below, we need to adjust the window to it's true requested size.  If we say we
	// want a window that is 800 by 600, that means we want the client rectangle to
	// be that big, not the entire window.  If we go into window mode, it will cut off
	// some of the client rect and stretch the remaining which causes slow down.  We fix this below.

	RECT rWindow;
	rWindow.left	= 0;								// Set Left Value To 0
	rWindow.right	= width;							// Set Right Value To Requested Width
	rWindow.top	    = 0;								// Set Top Value To 0
	rWindow.bottom	= height;							// Set Bottom Value To Requested Height

	AdjustWindowRect( &rWindow, dwStyle, false);		// Adjust Window To True Requested Size

														// Create the window
	hWnd = CreateWindow("qopdemo", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;								// If we could get a handle, return NULL

//	ShowWindow(hWnd, SW_SHOWNORMAL);					// Show the window
	ShowWindow(hWnd, SW_HIDE);					// Show the window
	UpdateWindow(hWnd);									// Draw the window

	SetFocus(hWnd);										// Sets Keyboard Focus To The Window	

	return hWnd;
}

//	This function resizes the viewport for OpenGL.
void SizeOpenGLScreen(int width, int height)			// Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero error
	{
		height=1;										// Make the Height Equal One
	}

	glViewport(0,0,width,height);						// Make our viewport the whole window
}

//	This function handles all the initialization for OpenGL.
void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!bSetupPixelFormat(g_hDC))						// This sets our pixel format/information
        PostQuitMessage (0);							// If there's an error, quit

    g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport

	GLenum err = glewInit();
	
	if (err != 0)
	{
		// problem: glewInit failed, something is seriously wrong 
		FatalError("Error: %s\n", (char *)glewGetErrorString(err));
	}

	glClearColor(0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)((float)window_width/(float)window_height);
	gluPerspective( 70, ratio, 0.5f, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


//	This function initializes the window.
void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect

	// load syncpoint file
	sp.Load("./data/syncpoints.ess");
	QueryPerformanceFrequency(&TimerFreq);
	QueryPerformanceCounter(&TimeStart);
}


void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory
		
	if(g_bFullScreen)									// If we were in full screen
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	UnregisterClass("qopdemo", g_hInstance);		// Free the window class

	PostQuitMessage (0);								// Post a QUIT message to the window
}

//	This function handles the window messages.
LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Don't worry about this if we are in full screen (otherwise may cause problems)
		{												// LoWord=Width, HiWord=Height
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));
			GetClientRect(hWnd, &g_rRect);				// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)	DeInit();				// Quit if we pressed ESCAPE
		SetEvent(g_exit_event);
		break;
 
    case WM_DESTROY:									// If the window is destroyed
        DeInit();										// Release memory and restore settings
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}

// our sync callback, called for every sync point

// this is awefull but this is the only way i can pass a callback to fmod and
// get access to ScenePlayer members from it at the same time.
signed char F_CALLBACKAPI synccallback(FSOUND_STREAM *stream,void *buff,int len,void *userdata)
{
	sp.take++;

	if(!_strcmpi((char *)buff, "finish")) {
		SendMessage(g_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI th_demothread(LPVOID lpParameter)
{
	HWND hWnd;
//	UnpackAll();
	SendDlgItemMessage(g_hWndOptions, IDC_PROGRESS1, PBM_SETPOS, (int) 10, 0);

	// Create our window with our function we create that passes in the:
	// Name, width, height, any flags for the window, if we want fullscreen of not, and the hInstance
	hWnd = CreateMyWindow("qop - continuosly eluding subject", window_width, window_height, 0, g_bFullScreen, g_hInstance);

	// If we never got a valid window handle, quit the program
	if(hWnd == NULL) {
		SetEvent(g_exit_event);
		return TRUE;
	}

	// INIT OpenGL
	Init(hWnd);													
	// Run our message loop and after it's done, return the result
	MainLoop();	
	SetEvent(g_exit_event);
	EndDialog(g_hWndOptions, 1);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwndDlg,	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	g_hWndOptions = hwndDlg;
	HWND startmenu;

	switch(uMsg)
	{
		case WM_INITDIALOG:
						{
							SetWindowPos(hwndDlg, HWND_TOP, GetSystemMetrics(SM_CXSCREEN)/3, GetSystemMetrics(SM_CYSCREEN)/4,0,0,SWP_SHOWWINDOW | SWP_NOSIZE);
							SetWindowText(hwndDlg, "qop - continuosly eluding subject");
							CheckDlgButton(hwndDlg, IDC_CHECK_FS, BST_CHECKED);
							CheckDlgButton(hwndDlg, IDC_CHECK_HQ, BST_CHECKED);
							CheckDlgButton(hwndDlg, IDC_CHECK_SOUND, BST_CHECKED);

							return true;
						}	
						break;
				
		case WM_COMMAND:
						{
							switch(wParam)
							{
								//
								case IDOK:
									IsDlgButtonChecked(hwndDlg, IDC_CHECK_FPS) ?  g_bFPS = true : g_bFPS = false;
									IsDlgButtonChecked(hwndDlg, IDC_CHECK_HQ) ? g_bHighQuality = true : g_bHighQuality = false;
									IsDlgButtonChecked(hwndDlg, IDC_CHECK_FS)? g_bFullScreen = true : g_bFullScreen = false;
									
									EnableWindow(GetDlgItem(hwndDlg,IDOK),FALSE);
									CreateThread(NULL, NULL, &th_demothread, (LPVOID )0 , 0, 0);
									break;

								case IDCANCEL:
									EndDialog(hwndDlg, 1);
									SetEvent(g_exit_event);
									break;

								case 0xDEAD:
									ShowWindow(hwndDlg, SW_HIDE);
									ShowWindow(g_hWnd, SW_SHOWNORMAL);
									if(g_bFullScreen) {
										ChangeToFullScreen(window_width, window_height);							// Go to full screen
									}
									startmenu = FindWindow("Shell_TrayWnd", NULL);
									ShowWindow(startmenu, SW_HIDE);

									break;

								default:
									return false;
									break;
							}
							return true;
						}
						break;
		case WM_CLOSE:
						{
							EndDialog(hwndDlg, 1);
							SetEvent(g_exit_event);
							return true;
						}
						break;
		default:
						{
							return false;
						}
						break;
	}
	return false;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	g_hInstance = hInstance;

	g_exit_event = CreateEvent(NULL, TRUE, FALSE, "event");
	g_bFullScreen = FALSE;

	InitCommonControls();
	int res = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), NULL, DlgProc, 0);
	WaitForSingleObject(g_exit_event, 0);

	HWND startmenu = FindWindow("Shell_TrayWnd", NULL);
	ShowWindow(startmenu, SW_SHOW);

	music.Stop();

	return 0;
}
