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

#define PERSPECTIVE 0

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

// Luz ambiente
GLfloat colorAmbient[4] = {0.1,0.1,0.1,1};
GLfloat localCor[4] ={0.4, 0.4, 0.4, 1.0};
GLfloat localPos[4] ={0, 1.0, 0, 1.0};
GLfloat localAttCon =1.0;
GLfloat localAttLin =0.05;
GLfloat localAttQua =0.0;

/*
 *	Inicialização dos parâmetros
 */
void init(void)
{
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
		glTranslatef(0, 0, 0);
		glutSolidTeapot(0.1);
	glPopMatrix();
	glPushMatrix();
		glColor4f(0.0, 1.0, 1.0, 1.0);
		glTranslatef(-0.2, 0, 0.15);
        glRotatef(0,0,1,0);
		glutSolidSphere(0.05, 100, 100);
	glPopMatrix();
	
	// Eixos
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex3i( -xC, 0, 0); 
		glVertex3i(xC, 0, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i(0, -yC, 0); 
		glVertex3i(0, yC, 0); 
	glEnd();
	glBegin(GL_LINES);
		glVertex3i( 0, 0, -zC); 
		glVertex3i( 0, 0,zC); 
	glEnd();
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
	gluPerspective(80, wScreen/hScreen, 0.1, zC*2);
	
	// Camara
	GLfloat obsL [] = {cos(anguloH-3.14/2)+obsP[0], obsP[1]+anguloV, sin(anguloH-3.14/2)+obsP[2]};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsL[0], obsL[1], obsL[2], 0, 1, 0);
	
	// Objectos do cenário
	cenario(PERSPECTIVE);
	
	// Actualiza
	glutSwapBuffers();
}

/*
 *	Controlo para as teclas com letras
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
	/*
	switch (key)
	{
		case 'a':
		case 'A':
			obsP[2]+=0.1*cos(anguloH+3.14/2);
			obsP[0]-=0.1*sin(anguloH+3.14/2);
			break;
			
		case 'd':
		case 'D':
			obsP[2]-=0.1*cos(anguloH+3.14/2);
			obsP[0]+=0.1*sin(anguloH+3.14/2);
			break;
			
		case 'w':
		case 'W':
			obsP[2]-=0.1*cos(anguloH);
			obsP[0]+=0.1*sin(anguloH);
			break;
			
		case 's':
		case 'S':
			obsP[2]+=0.1*cos(anguloH);
			obsP[0]-=0.1*sin(anguloH);
			break;
			
		case 27:
			exit(0);
			break;
	}
	*/
}

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

void mouseMovement(int x, int y) {
	int diffx = x-lastx; //check the difference between the current x and the last x position
	int diffy = y-lasty; //check the difference between the current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position
	anguloV -= (float) diffy*0.01; //set the xrot to xrot with the addition of the difference in the y position
	anguloH += (float) diffx*0.01;// set the xrot to yrot with the addition of the difference in the x position
}

/*
 *	Controlo para as teclas que não sejam com letras
 */
void teclasNotAscii(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			anguloH -= 0.1;
			break;
			
		case GLUT_KEY_RIGHT:
			anguloH += 0.1;
			break;
			
		case GLUT_KEY_UP:
			anguloV += 0.1;
			break;
			
		case GLUT_KEY_DOWN:
			anguloV -= 0.1;
			break;
			
		default:
			break;
	}
	glutPostRedisplay();
}

/*
 *	Timer
 */
void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec,Timer, 1);
	
	// Movimento
	if (frente)
	{
		obsP[2]-=0.01*cos(anguloH);
		obsP[0]+=0.01*sin(anguloH);
	}
	if (atras)
	{
		obsP[2]+=0.01*cos(anguloH);
		obsP[0]-=0.01*sin(anguloH);
	}
	if (esquerda)
	{
		obsP[2]+=0.01*cos(anguloH+3.14/2);
		obsP[0]-=0.01*sin(anguloH+3.14/2);
	}
	if (direita)
	{
		obsP[2]-=0.01*cos(anguloH+3.14/2);
		obsP[0]+=0.01*sin(anguloH+3.14/2);
	}
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
	
	glutSpecialFunc(teclasNotAscii);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindow);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
