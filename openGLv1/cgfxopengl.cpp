#ifdef __WINDOWS
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "cgfxopengl.h"

#pragma warning(disable:4305) //Disable implic float-double casting

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
	glClearColor(0.0, 0.0, 0.5, 0.0);	//clear background

	m_angle = 0.0f;

	return true;
}

bool CGfxOpenGL::Shutdown()
{
	return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
	if (height == 0) //Control divisor
		height = 1;

	glViewport(0, 0, width, height);	//reset viewport
	glMatrixMode(GL_PROJECTION);		//set projection matrix to current
	glLoadIdentity();					//rest projection matrix

	gluPerspective(52.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);		//set modelview matrix to current
	glLoadIdentity();				//reset modelview matrix

	m_windowWidth = width;
	m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
	m_angle += 0.1f;
}

void CGfxOpenGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear screen and depth buffer
	glLoadIdentity();									//reset the previous transformation

	//moveback 15 units and rotate about the y-axis
	glTranslatef(0.0, 0.0, -5.0f);
	glRotatef(m_angle, 0.0f, 0.0f, 1.0f);

	//draw the faces of diamond
	glBegin(GL_TRIANGLES);

		glColor3f(0.9f, 0.0f, 0.3f);
		glVertex3f(1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.9f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.3f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.9f, 0.0f, 0.3f);
		glVertex3f(0.5f, 0.5f, 0.0f);

		glColor3f(0.0f, 0.0f, 0.9f);
		glVertex3f(1.5f, 0.5f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.3f);
		glVertex3f(1.0f, 1.0f, 0.0f);

	glEnd();
}