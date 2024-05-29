// skybox class
// DigiBen (ported to c++ and extended by panoramix)

#include "stdafx.h"

CSkybox::CSkybox()
{
	m_skybox = -1;
}

CSkybox::~CSkybox()
{
	glDeleteLists(m_skybox, 1);
}

void CSkybox::Create(float x, float y, float z, float width, float height, float length, char *tex_filepath)
{
	char filepath[MAX_PATH];
	char fullpath[MAX_PATH];

	ZeroMemory(filepath, MAX_PATH);
	lstrcat(filepath, tex_filepath);
	
	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "bk.bmp");
	tex_back.LoadSkyboxTex(fullpath);

	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "ft.bmp");
	tex_front.LoadSkyboxTex(fullpath);

	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "dn.bmp");
	tex_bottom.LoadSkyboxTex(fullpath);

	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "up.bmp");
	tex_top.LoadSkyboxTex(fullpath);

	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "lf.bmp");
	tex_left.LoadSkyboxTex(fullpath);

	ZeroMemory(fullpath, MAX_PATH);
	lstrcat(fullpath, filepath);
	lstrcat(fullpath, "rt.bmp");
	tex_right.LoadSkyboxTex(fullpath);


	m_skybox = glGenLists(1);
	glNewList(m_skybox, GL_COMPILE);
	
	// This is the most important function of this tutorial.  This function
	// used to just create a silly colored cube in the RotateCube tutorial,
	// but now it creates something beautiful.  You'll notice we added
	// some more parameters to the function.  This way we can change the perspective
	// of the sky box.  It doesn't really look good if it's a perfect cube.  Some
	// textures look better at different ratios.  We assign the sky box textures
	// to each side of the box creating the illusion of a detailed 3D world.
	// You will notice I had to change the texture coordinates for every one
	// to be flipped correctly.  Also, depending on your culling, the vertex
	// order might need to be changed around.  I don't use culling in this tutorial
	// so it will work fine here, but be sure to remember this if you do.

	// Bind the BACK texture of the sky map to the BACK side of the cube
	tex_back.Use();

	// Since we want the sky box to be centered around X, Y, and Z for ease,
	// we do a little math to accomplish this.  We just change the X, Y and Z
	// to perform this task.  If we just minus half the width, height and length
	// from x, y and z it will give us the desired result.  Now when we create the
	// box it will center it around (x, y, z)

	// This centers the sky box around (x, y, z)
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// Bind the FRONT texture of the sky map to the FRONT side of the box
	tex_front.Use();

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);	
	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
	tex_bottom.Use();

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// Bind the TOP texture of the sky map to the TOP side of the box
	tex_top.Use();

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Bind the LEFT texture of the sky map to the LEFT side of the box
	tex_left.Use();

	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
	glEnd();

	tex_right.Use();
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
	glEndList();
}

void CSkybox::Draw()
{
	glCallList(m_skybox);
}

