// 3DSModel loader using lib3ds

#include "stdafx.h"

/* Constructor */
CModel3DS::CModel3DS()
{
	filename=0;
	camera=0;
	file=0;
	current_frame=0;
	first_frame = 0;
	last_frame = 0;
	textures = 0;
	dTime=0;
	oTime=0;
	strip_model = false;
	framerate=30;
	opacity=100;
	rendermode=GL_TRIANGLES;
	ratio = (float)((float)window_width/(float)window_height);
	setpos = CreateVector(0, 0, 0, 0);
	num_faces = 0;
	texturemappingflags = 0;
	max_nodes = 0;
}

/* Destructor */
CModel3DS::~CModel3DS()
{
	if(textures)
		delete [] textures;
//	if(nodearray)
//		delete [] nodearray;
}

void CModel3DS::SetStripping(bool value)
{
	strip_model = value;
}

void CModel3DS::SetCurrentFrame(float frame)
{
	if(frame<=last_frame)
		current_frame = frame;
	else
		current_frame = 0;
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

	Lib3dsNode *p = { 0 };
	for (p=file->nodes; p!=0; p=p->next) {
		GetTotalNodes(p);
	}

//	nodearray = new nodeElement[max_nodes];

	if (!file->cameras) {
		lib3ds_file_free(file);
		FatalError("ESModel3DS: No Camera found.\n", NULL);
		file=0;
	}

	SetActiveCamera(0);

	if(file->materials) {
		Lib3dsMaterial *mathelper = file->materials;
		int materialcount = 0;
		while(mathelper) {
			materialcount++;
			mathelper = mathelper->next;
		}
		
		mathelper = file->materials;
//		textures = new CTexture[materialcount];
		textures = new CDevilTexture[materialcount*12];
//		SetCurrentDirectory(path);
		for(int i=0;i<materialcount;i++) {

			if(strlen(mathelper->opacity_map.name)) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->opacity_map.name);
				texturemappingflags |= ES_OPACITYMAP;
				textures[i+2].isopacity = true;
				textures[i+2].isenvmap = false;
				textures[i+2].isdifuse = false;
				textures[i+2].Load(tmp_path);
//				textures[i].SetMask(true);
			} 

			char pepe = mathelper->texture1_map.name[0];
			if(pepe) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->texture1_map.name);
				textures[i+0].Load(tmp_path);
				texturemappingflags |= ES_DIFUSECOLORMAP;
				textures[i+0].isdifuse = true;
				textures[i+0].isopacity = false;
				textures[i+0].isenvmap = false;
			}

			if(strlen(mathelper->reflection_map.name)) {
				wsprintf(tmp_path, "%s\\%s", path, mathelper->reflection_map.name);
				texturemappingflags |= ES_REFLECTIONMAP;
				textures[i+1].LoadEnvMap(tmp_path);
				textures[i+1].isenvmap = true;
				textures[i+1].isdifuse = false;
				textures[i+1].isopacity = false;

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

void CModel3DS::GetTotalNodes(Lib3dsNode *node)
{
	max_nodes++;
	for(auto p=node->childs; p!=0; p=p->next) {
		GetTotalNodes(p);
	}
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
	lib3ds_file_eval(file,current_frame);
}

void CModel3DS::SetPos(float x, float y, float z)
{
	setpos.c[X] = x;
	setpos.c[Y] = y;
	setpos.c[Z] = z;
}

void CModel3DS::ApplyCameraTransform()
{
	glMultMatrixf(&cameramatrix[0][0]);
}

void CModel3DS::Draw(float sTime)
{
	Lib3dsNode *c,*t;
//	Lib3dsMatrix M;

//	glEnable(GL_TEXTURE_2D);

	if (!file) {
		return;
	}

	c=lib3ds_file_node_by_name(file, camera, LIB3DS_CAMERA_NODE);
	t=lib3ds_file_node_by_name(file, camera, LIB3DS_TARGET_NODE);
	if (!c || !t) {
		return;
	}
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( c->data.camera.fov, ratio, 0.01f, 10000.0);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glTranslatef(setpos.c[X], setpos.c[Y], setpos.c[Z]);
	glRotatef(-90, 1.0,0,0);

	{
		GLfloat a[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat c[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat p[] = {0.0f, 0.0f, 0.0f, 1.0f};
		Lib3dsLight *l;

		int li=GL_LIGHT0;
		for (l=file->lights; l; l=l->next) {
			glEnable(li);
			glEnable(GL_LIGHTING);

			l->color;

			glLightfv(li, GL_AMBIENT, l->color);
			glLightfv(li, GL_DIFFUSE, l->color);
			glLightfv(li, GL_SPECULAR, l->color);

			p[0] = l->position[0];
			p[1] = l->position[1];
			p[2] = l->position[2];
			glLightfv(li, GL_POSITION, p);

			if (l->spot_light) {
				p[0] = l->spot[0] - l->position[0];
				p[1] = l->spot[1] - l->position[1];
				p[2] = l->spot[2] - l->position[2];      
				glLightfv(li, GL_SPOT_DIRECTION, p);
			}

			++li;
		}
	}

	lib3ds_matrix_camera(cameramatrix, c->data.camera.pos, t->data.target.pos, c->data.camera.roll);
	glMultMatrixf(&cameramatrix[0][0]);
	{
		Lib3dsNode *p = { 0 };
		for (p=file->nodes; p!=0; p=p->next) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			render_node(p);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}
	}

	// this make us slave to the music clock AND sets the framerate to the 3ds frame rate
	dTime=sTime-oTime;
	oTime=sTime;
	current_frame+=(float)(dTime*(float)framerate);

	if ((int)current_frame>last_frame) {
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

	ASSERT(file);
	Lib3dsNode *p;
	for(p=node->childs; p!=0; p=p->next)
		render_node(p);

		if(node->type!=LIB3DS_OBJECT_NODE)
			return;
		if(strcmp(node->name,"$$$DUMMY")==0)
			return;

		// aca
		Lib3dsObjectData *d;

		glPushMatrix();

		d=&node->data.object;
		glMultMatrixf(&node->matrix[0][0]);
		glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);

		//

		Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, node->name);
		ASSERT(mesh);
		if (!mesh) {
			return;
		}

		Lib3dsVector *normalL=(Lib3dsVector *)malloc(3*sizeof(Lib3dsVector)*mesh->faces);
		Lib3dsMaterial *oldmat = (Lib3dsMaterial *)-1;
		Lib3dsMatrix M;
		lib3ds_matrix_copy(M, mesh->matrix);
		lib3ds_matrix_inv(M);
		glMultMatrixf(&M[0][0]);

		lib3ds_mesh_calculate_normals(mesh, normalL);

		for(unsigned int p=0; p<mesh->faces; ++p) {
			Lib3dsFace *f=&mesh->faceL[p];
			Lib3dsMaterial *mat=0;
			if (f->material[0])
				mat=lib3ds_file_material_by_name(file, f->material);

			// this really made things fast!
			if( mat != oldmat ) {
				if (mat) {
					glEnable(GL_COLOR_MATERIAL);

					float s;
					glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
					s = pow(2.0f, 10.0f*mat->shininess);

					if (s>128.0f) {
						s=128.0f;
					}
					glMaterialf(GL_FRONT, GL_SHININESS, s);
				
					unsigned int num_texturemaps=0;
					if(texturemappingflags & ES_DIFUSECOLORMAP)
						num_texturemaps++;
					if(texturemappingflags & ES_OPACITYMAP)
						num_texturemaps++;
					if(texturemappingflags & ES_REFLECTIONMAP)
						num_texturemaps++;

						if(textures[mat->user.i+0].isdifuse) {
							glActiveTextureARB(GL_TEXTURE0_ARB);
							glEnable(GL_TEXTURE_2D);
							textures[mat->user.i+0].Bind();
						} if(textures[mat->user.i+1].isenvmap) {
							glActiveTextureARB(GL_TEXTURE1_ARB);
							glEnable(GL_TEXTURE_2D);
							textures[mat->user.i+1].Bind();
						} if(textures[mat->user.i+2].isopacity) {
							glActiveTextureARB(GL_TEXTURE2_ARB);
							glEnable(GL_TEXTURE_2D);
							textures[mat->user.i+2].Bind();
						}

				} else {
					glEnable(GL_COLOR_MATERIAL);
					static const Lib3dsRgba a={0.6f, 0.6f, 0.6f, 1.0f};
					static const Lib3dsRgba d={0.8f, 0.8f, 0.8f, 1.0f};
					static const Lib3dsRgba s={1.0f, 1.0f, 1.0f, 1.0f};
					glMaterialfv(GL_FRONT, GL_AMBIENT, a);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
					glMaterialfv(GL_FRONT, GL_SPECULAR, s); 
				}
				oldmat = mat;
			} 
		}

		int numfacestimes3 = mesh->faces*3;

		CVector2 *p_texcoords = new CVector2[numfacestimes3];
		CVector3 *p_vertexarray = new CVector3[numfacestimes3];
		CVector3 *p_normal = new CVector3[numfacestimes3];

		unsigned short *p_indices = new unsigned short[numfacestimes3];

		for (unsigned int p=0; p<mesh->faces; ++p) {
			num_faces++;
			Lib3dsFace *f=&mesh->faceL[p];
			int i;

			for (i=0; i<3; ++i) {
				int ptimes3plusi = 3*p+i;
				unsigned short index = f->points[i];
				p_indices[ptimes3plusi] = index;
				p_normal[index].Set(normalL[ptimes3plusi][0], normalL[ptimes3plusi][1], normalL[ptimes3plusi][2]);
				if(mesh->texels)
					p_texcoords[index].Set(mesh->texelL[index][0], p_texcoords[index].y = mesh->texelL[index][1]);
			}
		}

		for(unsigned int i=0;i<mesh->points;i++)
			p_vertexarray[i].Set(mesh->pointL[i].pos[0], mesh->pointL[i].pos[1], mesh->pointL[i].pos[2]);

		if(texturemappingflags & ES_DIFUSECOLORMAP) {
			glClientActiveTextureARB(GL_TEXTURE0_ARB); 
			glEnable(GL_TEXTURE_2D);
			glTexCoordPointer(2, GL_FLOAT, 0, p_texcoords);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		if(texturemappingflags & ES_REFLECTIONMAP) {
			glClientActiveTextureARB(GL_TEXTURE1_ARB); 
			glEnable(GL_TEXTURE_2D);
			glTranslatef(0,0,0);
			glScalef(1, 1, 1);
			glTexCoordPointer(2, GL_FLOAT, 0, p_texcoords);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if(texturemappingflags & ES_OPACITYMAP) {
			glClientActiveTextureARB(GL_TEXTURE2_ARB); 
			glEnable(GL_TEXTURE_2D);
			glTexCoordPointer(2, GL_FLOAT, 0, p_texcoords);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glClientActiveTextureARB(GL_TEXTURE0_ARB); 
		glTexCoordPointer(2, GL_FLOAT, 0, p_texcoords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glMatrixMode(GL_PROJECTION);
		glMatrixMode(GL_MODELVIEW);

		glVertexPointer(3, GL_FLOAT, 0, p_vertexarray);
		glNormalPointer(GL_FLOAT, 0, p_normal);

		if(strip_model) {			// tarda bocha en hacer esto, pero manda menos vertices a la plaquita
			PrimitiveGroup *primgroups = NULL;
			unsigned short numgroups = 0;
			SetCacheSize(24);
			SetStitchStrips(true);
			SetMinStripSize(3);

			GLenum Type = 0;

			GenerateStrips(p_indices, numfacestimes3, &primgroups, &numgroups, false);
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
			delete [] primgroups;
		} else {
			glDrawElements(rendermode, numfacestimes3, GL_UNSIGNED_SHORT, p_indices);
		}
		if(normalL)
			free(normalL);
		if(p_texcoords)
			delete p_texcoords;
		if(p_vertexarray)
			delete [] p_vertexarray;
		if(p_normal)
			delete [] p_normal;
		if (p_indices)
			delete 	p_indices;
		
		glPopMatrix();
}
