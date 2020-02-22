/* 
 * Arquivos: main.c, struct.c, labirinto.obj, labirinto.mtl, personal.obj, personal.mtl, makefile
 * Autor: Daniel Martins Reis
 * 
 * Sistema Operacional utilizado: Linux Mint 18.3
 * 
 * Instalacao openGL em Linux:
 *      sudo apt-get update
 *      sudo apt-get install freeglut3 freeglut3-dev binutils-gold g++ cmake libglew-dev mesa-common-dev build-essential libglew1.5-dev libglm-dev
 * 
 * Compilacao em Linux:
 *      g++ main.c -lglut -lGL -lGLEW -lGLU -o labirinto
 *                          ou
 *                  make all
 * 
 * Execucao em Linux:
 *      ./labirinto
 * 
 * Intruções:
 *      Seta para cima: rotaciona camera pra frente
 *      Seta para baixo: rotaciona camera pra trás
 *      Seta para direita: rotaciona camera pra direita
 *      Seta para esquerda: rotaciona camera pra esquerda
 * 
 *      PgUp: move camera pra frente
 *      PgDn: move camera pra tras
 *      Home: move camera pra cima
 *      End: move camera pra baixo
 * 
 *      w: move personagem e camera pra frente
 *      s: move personagem e camera pra tras
 *      d: move personagem e camera pra direita
 *      a: move personagem e camera pra esquerda
 * 
 *      Esc: sai do jogo
 */

/*********************************************** BIBLIOTECAS ************************************************/
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "struct.c"

/****************************************** DECLARACAO DAS FUNCOES ******************************************/
void initGL(GLvoid);                                    /* inicializacao openGL */
void draw(GLvoid);                                      /* desenha os objetos */
void reshape(int largura, int altura);                  /* configuracoes para a janela */
void keyboard(unsigned char tecla, int x, int y);       /* manipula acoes com teclas */
void keyboardSpecial(int tecla, int x, int y);          /* manipula acoes com teclas especiais */
void setPosition(void);                                 /* posiciona */

/************************************* VARIAVEIS GLOBAIS E CONSTANTES ***************************************/
#define ESC 27
char filenameLabirintoOBJ [30] = "labirinto.obj", filenamePersonalOBJ [30] = "personal.obj";
char filenameLabirintoMLT [30] = "labirinto.mtl", filenamePersonalMTL [30] = "personal.mtl";
OBJ objLabirinto, objPersonal;

unsigned char teclaLast = ' ', teclaPenult = ' ', teclaAntPenult = ' ';    /* Ultima e penultima tecla pressionada */

int frame;
float xCamera = -215.0f, zCamera = -115.0f, rotUpDownCamera = 0.0f, posUpDownCamera = 0.0f;   /* Variaveis de controle da camera */

/* z -> frente/tras, y -> cima/baixo, x -> direita/esquerda */
float xPersonal = -215.0f, yPersonal = 0.0f, zPersonal = -120.0f;           /* Variaveis de controle do personagem */
int rotCamera = 180, rotPersonal = 0;                                       /* Rotacao da camera e do personagem */
float piover180 = M_PI / 180;                                               /* Constante PI/180 usada para converter o angulo em radiano para funcao de seno e cosseno */

int rotMessage = 0;
char message[] = "Fim de jogo! Voce ganhou!";

/* Definicao de vetores de cor, intensidade e posicao de fontes de luz */
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightAmbient[] = {0.2f, 0.2f, 0.4f, 1.0f};
GLfloat lightEmissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPos1[] = {180.0f, 100.0f, -60.0f, 1.0f};
GLfloat lightPos2[] = {180.0f, 100.0f, 50.0f, 1.0f};
GLfloat lightPos3[] = {-260.0f, 100.0f, -60.0f, 1.0f};
GLfloat lightPos4[] = {-260.0f, 100.0f, 50.0f, 1.0f};

/********************************************** DESENHA NA TELA **********************************************/
/* Funcao que desenha o labirinto */
void drawOBJLabirinto(void)
{
    setPosition();
  
    F *faux;            /* Cria uma celula auxiliar para percorrer a lista */
        
    /* Percorre a lista de faces */
    for(faux = objLabirinto.flistinit->prox; faux != NULL; faux = faux->prox)
    {
        /* Pega os vertices, normal e textura */
        V v1 = getV(objLabirinto, faux->vi1);
        V v2 = getV(objLabirinto, faux->vi2);
        V v3 = getV(objLabirinto, faux->vi3);
        VN n = getVN(objLabirinto, faux->ni1);
        VT vt1 = getVT(objLabirinto, faux->uvi1);
        VT vt2 = getVT(objLabirinto, faux->uvi2);
        VT vt3 = getVT(objLabirinto, faux->uvi3);
        
        /* Chamadas de funções OpenGL para desenho */
        glBegin(GL_TRIANGLES);
        
        Mtl mtl = getMtl(objLabirinto, faux->texture);
        /* Insere textura do .mtl */
        GLfloat ka[] = {mtl.Ka.r, mtl.Ka.g, mtl.Ka.b}; /* Ka */
        GLfloat kd[] = {mtl.Kd.r, mtl.Kd.g, mtl.Kd.b}; /* Kd */
        GLfloat ks[] = {mtl.Ks.r, mtl.Ks.g, mtl.Ks.b}; /* Ks */
            
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
        
        glColor3f(mtl.Kd.r, mtl.Kd.g, mtl.Kd.b);
        glNormal3f(n.x, n.y, n.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

/* Funcao que desenha o personagem */
void drawOBJPersonal(void)
{
    glTranslatef(xPersonal, yPersonal, zPersonal);
    glRotatef(rotPersonal, 0.0f, 1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
  
    F *faux;            /* Cria uma celula auxiliar para percorrer a lista */
    
    /* Percorre a lista de faces */
    for(faux = objPersonal.flistinit->prox; faux != NULL; faux = faux->prox)
    {
        /* Pega os vertices, normal e textura */
        V v1 = getV(objPersonal, faux->vi1);
        V v2 = getV(objPersonal, faux->vi2);
        V v3 = getV(objPersonal, faux->vi3);
        VN n = getVN(objPersonal, faux->ni1);
        VT vt1 = getVT(objPersonal, faux->uvi1);
        VT vt2 = getVT(objPersonal, faux->uvi2);
        VT vt3 = getVT(objPersonal, faux->uvi3);
        
        /* Chamadas de funções OpenGL para desenho */
        glBegin(GL_TRIANGLES);

        Mtl mtl = getMtl(objPersonal, faux->texture);
        /* Insere textura do .mtl */
        GLfloat ka[] = {mtl.Ka.r, mtl.Ka.g, mtl.Ka.b}; /* Ka */
        GLfloat kd[] = {mtl.Kd.r, mtl.Kd.g, mtl.Kd.b}; /* Kd */
        GLfloat ks[] = {mtl.Ks.r, mtl.Ks.g, mtl.Ks.b}; /* Ks */
            
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
        
        glColor3f(mtl.Kd.r, mtl.Kd.g, mtl.Kd.b);
        glNormal3f(n.x, n.y, n.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

/* Move a mensagem de que ja ganhou */
void moveMessage(void)
{
    rotMessage -= 2;
    glutPostRedisplay();
}

/* Desenha a mensagem de que ja ganhou */
void displayMessage(void)
{
    int len, i;

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotatef(rotMessage, 0.0f, 0.0f, 1.0f);
    glTranslatef(-750.0f, 0.0f, 0.0f);
    len = (int) strlen(message);
    for (i = 0; i < len; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, message[i]);
    glPopMatrix();
    glutSwapBuffers();
}

/* Funcao que informa se o jogador ganhou */
void verifyWin(void)
{   
    /* Se o jogador esta no quadrao -> informa mensagem */
    if (xPersonal >= 44 && xPersonal <= 90 && zPersonal <= -89 && zPersonal >= -119)
    {
        glutDestroyWindow(frame);   /* Fecha a janela */

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(600, 600);
        frame = glutCreateWindow("Computação Gráfica - OpenGL - Labirinto 3D - Daniel Reis");
        glutFullScreen();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 2000, 0, 2000);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(3.0);
        glTranslatef(1000, 1000, 0);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glColor3f(1.0, 1.0, 1.0);
        glutDisplayFunc(displayMessage);
        glutIdleFunc(moveMessage);
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(keyboardSpecial);
        glutMainLoop();
    }
}

void setPosition(void)
{
    glLoadIdentity();
    glRotatef(rotUpDownCamera, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotCamera, 0.0f, 1.0f, 0.0f);
    glTranslatef(-xCamera, posUpDownCamera, -zCamera);
}

/* Funcao de inicializacao do sistema */
void initGL(void)
{
    glutSetCursor(GLUT_CURSOR_NONE);                      /* Esconde o Cursor do mouse */
    glShadeModel(GL_SMOOTH);                              /* Tipo de Sombreamento Suavizado */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                 /* Cor de Limpeza de tela */

    glClearDepth(1.0f);                                   /* Inicialização do Buffer de profundidade */
    glEnable(GL_DEPTH_TEST);                              /* Habilita tipo de teste para o buffer */
    glDepthFunc(GL_LEQUAL);                               /* Define tipo de teste de profundidade */

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    /* Tipo de Pespectiva */
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);      /* Desenhar os dois lados de um objeto */

    /* Definicao de parametros de luz */
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightDiffuse);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);
}

/* Funcao de desenho dos objetos */
void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  /* Limpa a tela e o buffer */
    glLoadIdentity();
    setPosition();
    
    /* Insere iluminacao */
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos2);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos3);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos4);
    
    /* Local de fim */
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(44.0f, 0.0f, -89.0f);
    glVertex3f(44.0f, 0.0f, -119.0f);
    glVertex3f(90.0f, 0.0f, -119.0f);
    glVertex3f(90.0f, 0.0f, -89.0f);
    glEnd();

    drawOBJLabirinto();     /* Desenha labirinto */
    setPosition();          /* Posiciona */
    drawOBJPersonal();      /* Desenha personagem */
    glutSwapBuffers();      /* Troca os buffers de desenho e de exibicao */
    verifyWin();            /* Verifica se ja ganhou */
    
//  printf("xCamera = %f\tzCamera = %f\trotUpDownCamera = %f\tposUpDownCamera = %f\t", xCamera, zCamera, rotUpDownCamera, posUpDownCamera);
//  printf("xPersonal = %f\tyPersonal = %f\tzPersonal = %f\n", xPersonal, yPersonal, zPersonal);
}

/* Funcao de redesenho da janela */
void reshape(int largura, int altura)
{
    if (!altura)
        altura = 1;
    glViewport(0, 0, largura, altura);                    /* Determina as coordenadas da ViewPort */
    glMatrixMode(GL_PROJECTION);                          /* Seta proxima operacao para a pilha de projecoes */
    glLoadIdentity();                                     /* Reseta a matriz de projecao */
    gluPerspective(60, (GLfloat)largura/(GLfloat)altura, 1, 600);
    glMatrixMode(GL_MODELVIEW);                           /* Seta proxima operacao para a pilha de modelos */
    glLoadIdentity();                                     /* Resete a matriz de modelos de projecao */
}

/************************************************* MOVIMENTA *************************************************/
/* Funcao de controle de teclas comuns */
void keyboard(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        case 'd':
            /* Move camera */
            rotPersonal -= 90.0f;     /* Rotaciona personagem pra direita */
            rotCamera -= 90.0f;       /* Rotaciona camera pra direita */
            if (teclaLast == 'd' && teclaPenult == 'd' && teclaAntPenult == 'd')
            {
                zPersonal -= 5.0f;
                xPersonal += 5.0f;
            }
            else if (teclaLast == 'd' && teclaPenult == 'd')
            {
                zPersonal -= 5.0f;
                xPersonal -= 5.0f;
            }
            else if (teclaLast != 'd')
            {
                zPersonal += 5.0f;
                xPersonal += 5.0f;
            }
            else
            {
                zPersonal += 5.0f;
                xPersonal -= 5.0f;
            }
            break;
        case 'a':
            rotPersonal += 90.0f;     /* Rotaciona personagem pra esquerda */
            rotCamera += 90.0f;       /* Rotaciona camera pra esquerda */
            if (teclaLast == 'a' && teclaPenult == 'a' && teclaAntPenult == 'a')
            {
                zPersonal -= 5.0f;
                xPersonal -= 5.0f;
            }
            else if (teclaLast == 'a' && teclaPenult == 'a')
            {
                zPersonal -= 5.0f;
                xPersonal += 5.0f;
            }
            else if (teclaLast != 'a')
            {
                zPersonal += 5.0f;
                xPersonal -= 5.0f;
            }
            else
            {
                zPersonal += 5.0f;
                xPersonal += 5.0f;
            }
            break;
        case 'w':
            /* Move personagem pra frente */
            xPersonal += (float) sin (rotPersonal * piover180) * 2.0f;
            zPersonal += (float) cos (rotPersonal * piover180) * 2.0f;
            /* Move camera pra frente */
            xCamera -= (float) sin (rotCamera * piover180) * 2.0f;
            zCamera -= (float) cos (rotCamera * piover180) * 2.0f;
            break;
        case 's':
            /* Move personagem pra tras */
            xPersonal -= (float) sin (rotPersonal * piover180) * 2.0f;
            zPersonal -= (float) cos (rotPersonal * piover180) * 2.0f;
            /* Move camera pra tras */
            xCamera += (float) sin (rotCamera * piover180) * 2.0f;
            zCamera += (float) cos (rotCamera * piover180) * 2.0f;
            break;
        /* Finaliza */
        case ESC:
            glutDestroyWindow(frame);
            exit(0);
            break;
    }
    teclaAntPenult = teclaPenult;
    teclaPenult = teclaLast;
    teclaLast = tecla;
}

/* Funcao de controle de teclas especiais*/
void keyboardSpecial(int tecla, int x, int y)
{
    switch(tecla)
    { 
        case GLUT_KEY_RIGHT:        /* Rotaciona camera pra direita */
            rotCamera -= 5.0f;
            break;
        case GLUT_KEY_LEFT:         /* Rotaciona camera para esquerda */
            rotCamera += 5.0f;
            break;
        case GLUT_KEY_PAGE_UP:      /* Rotaciona camera para cima */
            rotUpDownCamera -= 5.0f;
            break;
        case GLUT_KEY_PAGE_DOWN:    /* Rotaciona camera para baixo */
            rotUpDownCamera += 5.0f;
            break;
        case GLUT_KEY_UP:           /* Move camera pra frente */
            xCamera -= (float) sin (rotCamera * piover180) * 2.0f;
            zCamera -= (float) cos (rotCamera * piover180) * 2.0f;
            break;
        case GLUT_KEY_DOWN:         /* Move camera pra tras */
            xCamera += (float) sin (rotCamera * piover180) * 2.0f;
            zCamera += (float) cos (rotCamera * piover180) * 2.0f;
            break;
        case GLUT_KEY_HOME:         /* Move camera pra cima */
            posUpDownCamera -= 5.0f;   
            break;
        case GLUT_KEY_END:          /* Move camera pra baixo */
            posUpDownCamera += 5.0f;
            break;
    }
}

/************************************************** MAIN ****************************************************/
int main(int argc, char* argv[])
{
    createOBJEmpty(&objLabirinto);                                              /* Cria o obj vazio */
    createOBJEmpty(&objPersonal);                                               /* Cria o obj vazio */
    loadOBJ(filenameLabirintoOBJ, filenameLabirintoMLT, &objLabirinto);         /* Carrega labirinto */
    loadOBJ(filenamePersonalOBJ, filenamePersonalMTL, &objPersonal);            /* Carrega personagem */
//  printOBJ(objLabirinto);                                                     /* Imprime o labirinto */
//  printOBJ(objPersonal);                                                      /* Imprime o personagem */
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    frame = glutCreateWindow("Computação Gráfica - OpenGL - Labirinto 3D - Daniel Reis");
    glutFullScreen();
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutIdleFunc(draw);
    initGL();
    glutMainLoop();
    return 0;
}
