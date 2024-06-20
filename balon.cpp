#include "glos.h"
#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include <math.h>

static int alfaY = 0, alfaZ = 0, cueZ = -100, cueMargin = 0, rotationY = 0;

GLubyte rasters[][13] = {

{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},//C
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},//I
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},//L
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},//N
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},//I
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},//C
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},//E
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},//A
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},//N
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//spatiu
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},//M
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},//A
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},//T
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},//E
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},//I
};

GLuint fontOffset;

void makeRasterFont()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    fontOffset = glGenLists(128);
    for (int i = 32; i < 127; i++) {
        glNewList(i + fontOffset, GL_COMPILE);
        {
            glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[i - 32]);
        }
        glEndList();
    }
}

void printString(const char* s)
{
    glPushAttrib(GL_LIST_BIT);
    glListBase(fontOffset);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*)s);
    glPopAttrib();
}


void myInit() {
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    makeRasterFont();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position0[] = { 0.0, 0.0, 2.0, 0.0 };
    GLfloat light_position1[] = { 2.0, 20.0, -2.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
}


void CALLBACK RotateYRight()
{
    alfaY = (alfaY + 5) % 360;
}

void CALLBACK RotateYLeft()
{
    alfaY = (alfaY - 5) % 360;
}

void CALLBACK RotateZUp()
{
    alfaZ = (alfaZ + 5) % 360;
}

void CALLBACK RotateZDown()
{
    alfaZ = (alfaZ - 5) % 360;
}


void CALLBACK display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    char teststring[33];
    for (int i = 32; i < 64; i += 32)
    {
        glRasterPos2i(50, -130);
        for (int j = 0; j < 25; j++)
            teststring[j] = (char)(i + j);
        printString(teststring);
    }
    glRotatef(alfaY, 0.0, 1.0, 0.0);
    glRotatef(alfaZ, 0.0, 0.0, 1.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);

    GLfloat diffuse_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular_white[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat shininess_white[] = { 0.0f };

    GLfloat diffuse_black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat specular_black[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat shininess_black[] = { 0.0f };

    GLfloat diffuse_green[] = { 0.3f, 0.6f, 0.2f, 1.0f };
    GLfloat specular_green[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat shininess_green[] = { 40.0f };

    GLfloat diffuse_brown[] = { 0.58f, 0.29f, 0.0f, 1.0f };
    GLfloat specular_brown[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat shininess_brown[] = { 40.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_green);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_green);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_green);
    glTranslatef(0.0, 20.0, 0.0);
    glScalef(1.0, 1.5, 1.0);
    auxSolidSphere(70.0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_brown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_brown);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_brown);

    glTranslatef(0.0, -90.0, 0.0);
    glScalef(1.0, 0.66, 1.0);
    auxSolidCube(40.0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_black);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_black);

    glTranslatef(19.0, 60.0, 19.0);
    auxSolidCylinder(1.0, 50.0);
    glTranslatef(-38.0, 0.0, 0.0);
    auxSolidCylinder(1.0, 50.0);
    glTranslatef(0.0, 0.0, -38.0);
    auxSolidCylinder(1.0, 50.0);
    glTranslatef(38.0, 0.0, 0.0);
    auxSolidCylinder(1.0, 50.0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_white);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess_white);

    glTranslatef(-19.0, -30.0, 19.0);
    auxSolidCylinder(10.0, 8.0);


    glFlush();
}

void CALLBACK IdleFunction() {
    rotationY++;
    display();
    Sleep(1000.0 / 30.0);
}

#define ISOTROPIC
#ifdef ISOTROPIC
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) {
        return;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-160.0 * (GLfloat)w / (GLfloat)h, 160.0 * (GLfloat)w / (GLfloat)h, -160.0, 160.0, -250.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
}
#else
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-160.0, 160.0, -160.0, 160.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}
#endif

int main(int argc, char** argv)
{
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("Balon");
    auxIdleFunc(IdleFunction);
    myInit();
    auxKeyFunc(AUX_a, RotateYLeft);
    auxKeyFunc(AUX_d, RotateYRight);
    auxKeyFunc(AUX_s, RotateZDown);
    auxKeyFunc(AUX_w, RotateZUp);

    auxReshapeFunc(myReshape);
    auxMainLoop(display);
    return(0);
}

