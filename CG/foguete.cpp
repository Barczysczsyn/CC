/////////////////////////////////////////////////////////////
// torpedo.cpp
//
// This program draws a torpedo composed of different pieces:
// (i) Body: GLU cylinder
// (ii) Nose: hemisphere
// (iii) Three fins: GLU partial disks
// (iv) Backside disk: GLU disk
// (v) Propeller stem: GLU cylinder
// (vi) Three propeller blades: bicubic Bezier patches
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the x, X, y, Y, z, Z keys to rotate the viewpoint.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265358979324

using namespace std;

// Begin globals.
static unsigned int base;   // Display lists base index.
static GLUquadricObj *qobj; // Pointer to GLU quadric object.
GLfloat luzAmbiente[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat corLuz[] = {1.0f, 1.0f, 0.8f, 1.0f};
GLfloat *forcaLuz;

GLfloat corFogo[] = {(float)(rand() % 2), (float)(rand() % 2), 0.0f, 1.0f};
GLfloat posFogo[] = {0.0f, -2.0f, 0.0f, 1};

GLfloat corPadrao[] = {0.0f, 0.0f, 0.0f, 1.0f};

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate torpedo.
static int animateMode = 0;                            // In animation mode?
static int animationPeriod = 1;                        // Speed of animation.
static float angle = 0;                                // Angle of torpedo propeller turn.

static unsigned int texture[2]; // Array of texture indices.

GLuint idTextura;
// End globals.

static float angulo = 0;

// Timer function.
void animate(int value)
{
   if (animateMode)
   {
      angulo -= 1;
      angle += 10;
      if (angle > 360.0)
         angle -= 360.0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Struct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};
// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;

   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);

   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *)&offset, 4);

   // Get the header size of the bitmap.
   infile.read((char *)&headerSize, 4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read((char *)&bmp->sizeX, 4);
   infile.read((char *)&bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *)bmp->data, size);

   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   {
      temp = bmp->data[i];
      bmp->data[i] = bmp->data[i + 2];
      bmp->data[i + 2] = temp;
   }

   return bmp;
}
void loadExternalTextures()
{
   // Array local para armazenar os ponteiros das imagens.
   BitMapFile *image[2];

   // Carrega as duas imagens (ajuste os nomes dos arquivos conforme necessário).
   image[0] = getBMPData("chao.bmp");
   image[1] = getBMPData("chao.bmp");

   // glGenTextures(GL_TEXTURE_2D, texture[0]);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   // Configuração de repetição e filtro da textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repete no eixo X
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repete no eixo Z
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Configura a textura 1 (corpo do torpedo).
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
}
// Initialization routine.
void setup(void)
{
   // fogo
   glLightfv(GL_LIGHT3, GL_AMBIENT, luzAmbiente);
   glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 100.0f);
   glLightfv(GL_LIGHT3, GL_DIFFUSE, corFogo);
   glLightfv(GL_LIGHT3, GL_SPECULAR, corFogo);
   glLightfv(GL_LIGHT3, GL_POSITION, posFogo);
   glEnable(GL_LIGHT3);

   glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
   glLightfv(GL_LIGHT0, GL_SPECULAR, corLuz);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);

   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); // Mudado de GLU_LINE para GLU_FILL
   gluQuadricNormals(qobj, GLU_SMOOTH); // Gera normais para a luz funcionar

   glEnable(GL_DEPTH_TEST);
   // fim do codigo de cor

   glClearColor(1.0, 1.0, 1.0, 0.0);

   // XXX quadrica do original
   //  Initialize GLU quadric object.
   // qobj = gluNewQuadric();
   // gluQuadricDrawStyle(qobj, GLU_FILL);

   // Generate display list base.
   base = glGenLists(2);

   // Texturas
   loadExternalTextures();

   // Propeller blade.
   glNewList(base + 1, GL_COMPILE);
   glPushMatrix();

   // Enable Bezier surface: a bicubic Bezier patch.
   // glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, controlPointsPropellerBlade[0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(5, 0.0, 1.0, 5, 0.0, 1.0);

   glTranslatef(0.0, 0.2, 0.0);

   // Create Bezier surface.
   glEvalMesh2(GL_LINE, 0, 5, 0, 5);

   glPopMatrix();
   glEndList();
}
void drawScene(void) {
   glListBase(base);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   // Afasta a câmera para enxergar toda a montanha e a órbita do foguete
   gluLookAt(0.0, 12.0, 35.0,  // Posição da câmera (X, Y, Z)
             0.0, 0.0, 0.0,    // Para onde ela olha (centro da montanha)
             0.0, 1.0, 0.0);   // Vetor UP

             GLfloat posLuz0[] = {10.0f, 20.0f, 10.0f, 1.0f};
             glLightfv(GL_LIGHT0, GL_POSITION, posLuz0);

   // Rotação manual da cena pelas teclas x, y, z
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // ==========================================
   // 1. CENÁRIO ESTÁTICO (Chão e Montanha)
   // ==========================================
   glPushMatrix();
      // Desenhar o chão na base (Y = -5)
      glColor3f(0.2f, 0.7f, 0.3f);
      glBegin(GL_QUADS);
         glVertex3f(-25.0f, -5.0f, -25.0f);
         glVertex3f( 25.0f, -5.0f, -25.0f);
         glVertex3f( 25.0f, -5.0f,  25.0f);
         glVertex3f(-25.0f, -5.0f,  25.0f);
      glEnd();

      // Desenhar montanha no centro (0, -5, 0)
      glColor3f(0.3f, 0.1f, 0.05f);
      glPushMatrix();
         glTranslatef(0.0f, -5.0f, 0.0f); // Apoia a base no chão
         glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Aponta o cone para cima
         glutSolidCone(10.0, 15.0, 20, 20);
      glPopMatrix();
   glPopMatrix();

   // ==========================================
   // 2. FOGUETE EM ÓRBITA
   // ==========================================
   glPushMatrix();
      // Passo A: Gira ao redor do centro da montanha (Eixo Y)
      glRotatef(angulo, 0.0f, 1.0f, 0.0f);

      // Passo B: Afasta do centro para definir o raio da órbita (raio = 14) e altura (Y = 2)
      glTranslatef(14.0f, 2.0f, 0.0f);

      // Nota: Como o cilindro/cone do torpedo aponta para +Z por padrão,
      // ao transladar em +X e girar no eixo Y, o bico já aponta para a frente do movimento!
      // Se ele voar de ré, descomente a linha abaixo:
      // glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

      glColor3f(0.5f, 0.5f, 0.5f);

      // Corpo do torpedo
      gluCylinder(qobj, 0.5, 0.5, 3.0, 15, 15);

      // Bico do torpedo
      glPushMatrix();
         glTranslatef(0.0, 0.0, 3.0);
         glutSolidCone(0.5, 1.0, 15, 5);
      glPopMatrix();


      // Disco traseiro
      gluDisk(qobj, 0.0, 0.5, 10, 10);

      // Haste traseira
      glPushMatrix();
         glTranslatef(0.0, 0.0, -0.3);
         gluCylinder(qobj, 0.2, 0.2, 0.3, 5, 5);
      glPopMatrix();

      // Fogo do propulsor (isolado em sua própria matriz para não deslocar o resto)
      glPushMatrix();
         GLfloat cordoFogo[] = {1.0f, (float)(rand() % 2), 0.0f, 1.0f};
         glMaterialfv(GL_FRONT, GL_EMISSION, cordoFogo);
         glTranslatef(0.0f, 0.0f, -0.6f);
         // Escala pequena aleatória para simular chama piscando sem ficar gigante
         float escalaFogo = 0.3f + ((rand() % 10) / 20.0f);
         glutSolidSphere(escalaFogo, 10, 10);
         glMaterialfv(GL_FRONT, GL_EMISSION, corPadrao); // Zera a emissão
      glPopMatrix();

   glPopMatrix(); // Fim da órbita do foguete

   glutSwapBuffers();
}

void resize(int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // Aumentado zFar de 50.0 para 150.0 para não cortar o fundo da cena
   gluPerspective(60.0, (float)w / (float)h, 1.0, 150.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
   case ' ':
      if (animateMode == 0)
         animateMode = 1;
      else
         animateMode = 0;
      glutPostRedisplay();
      break;
   case 'x':
      Xangle += 5.0;
      if (Xangle > 360.0)
         Xangle -= 360.0;
      glutPostRedisplay();
      break;
   case 'X':
      Xangle -= 5.0;
      if (Xangle < 0.0)
         Xangle += 360.0;
      glutPostRedisplay();
      break;
   case 'y':
      Yangle += 5.0;
      if (Yangle > 360.0)
         Yangle -= 360.0;
      glutPostRedisplay();
      break;
   case 'Y':
      Yangle -= 5.0;
      if (Yangle < 0.0)
         Yangle += 360.0;
      glutPostRedisplay();
      break;
   case 'z':
      Zangle += 5.0;
      if (Zangle > 360.0)
         Zangle -= 360.0;
      glutPostRedisplay();
      break;
   case 'Z':
      Zangle -= 5.0;
      if (Zangle < 0.0)
         Zangle += 360.0;
      glutPostRedisplay();
      break;
   case 27:
      exit(0);
      break;
   default:
      break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the viewpoint." << endl;
}

// Main routine.
int main(int argc, char **argv)
{

   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Foguete");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();

   return 0;
}
