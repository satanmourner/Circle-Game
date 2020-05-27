//circle game
//Sanaz Mahmoudi & Fate

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include<stdio.h>
#include <GL/freeglut.h>
using namespace std;

// Light and material Data
GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };
GLfloat fLightPosMirror[4] = { -100.0f, -100.0f, 50.0f, 1.0f };
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static GLfloat yTrans = 0.0f;         // Translate for animation
static GLfloat xTrans = 0.0f;         // Translate for animation

static GLint box_display_list;
int frame,tim,timebase=0;
char b[30];
static int score=0;

float x=0.0f,y=0.75f,z=-2.5f;
float lx=1.0f,ly=0.0f,lz=-1.0f;
int deltaMove = 0,w,h,move=0;
float ratio;
void *font=(void *)GLUT_BITMAP_8_BY_13;
float Y1=0.0f,Y2=0.0f,Y3=0.0f;
float acc=1.08;
float xx1=2.0,xx2=0.03f;


//////////////////////////////////////////////////
void initWindow();

void changeSize(int w1, int h1)
{

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h1 == 0)
        h1 = 1;

    w = w1;
    h = h1;
    ratio = 1.0f * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the clipping volume
    gluPerspective(45,ratio,0.1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z+8,
              x + lx,y + ly,z + lz,
              0.0f,1.0f,0.0f);
}
//////////////////////////////////////////////////
void SetupRC()
    {
    // Grayish background
    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Setup light parameters
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Mostly use material tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    }

//////////////////////////////////////////////////
// Draw the ground as a series of triangle strips. The
// shading model and colors are set such that we end up
// with a black and white checkerboard pattern.
void DrawGround(void)
    {
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 0.5f;
    GLfloat y = 0.0f;
    GLfloat fColor;
    GLfloat iStrip, iRun;
    GLint iBounce = 0;

    glShadeModel(GL_FLAT);
    for(iStrip = -fExtent; iStrip <= fExtent+400; iStrip += fStep)
        {
        glBegin(GL_TRIANGLE_STRIP);
            for(iRun = fExtent+400; iRun >= -fExtent; iRun -= fStep)
                {
                if((iBounce %2) == 0)
                    fColor = 1.0f;
                else
                    fColor = 0.0f;

                glColor4f(fColor, fColor, fColor, 0.5f);
                glVertex3f(iStrip, y, iRun);
                glVertex3f(iStrip + fStep, y, iRun);

                iBounce++;
                }
        glEnd();
        }
    glShadeModel(GL_SMOOTH);
    }

//////////////////////////////////////////////////
// Draw cable
float nums[100] = {0,0,0,0,0,1,1,1,0,1,1,1,1,1,2,2,2,2,1,1,0,2,2,1,2,0,1,2,2,2,2,1,1,
                1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,2};
int index=0 ;
void DrawLine(int i)
    {
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-7.0f, acc, -3.5f);
    glLineWidth(8.0f);

    Y1=nums[i];
    Y2=nums[i+1];
    Y3=nums[i+2];
    glBegin(GL_LINE_STRIP);
    glVertex3f(xx1,Y1,0.0f);
    glVertex3f(xx1+=3,Y2,0.0f);
    glVertex3f(xx1+=3,Y2,0.0f);
    glVertex3f(xx1+=3,Y3,0.0f);

    glEnd();

    glPopMatrix();
    }

//////////////////////////////////////////////////
//Draw ring
void DrawShape(){
    glColor3f(0.97f,0.50f,0.01f);
    glPushMatrix();
    glTranslatef(xTrans-2.0f, yTrans+acc, -3.5f);
    glRotatef(90,0.0,1.0,0.0);
    glutSolidTorus(xx2, 0.5f, 10, 25);
    if(yTrans>=0.48 || yTrans<=-0.48)
        exit(0);
    glPopMatrix();
    }

//////////////////////////////////////////////////
//list of line
GLuint createDL() {
    GLuint boxDL;
    // Create the id for the list
    boxDL = glGenLists(2);

    glNewList(boxDL+1,GL_COMPILE);


    glEndList();
    // start list
    glNewList(boxDL,GL_COMPILE);
    // start list
    for(int i = 0; i < 100; i++){
        index+=2;
        DrawLine(index);
            glPushMatrix();
            glCallList(boxDL+1);
            glPopMatrix();
        }
    glEndList();

    return(boxDL);
}

//////////////////////////////////////////////////
void initScene() {

    glEnable(GL_DEPTH_TEST);
    box_display_list = createDL();
    DrawLine(index);
}

//////////////////////////////////////////////////
//moving camera
void moveMeFlat(int i) {

    z = z + i*(lz)*0.01;
    x = x + i*(lx)*0.1;

	glLoadIdentity();
	gluLookAt(x, y, z+8,
		      x ,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
                          }

//////////////////////////////////////////////////
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////
void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////
void renderBitmapString(float x, float y, void *font,char *string)
{

  char *c;
  // set position to start drawing fonts
  glRasterPos2f(x, y);
  // loop all the characters in the string
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

///////////////////////////////////////////////////////////////////////
// Called to draw scene
 void RenderScene(void)
    {

        if (deltaMove)
            moveMeFlat(deltaMove);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPosMirror);
    glPushMatrix();
    glFrontFace(GL_CW);
    glScalef(1.0f, -1.0f, 1.0f);
    glCallList(box_display_list);
    glFrontFace(GL_CCW);
    glPopMatrix();

    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    glCallList(box_display_list);
    glPopMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    glCallList(box_display_list);
    glPopMatrix();


    glPushMatrix();
        // Move light under floor to light the "reflected" world
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPosMirror);
        glPushMatrix();
            glFrontFace(GL_CW);             // geometry is mirrored, swap orientation
            glScalef(1.0f, -1.0f, 1.0f);
            DrawShape();
            glFrontFace(GL_CCW);
        glPopMatrix();

        // Draw the ground transparently over the reflection
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         DrawGround();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

        // Restore correct lighting and draw the world correctly
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
        DrawShape();
    glPopMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
    DrawShape();
    glPopMatrix();

    glCallList(box_display_list);


     sprintf(b,"score:%d",score);

     glColor3f(0.97f,0.50f,0.01f);
    setOrthographicProjection();
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(30,15,(void *)font,"Circle Game");
    renderBitmapString(30,35,(void *)font,b);
    renderBitmapString(30,55,(void *)font,"Esc - Quit");
    glPopMatrix();
    resetPerspectiveProjection();
    // Do the buffer Swapnt




    glutSwapBuffers();
    }

///////////////////////////////////////////////////////////
// animation
void TimerFunction(int value)
    {

        score++;
        deltaMove = 1;

            xTrans+= 0.2f;   // Update Rotation
            yTrans-=0.03f;


        glutPostRedisplay();

    // Reset timer
    glutTimerFunc(100,TimerFunction,1);
    }

//////////////////////////////////////////////////
void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27)
        exit(0);
}

//////////////////////////////////////////////////
void SpecialKeys(int key, int x, int y)
{
     if(key == GLUT_KEY_UP)
        yTrans+= 0.55f;

    glutPostRedisplay();
}

//////////////////////////////////////////////////
void releaseKey(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_UP :	 if (yTrans > 0)
            yTrans = 0;
        break;
    }
}

//////////////////////////////////////////////////
//calling functions
void initWindow() {
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(SpecialKeys);
    glutSpecialUpFunc(releaseKey);
    glutDisplayFunc(RenderScene);
    glutIdleFunc(RenderScene);
    glutReshapeFunc(changeSize);
    initScene();
}

/////////////////////////////////////////////////////////////
// Main program entrypoint
int main(int argc, char* argv[])
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("OOOOOOOO CIRCLE OOOOOOOOO");
    glutTimerFunc(10, TimerFunction, 1);
    SetupRC();
    cout<<"\n This program represents a reflection";
    initWindow();
    glutMainLoop();
    return 0;
    }
