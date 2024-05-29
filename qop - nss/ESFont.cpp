// Font class 

#include "stdafx.h"

/* Constructor */
CFont::CFont()
{
	size.Set(0.04f,0.04f);
}

/* Destructor */
CFont::~CFont()
{

}

void CFont::Draw(char character)
{
	glLineWidth(0.5f);
	glEnable(GL_LINE_SMOOTH);
		switch(character) {
			case 'a':
				glBegin(GL_LINES);
					glVertex3f(0+size.x/2,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(0.f,size.y-size.y/1.4f,-1.f);

					glVertex3f(0.f,size.y-size.y/1.4f,-1.f);
					glVertex3f(size.x,size.y-size.y/1.4f,-1.f);
				glEnd();
				break;

			case 'b':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/4,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(0,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'c':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'd':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(size.x,size.y-size.y/4,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(0,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'e':
				glBegin(GL_LINES);
					glVertex3f(0.f,size.y-size.y/2.f,-1.f);
					glVertex3f(size.x,size.y-size.y/2.f,-1.f);

					glVertex3f(size.x,size.y-size.y/2.f,-1.f);
					glVertex3f(size.x,size.y-size.y/1.4f,-1.f);

					glVertex3f(size.x,size.y-size.y/1.4f,-1.f);
					glVertex3f(0.f,size.y-size.y/1.4f,-1.f);

					glVertex3f(0.f,size.y-size.y/2.f,-1.f);
					glVertex3f(0.f,0.f,-1.f);

					glVertex3f(0.f,0.f,-1.f);
					glVertex3f(size.x/2.f,0.f,-1.f);

				glEnd();
				break;

			case 'f':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(0,size.y-size.y/1.4f,-1);
					glVertex3f(size.x/2,size.y-size.y/1.4f,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

				glEnd();
				break;

			case 'g':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,(size.y-size.y/2)-size.y/1.4f,-1);

					glVertex3f(size.x,(size.y-size.y/2)-size.y/1.4f,-1);
					glVertex3f(size.x/2,(size.y-size.y/2)-size.y/1.4f,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(0,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'h':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/4,-1);
					glVertex3f(0,0,-1);

				glEnd();
				break;

			case 'i':
				glBegin(GL_LINES);
					glVertex3f(size.x/8.f,size.y-size.y/2.f,-1.f);
					glVertex3f(size.x/8.f,0.f,-1.f);

					glVertex3f(size.x/8.f,size.y-size.y/2.f,-1.f);
					glVertex3f(size.x/4.f,size.y-size.y/2.f,-1.f);

					glVertex3f(size.x/8.f,0.f,-1.f);
					glVertex3f(size.x/2.5f,0.f,-1.f);

					glVertex3f(size.x/8.f,size.y-size.y/4.f,-1.f);
					glVertex3f(size.x/8.f,size.y-size.y/3.f,-1.f);

				glEnd();
				break;

			case 'j':
				glBegin(GL_LINES);
					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(0,size.y/4,-1);

					glVertex3f(size.x,size.y - size.y/4,-1);
					glVertex3f(size.x,size.y - size.y/3,-1);

				glEnd();
				break;

			case 'k':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/4,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,size.y-size.y/1.4f,-1);
					glVertex3f(size.x,size.y-size.y/1.4f,-1);

					glVertex3f(size.x,size.y-size.y/1.4f,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x/2,size.y-size.y/1.4f,-1);
					glVertex3f(size.x/2,size.y-size.y/2,-1);

				glEnd();
				break;

			case 'l':
				glBegin(GL_LINES);
					glVertex3f(size.x/8,size.y-size.y/4,-1);
					glVertex3f(size.x/8,0,-1);

					glVertex3f(size.x/8,0,-1);
					glVertex3f(size.x/4,0,-1);

				glEnd();
				break;

			case 'm':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(size.x/2,size.y-size.y/2,-1);
					glVertex3f(size.x/2,size.y/4,-1);

				glEnd();
				break;

			case 'n':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

				glEnd();
				break;

			case 'o':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);
				glEnd();
				break;

			case 'p':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,-size.y/4,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);


				glEnd();
				break;

			case 'q':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,-size.y/4,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);


				glEnd();
				break;

			case 'r':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/1.4f,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

				glEnd();
				break;

			case 's':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x/2,size.y-size.y/2,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,size.y-size.y/1.4f,-1);

					glVertex3f(0,size.y-size.y/1.4f,-1);
					glVertex3f(size.x,size.y-size.y/1.4f,-1);

					glVertex3f(size.x,size.y-size.y/1.4f,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(0,0,-1);

				glEnd();
				break;

			case 't':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x/2,size.y-size.y/2,-1);
					glVertex3f(size.x/2,0,-1);

					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,size.y-size.y/1.4f,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/1.4f,-1);

				glEnd();
				break;

			case 'u':
				glBegin(GL_LINES);
					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(0,0,-1);
					glVertex3f(0,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'v':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x/2,0,-1);

					glVertex3f(size.x/2,0,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);
				glEnd();
				break;

			case 'w':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,0,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x/2,0,-1);
					glVertex3f(size.x/2,size.y/4,-1);
				glEnd();
				break;

			case 'x':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);
				glEnd();
				break;

			case 'y':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(size.x,-size.y/4,-1);

					glVertex3f(size.x,-size.y/4,-1);
					glVertex3f(size.x/2,-size.y/4,-1);

				glEnd();
				break;

			case 'z':
				glBegin(GL_LINES);
					glVertex3f(0,size.y-size.y/2,-1);
					glVertex3f(size.x,size.y-size.y/2,-1);

					glVertex3f(size.x,size.y-size.y/2,-1);
					glVertex3f(0,0,-1);

					glVertex3f(0,0,-1);
					glVertex3f(size.x,0,-1);
				glEnd();
				break;

		}
}