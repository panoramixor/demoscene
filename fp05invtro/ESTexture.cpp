#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
//#define STBI_ONLY_BMP
#include "lib/stb_image/stb_image.h"


CTexture::CTexture()
{
	envmap = false;
	ismask = false;
	b_init = false;
}

CTexture::~CTexture()
{

}

bool CTexture::GetMask()
{
	return ismask;
}

bool CTexture::SetMask(bool value)
{
	ismask = value;
	return ismask;
}


void CTexture::Load(char *name)
{
	// make the texture name all lower case
	texturename = _strlwr(_strdup(name));

	// strip "'s
	if (strstr(texturename, "\""))
		texturename = strtok(texturename, "\"");

	// check the file extension to see what type of texture
	if(strstr(texturename, ".bmp"))	
		LoadBMP(texturename);
	if(strstr(texturename, ".tga"))	
		LoadTGA(texturename);
	if(strstr(texturename, ".jpg"))	
		LoadJPG(texturename);
	if(strstr(texturename, ".raw"))	
		LoadRAW(texturename);
}

void CTexture::LoadFromResource(char *name)
{
	// make the texture name all lower case
	texturename = _strlwr(_strdup(name));

	// check the file extension to see what type of texture
	if(strstr(texturename, ".bmp"))
		LoadBMPResource(name);
	if(strstr(texturename, ".tga"))	
		LoadTGAResource(name);
}


void CTexture::Use()
{
	glEnable(GL_TEXTURE_2D);								// Enable texture mapping
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Bind the texture as the current one
	if(envmap) {
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	} else {
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}

	if(ismask) {
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_DST_ALPHA);
	}
}

void CTexture::LoadFrameBuffer(int width, int height)
{
	HGLOBAL image;

	GLint viewport[4];							
	glGetIntegerv(GL_VIEWPORT, viewport);		// get our viewport

	if(!b_init) {
		image = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, sizeof(float)*width*height*3);
		glGenTextures(1, &texture[0]);
	}

	glReadBuffer(GL_BACK);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

//	glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	if(!b_init)
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Set the texture quality
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (viewport[2]/2)-(width/2),(viewport[3]/2)-(height/2), width, height);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	if(!b_init)
		GlobalFree(image);
	
	b_init = true;
}

void CTexture::LoadBMPKeyColor(char* name, unsigned char r, unsigned char g, unsigned char b)
{
	LogLine("ESTexture->LoadBMPKeyColor: %s\n", name);
	//* AUX_RGBImageRec *pImage_RGB = auxDIBImageLoad(name);
	int width, height, numComponents;
	unsigned char* pImage_RGB = stbi_load(name, &width, &height, &numComponents, 3);


	unsigned char* pImage_RGBA = NULL;

	if (pImage_RGB != NULL)
	{
		int imageSize_RGB = width * height * 3;
		int imageSize_RGBA = width * height * 4;

		// allocate buffer for a RGBA image
		pImage_RGBA = new unsigned char[imageSize_RGBA];

		//
		// Loop through the original RGB image buffer and copy it over to the 
		// new RGBA image buffer setting each pixel that matches the key color
		// transparent.
		//

		int i, j;

		for (i = 0, j = 0; i < imageSize_RGB; i += 3, j += 4)
		{
			// Does the current pixel match the selected color key?
			if (pImage_RGB[i] == r &&
				pImage_RGB[i + 1] == g &&
				pImage_RGB[i + 2] == b)
			{
				pImage_RGBA[j] = 0;
				pImage_RGBA[j + 1] = 0;
				pImage_RGBA[j + 2] = 0;
				pImage_RGBA[j + 3] = 0;   // If so, set alpha to fully transparent.
			}
			else
			{
				pImage_RGBA[j] = pImage_RGB[i];
				pImage_RGBA[j + 1] = pImage_RGB[i + 1];
				pImage_RGBA[j + 2] = pImage_RGB[i + 2];
				pImage_RGBA[j + 3] = 255; // If not, set alpha to fully opaque.
			}
		}

		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Don't forget to use GL_RGBA for our new image data... we support Alpha transparency now!
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage_RGBA);
	}

	if (pImage_RGB)
		stbi_image_free(pImage_RGB);

	if (pImage_RGBA)
		delete[] pImage_RGBA;

}



void CTexture::LoadBMP(char* name)
{
	LogLine("ESTexture->LoadBMP: %s\n", name);

	// Create a place to store the texture
	//* AUX_RGBImageRec *TextureImage[1];
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);

	// Just in case we want to use the width and height later

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


		// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage);

	// Cleanup
	stbi_image_free(TextureImage);
}



void CTexture::LoadRAW(char *filename) {
	DWORD filesize;
	HGLOBAL pixels=NULL;
	DWORD tmp;
	HANDLE h_file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h_file != INVALID_HANDLE_VALUE) {
		filesize = GetFileSize(h_file, NULL);
		pixels = GlobalAlloc(GMEM_FIXED, filesize);
		ReadFile(h_file, pixels, filesize, &tmp,NULL);
		CloseHandle(h_file);
	}

	if(pixels) {
		// Generate the OpenGL texture id
		glGenTextures(1, &texture[0]);

		// Bind this texture to its id
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		// Use mipmapping filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
		GlobalFree(pixels);
	}
}

void CTexture::LoadSkyboxTex(char* name)
{
	LogLine("ESTexture->LoadSkyboxTex: %s\n", name);

	// Create a place to store the texture
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage);

	// Cleanup
	stbi_image_free(TextureImage);
}


void CTexture::LoadTGA(char *name)
{
	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLubyte		TGAcompare[12];								// Used to compare TGA header
	GLubyte		header[6];									// First 6 useful bytes of the header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	FILE *file;
	if((file = fopen(name, "rb")) == NULL)	{						// Open the TGA file	
		FatalError("cant open %s!", name);	
		return;	
	}
		
	LogLine("ESTexture->LoadTGA: %s\n", name);
	
	// Load the file and perform checks
	int tmp = (int)fread(TGAcompare,1,sizeof(TGAcompare),file);
	if(tmp != sizeof(TGAcompare))
		FatalError("cant open %s!", name);

	if(memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) != 0)
		FatalError("cant open %s!", name);

	if(fread(header,1,sizeof(header),file) != sizeof(header))
		FatalError("cant open %s!", name);

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = header[1] * 256 + header[0];
	height = header[3] * 256 + header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (header[4] != 24 && header[4] != 32))				// Is it 24 or 32 bit?
	{
		fclose(file);										// If anything didn't check out then close the file and return
		return;
	}

	bpp				= header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Make sure the data is allocated write and load it
	if(imageData == NULL ||									// Does the memory storage exist?
	   fread(imageData, 1, imageSize, file) != imageSize)	// Does the image size match the memory reserved?
	{
		if(imageData != NULL)								// Was the image data loaded
			free(imageData);								// If so, then release the image data

		fclose(file);										// Close the file
//		return;
	}

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// We are done with the file so close it
	fclose(file);

	// Set the type	
	if (bpp == 24)
		type = GL_RGB;
	if (bpp == 32)
		type = GL_RGBA;

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
}


void CTexture::LoadBMPResource(char *name)
{
	// Find the bitmap in the bitmap resources
	HRSRC hrsrc = FindResource(0, name, RT_BITMAP);

	// If you can't find it then return
	if (hrsrc==0)
		return;

	// Load the bitmap
	HGLOBAL resource = LoadResource(0, hrsrc);
	
	// If you can't load it then return
	if (resource==0)
		return;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into a bitmap
	BITMAP *bmp = (BITMAP*)buffer;

	// Get the height and width for future use
	width = bmp->bmWidth;
	height = bmp->bmHeight;

	// Reverse the blue colot bit and the red color bit
	unsigned char *ptr = (unsigned char *)buffer+sizeof(BITMAPINFO)+2;
	unsigned char temp;

	for (int i = 0; i < width*height; i++)
	{
		temp = ptr[i*3];
		ptr[i*3] = ptr[i*3+2];
		ptr[i*3+2] = temp;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)buffer+sizeof(BITMAPINFO)+2);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, bmp->bmBits);

	// Cleanup
	free(buffer);
	free(bmp);
}

void CTexture::LoadTGAResource(char *name)
{
	// struct to cast the resource into
	struct TGAstruct {
		GLubyte	TGAcompare[12];								// Used to compare TGA header
		GLubyte	header[6];									// First 6 useful bytes of the header
	};

	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	// Find the targa in the "TGA" resources
	HRSRC hrsrc = FindResource(0, name, "TGA");

	// If you can't find it then return
	if (hrsrc==0)
		return;

	// Load the targa
	HGLOBAL resource = LoadResource(0, hrsrc);

	// If you can't load it then return
	if (resource==0)
		return;

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into the targa struct
	TGAstruct *top = (TGAstruct*)buffer;

	// Make sure it checks out against our comparison header
	if (memcmp(TGAheader,top,sizeof(TGAheader)) != 0)
		return;

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = top->header[1] * 256 + top->header[0];
	height = top->header[3] * 256 + top->header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (top->header[4] != 24 && top->header[4] != 32))		// Is it 24 or 32 bit?
	{
		// If anything didn't check out then close the file and return
		return;
	}

	bpp				= top->header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Load the data in
	memcpy(imageData, (GLubyte*)buffer+18, imageSize);

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// Set the type
	if (bpp == 24)
		type = GL_RGB;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	free(buffer);
	free(top);
}

void CTexture::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}

// Load as env-map
void CTexture::LoadEnvMap(char *name)
{
	LogLine("ESTexture->LoadEnvMap: %s\n", name);

	Load(name);	
	envmap = true;

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

void CTexture::LoadJPG(char* name)
{
	LogLine("ESTexture->LoadJPG: %s\n", name);
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &texture[0]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.
	// GL_NEAREST is faster than GL_LINEAR, but looks blochy and pixelated.  Good for slower computers though.
	// Read more about the MIN and MAG filters at the bottom of main.cpp

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(TextureImage); // Free the image structure

}


void CTexture::RenderToTexture()
{
	int type = GL_RGB;
	int channels = 3;
	int size = 128;
	// Create a pointer to store the blank image data
	unsigned int *pTexture = NULL;											
//	unsigned int pTexture[128*128*3];
//	HGLOBAL pTexture = GlobalAlloc(GMEM_FIXED, size*size*channels);

	// We need to create a blank texture to render our dynamic texture to.
	// To do this, we just create an array to hold the data and then give this
	// array to OpenGL.  The texture is stored on the video card so we can
	// destroy the array immediately afterwards.
	// This function takes the texture array to store the texture, the
	// size of the texture for width and the heigth, the channels (1, 3 or 4),
	// the type (LUMINANCE, RGB, RGBA, etc..) and the texture ID to assign it too.

	// Allocate and init memory for the image array and point to it from pTexture
	pTexture = new unsigned int [size * size * channels];
	memset(pTexture, 0, size * size * channels * sizeof(unsigned int));	

	// Register the texture with OpenGL and bind it to the texture ID
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	// Create the texture and store it on the video card
	glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, (unsigned int *)pTexture);
	
	// Set the texture quality
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Since we stored the texture space with OpenGL, we can delete the image data
	delete [] pTexture;															
//	GlobalFree(pTexture);
	glViewport(0, 0, size, size);
}

void CTexture::UseRenderToTexture()
{
	int size = 128;
	
		// Before we copy the screen to a texture, we need to specify the current
		// texture to draw to by calling glBindTexture() with the appropriate texture 
		glBindTexture(GL_TEXTURE_2D, texture[0]);				

		// Now comes the moment we have all been waiting for, we render the screen
		// to the texture.  We pass in the texture type, detail level (0), pixel format,
		// the x and y position to start from, the width and height to grab, and a border.
		// If you only want a part of the screen, this works great for that.
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, size, size, 0);

		// Here we clear the screen and depth bits of the small viewport
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

		// Set our viewport back to it's normal size
		glViewport(0, 0, window_width, window_height);	

		// Make the color of the background black for the texture map
		glClearColor(0.0f, 0.0f, 0.0f, 0.15f);			
}