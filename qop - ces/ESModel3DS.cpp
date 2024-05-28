// 3DS scene player class

#include "stdafx.h"

//extern float sTime;

/* Constructor */
CModel3DS::CModel3DS()
{
	filename=0;
	camera=0;
	file=0;
	current_frame=0;
	first_frame = 0;
	last_frame = 0;
	dTime=0;
	oTime=0;
	framerate=30;
	opacity=100;
	rendermode=GL_TRIANGLES;
	ratio = (float)((float)window_width/(float)window_height);
	//setpos = CreateVector(0, 0, 0, 0);
	setpos.Set(0, 0, 0);

	num_faces = 0;
}

/* Destructor */
CModel3DS::~CModel3DS()
{
	delete [] textures;
}

void CModel3DS::SetCurrentFrame(float frame)
{
	current_frame = frame;
	lib3ds_file_eval(file,frame);
}

void CModel3DS::SetFrameRate(float rate)
{
	framerate=rate;
}

void CModel3DS::SetRenderMode(int mode)
{
	rendermode=mode;
}

bool CModel3DS::Load(char *filename)
{
	char tmp_path[1024];
	// get current directory (the nasty way)
	char path[MAX_PATH];
	char *lpfilename;
	memset(&path, 0, sizeof(path));
	GetFullPathName(filename, MAX_PATH, path, &lpfilename);
	lpfilename[-1]=0x0;

	char parentpath[MAX_PATH];
	memset(&parentpath, 0, sizeof(parentpath));
	GetCurrentDirectory(MAX_PATH, parentpath);

	LogLine("ESModel3DS->Load: %s\n", filename);

	file=lib3ds_file_load(filename);
	if (!file) {
		FatalError("ESModel3DS: Error loading %s\n", filename);
	}

	last_frame = GetAnimationFrames();

	if (!file->cameras) {
		lib3ds_file_free(file);
		FatalError("ESModel3DS: No Camera found.\n", NULL);
		file=0;
	}

	if(file->materials) {
		Lib3dsMaterial *mathelper = file->materials;
		int materialcount = 0;
		while(mathelper) {
			materialcount++;
			mathelper = mathelper->next;
		}
		
		mathelper = file->materials;
		textures = new CTexture[materialcount];
//		SetCurrentDirectory(path);
		for(int i=0;i<materialcount;i++) {

			if(strlen(mathelper->opacity_map.name)) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->opacity_map.name);
				textures[i].Load(tmp_path);
				textures[i].SetMask(true);
			} 

			if(strlen(mathelper->texture1_map.name)) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->texture1_map.name);
				textures[i].Load(tmp_path);
			}

			if(strlen(mathelper->reflection_map.name)) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->reflection_map.name);
				textures[i].LoadEnvMap(tmp_path);
			}

			mathelper->user.i = i;
			mathelper = mathelper->next;
		}
//		SetCurrentDirectory(parentpath);
	}

	if (!camera) {
		camera=file->cameras->name;
	}

	lib3ds_file_eval(file,current_frame);
	return(false);
}

void CModel3DS::SetActiveCamera(int numcamera)
{
	Lib3dsCamera *c = file->cameras;
	for(int i=0;i<numcamera;i++) {
		c = c->next;
	}
	if(c) {
		camera=c->name;
	}
//	current_frame=0;
	lib3ds_file_eval(file,current_frame);
}

void CModel3DS::SetAnimationRange(int f_frame, int l_frame)
{
	if((f_frame > file->frames) || (f_frame > l_frame)) {
		f_frame = 0;
	} else {
        first_frame = f_frame;
	}

	if((l_frame > file->frames) || (l_frame < f_frame)) {
		l_frame = file->frames;
	} else
		last_frame = l_frame;

	current_frame = (float)first_frame;
}

void CModel3DS::SetPos(float x, float y, float z)
{
	setpos.Set(x, y, z);
}

void CModel3DS::ApplyCameraTransform()
{
	glMultMatrixf(&cameramatrix[0][0]);
}

void CModel3DS::Draw(float sTime)
{
	Lib3dsNode *c,*t;
//	Lib3dsMatrix M;

	glEnable(GL_TEXTURE_2D);

	if (!file) {
		return;
	}

	c=lib3ds_file_node_by_name(file, camera, LIB3DS_CAMERA_NODE);
	t=lib3ds_file_node_by_name(file, camera, LIB3DS_TARGET_NODE);
	if (!c || !t) {
		return;
	}
//	glLoadIdentity();
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( c->data.camera.fov, ratio, 0.5f, 2000.0);
//	gluPerspective( sTime, ratio, 0.5f, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	*/
	glLoadIdentity();
	glTranslatef(setpos.x, setpos.y, setpos.z);
	glRotatef(-90, 1.0,0,0);
//	glRotatef(sTime*30, 0, 1, 0);
	{
		GLfloat a[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat c[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat p[] = {0.0f, 0.0f, 0.0f, 1.0f};
		Lib3dsLight *l;

		int li=GL_LIGHT0;
		for (l=file->lights; l; l=l->next) {
			glEnable(li);
			glEnable(GL_LIGHTING);

			glLightfv(li, GL_AMBIENT, a);
			glLightfv(li, GL_DIFFUSE, c);
			glLightfv(li, GL_SPECULAR, c);

			p[0] = l->position[0];
			p[1] = l->position[1];
			p[2] = l->position[2];
			glLightfv(li, GL_POSITION, p);

			if (!l->spot_light) {
				continue;
			}

			p[0] = l->spot[0] - l->position[0];
			p[1] = l->spot[1] - l->position[1];
			p[2] = l->spot[2] - l->position[2];      
			glLightfv(li, GL_SPOT_DIRECTION, p);
			++li;
		}
	}

	lib3ds_matrix_camera(cameramatrix, c->data.camera.pos, t->data.target.pos, c->data.camera.roll);
	glMultMatrixf(&cameramatrix[0][0]);
	{
		Lib3dsNode *p;
		for (p=file->nodes; p!=0; p=p->next) {
			render_node(p);
		}
	}

	// this make us slave to the music clock AND sets the framerate to the 3ds frame rate
	dTime=sTime-oTime;
	oTime=sTime;
	current_frame+=(float)(dTime*(float)framerate);

	if (current_frame>last_frame) {
		current_frame=(float)first_frame;
	}

	lib3ds_file_eval(file, current_frame);
}

int CModel3DS::GetAnimationFrames()
{
	return(file->frames);
}

void CModel3DS::render_node(Lib3dsNode *node)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	ASSERT(file);
	{
		Lib3dsNode *p;
		for (p=node->childs; p!=0; p=p->next) {
			render_node(p);
		}
	}
	if (node->type==LIB3DS_OBJECT_NODE) {
		if (strcmp(node->name,"$$$DUMMY")==0) {
			return;
		}

		if (!node->user.d) {
			Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, node->name);
			ASSERT(mesh);
			if (!mesh) {
				return;
			}

			node->user.d=glGenLists(1);
			glNewList(node->user.d, GL_COMPILE);
			{
				unsigned p;
				Lib3dsVector *normalL=(Lib3dsVector *)malloc(3*sizeof(Lib3dsVector)*mesh->faces);
				Lib3dsMaterial *oldmat = (Lib3dsMaterial *)-1;
				{
					Lib3dsMatrix M;
					lib3ds_matrix_copy(M, mesh->matrix);
					lib3ds_matrix_inv(M);
					glMultMatrixf(&M[0][0]);
				}
				lib3ds_mesh_calculate_normals(mesh, normalL);

				for (p=0; p<mesh->faces; ++p) {
					Lib3dsFace *f=&mesh->faceL[p];
					Lib3dsMaterial *mat=0;
					if (f->material[0]) {
						mat=lib3ds_file_material_by_name(file, f->material);
					}

					// this really made thing fast!
					if( mat != oldmat ) {
						if (mat) {
							float s;
							glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
							glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
							s = (float)pow(2.0f, 10.0f*mat->shininess);
							if (s>128.0f) {
								s=128.0f;
							}
							glMaterialf(GL_FRONT, GL_SHININESS, s);
							textures[mat->user.i].Use();
						} else {
							static const Lib3dsRgba a={0.2f, 0.2f, 0.2f, 1.0f};
							static const Lib3dsRgba d={0.8f, 0.8f, 0.8f, 1.0f};
							static const Lib3dsRgba s={0.0f, 0.0f, 0.0f, 1.0f};
							glMaterialfv(GL_FRONT, GL_AMBIENT, a);
							glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
							glMaterialfv(GL_FRONT, GL_SPECULAR, s);
						}
						oldmat = mat;
					} 
				}

				CVector2 *p_texcoords = new CVector2[mesh->faces*3];
				CVector3 *p_vertexarray = new CVector3[mesh->faces*3];
				CVector3 *p_normal = new CVector3[mesh->faces*3];

				unsigned short *p_indices = new unsigned short[mesh->faces*3];

				for(unsigned int i=0;i<mesh->points;i++) {
					p_vertexarray[i].Set(mesh->pointL[i].pos[0], mesh->pointL[i].pos[1], mesh->pointL[i].pos[2]);
				}

				for (p=0; p<mesh->faces; ++p) {
					num_faces++;
					Lib3dsFace *f=&mesh->faceL[p];
					int i;

					for (i=0; i<3; ++i) {
						unsigned short index = f->points[i];
						p_indices[3*p+i] = index;
						p_normal[index].Set(normalL[3*p+i][0], normalL[3*p+i][1], normalL[3*p+i][2]);
						if(mesh->texels) {
							p_texcoords[index].x = mesh->texelL[index][0];
							p_texcoords[index].y = mesh->texelL[index][1];
						}
					}
				}

				glTexCoordPointer(2, GL_FLOAT, 0, p_texcoords);
				glVertexPointer(3, GL_FLOAT, 0, p_vertexarray);
				glNormalPointer(GL_FLOAT, 0, p_normal);

				PrimitiveGroup *primgroups = NULL;
				unsigned short numgroups = 0;
				SetCacheSize(24);
				SetStitchStrips(true);
				SetMinStripSize(2);

				GLenum Type = 0;

				GenerateStrips(p_indices, mesh->faces*3, &primgroups, &numgroups, false);
				for (unsigned int i = 0; i < numgroups; ++i) {
					switch (primgroups[i].type) {
						case PT_LIST:
							Type = GL_TRIANGLES;
							break;
						case PT_STRIP:
							Type = GL_TRIANGLE_STRIP;
							break;
						case PT_FAN:
							Type = GL_TRIANGLE_FAN;
							break;
					}
					
					glDrawElements(Type, primgroups[i].numIndices, GL_UNSIGNED_SHORT, primgroups[i].indices);
				}

//				glDrawElements(rendermode, mesh->faces*3, GL_UNSIGNED_SHORT, p_indices);

				free(normalL);
				delete [] p_texcoords;
				delete [] p_vertexarray;
				delete [] p_normal;
				delete [] primgroups;
			}
			glEndList();

		} else {
				Lib3dsObjectData *d;

				glPushMatrix();

				d=&node->data.object;
				glMultMatrixf(&node->matrix[0][0]);
				glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);

				glCallList(node->user.d);

				glPopMatrix();
		}
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}
