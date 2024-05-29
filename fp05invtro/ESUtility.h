#ifndef __ES_UTILITY_H_
#define __ES_UTILITY_H_

float esGetFps(float fTime);
void LogLine(char *formatstring, ...);
void FatalError(char *formatstring, char *datastring);
void FatalErrorFilter(LPVOID pepe);
bool checkfilename(char *);

#define dist(a, b, c, d) sqrt(float((a - c) * (a - c) + (b - d) * (b - d)))

typedef struct tagSyncPoint {
	char name[64];
	unsigned int offset;
} SyncPoint;

CVector3 SplineInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t);
CVector3 CatmullRomInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t);
bool CheckKeyState(int vKey);
float Difference(float currentvalue, float lastvalue);
//float Difference(float currentvalue, float lastvalue);
//KeyFrame KeyFrameInterpolate(KeyFrame source, KeyFrame destination, float timepassed);
float LinearInterpolate(float source, float destination, float timepassed);
float LinearEaseInterpolate(float source, float destination, float timepassed);
void setOrthographicProjection();
void resetPerspectiveProjection();
bool checkfilename(char *);
void esDrawFps(float fTime);
bool IsPowerOfTwo(int n);
#endif // __ES_UTILITY_H_