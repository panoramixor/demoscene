// texture class

#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
//#define STBI_ONLY_BMP
#include "stb_image.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexture::CTexture()
{

}

CTexture::~CTexture()
{

}

// The function pointers for multitexturing.
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

// This function gets the required multi texture functions.
void InitMultiTexture(void)
{
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
						wglGetProcAddress("glActiveTextureARB");

	if(glActiveTextureARB == 0)
	{
		printf("Error: can not load multi-texture extension");
		exit(1);
	}

	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)
						wglGetProcAddress("glMultiTexCoord2fARB");

	if(glActiveTextureARB == 0)
	{
		printf("Error: can not load multi-texture extension");
		exit(1);
	}

}

void CTexture::MultiUse(CTexture g_texture1,  CTexture g_texture2)
{
	InitMultiTexture();

	//Setup the first texture unit.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	//Enable 2D textures.
	glEnable(GL_TEXTURE_2D);
	//Bind the texture.
	glBindTexture(GL_TEXTURE_2D, g_texture1.ID);
	//Use sphere mapping auto coord generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//Turn on auto coord generation. N.B.! This is a per texture unit operation here!
	//Confused me at first, I thought glEnable was global, but this value is local to each
	//Texture unit!!!
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	//Multiply this texture by the ships material colors.
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MULT);

	
	//Setup the 2nd texture unit.
	glActiveTextureARB(GL_TEXTURE1_ARB);
	//Enable 2D textures.
	glEnable(GL_TEXTURE_2D);
	//bind out texture.
	glBindTexture(GL_TEXTURE_2D, g_texture2.ID);

	//Again setup sphere mapping, and turn it on.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	//Set to additive. Where the centre (singularity) of the texture appears, will add white
	//Onto the ship, giving a specular "white out" effect.
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
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
//		LoadEnvMapJPG(texturename);

}

void CTexture::Use()
{
	glEnable(GL_TEXTURE_2D);								// Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Bind the texture as the current one
}

void CTexture::LoadFrameBuffer()
{
//	unsigned char *image = (unsigned char*)malloc(sizeof(unsigned char)*100*76*6/*window_width*window_height*3*/);
	HGLOBAL image = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, window_width* window_height*3);
//	BYTE image[window_width*window_height*3];
		glReadPixels( 0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, image );
		// Generate the OpenGL texture id
		glGenTextures(1, &texture[0]);

		// Bind this texture to its id
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		// Use mipmapping filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);

		// Generate the mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, image);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, bmp->bmBits);
	GlobalFree(image);
}

void CTexture::LoadBMP(char* name)
{
	LogLine("ESTexture->LoadBMP: %s\n", name);

	// Create a place to store the texture
	//* AUX_RGBImageRec *TextureImage[1];
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 3);

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



void CTexture::LoadSkyboxTex(char* name)
{
	LogLine("ESTexture->LoadSkyboxTex: %s\n", name);

	stbi_set_flip_vertically_on_load(true);
	// Create a place to store the texture
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 3);

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

	stbi_set_flip_vertically_on_load(false);
	// Cleanup
	stbi_image_free(TextureImage);
}


void CTexture::LoadTGA(char* name)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage);

	// Cleanup
	stbi_image_free(TextureImage);
	stbi_set_flip_vertically_on_load(false);
}

/*
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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	free(buffer);
	free(top);
}
*/

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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void CTexture::LoadJPGClamped(char *name)
{
	LogLine("ESTexture->LoadJPG: %s\n", name);
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &texture[0]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.
	// GL_NEAREST is faster than GL_LINEAR, but looks blochy and pixelated.  Good for slower computers though.
	// Read more about the MIN and MAG filters at the bottom of main.cpp
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the image data that we loaded since OpenGL stored it as a texture

	stbi_image_free(TextureImage); // Free the image structure
}

void CTexture::LoadEnvMapJPG(char *name)
{
	LogLine("ESTexture->LoadEnvMapJPG: %s\n", name);
	int width, height, numComponents;
	unsigned char* TextureImage = stbi_load(name, &width, &height, &numComponents, 4);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &texture[0]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//Turn on auto coord generation. N.B.! This is a per texture unit operation here!
	//Confused me at first, I thought glEnable was global, but this value is local to each
	//Texture unit!!!
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.
	// GL_NEAREST is faster than GL_LINEAR, but looks blochy and pixelated.  Good for slower computers though.
	// Read more about the MIN and MAG filters at the bottom of main.cpp
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the image data that we loaded since OpenGL stored it as a texture
	stbi_image_free(TextureImage); // Free the image structure
}

void CTexture::LoadJPGAndFilter(char *name, GLint minfilter, GLint magfilter)
{
	LogLine("ESTexture->LoadJPGAndFilter: %s\n", name);
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
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minfilter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magfilter);

	// Now we need to free the image data that we loaded since OpenGL stored it as a texture
	stbi_image_free(TextureImage); // Free the image structure
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