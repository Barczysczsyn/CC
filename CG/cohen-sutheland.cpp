/*
 * Implementacao do algoritmo de Cohen-Sutherland
 *      para o recorte de segmentos de reta
 *
 * Implementacao baseada no livro:
 * Computer Graphics: Principles and Practice
 *
 * COMANDOS:
 *          r - recorta
 *          espaco - apaga vertices e linha
 *
 */

// diretivas de compilacao condicional
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// macro que define numero de vertices
#define N_VERTEX 2
// macros que definem as posicoes das regioes
#define TOP 8
#define BOTTOM 4
#define LEFT 1
#define RIGHT 2
// macros que definem os limites para a regiao 0000
#define X_MAX 0.5  // xmax
#define X_MIN -0.5 // xmin
#define Y_MAX 0.5  // ymax
#define Y_MIN -0.5 // ymin

GLint currentVertex = 0; // Vertice corrente
GLdouble modelview[16];  // matriz modelview
GLdouble projection[16]; // matriz projecao
/*
    Dados do vetor viewport:
    viewport[0]=x
    viewport[1]=y
    viewport[2]=width
    viewport[3]=height
*/
GLint viewport[4];
// variaveis utilizadas para pegar as coordenadas do mouse
GLfloat winX, winY, winZ;
GLdouble posX, posY, posZ;
// Matriz que guarda as coordenadas dos vertices
GLfloat vertexes[6][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0}};
typedef int outcode;
outcode code;

// Desenha pontos
void drawPoints()
{
    glPointSize(5.0); // Definindo expessura do ponto
    glBegin(GL_POINTS);
    if (currentVertex > 0)
    {
        for (int i = 0; i < currentVertex; i++)
            glVertex3fv(&vertexes[i][0]); // desenha
    }
    glEnd();
}

// verifica posicao
outcode
CompOutCode(double x, double y)
{
    code = 0; // dentro de 0000

    if (y > Y_MAX)
    {
        code = TOP; // topo
    }
    else
    {
        if (y < Y_MIN)
        {
            code = BOTTOM; // baixo
        }
        if (x > X_MAX)
        {
            code = RIGHT; // direita
        }
        if (x < X_MIN)
        {
            code = LEFT; // esquerda
        }
    }

    return code;
}

// Algoritmo de Cohen-Sutherland
void CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax)
{
    bool accept, done;                      // variaveis de verificacao
    outcode outcode0, outcode1, outcodeOut; // declaracao das variaveis do tipo outcode

    accept = done = false; // inicializando variaveis de verificacao

    outcode0 = CompOutCode(x0, y0); // pega posicao a qual o ponto1 se encontra
    outcode1 = CompOutCode(x1, y1); // pega posicao a qual o ponto2 se encontra

    while (!done)
    {
        if (!outcode0 && !outcode1)
        {
            accept = done = true; // aceita e sai do loop
        }
        else
        {
            if (outcode0 & outcode1)
            {                      // quanto esta totalmente fora de  0000
                done = true;       // termina o loop
                currentVertex = 0; // zera a quantidade de vertices
            }
            else
            {
                // Houve falha em ambos dos testes, entao e' calculado o segmento da linha
                // a partir de um ponto fora de uma intersecção com a borda do retangulo.

                // Pelo menos um ponto de extremidade está fora do retângulo, aqui e' pego esse ponto
                outcodeOut = outcode0 ? outcode0 : outcode1;

                double x, y;
                // Agora achando o ponto de interseccao
                if (outcodeOut & TOP)
                { // Divide linha ao topo do retangulo
                    x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                    y = ymax;
                }
                else
                {
                    if (outcodeOut & BOTTOM)
                    { // Divide linha na parte de baixo do retangulo
                        x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                        y = ymin;
                    }
                    else
                    {
                        if (outcodeOut & RIGHT)
                        { // Divide linha na borda direita do retangulo
                            y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                            x = xmax;
                        }
                        else
                        {
                            if (outcodeOut & LEFT)
                            { // Divide linha na borda esquerda do retangulo
                                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                                x = xmin;
                            }
                        }
                    }
                }

                // Agora movemos o ponto que esta fora p/ a interseccao do retangulo
                if (outcodeOut == outcode0)
                {
                    x0 = x;
                    y0 = y;
                    outcode0 = CompOutCode(x0, y0);
                }
                else
                {
                    x1 = x;
                    y1 = y;
                    outcode1 = CompOutCode(x1, y1);
                }
            }
        }
    }

    if (accept)
    {                        // Atribui novos valores para os vertices
        vertexes[0][0] = x0; // Coordenada x do ponto 1
        vertexes[0][1] = y0; // Coordenada y do ponto 1
        vertexes[1][0] = x1; // Coordenada x do ponto 2
        vertexes[1][1] = y1; // Coordenada y do ponto 2
    }
}

// Imprime strings com o codigo de 4 bits de cada regiao
// parametros:   float posStrX: posicao x da string
//               float posStrY: posicao y da string
//               const char *str: string a ser exibida
void printStrings(float posStrX, float posStrY, const char *str)
{
    // mudando cor da linha para verde
    glColor3f(0.0, 1.0, 0.0);
    // posicionando string na tela
    glRasterPos2f(posStrX, posStrY);

    // String exibida na tela
    char *p = (char *)str; // atribui da variavel *p

    while (*p != '\0')
    { // percorre string ate o final
        // funcao da glut que imprime char por char na tela
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
    }
}

// Desenha linhas que delimitam as regioes
void drawRegionLines()
{
    glBegin(GL_LINES);
    // mudando cor da linha para branco
    glColor3f(1.0, 1.0, 1.0);

    // desenha linhas das regioes
    for (float i = -0.5; i <= 0.5; i = i + 1)
    {
        glVertex2f(-1, i); // coordenada inicial da linha
        glVertex2f(1, i);  // coordenada final da linha
    }

    // desenha linhas das regioes
    for (float i = -0.5; i <= 0.5; i = i + 1)
    {
        glVertex2f(i, -1); // coordenada inicial da linha
        glVertex2f(i, 1);  // coordenada final da linha
    }
    glEnd();
}

// linha entre os vertices e' desenhada aqui
void drawLineBetweenVertexes()
{
    glBegin(GL_LINE_STRIP);
    // Setando a cor da linha para amarelo
    glColor3f(1.0f, 1.0f, 0.0f);
    // se a quantidade de vertices for satisfeita
    if (currentVertex == N_VERTEX)
    { // entao as linhas entre os vertices sao desenhadas
        for (int i = 0; i < N_VERTEX; i++)
            glVertex3fv(&vertexes[i][0]); // a linha e' desenhada com a funcao glVerte3fv
    }
    glEnd();
}

// rotina callback de display OpenGL
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define valores p/ glClear
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Mudando a cor para vermelho
    //          R     G     B
    glColor3f(1.0f, 0.0f, 0.0f);

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Aqui sao desenhados os pontos(mostrados a partir do clique do mouse)
    drawPoints();

    // linha entre os vertices e' desenhada aqui
    drawLineBetweenVertexes();

    printStrings(-0.8, 0.7, "1001");   // imprimindo string 1001
    printStrings(-0.05, 0.7, "1000");  // imprimindo string 1000
    printStrings(0.7, 0.7, "1010");    // imprimindo string 1010
    printStrings(-0.8, 0.0, "0001");   // imprimindo string 0001
    printStrings(-0.05, 0.0, "0000");  // imprimindo string 0000
    printStrings(0.7, 0.0, "0010");    // imprimindo string 0010
    printStrings(-0.8, -0.8, "0101");  // imprimindo string 0101
    printStrings(-0.05, -0.8, "0100"); // imprimindo string 0100
    printStrings(0.7, -0.8, "0110");   // imprimindo string 0110

    drawRegionLines(); // Desenha linhas que delimitam as regioes
    glutSwapBuffers(); // Fazendo troca de Buffers
}

// Funcao callback p/ teclado
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:     // se pressionar ESC
        exit(0); // sai do programa
        break;

    case 'r': // Faz chamada ao algoritmo Cohen-Sutherland para recorte
        CohenSutherlandLineClipAndDraw(vertexes[0][0], vertexes[0][1], vertexes[1][0], vertexes[1][1], X_MIN, X_MAX, Y_MIN, Y_MAX);
        glutPostRedisplay(); // faz redisplay
        break;

    case ' ':                // comando que apaga linhas e vertices
        currentVertex = 0;   // zera quantidade de vertices
        glutPostRedisplay(); // faz redisplay
        break;
    }
}

// funcao callback para mouse
void mouse(int button, int state, int x, int y)
{
    // caso botao do mouse seja pressionado
    if (state == GLUT_DOWN)
    {
        // passando para coordenadas do mundo real
        winX = (float)x;                      // passando o valor de x por cast, pois a variavel winX e' do tipo GLfloat
        winY = (float)viewport[3] - (float)y; // viewport[3] e y sao inteiros, por isso sao passados por cast p/ winY que e' GLfloat
        // funcao que pega as coordenadas de x, y
        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
        // guardando a posicao de x quando pressiona o botao direito do mouse
        vertexes[currentVertex][0] = posX;
        // guardando a posicao de y quando pressiona o botao direito do mouse
        vertexes[currentVertex][1] = posY;
        // incrementa a quantidade de vertices para o desenho dos pontos do
        // vertice e para o desenho da linha
        if (currentVertex < N_VERTEX)
        {
            currentVertex++; // incrementando
        }
        // fazendo redisplay
        glutPostRedisplay();
    }
}

// Funcao principal
int main(int argc, char *argv[])
{
    // inicializando glut
    glutInit(&argc, argv);
    // definindo tamanho da tela
    glutInitWindowSize(800, 600);
    // definindo posicao inicial da janela
    glutInitWindowPosition(100, 15);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    // definindo nome da janela
    glutCreateWindow("Algoritmo Cohen-Sutherland");
    // rotina callback de display OpenGL
    glutDisplayFunc(renderScene);
    // Rotina callback para teclado
    glutKeyboardFunc(keyboard);
    // rotina callback para o mouse
    glutMouseFunc(mouse);
    // loop principal do glut
    glutMainLoop();

    return EXIT_SUCCESS;
}
