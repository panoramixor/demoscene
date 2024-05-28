// flashparty 2003 invitation
// - panoramix/qop
// - akira/kikencorp

#include "stdafx.h"
#include "resource.h"

HDC hDC;
HGLRC hRC;

char szAppName[] = "fp2003 invtro";
HANDLE		  hProcess;													// Demo process handle, we use it to set it as THREAD_PRIORITY_TIME_CRITICAL

extern bool Draw();
extern bool init();

GLsizei 	  glWidth;													  // Current Window width
GLsizei       glHeight; 												 // Current window height

LARGE_INTEGER	TimerFreq;													   // Timer Frequency
LARGE_INTEGER	TimeStart;														 // Time of start
LARGE_INTEGER	TimeCur;															  // Current time
float			fTime;										 // Time passed since first frame
float			fDeltaTime;									  // Time passed since last frame
float			fOldTime;

extern LARGE_INTEGER TimerFreq;
extern LARGE_INTEGER TimeStart;

bool		  bReady = false;										 // Ready to start rendering?
bool          bFinished = false;										  // Reached end of demo?
bool          init_flag = false;

// Global functions

void Init_Music();															   // Start the music
void Stop_Music();																// Stop the music
void ShutDown();

bool window_isFullScreen=true;
int bitsPerPixel=32;

int window_width=1024;
int window_height=768;


// Set up pixel format.

int nPixelFormat;

static PIXELFORMATDESCRIPTOR pfd = {					
	sizeof(PIXELFORMATDESCRIPTOR),										// Size of this structure
	1,																 // Version of this structure
	PFD_DRAW_TO_WINDOW |										// Draw to Window (not to bitmap)
	PFD_SUPPORT_OPENGL |										// Support OpenGL calls in window
	PFD_DOUBLEBUFFER,													  // Double buffered mode
	PFD_TYPE_RGBA,															   // RGBA Color mode
	32,																// Want the display bit depth
	0,0,0,0,0,0,													   // Not used to select mode
	0,0,															   // Not used to select mode
	0,0,0,0,0,														   // Not used to select mode
	24,																      // Size of depth buffer
	0,																   // Not used to select mode
	0,																   // Not used to select mode
	PFD_MAIN_PLANE,															// Draw in main plane
	0,																   // Not used to select mode
	0,0,0
};									

//-----------------------------------------------------------------------------------------------
// SkeletonProc(): Main loop.
//-----------------------------------------------------------------------------------------------

LRESULT CALLBACK SkeletonProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Keep drawing stuff around.

    switch( uMsg )
    {
		case WM_SYSCOMMAND:												// Intercept System Commands
		{
			switch (wParam)												// Check System Calls
			{
				case SC_SCREENSAVE:										// Screensaver Trying To Start?
				case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
				return 0;												// Prevent From Happening
			}
			break;														// Exit
		}
		return 0;					

	//-------------------------------------------------------------------------------------------
	// Create device context and initialize it.
	//-------------------------------------------------------------------------------------------

    case WM_CREATE:

		// Remember our mSloth drawing context.
		hDC = GetDC( hWnd );

		// Choose a pixel format that best matches that described in pfd
		nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	
		// Set the pixel format for the device context
		SetPixelFormat(hDC, nPixelFormat, &pfd);
		
		// Yeppers, make something that OpenGL understands.
		hRC = wglCreateContext( hDC );
		wglMakeCurrent( hDC, hRC );

		init();

		break;

	//-------------------------------------------------------------------------------------------
	// Set viewport size and that's final.
	//-------------------------------------------------------------------------------------------

	case WM_SIZE:
		glWidth = window_width;
		glHeight = window_height;
		glViewport(0, 0, glWidth, glHeight);														   // Set viewport dimensions
		break;

	//-------------------------------------------------------------------------------------------
	// Key pressed, Mouse button pressed, or just been ordered to get lost. Shutdown.
	//-------------------------------------------------------------------------------------------
	
	case WM_CHAR:
	case WM_LBUTTONDOWN:
	case WM_DESTROY:

		// Stop the music.
		ShutDown();
		break;
	
	//-------------------------------------------------------------------------------------------
	// Draw a frame.
	//-------------------------------------------------------------------------------------------

	case WM_PAINT:
		if( bReady )
		{
			// Get the current time, and update the time controller.
			QueryPerformanceCounter(&TimeCur);
			fOldTime = fTime;
			fTime = (float)((double)(TimeCur.QuadPart-TimeStart.QuadPart)/(double)TimerFreq.QuadPart);
			fDeltaTime = fTime - fOldTime;

			// todo:
			Draw();

			// Make it all visible at once!
			SwapBuffers( hDC );
			ValidateRect( hWnd, NULL );

			switch (bFinished)
				case true:
					ShutDown();
					break;	
				case false:
					break;
		}
		break;

	//-------------------------------------------------------------------------------------------

	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------
// WinMain(): Application entry point. Main initialization and shutdown.
//-----------------------------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	HWND	 hWnd;
//	WNDCLASS wc;
	WNDCLASSEX wc;// this will hold the class we create


	// Set our own process as a HIGH_PRIORITY
	hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	SetThreadPriority(hProcess, THREAD_PRIORITY_HIGHEST);		// I know this is not really the highest, but I crashed win2k 3 times with THREAD_PRIORITY_TIME_CRITICAL

	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style	       = CS_DBLCLKS | CS_OWNDC |
						 CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)SkeletonProc;; // pointer to our Callback function defined above
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon	       = LoadIcon(NULL, IDI_APPLICATION); //load a generic Window icon
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);     //load a generic mouse pointer
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH); // to clear the screen black
	wc.lpszMenuName	= NULL;  // no menu 
	wc.lpszClassName	= szAppName; // give it a class name
	wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);

	if( !RegisterClassEx( &wc ) ) {
		MessageBox( NULL, "No se logro registrar la clase de la ventana!",          // Message Box
				    "Fatal Blow", MB_OK | MB_ICONERROR );
		return FALSE;						  // Failed to create window, so just quit right away
	}

	// Change display mode

	DEVMODE device_mode;

	memset(&device_mode, 0, sizeof(DEVMODE));

	device_mode.dmSize       = sizeof(DEVMODE);
	device_mode.dmBitsPerPel = bitsPerPixel;
	device_mode.dmPelsWidth  = window_width;
	device_mode.dmPelsHeight = window_height;
	device_mode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN | CDS_SET_PRIMARY);

	DWORD dwStyle;
	DWORD dwExStyle;
	RECT  windowRect;

	dwExStyle = WS_EX_APPWINDOW;
	dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	ShowCursor(false);

	windowRect.left=(long)0;			// Set Left Value To 0
	windowRect.right=(long)window_width;			// Set Right Value To Requested Width
	windowRect.top=(long)0;				// Set Top Value To 0
	windowRect.bottom=(long)window_height;			// Set Bottom Value To Requested Height

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE,	dwExStyle);

	// create main window
	hWnd = CreateWindowEx(NULL, szAppName, szAppName, dwStyle, 0,0, windowRect.right-windowRect.left, windowRect.bottom-windowRect.top, NULL, NULL,	hInstance, NULL);

	if( hWnd == NULL ) {
		MessageBox( NULL, "No se pudo crear la ventana!.", "Error fatal", MB_OK | MB_ICONERROR );
		return FALSE;						  // Failed to create window, so just quit right away
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetForegroundWindow(hWnd);

	// Reset the timer
	QueryPerformanceFrequency(&TimerFreq);
	QueryPerformanceCounter(&TimeStart);

	//-------------------------------------------------------------------------------------------
	// Prepare the demo for rendering. All demo specific initialization goes here.
	//-------------------------------------------------------------------------------------------

	// Enable clearing of the depth buffer
	glClearDepth(1.0f);

	// Reset perspective.
	glViewport(0, 0, (GLsizei) glWidth, (GLsizei) glHeight); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set perspective 
	gluPerspective(45, (GLfloat) glWidth / (GLfloat) glHeight, 0.001f, 100.0);
	glMatrixMode(GL_MODELVIEW);
	
	// Reset the view matrix
	glLoadIdentity();	
	
	// Select "nicest" perspective correction.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Start the music.
	Init_Music();

	// We're now ready to render ----------------------------------------------------------------

	bReady = true;

	BOOL bGotMsg;
	MSG  msg;

	PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
	while( (msg.message != WM_QUIT) && (!bFinished))
    {
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.

		bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

		// If there is a message, translate and dispatch it.

		if( bGotMsg ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
        } else 
			InvalidateRect( hWnd, NULL, FALSE );
		
    }

	return msg.wParam;
}

void ShutDown(){

	// Not ready to render anymore.
	bReady = false;

	// Destroy the display lists.
	glDeleteLists(0, 100);

	// stop music
	Stop_Music();

	// Remove DC and switch display mode back to what it was..
	ChangeDisplaySettings(0, 0);
	wglMakeCurrent( hDC, NULL );
	wglDeleteContext( hRC );

	// Send quit message
	PostQuitMessage( WM_QUIT );
}


