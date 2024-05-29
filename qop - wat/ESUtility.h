// Utility functions

#ifndef __ES_UTILITY_H_
#define __ES_UTILITY_H_

	float esGetFps(float fTime);

	void crossfade(CPicture picture, CTexture mask, KeyFrame ColorFadeIn[], KeyFrame ColorFadeOut[], float starttime);
	void reset_noise();
	void noise_sequence(KeyFrame SceneKeyFrames[], int MAX_KEYFRAMER_INDEX, int looper, float scenetime);
	void set_screen_gamma(float value);
	void FatalError(char *formatstring, char *datastring);
	CVector3 SplineInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t);
	CVector3 CatmullRomInterpolate(CVector3 p1, CVector3 p2, CVector3 p3, CVector3 p4, float t);
	void SaveKeyFrame(char *filename, KeyFrame kd);
	void AddKeyFrame(KeyFrame kd);
	void AddKeyFrame(KeyFrame kd, KeyFrame kfArray[]);
	void SaveCameraPath(char *filename);
	int LoadCameraPath(char *filename, KeyFrame kfArray[]);
	int LoadCameraPath(char *filename);
	bool CheckKeyState(int vKey);
	float Difference(float currentvalue, float lastvalue);

	void ResetKeyFrameData(KeyFrame kd);

	float LinearInterpolate(float source, float destination, float timepassed);
	void CaptureScreen();
	void setOrthographicProjection();
	void resetPerspectiveProjection();
	float GetMusicVolume();

#endif // __ES_UTILITY_H_