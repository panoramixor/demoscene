#include "stdafx.h"

CCamPath::CCamPath()
{
	kfindex = 0;
}

CCamPath::~CCamPath()
{

}

void CCamPath::AppendKeyFrame(KeyFrame kd)
{
	char buf[256] = { 0 };
	kfArray[kfindex] = kd;
#ifdef _DEBUG
	LogLine("Added keyframe %d: ", kfindex);
	sprintf(buf, "time: %fms position(%f, %f, %f) rotation(%f, %f, %f) mode: %i\n", kd.keyframeTime, kd.positionX, kd.positionY, kd.positionZ, kd.rotationX, kd.rotationY, kd.rotationZ, kd.kinterpolationmode);
	LogLine("%s", buf);
#endif

	kfindex++;
}

void CCamPath::Load(char *filename)
{
	kfindex = 0;
	KeyFrame kd;
	char pepe[1024]="";
	static int tmpkfindex = 0;
	FILE* loadfile;
	if((loadfile = fopen(filename, "r")) == NULL)
		FatalError("cant open %s!", filename);
	fscanf(loadfile, "%d", &tmpkfindex);

	for(int i=0; i<= tmpkfindex; i++)
	{
		fscanf(loadfile, "%f %f %f %f %f %f %f %i\n", &kd.keyframeTime, &kd.positionX, &kd.positionY, &kd.positionZ, &kd.rotationX, &kd.rotationY, &kd.rotationZ, &kd.kinterpolationmode);
		AppendKeyFrame(kd);
	}
}