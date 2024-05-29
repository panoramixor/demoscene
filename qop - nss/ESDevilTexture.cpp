// texture class using DevIL image library

#include "stdafx.h"

CDevilTexture::CDevilTexture()
{
	isenvmap = false;
	isdifuse = true;
	isopacity = false;
	mask = 0;
}

unsigned char *CDevilTexture::GetData()
{
	ilBindImage(images);
	return ilGetData();
}

CDevilTexture::~CDevilTexture()
{
	ilDeleteImages(1, &images);
	if(mask)
		delete mask;
}

void CDevilTexture::Bind()
{
//	ilBindImage(images);
//	iluFlipImage();
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(activetexture);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id[0]);
	if(isenvmap) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	} 
	else if(isdifuse){
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	} else if(isopacity) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	} else {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

CDevilTexture *CDevilTexture::CreateMask()
{
	mask = new CDevilTexture;
//	ilGenImages(1, &mask->images);
	mask->Load(sz_filename);
	mask->data = mask->GetData();
	ilBindImage(mask->images);
	iluGetImageInfo(&mask->ILtexture);

	ilSetData(mask->GetData());
	mask->id[0] = ilutGLBindMipmaps();

	return mask;
}

CDevilTexture *CDevilTexture::KeyColorMask(int r, int g, int b)
{
	mask = new CDevilTexture;
//	ilGenImages(1, &mask->images);
	mask->Load(sz_filename);
	mask->data = mask->GetData();
	ilBindImage(mask->images);
	iluGetImageInfo(&mask->ILtexture);

	for(unsigned int x=0;x<mask->ILtexture.Width;x++) {
		for(unsigned int y=0;y<mask->ILtexture.Height;y++) {
			if((mask->data[(y*mask->ILtexture.Width*4+x*4)] == r) && (mask->data[(y*ILtexture.Width*4+x*4)+1] == g) && (mask->data[(y*ILtexture.Width*4+x*4)+2] == b)) {
				mask->data[((y*mask->ILtexture.Width)*4+x*4)] = 255;
				mask->data[((y*mask->ILtexture.Width)*4+x*4)+1] = 255;
				mask->data[(y*mask->ILtexture.Width*4+x*4)+2] = 255;
			} else {
				mask->data[(y*mask->ILtexture.Width*4+x*4)] = 0;
				mask->data[(y*mask->ILtexture.Width*4+x*4)+1] = 0;
				mask->data[(y*mask->ILtexture.Width*4+x*4)+2] = 0;
			}
		}
	}

	ilBindImage(images);
	iluGetImageInfo(&ILtexture);
	data = GetData();

	for(unsigned int x=0;x<ILtexture.Width;x++) {
		for(unsigned int y=0;y<ILtexture.Height;y++) {
			if((data[(y*ILtexture.Width*4+x*4)] == r) && (data[(y*ILtexture.Width*4+x*4)+1] == g) && (data[(y*ILtexture.Width*4+x*4)+2] == b)) {

				data[((y*ILtexture.Width)*4+x*4)] = 0;
				data[((y*ILtexture.Width)*4+x*4)+1] = 0;
				data[((y*ILtexture.Width)*4+x*4)+2] = 0;
			}
		}
	}

	ilSetData(GetData());
	id[0] = ilutGLBindMipmaps();

	ilSetData(mask->GetData());
	mask->id[0] = ilutGLBindMipmaps();

	return mask;
}

void CDevilTexture::LoadEnvMap(char *filename)
{
	LogLine("ESDevILTexture: Loading->%s\n", filename);
	ilGenImages(1, &images);
	ilBindImage(images);
	strcpy(sz_filename, filename);
	if(!ilLoadImage(filename)) {
		LogLine("ERROR: %s %s\r\n", iluErrorString(ilGetError()), filename);
	}
	glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &activetexture);
	activetexture+=GL_TEXTURE0_ARB;
	isenvmap = true;
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluGetImageInfo(&ILtexture);
		iluFlipImage();
	data = ilGetData();
	id[0] = ilutGLBindMipmaps();

}


void CDevilTexture::Draw()
{
	glActiveTextureARB(activetexture);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(0,0,-1);
	ilBindImage(images);
	iluGetImageInfo(&ILtexture);
	data = GetData();
	id[0] = ilutGLBindMipmaps();
	glBindTexture(GL_TEXTURE_2D, id[0]);
	glDisable(GL_LIGHTING);
	float width = 1.032f;
	float height = 1.032f;
	float x=-width/2.0f;
	float y=-height/2.0f;
	float z=-4;

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(x+width, y, z);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(x, y+height, z);
	glEnd();

}

void CDevilTexture::Load(char *filename)
{
	LogLine("ESDevILTexture: Loading->%s\n", filename);
	ilGenImages(1, &images);
	ilBindImage(images);
	strcpy(sz_filename, filename);
	if(!ilLoadImage(filename)) {
		LogLine("ERROR: %s %s\r\n", iluErrorString(ilGetError()), filename);
	}
	if(strstr(filename,".png")) {
	}

	glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &activetexture);
	if(activetexture < GL_TEXTURE0_ARB)
		activetexture+=GL_TEXTURE0_ARB;
	ilGetInteger(IL_IMAGE_FORMAT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	if(strstr(filename, ".png")) {
		iluFlipImage(); 
	}
	iluImageParameter(ILU_FILTER, ILU_BILINEAR);
	iluGetImageInfo(&ILtexture);
	data = ilGetData();
	id[0] = ilutGLBindMipmaps();
}