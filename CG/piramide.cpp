////////////////////////////////////////////////////////////////////////////////////
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

// TODO como no geogebra classsic
//[x] rotacao e translacao apenas nos eixos canonicos (x,y,z)

//[x] como vou tornar isso 3D?
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define CIRCLE 4
#define PIRAMIDE 5
#define NUMBERPRIMITIVES 4

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height;    // OpenGL window size.
static float pointSize = 3.0;    // Size of point
static int primitive = PIRAMIDE; // Current drawing primitive.
static int pointCount = 0;       // Number of  specified points.
static int tempX, tempY;         // Co-ordinates of clicked point.
static int isGrid = 1;           // Is there grid?

// coordenada do ponto inicial da piramide
static int xini, yini;
// so pra saber se vai mexer no topo ou nao
static int topo;
// rotacionar cena

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.

// variaveis globais pra registrar o ponteiro do mouse
int MouseX, MouseY;

// Point class.
class Point
{
public:
    Point(int xVal, int yVal)
    {
        x = xVal;
        y = yVal;
    }
    void drawPoint(void); // Function to draw a point.
    // tive que adicionar gets tbm

    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }

private:
    int x, y;          // x and y co-ordinates of point.
    static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(x, y, 0.0);
    glEnd();
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints(void)
{
    // Loop through the points array drawing each point.
    pointsIterator = points.begin();
    while (pointsIterator != points.end())
    {
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

// Line class.
class Line
{
public:
    Line(int x1Val, int y1Val, int x2Val, int y2Val)
    {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }
    void drawLine();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};

// Function to draw a line.
void Line::drawLine()
{
    glBegin(GL_LINES);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
    // Loop through the lines array drawing each line.
    linesIterator = lines.begin();
    while (linesIterator != lines.end())
    {
        linesIterator->drawLine();
        linesIterator++;
    }
}

// Rectangle class.
class Rectangle
{
public:
    Rectangle(int x1Val, int y1Val, int x2Val, int y2Val)
    {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }
    void drawRectangle();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
    // Loop through the rectangles array drawing each rectangle.
    rectanglesIterator = rectangles.begin();
    while (rectanglesIterator != rectangles.end())
    {
        rectanglesIterator->drawRectangle();
        rectanglesIterator++;
    }
}

// XXX PIRAMIDE

class Piramide
{
public:
    Piramide(vector<Point> pts, int altura, int xt, int yt)
    {
        pontos = pts;
        h = altura;
        xtopo = xt;
        ytopo = yt;
    }
    void drawPiramide();

private:
    int xtopo, ytopo, h; // x and y co-ordinates of diagonally opposite vertices.
    vector<Point> pontos;
};

void Piramide::drawPiramide()
{

    vector<Point>::iterator pontosI;
    glBegin(GL_LINE_LOOP);

    pontosI = pontos.begin();
    while (pontosI != pontos.end())
    {
        // piramidesIterator->drawPiramide();
        glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
        pontosI++;
    }
    glEnd();

    glBegin(GL_LINES);

    // puxa uma linha de cada extremidade do retangulo para a ponta da piramide
    pontosI = pontos.begin();
    while (pontosI != pontos.end())
    {
        // piramidesIterator->drawPiramide();
        glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
        glVertex3f(xtopo, ytopo, h);
        pontosI++;
    }
    glEnd();
}

// Vector of rectangles.
vector<Piramide> piramides;

// Iterator to traverse a Rectangle array.
vector<Piramide>::iterator piramidesIterator;

// armazena os pontos temporarios criados antes da piramide ser completa
vector<Point> pontoPiramideTemp;

// vector<Point>::iterator pontoPiramideIterator;

// Function to draw all rectangles in the rectangles array.
void drawPiramides(void)
{
    // Loop through the rectangles array drawing each rectangle.
    piramidesIterator = piramides.begin();
    while (piramidesIterator != piramides.end())
    {
        piramidesIterator->drawPiramide();
        piramidesIterator++;
    }
}
// XXX piramide
//  Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
    if (primitive == POINT)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.9 * height, 0.1 * width, height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.9 * height, 0.1 * width, height);

    // Draw point icon.
    glPointSize(pointSize);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(0.05 * width, 0.95 * height, 0.0);
    glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
    if (primitive == LINE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw line icon.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.025 * width, 0.825 * height, 0.0);
    glVertex3f(0.075 * width, 0.875 * height, 0.0);
    glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
    if (primitive == RECTANGLE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw rectangle icon.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
    glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawCircleSelectionBox(void)
{
    if (primitive == CIRCLE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw rectangle icon.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
    glEnd();
}
// Function to draw rectangle selection box in left selection area.
void drawPiramideSelectionBox(void)
{
    if (primitive == PIRAMIDE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

    // Draw rectangle icon.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.015 * width, 0.62 * height, 0.0);
    glVertex3f(0.08 * width, 0.62 * height, 0.0);
    glVertex3f(0.05 * width, 0.68 * height, 0.0);
    glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
    // desenha o inicial da piramide

    glColor3f(1.0, 1.0, 0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(xini, yini, 0.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glVertex3f(tempX, tempY, 0.0);
    glEnd();
    // pra piramide
    // glVertex3f(tX2, tY2, 0.0);

    // desenha os pontos temporarios

    vector<Point>::iterator pontosI;
    pontosI = pontoPiramideTemp.begin();
    while (pontosI != pontoPiramideTemp.end())
    {
        // piramidesIterator->drawPiramide();
        // glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
        pontosI->drawPoint();
        pontosI++;
    }

    // desenhar as linhas da base da piramide
    if (pointCount > 0)
    {
        glBegin(GL_LINES);
        // glColor3f(0.0, 1.0, 0.0);
        pontosI = pontoPiramideTemp.begin();
        // para começar pega o primeiro ponto
        int xant = pontosI->getX(), yant = pontosI->getY();
        pontosI++;
        while (pontosI != pontoPiramideTemp.end())
        {
            // piramidesIterator->drawPiramide();
            glVertex3f(xant, yant, 0.0f);
            glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
            xant = pontosI->getX();
            yant = pontosI->getY();
            pontosI++;
        }
        glEnd();

        // volta pro ultimo ponto
        pontosI = pontoPiramideTemp.end();
        /// funcionou com isso daqui, nao sei como kkkkkk
        pontosI--;
        glBegin(GL_LINES);

        // desenha uma linha ate o mouse
        glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
        glVertex3f(MouseX, MouseY, 0);
        glEnd();
        // sem clicar
    }

    if (topo)
    {
        // desenha a ultima linha que fecha o loop da base da piramide
        glBegin(GL_LINES);

        pontosI = pontoPiramideTemp.begin();
        glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);

        pontosI = pontoPiramideTemp.end();
        pontosI--;
        glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
        //glEnd();

        // puxa uma linha de cada extremidade do retangulo para a ponta da piramide
        //glBegin(GL_LINES);
        // vector<Point>::iterator pontosI;
        pontosI = pontoPiramideTemp.begin();
        while (pontosI != pontoPiramideTemp.end())
        {
            // piramidesIterator->drawPiramide();
            glVertex3f(pontosI->getX(), pontosI->getY(), 0.0f);
            // TODO registrar altura
            glVertex3f(MouseX, MouseY, 1);
            pontosI++;
            // std::cout << "Passive Mouse Position: X=" << x << " Y=" << y << std::endl;
        }
        glEnd();
    }
    // avisa o glut que a tela mudou
    glutPostRedisplay();
}

// Function to draw a grid.
void drawGrid(void)
{
    int i;

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);
    glColor3f(0.75, 0.75, 0.75);

    glBegin(GL_LINES);
    for (i = 2; i <= 9; i++)
    {
        glVertex3f(i * 0.1 * width, 0.0, 0.0);
        glVertex3f(i * 0.1 * width, height, 0.0);
    }
    for (i = 1; i <= 9; i++)
    {
        glVertex3f(0.1 * width, i * 0.1 * height, 0.0);
        glVertex3f(width, i * 0.1 * height, 0.0);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    // para nao ficar rodando pra sempre
    // TODO nao funcionou ainda
    glLoadIdentity();

    drawPointSelectionBox();
    drawLineSelectionBox();
    drawRectangleSelectionBox();
    drawCircleSelectionBox();
    drawPiramideSelectionBox();
    drawInactiveArea();

    // rotacionar cena

    // Rotate scene.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    drawPoints();
    drawLines();
    drawRectangles();
    drawPiramides();
    if (((primitive == LINE) || (primitive == RECTANGLE)) &&
        (pointCount == 1))
        drawTempPoint();

    // tambem desenha nas piramides
    if ((primitive == PIRAMIDE))
        drawTempPoint();
    if (isGrid)
        drawGrid();

    glutSwapBuffers();
}

// XXX
//  Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
    printf("%d", primitive);
    if (y < (1 - NUMBERPRIMITIVES * 0.1) * height)
        primitive = INACTIVE;
    else if (y < (1 - 3 * 0.1) * height)
        primitive = PIRAMIDE;
    else if (y < (1 - 2 * 0.1) * height)
        primitive = RECTANGLE;
    else if (y < (1 - 1 * 0.1) * height)
        primitive = LINE;
    else
        primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        y = height - y; // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if (x < 0 || x > width || y < 0 || y > height)
            ;

        // Click in left selection area.
        else if (x < 0.1 * width)
        {
            pickPrimitive(y);
            pointCount = 0;
        }

        // Click in canvas.
        else
        {
            if (primitive == POINT)
                points.push_back(Point(x, y));
            else if (primitive == LINE)
            {
                if (pointCount == 0)
                {
                    tempX = x;
                    tempY = y;
                    pointCount++;
                }
                else
                {
                    lines.push_back(Line(tempX, tempY, x, y));
                    pointCount = 0;
                }
            }
            else if (primitive == RECTANGLE)
            {
                if (pointCount == 0)
                {
                    tempX = x;
                    tempY = y;
                    pointCount++;
                }
                else
                {
                    rectangles.push_back(Rectangle(tempX, tempY, x, y));
                    pointCount = 0;
                }
            }
            else if (primitive == PIRAMIDE)
            {
                printf("x %d y %d", x, y);
                fflush(stdout);

                if (pointCount == 0)
                {
                    pontoPiramideTemp.push_back(Point(x, y));
                    xini = x;
                    yini = y;
                    pointCount++;
                }
                else if (pointCount > 0)
                {
                    if (topo == 1)
                    {
                        // vai colocar o topo
                        piramides.push_back(Piramide(pontoPiramideTemp, 1, x, y));
                        pointCount = 0;
                        xini = 0;
                        yini = 0;
                        // limpa todos os pontos temporarios
                        pontoPiramideTemp.clear();
                        topo = 0;
                    }
                    // para finalizar a piramide precisa ter ao menos 3 vertices
                    else if ((x < (xini + 5)) && (x > (xini - 5)) && (y < (yini + 5)) && (y > (yini - 5)) && (pointCount > 2))
                    {
                        printf("detrno");
                        fflush(stdout);
                        // se finaliza a piramide
                        //[ ] altura 5 por enquanto pq eu nao sei como vai ser inserida
                        //[ ] por enquanto o topo vai ser no centro, entao manda 0
                        topo = 1;
                        // pontoPiramideTemp.push_back(Point(x, y));
                    }
                    else
                    {
                        // TODO testar
                        pontoPiramideTemp.push_back(Point(x, y));
                        // tX2 = x;
                        // tY2 = y;
                        pointCount++;
                    }
                }
            }
        }
    }
    glutPostRedisplay();
}

// Callback for mouse movement when NO buttons are pressed
void processPassiveMotion(int x, int y)
{

    y = height - y; // Correct from mouse to OpenGL co-ordinates.
    // sem clicar
    MouseX = x;
    MouseY = y;
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

    // Set viewing box dimensions equal to window dimensions.
    glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

    // Pass the size of the OpenGL window to globals.
    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;

    case 'x':
        Xangle += 1.0;
        if (Xangle > 360.0)
            Xangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'X':
        Xangle -= 1.0;
        if (Xangle < 0.0)
            Xangle += 360.0;
        glutPostRedisplay();
        break;
    case 'y':
        Yangle += 1.0;
        if (Yangle > 360.0)
            Yangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Y':
        Yangle -= 1.0;
        if (Yangle < 0.0)
            Yangle += 360.0;
        glutPostRedisplay();
        break;
    case 'z':
        Zangle += 1.0;
        if (Zangle > 360.0)
            Zangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Z':
        Zangle -= 1.0;
        if (Zangle < 0.0)
            Zangle += 360.0;
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
    points.clear();
    lines.clear();
    rectangles.clear();
    primitive = INACTIVE;
    pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
    if (id == 1)
    {
        clearAll();
        glutPostRedisplay();
    }
    if (id == 2)
        exit(0);
    if (id == 3)
        system("xdg-open https://www.youtube.com/shorts/ObPCjyqfVjo");
    // TODO tirar isso no final
}

// The sub-menu callback function.
void grid_menu(int id)
{
    if (id == 3)
        isGrid = 1;
    if (id == 4)
        isGrid = 0;
    glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
    int sub_menu;
    sub_menu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", sub_menu);
    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Quit", 2);
    // TODO tirar isso no final
    glutAddMenuEntry("Tomar no cu", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Left click on a box on the left to select a primitive." << endl
         << "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
         << "Right click for menu options." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("canvas.cpp");
    setup();
    glutPassiveMotionFunc(processPassiveMotion);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);

    makeMenu(); // Create menu.

    glutMainLoop();

    return 0;
}