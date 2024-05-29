
#ifndef __CSCENEPLAYER_H__
#define __CSCENEPLAYER_H__

#define MAX_SYNC_POINTS 4096

typedef struct tagSceneElement {
	CScene *p_scene;
	unsigned int take_start;
	unsigned int take_end;
} SceneElement;

class CScenePlayer
{
public:
	CScenePlayer();
	~CScenePlayer();

	void Play();
	bool Load(char *filename);
	int take;
private:
	SyncPoint syncpoints[MAX_SYNC_POINTS];
	unsigned int numsyncpoints;
	unsigned int current_take;
	unsigned int sceneindex;
	bool bInitSequence;
	SceneElement scenearray[20];
};

#endif /*__CSCENEPLAYER_H__*/