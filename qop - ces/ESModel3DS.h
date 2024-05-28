// 3DS scene player class

#ifndef __CMODEL3DS_H__
#define __CMODEL3DS_H__

#pragma pack(push)
#pragma pack(1)
typedef struct _arrayElement {
	float uv[2];
	float normal[3];
	float face[3][3];
} arrayElement;
#pragma pack(pop)

class CModel3DS
{
public:
	CModel3DS();
	~CModel3DS();
	bool Load(char *filename);
	void Draw();
	void SetActiveCamera(int numcamera);
	void SetAnimationRange(int first_frame, int last_frame);
	void SetFrameRate(float rate);
	int GetAnimationFrames();
	void Draw(float sTime);
	void SetCurrentFrame(float frame);
	void ApplyCameraTransform();
	void SetRenderMode(int mode);
	void SetPos(float x, float y, float z);
private:
	Lib3dsMatrix cameramatrix;
	char* filename;
	char* camera;
	Lib3dsFile *file;
	float current_frame;
	float ratio;
	float oTime;
	float dTime;
	float framerate;
	float opacity;
	int first_frame;
	int last_frame;
	int rendermode;
	static int camera_menu_id;
	void render_node(Lib3dsNode *node);
	void render_node2(Lib3dsNode *node);
	CTexture *textures;
	unsigned int num_faces;
//	VECTOR setpos;
	CVector3 setpos;
	CVector2 *p_texcoords;
	CVector3 *p_vertexarray;
	CVector3 *p_normal;

};

#endif /*__MODEL3DS_H__*/