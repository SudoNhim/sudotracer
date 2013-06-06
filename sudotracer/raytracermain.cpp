//Runs the raytracer and displays results using opengl/glut

#include <stdio.h>
#include <malloc.h>
#include <GL/glut.h>
#include "renderer.h"

const int width = 640;
const int height = 480;
unsigned char *imagedata;
PixelMap *pmap;

void initGLUT(int argc, char **argv) 
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(width, height);
    glutCreateWindow("Raytracing");
}

void initGL()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.3, 0.5, 0.3, 1);
	glEnable(GL_TEXTURE_2D);
}

void initTex()
{
	// Allocate data for the image
	int nBytes = width*height*3;
	imagedata = (unsigned char*)malloc(nBytes);

	// Generate an ID and bind it as the current texture
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void drawGL()
{
	// Load the image into texture memory
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata );

	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 1);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(1, 0); glVertex2f(1, 0);
	glEnd();
	
	glFlush();
}

void drawScene()
{
	// Render the image
	pmap->renderImage();
	// Draw the textured rectangle
	drawGL();
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		pmap->adaptiveAA();
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	//Init image
	initGLUT(argc,argv);
	initGL();
	initTex();
	initScene();

	// Initiate mapping from pixels to samples
	pmap = new PixelMap(imagedata, width, height);

	//Init ray intersection trees

	//Init photon mapping

	//loop
	glutDisplayFunc(drawScene);
	glutMouseFunc(mouse);

	drawScene();

    glutMainLoop();
	return 0;
}