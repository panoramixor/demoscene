#include "stdafx.h"

CDevilTexture::CDevilTexture()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
	isenvmap = false;
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
	glBindTexture(GL_TEXTURE_2D, id);
	if(isenvmap) {
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	} else {
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}

}

CDevilTexture *CDevilTexture::CreateMask(int r, int g, int b)
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
	id = ilutGLBindMipmaps();

	ilSetData(mask->GetData());
	mask->id = ilutGLBindMipmaps();

	return mask;
}

void CDevilTexture::LoadEnvMap(char *filename)
{
	LogLine("ESDevILTexture: Loading->%s\r\n", filename);
	ilGenImages(1, &images);
	ilBindImage(images);
	strcpy(sz_filename, filename);
	if(!ilLoadImage(filename)) {
		LogLine("ERROR: %s %s\r\n", iluErrorString(ilGetError()), filename);
	}
	isenvmap = true;
//	glGenTextures(1, &id);
//	glBindTexture(GL_TEXTURE_2D, id);
//	ilGetInteger(IL_IMAGE_TYPE); /* == IL_TGA) {
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//	iluImageParameter(ILU_FILTER, ILU_BILINEAR);
//	iluScale(1280, 1024, 32);
	iluGetImageInfo(&ILtexture);
		iluFlipImage();
//	}*/
	data = ilGetData();
	id = ilutGLBindMipmaps();

//	envmap = true;

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

}


void CDevilTexture::Draw()
{
//	Bind();
	glLoadIdentity();
	glRasterPos3f(0,0,0);
	glTranslatef(0,0,-10);
	int fr = ILtexture.Format;
	fr = GL_RGBA8;
	ilBindImage(images);
	glDrawBuffer(GL_FRONT);
	glDrawPixels(ILtexture.Width, ILtexture.Height, fr, GL_UNSIGNED_BYTE, data);
}

void CDevilTexture::Load(char *filename)
{
	LogLine("ESDevILTexture: Loading->%s\r\n", filename);
	ilGenImages(1, &images);
	ilBindImage(images);
	strcpy(sz_filename, filename);
	if(!ilLoadImage(filename)) {
		LogLine("ERROR: %s %s\r\n", iluErrorString(ilGetError()), filename);
	}
	
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluGetImageInfo(&ILtexture);
	iluFlipImage();
	data = ilGetData();
	id = ilutGLBindMipmaps();
}