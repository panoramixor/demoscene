// Utility functions

#include "stdafx.h"

extern HWND hEditDlg;

int filecount=0;
char filename[255];
extern float fTime;
static int noise_index = 0;

bool IsPowerOfTwo(int n)
{
  return ((n&(n-1))==0);
}


float esGetFps(float fDeltaTime)
{
 
	static float secondTime = 0.0f;
	static int frameCounter = 0;

	frameCounter++;
	secondTime += fDeltaTime;

	if (secondTime >= 1.f)
	{
		// a second has passed
 		frameCounter = 0;
		secondTime = 0.0f;
	}

	return (float)frameCounter/secondTime;
}




void setOrthographicProjection() {

		glPushMatrix();
		// switch to projection mode
		glMatrixMode(GL_PROJECTION);
		// reset matrix
		glLoadIdentity();
		// set a 2D orthographic projection
		gluOrtho2D(0, window_width, 0, window_height);
		// invert the y axis, down is positive
		glScalef(1, -1, 1);
		// upper left corner
		glTranslatef(0, -window_height, 0);
		glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void set_screen_gamma(float value)
{	
	BYTE ramp[256*3];
	float exponent=1.0f/value;
	for (unsigned int i=0;i<256;++i)	{
		float linear=float(i)*1.0f/255u;
		float corrected=(float)pow(linear,exponent);
		BYTE entry=BYTE(corrected*65535);
		ramp[i]=ramp[i+256]=ramp[i+512]=entry;	
	}        
	
	HDC screen=GetDC(NULL);
	SetDeviceGammaRamp(screen,ramp);
	ReleaseDC(NULL,screen);
}

void FatalErrorFilter(LPVOID pepe)
{
	FatalError("Exception: %d", (char *)pepe);
}


void FatalError(char *formatstring, char *datastring) {
	char buf[MAX_PATH+80];
	sprintf(buf, formatstring, datastring);
//	MessageBox(NULL, buf, "FATAL ERROR!", MB_OK);
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	LogLine(formatstring, datastring);
	LogLine("GetLastError: %s\n", (char *)lpMsgBuf);
	ExitProcess(NULL);
	return;
}

void LogLine(char *formatstring, ...)
{
	va_list args;
	va_start(args, formatstring);

	char msg[1024];

	memset(msg, 0, sizeof(msg));
	FILE *logfile = fopen("demo.log", "a+");
	_vsnprintf(msg, sizeof(msg), formatstring, args);
	fprintf(logfile, msg);
	fclose(logfile);
}



float Difference(float source, float destination)
{
	if(destination == source)
		return 0;
	if(destination < source)
	{
		return -(source-destination);
	}
	else
	{
		return (destination-source);
	}
	return (destination-source);
}

float LinearInterpolate(float source, float destination, float timepassed)
{
	float newvalue = source + (timepassed*(destination - source));
	return newvalue;
}

void DisableAllTextures()
{
	for(GLint texture_unit=GL_TEXTURE0_ARB;texture_unit<=GL_TEXTURE31_ARB;texture_unit++) {
		glActiveTextureARB(texture_unit);
		int a = GL_INVALID_ENUM;
		glDisable(GL_TEXTURE_2D);
	}
}

float LinearEaseInterpolate(float source, float destination, float timepassed)
{
	float t1;
	if(timepassed >= 0.5f) {
		t1 = 1 - (2 * ((1-timepassed) * (1-timepassed)));
	} else {
		t1 = 2 * (timepassed * timepassed);
	}
	float newvalue = source + (t1*(destination - source));
	return newvalue;
}

//	This function returns an XYZ point along the curve, depending on t (0 to 1)
CVector3 SplineInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t)
{
	float var1, var2, var3;
    CVector3 vPoint;
	 vPoint.Set(0.0f, 0.0f, 0.0f);

	// Store the (1 - t) in a variable because it is used frequently
    var1 = 1 - t;
	
	// Store the (1 - t)^3 into a variable to cut down computation and create clean code
    var2 = var1 * var1 * var1;

	// Store the t^3 in a variable to cut down computation and create clean code
    var3 = t * t * t;

    vPoint.x = var2*p1.x + 3*t*var1*var1*p2.x + 3*t*t*var1*p3.x + var3*p4.x;
    vPoint.y = var2*p1.y + 3*t*var1*var1*p2.y + 3*t*t*var1*p3.y + var3*p4.y;
    vPoint.z = var2*p1.z + 3*t*var1*var1*p2.z + 3*t*t*var1*p3.z + var3*p4.z;

	// Now we should have the point on the curve, so let's return it.
    return(vPoint);				
}

CVector3 CatmullRomInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t)
{
	CVector3 vPoint;
	vPoint.Set(0.0f, 0.0f, 0.0f);
	vPoint.x = 0.5f *((2 * p2.x) + (-p1.x + p3.x) * t + (2*p1.x - 5*p2.x + 4*p3.x - p4.x) * (t*t) + (-p1.x + 3*p2.x - 3*p3.x + p4.x) * (t*t*t));
	vPoint.y = 0.5f *((2 * p2.y) + (-p1.y + p3.y) * t + (2*p1.y - 5*p2.y + 4*p3.y - p4.y) * (t*t) + (-p1.y + 3*p2.y - 3*p3.y + p4.y) * (t*t*t));
	vPoint.z = 0.5f *((2 * p2.z) + (-p1.z + p3.z) * t + (2*p1.z - 5*p2.z + 4*p3.z - p4.z) * (t*t) + (-p1.z + 3*p2.z - 3*p3.z + p4.z) * (t*t*t));
	return vPoint;
}

void CaptureScreen()
{
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	glFlush();		// make sure we get the hole thing...
	glReadBuffer(GL_BACK);
	unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char)*window_width*window_height*3);
	sprintf(filename,"./capture/a%08d.bmp", filecount);

	FILE *file = fopen(filename, "wb");
	if( image!=NULL )
	{
		if( file!=NULL ) 
		{
			glReadPixels( 0, 0, window_width, window_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image );
//			glReadPixels( 0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, image );
			memset( &bf, 0, sizeof( bf ) );
			memset( &bi, 0, sizeof( bi ) );

			bf.bfType = 'MB';
			bf.bfSize = sizeof(bf)+sizeof(bi)+window_width*window_height*3;
			bf.bfOffBits = sizeof(bf)+sizeof(bi);
			bi.biSize = sizeof(bi);
			bi.biWidth = window_width;
			bi.biHeight = window_height;
			bi.biPlanes = 1;
			bi.biBitCount = 24;
			bi.biSizeImage = window_width*window_height*3;

			fwrite( &bf, sizeof(bf), 1, file );
			fwrite( &bi, sizeof(bi), 1, file );
			fwrite( image, sizeof(unsigned char), window_width*window_height*3, file );
			fflush(file);
			fclose( file );
		}
		free( image );
	}
	filecount++;
}

bool checkfilename(char *filename)
{
	HANDLE h_file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_file == INVALID_HANDLE_VALUE)
		return false;
	else {
		CloseHandle(h_file);
		return true;
	}
}
