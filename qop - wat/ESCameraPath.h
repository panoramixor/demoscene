// camera animation path

#ifndef __CCAMPATH_H__
#define __CCAMPATH_H__

class CCamPath
{
public:
	CCamPath();
	~CCamPath();
	KeyFrame kfArray[MAX_SCENEKEYFRAMES];
	void Load(char *filename);
	void AppendKeyFrame(KeyFrame kd);
	int kfindex;
};

#endif /*__CCAMPATH_H__*/