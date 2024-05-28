// texture class

#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
//#define STBI_ONLY_BMP
#include "stb_image.h"

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

void CTexture::LoadBMPKeyColor(char *name, unsigned char r, unsigned char g, unsigned char b)
{
	LogLine("ESTexture->LoadBMPKeyColor: %s\n", name);
    //* AUX_RGBImageRec *pImage_RGB = auxDIBImageLoad(name);
	int width, height, numComponents;
	unsigned char* pImage_RGB = stbi_load(name, &width, &height, &numComponents, 3);

	
    unsigned char *pImage_RGBA = NULL;

    if( pImage_RGB != NULL )
    {
        int imageSize_RGB  = width * height * 3;
        int imageSize_RGBA = width * height * 4;

        // allocate buffer for a RGBA image
        pImage_RGBA = new unsigned char[imageSize_RGBA];

        //
        // Loop through the original RGB image buffer and copy it over to the 
        // new RGBA image buffer setting each pixel that matches the key color
        // transparent.
        //

        int i, j;

        for( i = 0, j = 0; i < imageSize_RGB; i += 3, j += 4 )
        {
            // Does the current pixel match the selected color key?
            if( pImage_RGB[i]   == r &&
                pImage_RGB[i+1] == g &&
                pImage_RGB[i+2] == b )
            {
				pImage_RGBA[j]   = 0;
				pImage_RGBA[j+1] = 0;
				pImage_RGBA[j+2] = 0;
                pImage_RGBA[j+3] = 0;   // If so, set alpha to fully transparent.
			}
            else
            {
				pImage_RGBA[j]   = pImage_RGB[i];
				pImage_RGBA[j+1] = pImage_RGB[i+1];
				pImage_RGBA[j+2] = pImage_RGB[i+2];
				pImage_RGBA[j+3] = 255; // If not, set alpha to fully opaque.
			}
        }

        glGenTextures(1, &texture[0]);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Don't forget to use GL_RGBA for our new image data... we support Alpha transparency now!
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage_RGBA );
    }

    if( pImage_RGB )
		stbi_image_free( pImage_RGB);

    if( pImage_RGBA )
        delete [] pImage_RGBA;

}

void CTexture::LoadBMP(char *name)
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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage);

	// Cleanup
	stbi_image_free(TextureImage);
}


void CTexture::LoadSkyboxTex(char *name)
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