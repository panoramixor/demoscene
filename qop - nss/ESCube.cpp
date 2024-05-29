// cube thing

#include "stdafx.h"

/* Constructor */
CCube::CCube()
{
	lid = 0xff;
}

/* Destructor */
CCube::~CCube()
{

}


void CCube::Create(float width, float height, float lenght)
{
	coord_array[0].Set(-width/2, height/2, lenght/2);
	coord_array[1].Set(width/2, height/2, lenght/2);
	coord_array[2].Set(width/2, -height/2, lenght/2);
	coord_array[3].Set(-width/2, -height/2, lenght/2);

	coord_array[4].Set(-width/2, height/2, -lenght/2);
	coord_array[5].Set(width/2, height/2, -lenght/2);
	coord_array[6].Set(width/2, -height/2, -lenght/2);
	coord_array[7].Set(-width/2, -height/2, -lenght/2);

	// /_/
	index_array[0].idx[0] = 0;
	index_array[0].idx[1] = 1;
	index_array[0].idx[2] = 2;
	index_array[0].idx[3] = 3;

	// _/
	index_array[1].idx[0] = 1;
	index_array[1].idx[1] = 5;
	index_array[1].idx[2] = 6;
	index_array[1].idx[3] = 2;

	index_array[2].idx[0] = 5;
	index_array[2].idx[1] = 4;
	index_array[2].idx[2] = 7;
	index_array[2].idx[3] = 6;

	index_array[3].idx[0] = 0;
	index_array[3].idx[1] = 4;
	index_array[3].idx[2] = 7;
	index_array[3].idx[3] = 3;

	index_array[4].idx[0] = 0;
	index_array[4].idx[1] = 1;
	index_array[4].idx[2] = 5;
	index_array[4].idx[3] = 4;

	index_array[5].idx[0] = 3;
	index_array[5].idx[1] = 2;
	index_array[5].idx[2] = 6;
	index_array[5].idx[3] = 7;
}

void CCube::Draw(float sTime)
{
	if(lid == 0xff) {
		lid = glGenLists(1);
		glNewList(lid, GL_COMPILE);
		glBegin(GL_QUADS);
			for(int i=0;i<6;i++) {
				glTexCoord2f(0+sTime, 1+sTime);
				glVertex3f(coord_array[index_array[i].idx[0]].x, coord_array[index_array[i].idx[0]].y, coord_array[index_array[i].idx[0]].z);
				glTexCoord2f(1+sTime, 1+sTime);
				glVertex3f(coord_array[index_array[i].idx[1]].x, coord_array[index_array[i].idx[1]].y, coord_array[index_array[i].idx[1]].z);
				glTexCoord2f(1+sTime, 0+sTime);
				glVertex3f(coord_array[index_array[i].idx[2]].x, coord_array[index_array[i].idx[2]].y, coord_array[index_array[i].idx[2]].z);
				glTexCoord2f(0+sTime, 0+sTime);
				glVertex3f(coord_array[index_array[i].idx[3]].x, coord_array[index_array[i].idx[3]].y, coord_array[index_array[i].idx[3]].z);
			}
		glEnd();
		glEndList();
	}
	glCallList(lid );
}