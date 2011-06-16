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
#define WEAPON 2

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
GLfloat alturaSala12 = 1.8;
GLfloat sala1[] = {-2, 2, 2, -2};
GLfloat sala12[] = {-0.35, -2, 0.35, -2.1};
GLfloat sala2[] = {-2, -2.1, 2, -5};
GLfloat sala23[] = {2, -3.25, 2.1, -3.95};
GLfloat sala3[] = {2.1, 2, 11, -5};

// Câmara
GLfloat anguloH = 0;
GLfloat anguloV = 0;
GLfloat obsP[] = {9, 0.6, -4};
GLfloat velCamara = 0.003;
GLfloat passo = 0.0;

// Controlos
bool frente = false;
bool direita = false;
bool atras = false;
bool esquerda = false;
GLfloat vel = 0.03;
bool fullscreen = true;
GLfloat recoil = 0;
bool text_vidro = true;

// Luz ambiente
GLfloat colorAmbient[4] = {0.1,0.1,0.1,1};
GLfloat localCor[4] ={0.4, 0.4, 0.4, 1.0};
GLfloat localPos1[4] ={1.9, 1.0, 1.9, 1.0};
GLfloat localPos2[4] ={-1, 1.0, -3.5, 1.0};
GLfloat localPos3[4] ={5, 1.0, -2, 1.0};
GLfloat localAttCon =1.0;
GLfloat localAttLin =0.05;
GLfloat localAttQua =0.0;
GLint lampadas[] = {true, true};

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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/chao.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
    // Vidro
	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("img/glass.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	// Aluminio
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/aluminium.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	
	// Caixa
	glGenTextures(1, &texture[7]);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/caixa.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	// Arma

	glGenTextures(1, &texture[8]);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/arma.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
		
	// parede de vidro

	glGenTextures(1, &texture[9]);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("img/glassblock.bmp");
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
    
    textures();
    
	// Nevoeiro
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.2f);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 5.0f);
	glFogf(GL_FOG_END, 13.0f);
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

/*
 *	Calcula o valor absoluto de um número
 */
float mod(float n)
{
	if (n == 0)
		return 0.0;
	return sqrt(n*n);
}

/*
 *	Liga ou desliga a luz da sala pretendida
 */
bool iluminaSala(int sala)
{
	GLint light;
	switch (sala)
	{
		case 1:
			light = GL_LIGHT0;
			break;
		case 2:
			light = GL_LIGHT1;
			break;
		case 3:
			light = GL_LIGHT2;
			break;
	}
	if (lampadas[sala-1])
	{
		glEnable(light);
		return true;
	}
	else
	{
		glDisable(light);
		return false;
	}
}

/*
 *	Apaga todas as luzes de todas as salas
 */
void apagaLuzes()
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
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

void criaParedeN(float x0, float y0, float z0, float x1, float y1, float z1, int n)
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f( x0, y0, z0); 
			glTexCoord2f(n*1.0f,0.0f); glVertex3f( x1, y0, z1); 
			glTexCoord2f(n*1.0f,n*1.0f); glVertex3f( x1, y1, z1); 
			glTexCoord2f(0.0f,n*1.0f); glVertex3f( x0, y1, z0);
		glEnd();
	glPopMatrix();
}

void criaParedeTexturaUnica(float x0, float y0, float z0, float x1, float y1, float z1)
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f( x0, y0, z0); 
			glTexCoord2f(1.0f,0.0f); glVertex3f( x1, y0, z1); 
			glTexCoord2f(1.0f,1.0f); glVertex3f( x1, y1, z1); 
			glTexCoord2f(0.0f,1.0f); glVertex3f( x0, y1, z0);
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

void criaHorizontalTexturaUnica(float x0, float y0, float z0, float x1, float y1, float z1)
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f( x0, y0, z0); 
			glTexCoord2f(0.0f,1.0f); glVertex3f( x0, y0, z1); 
			glTexCoord2f(1.0f,1.0f); glVertex3f( x1, y1, z1); 
			glTexCoord2f(1.0f,0.0f); glVertex3f( x1, y1, z0);
		glEnd();
	glPopMatrix();
}

/*
 *	Desenha a arma do jogador
 */
void criaArma()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[8]);
	iluminaSala(1);
	iluminaSala(2);
	iluminaSala(3);
	criaParede(0, 0, 0, 0.5, 0.14, 0);
	criaHorizontal(0, 0.14, -0.08, 0.5, 0.14, 0);
	criaParede(0, 0, -0.1, 0.5, 0.14, -0.1);
	criaHorizontal(0, 0.14, -0.18, 0.5, 0.14, -0.1);
	apagaLuzes();
	glDisable(GL_TEXTURE_2D);
}

/*
 *	Cria uma caixa de madeira
 */
void criaCaixa(float x, float y, float z, float angulo)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[7]);
	GLfloat tamCaixa = 0.2;
	
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angulo,0,1,0);
		criaParedeTexturaUnica(-tamCaixa, -tamCaixa, -tamCaixa, -tamCaixa, tamCaixa, tamCaixa);
		criaParedeTexturaUnica(tamCaixa, -tamCaixa, -tamCaixa, -tamCaixa, tamCaixa, -tamCaixa);
		criaParedeTexturaUnica(-tamCaixa, -tamCaixa, tamCaixa, tamCaixa, tamCaixa, tamCaixa);
		criaParedeTexturaUnica(tamCaixa, -tamCaixa, tamCaixa, tamCaixa, tamCaixa, -tamCaixa);
		criaHorizontalTexturaUnica(-tamCaixa, tamCaixa, -tamCaixa, tamCaixa, tamCaixa, tamCaixa);
		criaHorizontalTexturaUnica(-tamCaixa, -tamCaixa, tamCaixa, tamCaixa, -tamCaixa, -tamCaixa);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

/*
 *	Desenha as paredes, chão e tecto do pavilhão
 */
void edificio()
{
	glEnable(GL_TEXTURE_2D);
	
	// Chão e tecto
	apagaLuzes();
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	criaHorizontal(sala3[2], 0, sala1[1], sala1[0], 0, sala3[3]);
	criaHorizontal(sala1[0], alturaSala12, sala1[1], sala1[2], alturaSala12, sala1[3]);
	criaHorizontal(sala2[0], alturaSala12, sala2[1], sala2[0]+2, alturaSala12, sala2[3]);
	criaHorizontal(sala1[0], alturaEdificio+1.5, sala1[1], sala3[2], alturaEdificio+1.5, sala3[3]);
	
	// Sala 1
	iluminaSala(1);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala1[0], 0, sala1[3], sala12[0], 1.25, sala1[3]);
	criaParede(sala12[2], 0, sala1[3], sala1[2], 1.25, sala1[3]);
	criaParede(sala1[0], 1.25, sala1[3], sala1[2], alturaSala12, sala1[3]);
	criaParede(sala3[0], 0, sala1[1], sala1[0], alturaEdificio+1.5, sala1[1]);
	criaParede(sala1[0], 0, sala1[1], sala1[0], alturaSala12, sala1[3]);
	criaParede(sala1[2], 0, sala1[3], sala1[2], alturaSala12, sala1[1]);
	
	// Sala 1 <-> Sala 2
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParede(sala12[0], 0, sala12[1], sala12[0], 1.25, sala12[3]);
	criaParede(sala12[2], 0, sala12[3], sala12[2], 1.25, sala12[1]);
	criaHorizontal(sala12[0], 1.25, sala12[1], sala12[2], 1.25, sala12[3]);
	apagaLuzes();
	
	// Sala 2
	iluminaSala(2);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala12[0], 0, sala2[1], sala2[0], 1.25, sala2[1]);
	criaParede(sala2[2], 0, sala2[1], sala12[2], 1.25, sala2[1]);
	criaParede(sala2[2], 1.25, sala2[1], sala2[0], alturaSala12, sala2[1]);
	criaParede(sala2[0], 0, sala2[3], sala3[0], alturaEdificio+1.5, sala2[3]);
	criaParede(sala2[0], 0, sala2[1], sala2[0], alturaSala12, sala2[3]);
	criaParede(sala2[2], 1.25, sala2[3], sala2[2], alturaSala12, sala2[1]);
	criaParede(sala2[2], 0, sala23[1], sala2[2], 1.25, sala2[1]);
	criaParede(sala2[2], 0, sala2[3], sala2[2], 1.25, sala23[3]);
	
	// Sala 2 <-> Sala 3
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParede(sala23[2], 0, sala23[1], sala23[0], 1.25, sala23[1]);
	criaParede(sala23[0], 0, sala23[3], sala23[2], 1.25, sala23[3]);
	criaHorizontal(sala23[0], 1.25, sala23[1], sala23[2], 1.25, sala23[3]);
	apagaLuzes();
	
	// Sala 3
	iluminaSala(3);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParede(sala3[0], 1.25, sala3[1], sala3[0], alturaSala12, sala3[3]);
	criaParede(sala3[0], 0, sala3[1], sala3[0], 1.25, sala23[1]);
	criaParede(sala3[0], 0, sala23[3], sala3[0], 1.25, sala3[3]);
	criaParede(sala1[0]+2, alturaSala12, sala1[1], sala2[0]+2, alturaEdificio+1.5, sala2[3]);
	criaParede(sala3[2], 0, sala3[1], sala3[0], alturaEdificio+1.5, sala3[1]);
	criaParede(sala3[0], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[3]);
	criaParede(sala3[2], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[1]);
	
	// -------interior sala 3-----------
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	criaParede(sala3[2]-2, 0, sala3[3]+2, sala3[2]-2, alturaSala12, sala3[3]);
	criaParede(sala3[2]-2.1, 0, sala3[3], sala3[2]-2.1, alturaSala12, sala3[3]+2);
	criaParede(sala3[2]-2.1, 0, sala3[3]+2, sala3[2]-2, alturaSala12, sala3[3]+2);
	criaParede(sala3[2]-2, alturaSala12-0.1, sala3[3]+2, sala3[2]-1.1, alturaSala12, sala3[3]+2);
	criaParede(sala3[2]-2, 0, sala3[3]+2, sala3[2]-1.1, 0.1, sala3[3]+2);
	criaHorizontal(sala3[2]-2, alturaSala12-0.1, sala3[3]+2, sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.8);
	criaHorizontal(sala3[2]-1.1, 0.1, sala3[3]+2, sala3[2]-2, 0.1, sala3[3]+1.8);
	criaParede(sala3[2]-1.1, 0, sala3[3]+2, sala3[2]-1, alturaSala12, sala3[3]+2);
	criaParede(sala3[2]-1.1, 0, sala3[3]+1.8, sala3[2]-1.1, alturaSala12, sala3[3]+2);
	criaParede(sala3[2]-1, 0, sala3[3]+2, sala3[2]-1, alturaSala12, sala3[3]+1.8);
	
	criaParede(sala3[2]-1, 0, sala3[3]+1.8, sala3[2]-1.1, alturaSala12, sala3[3]+1.8);
	criaHorizontal(sala3[2]-1.1, alturaSala12-0.1, sala3[3]+2, sala3[2]-2, alturaSala12-0.1, sala3[3]+1.8);
	criaHorizontal(sala3[2]-2, 0.1, sala3[3]+2, sala3[2]-1.1, 0.1, sala3[3]+1.8);
	criaParede(sala3[2]-1.1, 0, sala3[3]+1.8, sala3[2]-2, 0.1, sala3[3]+1.8);
	criaParede(sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.8, sala3[2]-2, alturaSala12, sala3[3]+1.8);
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.5,0.5,0.5,1);
	GLfloat i=0;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[9]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.9,0.9,0.9,0.5);
    criaParedeN(sala3[2]-2, 0.1, sala3[3]+1.8, sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.8,5);
    criaParedeN(sala3[2]-2, 0.1, sala3[3]+1.9, sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.9,5);
    criaParedeN(sala3[2]-2, 0.1, sala3[3]+2, sala3[2]-1.1, alturaSala12-0.1, sala3[3]+2,5);
    
    criaParedeN(sala3[2]-1.1, 0.1, sala3[3]+2, sala3[2]-2, alturaSala12-0.1, sala3[3]+2,5);
    criaParedeN(sala3[2]-1.1, 0.1, sala3[3]+1.9, sala3[2]-2, alturaSala12-0.1, sala3[3]+1.9,5);
    criaParedeN(sala3[2]-1.1, 0.1, sala3[3]+1.8, sala3[2]-2, alturaSala12-0.1, sala3[3]+1.8,5);
    
    
    
    glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	
	apagaLuzes();
	
	// Vidro
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,1,1,0.5);
    criaParedeTexturaUnica(sala12[2], 0, sala12[1], sala12[0], 1.25, sala12[1]);
	
	// Aluminio
	glBindTexture(GL_TEXTURE_2D,texture[6]);
	glDisable(GL_BLEND);
	glPushMatrix();
	    criaParede(sala12[2], 1.2, sala12[1]-0.005, sala12[0], 1.25, sala12[1]-0.005);
	    criaParede(sala12[2], 0, sala12[1]-0.005, sala12[0], 0.05, sala12[1]-0.005);
	    criaParede(sala12[2], 0, sala12[1]-0.005, sala12[2]-0.05, 1.25, sala12[1]-0.005);
	    criaParede(sala12[0]+0.05, 0, sala12[1]-0.005, sala12[0], 1.25, sala12[1]-0.005);
	    glDisable(GL_TEXTURE_2D);
	    glColor4f(0,0,0,1);
	    criaParede(sala12[0]+0.055, 0.05, sala12[1], sala12[0]+0.05, 1.25, sala12[1]-0.005);
	    criaParede(sala12[2]-0.05, 0.05, sala12[1]-0.005, sala12[2]-0.055, 1.25, sala12[1]);
        criaHorizontal(sala12[0], 1.2, sala12[1], sala12[2], 1.2, sala12[1]-0.005);
        criaHorizontal(sala12[2], 0.05, sala12[1], sala12[0], 0.05, sala12[1]-0.005);
	glPopMatrix(); 
	
	
}

/*
 *	Desenha o mapa
 */
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
 *	Trata da definição das luzes
 */
void iluminacao()
{
    glLightfv(GL_LIGHT0, GL_POSITION, localPos1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, localCor);
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    glLightfv(GL_LIGHT1, GL_POSITION, localPos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, localCor);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    glLightfv(GL_LIGHT2, GL_POSITION, localPos3);
    glLightfv(GL_LIGHT2, GL_AMBIENT, localCor);
    glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, localAttQua);
    apagaLuzes();
}

/*
 *	Preenche o cenário com os vários objectos
 */
void cenario(int view)
{
    iluminacao();
	
	// Sala 1
	if (view == PERSPECTIVE)
		iluminaSala(1);
	else
		glEnable(GL_LIGHT0);
	
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
	glColor4f(1.0, 1.0, 1.0, 1.0);
	criaCaixa(1, 0.2, 1.5, 30);
	criaCaixa(0.55, 0.2, 1.7, 10);
	criaCaixa(0.7, 0.6, 1.5, 55);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	apagaLuzes();
	
	// Sala 2
	if (view == PERSPECTIVE)
		iluminaSala(2);
	else
		glEnable(GL_LIGHT1);
	
	criaCaixa(1, 0.2, -3.5, 35);
	
	apagaLuzes();
	
	// Edifício
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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	cenario(PERSPECTIVE);
	
	// Minimapa
	glViewport (wExtra/2, hExtra/2, wScreen/6, wScreen/6);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho (-xC*2.5, xC*2.5, -yC*2.5, yC*2.5, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsP[0], 0, obsP[2], sin(anguloH), 0, -cos(anguloH));
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	cenario(MAP);
	
	// Arma
	glViewport (wExtra/2+wScreen/2, hExtra/2, wScreen/2, hScreen/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, wScreen/hScreen, 0.1, zC*5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.45, 0.3+0.01*sin(passo), 0.12, 0, 0.01*sin(passo)-recoil, 0, 0, 1, 0);
	recoil = 0;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	criaArma();
	
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
		
		case '1':
		case '2':
		case '3':
			lampadas[key-'1'] = !lampadas[key-'1'];
			break;
		
		case 'U':
		case 'u':
		    obsP[1]++;
		    break;
		
		case 'j':
		case 'J':
		    obsP[1]--;
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
		
		case 'V':
		case 'v':
		    // Vidro
	        if(text_vidro){
	            glGenTextures(1, &texture[5]);
	            glBindTexture(GL_TEXTURE_2D, texture[5]);
	            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	            imag.LoadBmpFile("img/broken_glass.bmp");
	            glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	            imag.GetNumCols(),
		            imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		            imag.ImageData());
		        text_vidro=false;
		    }
		    else {
	            // Vidro
	            glGenTextures(1, &texture[5]);
	            glBindTexture(GL_TEXTURE_2D, texture[5]);
	            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	            imag.LoadBmpFile("img/glass.bmp");
	            glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	            imag.GetNumCols(),
	                imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
	                imag.ImageData());
	            text_vidro=true;
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

void mouseClick(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		recoil = 0.12;
	}
}

/*
 *	Controlo da câmara com o rato
 */
void mouseMovement(int x, int y) {
	float limite = 3.14/2;
	int diffx = x - wScreen/2;
	int diffy = y - hScreen/2;
	anguloV -= (float) diffy * velCamara;
	anguloH += (float) diffx * velCamara;
	if (anguloV > limite)
		anguloV = limite;
	if (anguloV < -limite)
		anguloV = -limite;
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
	glutInitWindowPosition (200, 50); 
	glutCreateWindow ("Projecto CG");
  
	init();
	
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(display); 
	glutReshapeFunc(resizeWindow);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}
