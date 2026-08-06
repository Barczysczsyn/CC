/////////////////////////////////
// box.cpp
//
// This program draws a wire box.
//
// Sumanta Guha.
/////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glLoadIdentity();

   // Modeling transformations.


   //roda
   glPushMatrix();
   glTranslatef(-5, -2.5, -17.5);
   glScalef(0.5, 0.5, 1);
   glutWireTorus(1, 3, 10, 20);
   glPopMatrix();
   //roda

   //roda
   glPushMatrix();
   glTranslatef(-5, -2.5, -12.5);
   glScalef(0.5, 0.5, 1);
   glutWireTorus(1, 3, 10, 20);
   glPopMatrix();
   //roda

   //roda
   glPushMatrix();
   glTranslatef(5, -2.5, -17.5);
   glScalef(0.5, 0.5, 1);
   glutWireTorus(1, 3, 10, 20);
   glPopMatrix();
   //roda

   //roda
   glPushMatrix();
   glTranslatef(5, -2.5, -12.5);
   glScalef(0.5, 0.5, 1);
   glutWireTorus(1, 3, 10, 20);
   glPopMatrix();
   //roda

   glPushMatrix();
   glScalef(2, -0.5, 1);
   //o melhor valor deve estar entre 6.5 e 7
   glTranslatef(0, -6.5, -15);
   glutWireCube(5);
   glPopMatrix();


   //base do carro
   glScalef(3, 1, 1);
   glTranslatef(0, 0, -15);
   glutWireCube(5);
   // aparentemente as instrucoes sao executadas ao contrario
   // menos o push e pop

   glFlush();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
      exit(0);
      break;
   default:
      break;
   }
}

// Main routine.
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("box.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}
