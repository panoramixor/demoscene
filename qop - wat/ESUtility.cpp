// Utility functions

#include "stdafx.h"

CTexture ESFrameBufferTexture;
CPixelBuffer ESPixelBuffer;

int filecount;
char filename[255];
extern CTexture ESNoise;
CTexture framebuffer;
extern float fTime;
static int noise_index = 0;

float esGetFps(float fTime)
{
 
	// We like to use floating point seconds, just for kicks.
	static float	lastTime = 0.0f;
	static float	g_frameTime = 0;
	static float	lastSecTime = fTime;
	static long	frameCounter = 0;
	static char s[255];

	frameCounter++;
	g_frameTime = fTime-lastTime;
	lastTime = fTime;

	if (fTime-lastSecTime > 1.f)
	{
		// a second has passed
 		frameCounter = 0;
		lastSecTime = fTime;
	}

	return (float)frameCounter/(fTime-lastSecTime);
}

void reset_noise()
{
	noise_index = 0;
}

void noise_sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime)
{
	KeyFrame kd;
	float x, y, z;
	float height, width;
	x = 0;
	y = 0;
	z = -2;
	width = 5.34f;
	height = 4;
	x = x - width  / 2;
	y = y - height / 2;
	float r, g, b, a;
	r = g = b = a = 0;
	float myscenetime = (fTime-scenetime)*1000;
	if(noise_index < MAX_KEYFRAMER_INDEX)
	{
		kd.KeyFrameInterpolate(SceneKeyFrames[noise_index], SceneKeyFrames[noise_index+1], myscenetime - SceneKeyFrames[noise_index].keyframeTime, noise_index, NULL);
		if (kd.keyframeTime < 1)
		{
				r = -Difference(SceneKeyFrames[noise_index].rotationX, kd.rotationX);
				g = -Difference(SceneKeyFrames[noise_index].rotationY, kd.rotationY);
				b = -Difference(SceneKeyFrames[noise_index].rotationZ, kd.rotationZ);
				a = (r+g+b)/3;
		}
		else
		{
				noise_index++;//= 2;

				r = SceneKeyFrames[noise_index].rotationX;
				g = SceneKeyFrames[noise_index].rotationY;
				b = SceneKeyFrames[noise_index].rotationZ;
				a = (r+g+b)/3;
		}
	}
	else
	{
		noise_index = 0;
	}
	glLoadIdentity();
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
		ESNoise.Use();
		glColor4f(r/3, g/3, b/3, a/3);
		glBegin(GL_QUADS);		
			glTexCoord2f(1.0f, 0.0f+fTime*4); glVertex3f(x,			y,			z);
			glTexCoord2f(1.0f, 1.0f+fTime*4); glVertex3f(x,			y + height, z);
			glTexCoord2f(0.0f, 1.0f+fTime*4); glVertex3f(x + width, y + height, z); 
			glTexCoord2f(0.0f, 0.0f+fTime*4); glVertex3f(x + width, y,			z);
		glEnd();
	glPopMatrix();

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
		glTranslatef(0,  -(float)window_height, 0);
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

void FatalError(char *formatstring, char *datastring) {
	char buf[MAX_PATH+80];
	sprintf(buf, formatstring, datastring);
	LogLine(formatstring, datastring);
	ExitProcess(NULL);
	return;
}

void LogLine(char* formatstring, ...)
{
	va_list args;
	va_start(args, formatstring);

	char msg[1024] = { 0 };

	memset(msg, 0, sizeof(msg));
	FILE* logfile = fopen("demo.log", "a+");
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

