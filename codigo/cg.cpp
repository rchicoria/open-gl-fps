/*
 *	Autores:
 *
 *	Ricardo Pinto Lopes 2008114843
 *	Rui Pedro da Cunha Santos Chicória 2008115099
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "RgbImage.hpp"

#define PERSPECTIVE 0
#define MAP 1

/*
 *	Variáveis e constantes globais
 */
// Geral
GLfloat xC = 1.0, yC = 1.0, zC = 1.0;
GLint wScreen = 800;
GLint hScreen = 600;
GLint msec = 10;

// Câmara
GLfloat anguloH = 0;
GLfloat anguloV = 0;
GLfloat obsP[] = {0, 0.3, 1};
GLint lastx = wScreen/2;
GLint lasty = hScreen/2;

// Controlos
bool frente = false;
bool direita = false;
bool atras = false;
bool esquerda = false;
GLfloat vel = 0.02;

// Luz ambiente
GLfloat colorAmbient[4] = {0.1,0.1,0.1,1};
GLfloat localCor[4] ={0.4, 0.4, 0.4, 1.0};
GLfloat localPos[4] ={0, 1.0, 0, 1.0};
GLfloat localAttCon =1.0;
GLfloat localAttLin =0.05;
GLfloat localAttQua =0.0;

// Texturas
GLuint texture[10];
RgbImage imag;

// Nevoeiro
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };
GLuint fogfilter = 1;					// Which Fog To Use
GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};		// Fog Color

/*
 *	Cria as várias texturas
 */
void textures()
{
	// Parede
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/parede.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());	
	
	// Chão
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/chao.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

/*
 *	Inicialização dos parâmetros
 */
void init(void)
{
	glutSetCursor(GLUT_CURSOR_NONE); 
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, localCor);
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    textures();
	
	// Nevoeiro
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 3.0f);
	glFogf(GL_FOG_END, 10.0f);
	glEnable(GL_FOG);
}

/*
 *	Função para adaptar a imagem aos redimensionamentos da janela
 */
void resizeWindow(GLsizei w, GLsizei h)
{
 	wScreen = w;
	hScreen = h;
	glutPostRedisplay();
}

/*
 *	Preenche o cenário com os vários objectos
 */
void cenario(int view)
{
	// Objectos para teste
	glPushMatrix();
		glColor4f(0.0, 1.0, 0.0, 1.0);
		glTranslatef(0, 0.5, 0);
		glutSolidTeapot(0.1);
	glPopMatrix();
	glPushMatrix();
		glColor4f(0.0, 1.0, 1.0, 1.0);
		glTranslatef(-0.2, 1, 0.15);
        glRotatef(0,0,1,0);
		glutSolidSphere(0.05, 100, 100);
	glPopMatrix();
	
	// Parede z = 20
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -2*xC, 0, -2*xC); 
			glTexCoord2f(3.0f,0.0f); glVertex3i( 2*xC, 0, -2*xC); 
			glTexCoord2f(3.0f,3.0f); glVertex3i( 2*xC, yC*4, -2*xC); 
			glTexCoord2f(0.0f,3.0f); glVertex3i( -2*xC, yC*4, -2*xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// Parede z = -20
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -2*xC, 0, 2*xC); 
			glTexCoord2f(3.0f,0.0f); glVertex3i( 2*xC, 0, 2*xC); 
			glTexCoord2f(3.0f,3.0f); glVertex3i( 2*xC, yC*4, 2*xC); 
			glTexCoord2f(0.0f,3.0f); glVertex3i( -2*xC, yC*4, 2*xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// Parede x = 20
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 2*xC, 0, -2*xC); 
			glTexCoord2f(3.0f,0.0f); glVertex3i( 2*xC, 0, 2*xC); 
			glTexCoord2f(3.0f,3.0f); glVertex3i( 2*xC, yC*4, 2*xC); 
			glTexCoord2f(0.0f,3.0f); glVertex3i( 2*xC, yC*4, -2*xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// Parede x = -20
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -2*xC, 0, -2*xC); 
			glTexCoord2f(3.0f,0.0f); glVertex3i( -2*xC, 0, 2*xC); 
			glTexCoord2f(3.0f,3.0f); glVertex3i( -2*xC, yC*4, 2*xC); 
			glTexCoord2f(0.0f,3.0f); glVertex3i( -2*xC, yC*4, -2*xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// Chão
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( -2*xC, 0, -2*xC ); 
			glTexCoord2f(10.0f,0.0f); glVertex3i( 2*xC, 0, -2*xC ); 
			glTexCoord2f(10.0f,10.0f); glVertex3i( 2*xC, 0, 2*xC); 
			glTexCoord2f(0.0f,10.0f); glVertex3i( -2*xC, 0,  2*xC); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	// Ponto do mapa
	glClear(GL_DEPTH_BUFFER_BIT);
	if(view==MAP){
	    glPushMatrix();
		    glColor4f(0.0, 0.0, 0.0, 1.0);
		    glTranslatef(obsP[0],obsP[1],obsP[2]);
		    glRotatef(180-anguloH*57, 0,1,0);
		    glutSolidCone(0.03, 0.1, 20, 20);
	    glPopMatrix();
	}
}

void arma()
{
	/* Neste momento desenha um cubo vermelho
	glPushMatrix();
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glTranslatef(0, 0, 0);
        glRotatef(3.14,0,1,0);
        glRotatef(45,1,0,0);
		glutSolidCube(1);
	glPopMatrix();
	*/
}

/*
 *	Função que trata da imagem que é mostrada
 */
void display(void)
{
	// Apaga a imagem antiga
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.92, 0.92, 0.92, 1.0);
	
	// Janela de visualização
	glViewport(0, 0, wScreen, hScreen);
	
	// Projecção
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, wScreen/hScreen, 0.1, zC*10);
	
	// Camara
	GLfloat obsL [] = {cos(anguloH-3.14/2)+obsP[0], obsP[1]+anguloV, sin(anguloH-3.14/2)+obsP[2]};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsL[0], obsL[1], obsL[2], 0, 1, 0);
	
	// Objectos do cenário
	cenario(PERSPECTIVE);
	
	// Minimapa
	glViewport (0, 0, 200, 200);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho (-xC, xC, -yC, yC, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], 0.5, obsP[2], obsP[0], 0, obsP[2], sin(anguloH), 0, -cos(anguloH));
	cenario(MAP);
	
	// Arma
	glViewport (wScreen/2, 0, wScreen/2, hScreen/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho (-xC, xC, -yC, yC, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1, 0.2, 0.2, 1.0, 0.5, 0.5, 0, -1, 0);
	arma();
	
	// Actualiza
	glutSwapBuffers();
}

/*
 *	Controlo para as teclas com letras quando são premidas
 */
void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
		case 'A':
			esquerda = true;
			break;
			
		case 'd':
		case 'D':
			direita = true;
			break;
			
		case 'w':
		case 'W':
			frente = true;
			break;
			
		case 's':
		case 'S':
			atras = true;
			break;
			
		case 27:
			exit(0);
			break;
	}
}

/*
 *	Controlo para as teclas com letras quando são largadas
 */
void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
		case 'A':
			esquerda = false;
			break;
			
		case 'd':
		case 'D':
			direita = false;
			break;
			
		case 'w':
		case 'W':
			frente = false;
			break;
			
		case 's':
		case 'S':
			atras = false;
			break;
	}
}

/*
 *	Controlo da câmara com o rato
 */
void mouseMovement(int x, int y) {
	int diffx = x - wScreen/2;
	int diffy = y - hScreen/2;
	anguloV -= (float) diffy * 0.01;
	anguloH += (float) diffx * 0.01;
	if (anguloV > 3.0) anguloV = 3.0;
	if (anguloV < -3.0) anguloV = -3.0;
}

/*
 *	Timer
 */
void Timer(int value)
{
	// Movimento
	if (frente)
	{
		obsP[2] -= vel * cos(anguloH);
		obsP[0] += vel * sin(anguloH);
	}
	if (atras)
	{
		obsP[2] += vel * cos(anguloH);
		obsP[0] -= vel * sin(anguloH);
	}
	if (esquerda)
	{
		obsP[2] += vel * cos(anguloH + 3.14/2);
		obsP[0] -= vel * sin(anguloH + 3.14/2);
	}
	if (direita)
	{
		obsP[2] -= vel * cos(anguloH + 3.14/2);
		obsP[0] += vel * sin(anguloH + 3.14/2);
	}
	
	// Rato
	glutWarpPointer(wScreen/2, hScreen/2);
	
	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);
}

/*
 *	Main
 */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (200, 350); 
	glutCreateWindow ("Projecto CG");
  
	init();
	
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindow);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
