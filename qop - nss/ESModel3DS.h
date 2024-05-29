// 3DSModel loader using lib3ds

#ifndef __CMODEL3DS_H__
#define __CMODEL3DS_H__

#pragma pack(push)
#pragma pack(1)
typedef struct _arrayElement {
	float uv[2];
	float normal[3];
	float face[3][3];
} arrayElement;

typedef struct _nodeElement {
	CVector2 *p_texcoords;
	CVector3 *p_vertexarray;
	CVector3 *p_normal;
} nodeElement;

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
	void CModel3DS::SetStripping(bool value);
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
	unsigned int max_nodes;
	static int camera_menu_id;
	
	void render_node(Lib3dsNode *node);
	void GetTotalNodes(Lib3dsNode *node);
	void render_node2(Lib3dsNode *node);
//	CTexture *textures;
	CDevilTexture *textures;
	unsigned int num_faces;
	DWORD texturemappingflags;
	VECTOR setpos;
	bool strip_model;
	nodeElement *nodearray;
};

#endif /*__MODEL3DS_H__*/