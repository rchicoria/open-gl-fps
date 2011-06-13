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
GLint wExtra = 0;
GLint hExtra = 0;
GLfloat factorResize = 1.0;
GLint msec = 10;

// Edifício  salaX[] = {x0, z0, x1, z1}
GLfloat alturaEdificio = 2.5;
GLfloat sala1[] = {-2, 2, 2, -2};
GLfloat sala12[] = {-0.35, -2, 0.35, -2.1};
GLfloat sala2[] = {-2, -2.1, 2, -5};
GLfloat sala23[] = {2, -3.25, 2.1, -3.95};
GLfloat sala3[] = {2.1, 2, 11, -5};

// Câmara
GLfloat anguloH = 0;
GLfloat anguloV = 0;
GLfloat obsP[] = {0, 0.6, -4};
GLfloat velCamara = 0.003;
GLfloat passo = 0.0;

// Controlos
bool frente = false;
bool direita = false;
bool atras = false;
bool esquerda = false;
GLfloat vel = 0.03;
bool fullscreen = true;

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
GLuint fogfilter = 2;
GLfloat fogColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};

/*
 *	Cria as várias texturas
 */
void textures()
{
	// Preto
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/preto.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	// Branco
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/branco.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
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
	
	// Madeira
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/madeira.bmp");
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
	glutFullScreen();
	
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
	glFogf(GL_FOG_DENSITY, 0.2f);
	//glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 15.0f);
	glEnable(GL_FOG);
	
	glEnable(GL_CULL_FACE);
}

/*
 *	Função para adaptar a imagem aos redimensionamentos da janela
 */
void resizeWindow(GLsizei w, GLsizei h)
{
	float ratio = (float)wScreen/(float)hScreen;
	if ((float)w/(float)h > ratio)
	{
		wScreen = wScreen * (float)h/(float)hScreen;
		hScreen = h;
		wExtra = w - wScreen;
		factorResize = (float)h/600.0;
	}
	else
	{
		hScreen = hScreen * (float)w/(float)wScreen;
		wScreen = w;
		hExtra = h - hScreen;
		factorResize = (float)w/800.0;
	}
	glutPostRedisplay();
}

float mod(float n)
{
	if (n == 0)
		return 0.0;
	return sqrt(n*n);
}

/*
 *	Cria uma parede com textura com base no ponto do canto inf. esq. e canto sup. dir.
 */
void criaParede(float x0, float y0, float z0, float x1, float y1, float z1)
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f( x0, y0, z0); 
			glTexCoord2f(mod(x0-x1)+mod(z0-z1),0.0f); glVertex3f( x1, y0, z1); 
			glTexCoord2f(mod(x0-x1)+mod(z0-z1),mod(y0-y1)); glVertex3f( x1, y1, z1); 
			glTexCoord2f(0.0f,mod(y0-y1)); glVertex3f( x0, y1, z0);
		glEnd();
	glPopMatrix();
}

/*
 *	Cria um tecto com textura com base no ponto do canto inf. esq. e canto sup. dir.
 */
 void criaHorizontal(float x0, float y0, float z0, float x1, float y1, float z1)
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f( x0, y0, z0); 
			glTexCoord2f(0.0f,mod(z0-z1)); glVertex3f( x0, y0, z1); 
			glTexCoord2f(mod(x0-x1)+mod(y0-y1),mod(z0-z1)); glVertex3f( x1, y1, z1); 
			glTexCoord2f(mod(x0-x1)+mod(y0-y1),0.0f); glVertex3f( x1, y1, z0);
		glEnd();
	glPopMatrix();
}

/*
 *	Desenha as paredes, chão e tecto do pavilhão
 */
void edificio()
{
	glEnable(GL_TEXTURE_2D);
	
	// Chão e tecto
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	criaHorizontal(sala3[2], 0, sala1[1], sala1[0], 0, sala3[3]);
	criaHorizontal(sala1[0], alturaEdificio, sala1[1], sala2[2], alturaEdificio, sala2[3]);
	criaHorizontal(sala3[0], alturaEdificio+1.5, sala3[1], sala3[2], alturaEdificio+1.5, sala3[3]);
	
	// Sala 1
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala1[0], 0, sala1[3], sala12[0], 1.25, sala1[3]);
	criaParede(sala12[2], 0, sala1[3], sala1[2], 1.25, sala1[3]);
	criaParede(sala1[0], 1.25, sala1[3], sala1[2], alturaEdificio, sala1[3]);
	criaParede(sala1[2], 0, sala1[1], sala1[0], alturaEdificio, sala1[1]);
	criaParede(sala1[0], 0, sala1[1], sala1[0], alturaEdificio, sala1[3]);
	criaParede(sala1[2], 0, sala1[3], sala1[2], alturaEdificio, sala1[1]);
	
	// Sala 1 <-> Sala 2
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParede(sala12[0], 0, sala12[1], sala12[0], 1.25, sala12[3]);
	criaParede(sala12[2], 0, sala12[3], sala12[2], 1.25, sala12[1]);
	criaHorizontal(sala12[0], 1.25, sala12[1], sala12[2], 1.25, sala12[3]);
	
	// Sala 2
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala12[0], 0, sala2[1], sala2[0], 1.25, sala2[1]);
	criaParede(sala2[2], 0, sala2[1], sala12[2], 1.25, sala2[1]);
	criaParede(sala2[2], 1.25, sala2[1], sala2[0], alturaEdificio, sala2[1]);
	criaParede(sala2[0], 0, sala2[3], sala2[2], alturaEdificio, sala2[3]);
	criaParede(sala2[0], 0, sala2[1], sala2[0], alturaEdificio, sala2[3]);
	criaParede(sala2[2], 1.25, sala2[3], sala2[2], alturaEdificio, sala2[1]);
	criaParede(sala2[2], 0, sala23[1], sala2[2], 1.25, sala2[1]);
	criaParede(sala2[2], 0, sala2[3], sala2[2], 1.25, sala23[3]);
	
	// Sala 2 <-> Sala 3
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParede(sala23[2], 0, sala23[1], sala23[0], 1.25, sala23[1]);
	criaParede(sala23[0], 0, sala23[3], sala23[2], 1.25, sala23[3]);
	criaHorizontal(sala23[0], 1.25, sala23[1], sala23[2], 1.25, sala23[3]);
	
	// Sala 3
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala3[0], 1.25, sala3[1], sala3[0], alturaEdificio+1.5, sala3[3]);
	criaParede(sala3[0], 0, sala3[1], sala3[0], 1.25, sala23[1]);
	criaParede(sala3[0], 0, sala23[3], sala3[0], 1.25, sala3[3]);
	criaParede(sala3[2], 0, sala3[1], sala3[0], alturaEdificio+1.5, sala3[1]);
	criaParede(sala3[0], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[3]);
	criaParede(sala3[2], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[1]);
	
	glDisable(GL_TEXTURE_2D);
	
	// Vidro
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glColor4f(0.5,1,1,0.5);
		glTranslatef(sala12[0]+(sala12[2]-sala12[0])/2, 0, sala12[1]);
		glRectf(sala12[2], 0, sala12[0], 1.25);
	glPopMatrix();
}

void mapa()
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	criaHorizontal(sala3[2]+0.1, -0.1, sala1[1]+0.1, sala1[0]-0.1, -0.1, sala3[3]-0.1); // Chão
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	criaHorizontal(sala1[2], 0, sala1[1], sala1[0], 0, sala1[3]); // Sala 1
	criaHorizontal(sala12[2], 1.25, sala12[1], sala12[0], 1.25, sala12[3]); // Sala 1 <-> Sala 2
	criaHorizontal(sala2[2], 0, sala2[1], sala2[0], 0, sala2[3]); // Sala 2
	criaHorizontal(sala23[2], 1.25, sala23[1], sala23[0], 1.25, sala23[3]); // Sala 2 <-> Sala 3
	criaHorizontal(sala3[2], 0, sala3[1], sala3[0], 0, sala3[3]); // Sala 3
	
	glDisable(GL_TEXTURE_2D);
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
	
	if (view == PERSPECTIVE)
		edificio();
	else
		mapa();
	
	// Ponto do mapa
	glClear(GL_DEPTH_BUFFER_BIT);
	if(view==MAP){
	    glPushMatrix();
		    glColor4f(0.0, 0.0, 0.0, 1.0);
		    glTranslatef(obsP[0],obsP[1],obsP[2]);
		    glRotatef(180-anguloH*57, 0,1,0);
		    glutSolidCone(0.06, 0.2, 20, 20);
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
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	// Janela de visualização
	glViewport(wExtra/2, hExtra/2, wScreen, hScreen);
	
	// Projecção
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, wScreen/hScreen, 0.1, zC*15);
	
	// Camara
	GLfloat obsL [] = {cos(anguloH-3.14/2)+obsP[0], obsP[1]+anguloV+0.04*sin(passo), sin(anguloH-3.14/2)+obsP[2]};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1]+0.04*sin(passo), obsP[2], obsL[0], obsL[1], obsL[2], 0, 1, 0);
	
	// Objectos do cenário
	cenario(PERSPECTIVE);
	
	// Minimapa
	glViewport (wExtra/2, hExtra/2, wScreen/6, wScreen/6);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho (-xC*2.5, xC*2.5, -yC*2.5, yC*2.5, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsP[0], 0, obsP[2], sin(anguloH), 0, -cos(anguloH));
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
			
		case 'f':
		case 'F':
			if (fullscreen)
			{
				glutReshapeWindow(800, 600);
				wExtra = 0;
				hExtra = 0;
				fullscreen = false;
			}
			else
			{
				glutFullScreen();
				fullscreen = true;
			}
			break;
			
		case 27: // Esq
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
	anguloV -= (float) diffy * velCamara;
	anguloH += (float) diffx * velCamara;
	if (anguloV > 3.0)
		anguloV = 3.0;
	if (anguloV < -3.0)
		anguloV = -3.0;
}

/*
 *	Timer
 */
void Timer(int value)
{
	int anda = 0;
	
	// Movimento
	if (frente)
	{
		obsP[2] -= vel * cos(anguloH) * factorResize;
		obsP[0] += vel * sin(anguloH) * factorResize;
		anda = 1;
	}
	if (atras)
	{
		obsP[2] += vel * cos(anguloH) * factorResize;
		obsP[0] -= vel * sin(anguloH) * factorResize;
		anda = 1;
	}
	if (esquerda)
	{
		obsP[2] += vel * cos(anguloH + 3.14/2) * factorResize;
		obsP[0] -= vel * sin(anguloH + 3.14/2) * factorResize;
		anda = 1;
	}
	if (direita)
	{
		obsP[2] -= vel * cos(anguloH + 3.14/2) * factorResize;
		obsP[0] += vel * sin(anguloH + 3.14/2) * factorResize;
		anda = 1;
	}
	if (anda > 0)
	{
		passo += vel * 5 * factorResize;
		if (passo > 3.14)
			passo -= 3.14;
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
