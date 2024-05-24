// Include engine headers:
#include <WickedEngine.h>
#include "demoscene.h"
#include <Windows.h>

using namespace std;
using namespace wiScene;
using namespace wiECS;

// Global Variables:
HINSTANCE hInst;									// current instance
WCHAR szTitle[] = L"qop - flytrap";			// The title bar text
WCHAR szWindowClass[] = L"dmclass";					// the main window class name
Demo DemoClass;
HWND gHWND=0;
#ifdef _DEBUG
	#define bFullScreen FALSE // Set full screen as default
#else
	#define bFullScreen TRUE // Set full screen as default
#endif

//#define SCREEN_WIDTH 1920
//#define SCREEN_HEIGHT 1080

DWORD SCREEN_WIDTH = 1920;
DWORD SCREEN_HEIGHT = 1080;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void ChangeToFullScreen()
{
	DEVMODE dmSettings;     // Device Mode variable
	memset(&dmSettings, 0, sizeof(dmSettings));     // Makes Sure Memory's Cleared

	// Get current settings -- This function fills our the settings
	// This makes sure NT and Win98 machines change correctly
	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings))
	{
		// Display error message if we couldn't get display settings
		MessageBox(NULL, L"Could Not Enum Display Settings", L"Error", MB_OK);
		return;
	}
//	dmSettings.dmPelsWidth = SCREEN_WIDTH;       // Selected Screen Width
//	dmSettings.dmPelsHeight = SCREEN_HEIGHT;     // Selected Screen Height


	SCREEN_WIDTH = dmSettings.dmPelsWidth;
	SCREEN_HEIGHT = dmSettings.dmPelsHeight;

	// This function actually changes the screen to full screen
	// CDS_FULLSCREEN Gets Rid Of Start Bar.
	// We always want to get a result from this function to check if we failed
	int result = ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);
	// Check if we didn't receive a good return message From the function
	if (result != DISP_CHANGE_SUCCESSFUL)
	{
		// Display the error message and quit the program
		MessageBox(NULL, L"Display Mode Not Compatible", L"Error", MB_OK);
		PostQuitMessage(0);
	}
}


#ifdef _DEBUG
//int wmain(int argc, char * argv[])
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	wchar_t pass_args[4096] = { 0 };
	for (auto argnum = 0; argnum < argc; argnum++) {
		wcscat(pass_args, argv[argnum]);
		wcscat(pass_args, L" ");
	}
//	GetModuleFileName(GetModuleHandle(NULL), filename, 1024);
	wWinMain(GetModuleHandle(NULL), 0, pass_args, SW_SHOW);
}
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)

{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	wiStartupArguments::Parse(lpCmdLine); // if you wish to use command line arguments, here is a good place to parse them...

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DemoClass.Run(); // run the update - render loop (mandatory)
		}
	}
	return (int)msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = szWindowClass;

	return RegisterClassExW(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	DWORD dwStyle=0;
	if (bFullScreen)    // Check if we wanted full screen mode
	{   // Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen();    // Go to full screen
		ShowCursor(FALSE);       // Hide the cursor
	}
	else {
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT rWindow;
	rWindow.left = 0;           // Set Left Value To 0
	rWindow.right = SCREEN_WIDTH;      // Set Right Value To Requested Width
	rWindow.top = 0;            // Set Top Value To 0
	rWindow.bottom = SCREEN_HEIGHT;    // Set Bottom Value To Requested Height
	AdjustWindowRect(&rWindow, dwStyle, false);    // Adjust Window To True Requested Size

	gHWND = CreateWindowW(szWindowClass, szTitle, dwStyle, 0, 0, rWindow.right - rWindow.left, rWindow.bottom - rWindow.top, nullptr, nullptr, hInstance, nullptr);

	if (!gHWND)
	{
		return FALSE;
	}

	ShowWindow(gHWND, nCmdShow);
	UpdateWindow(gHWND);

	SetForegroundWindow(gHWND);

	DemoClass.SetWindow(gHWND); // assign window handle (mandatory)

	if (bFullScreen) {
		DemoClass.fullscreen = TRUE;
	}
	return TRUE;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}

		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE) {
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
			break;
		}

		case WM_SIZE:
		{
			if (!bFullScreen) {
				if (wiRenderer::GetDevice() != nullptr) {
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);

					wiRenderer::GetDevice()->SetResolution(width, height);
				}
				else {
					wiRenderer::GetDevice();
				}
			}
			break;
		}

		case WM_DESTROY:
		{
			if (bFullScreen) {
				ChangeDisplaySettings(NULL, 0); // If So Switch Back To The Desktop
				ShowCursor(TRUE);              // Show Mouse Pointer
			}
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
