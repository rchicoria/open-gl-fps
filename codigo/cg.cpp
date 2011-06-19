 /*
 *	Autores:
 *
 *	Ricardo Pinto Lopes 2008114843
 *	Rui Pedro da Cunha Santos Chicória 2008115099
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "RgbImage.hpp"

#define PERSPECTIVE 0
#define MAP 1
#define WEAPON 2
#define PI 3.14159

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
GLfloat alturaSala12 = 1.75;
GLfloat sala1[] = {-2, 2, 2, -2};
GLfloat sala12[] = {-0.35, -2, 0.35, -2.1};
GLfloat sala2[] = {-2, -2.1, 2, -5};
GLfloat sala23[] = {2, -3.25, 2.1, -3.95};
GLfloat sala3[] = {2.1, 2, 11, -5};

// Câmara
GLfloat anguloH = 0;
GLfloat anguloV = 0;
GLfloat obsP[] = {0, 0.6, -3.5};
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
GLint   ctrlLant=0;

// Luz ambiente
GLfloat colorAmbient[4] = {0.1,0.1,0.1,1};
GLfloat localCor[4] ={0.4, 0.4, 0.4, 1.0};
GLfloat localDif[4] ={1,1,1,1};
GLfloat localSpec[4] ={1,1,1,1};
GLfloat localPos1[4] ={1.9, 1.0, 1.9, 1.0};
GLfloat localPos2[4] ={-1, 1.0, -3.5, 1.0};
GLfloat localPos3[4] ={5, alturaEdificio, -3, 1.0};
GLfloat localAttCon =1.0;
GLfloat localAttLin =0.05;
GLfloat localAttQua =0.0;
GLint lampadas[] = {true, true, true};
GLint fundidas[] = {false, false, false};

//lanterna

GLfloat concentracao = 0.3;
GLfloat ang = 15;

// Texturas
GLuint texture[20];
RgbImage imag;

// Nevoeiro
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };
GLuint fogfilter = 2;
GLfloat fogColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};

// Furos dos tiros
GLint furosMax = 10;
GLfloat furos[10][5];
GLint furosPos = 0;
GLint furosTam = 0;

// Alvos
GLint numAlvos = 4;
GLfloat alvos[4][6] = { // {x, y, z, angulo, estado (0 ok 90 ko), sala}
	{-0.9, 0, -4.5, 90, 0, 2},
	{2, alturaSala12+0.01, 0.5, 90, 0, 3},
	{2, alturaSala12+0.01, -1.5, 90, 0, 3},
	{sala3[2]-0.5, 0, sala3[3]+1.7, 0, 0, 3},
};
GLint alvosOK = 4;
GLfloat lAlvo = 0.434;
GLfloat aAlvo = 0.981;

// Materiais
GLfloat goldAmbient[] = {0.24725, 0.1995, 0.0745};
GLfloat goldDiffuse[] = {0.75164, 0.60648, 0.22648};
GLfloat goldSpecular[] = {0.628281, 0.555802, 0.366065};
GLfloat goldShininess = 128 * 0.4;
GLfloat rubyAmbient[] = {0.1745, 0.01175, 0.01175};
GLfloat rubyDiffuse[] = {0.61424, 0.04136, 0.04136};
GLfloat rubySpecular[] = {0.727811, 0.626959, 0.626959};
GLfloat rubyShininess = 128 * 0.6;

void iluminacao(GLfloat lookLant[]);

/*
 *	Cria uma nova textura e guarda-a no array de texturas
 */
void criaTextura(GLint index, const char* url)
{
	glGenTextures(1, &texture[index]);
	glBindTexture(GL_TEXTURE_2D, texture[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile(url);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

/*
 *	Cria as várias texturas
 */
void textures()
{
	criaTextura(0, "img/preto.bmp");
	criaTextura(1, "img/branco.bmp");
	criaTextura(2, "img/parede.bmp");
	criaTextura(3, "img/madeira.bmp");
	criaTextura(4, "img/chao.bmp");
	criaTextura(5, "img/glass.bmp");
	criaTextura(6, "img/aluminium.bmp");
	criaTextura(7, "img/caixa.bmp");
	criaTextura(8, "img/arma.bmp");
	criaTextura(9, "img/glassblock.bmp");
	criaTextura(10, "img/broken_glass.bmp");
	criaTextura(11, "img/tecto.bmp");
	criaTextura(12, "img/tecto2.bmp");
	criaTextura(13, "img/alvo.bmp");
}

/*
 *	Inicialização dos parâmetros
 */
void init(void)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    
    textures();
    
    glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
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
	GLfloat ratio = (GLfloat)wScreen/(GLfloat)hScreen;
	if ((GLfloat)w/(GLfloat)h > ratio)
	{
		wScreen = wScreen * (GLfloat)h/(GLfloat)hScreen;
		hScreen = h;
		wExtra = w - wScreen;
		factorResize = (GLfloat)h/600.0;
	}
	else
	{
		hScreen = hScreen * (GLfloat)w/(GLfloat)wScreen;
		wScreen = w;
		hExtra = h - hScreen;
		factorResize = (GLfloat)w/800.0;
	}
	glutPostRedisplay();
}

/*
 *	Calcula o valor absoluto de um número
 */
GLfloat mod(GLfloat n)
{
	if (n == 0)
		return 0.0;
	return sqrt(n*n);
}

/*
 *	Calcula o mínimo entre dois valores
 */
GLfloat min(GLfloat a, GLfloat b)
{
	if (a < b)
		return a;
	return b;
}

/*
 *	Calcula o máximo entre dois valores
 */
GLfloat max(GLfloat a, GLfloat b)
{
	if (a > b)
		return a;
	return b;
}

/*
 *	Liga ou desliga a luz da sala pretendida
 */
void iluminaSala(GLint sala, GLint view=PERSPECTIVE)
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
	if (lampadas[sala-1] || view == MAP)
		glEnable(light);
	else
		glDisable(light);
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
void criaParede(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
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

void criaParedeN(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1, GLint n)
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

void criaParedeTexturaUnica(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
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

void criaParedeDefinida(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
{
	GLfloat step = 0.5;
	if (x0 == x1) // Virada para Este
	{
		if (z0 < z1){
		    glNormal3f(-1,0,0);
			for (GLfloat z = z0; z < z1; z += step)
				for (GLfloat y = y0; y < y1; y += step)
					criaParede(x0, y, z, x0, min(y+step, y1), min(z+step, z1));
		}
		else{
			glNormal3f(1,0,0);
			for (GLfloat z = z0; z > z1; z -= step)
				for (GLfloat y = y0; y < y1; y += step)
					criaParede(x0, y, z, x0, min(y+step, y1), max(z-step, z1));
		}
	}
	else // Virada para Norte
	{
		if (x0 < x1){
		    glNormal3f(0,0,1);
			for (GLfloat x = x0; x < x1; x += step)
				for (GLfloat y = y0; y < y1; y += step)
					criaParede(x, y, z0, min(x+step, x1), min(y+step, y1), z0);
		}
		else{
		    glNormal3f(0,0,-1);
			for (GLfloat x = x0; x > x1; x -= step)
				for (GLfloat y = y0; y < y1; y += step)
					criaParede(x, y, z0, max(x-step, x1), min(y+step, y1), z0);
		}
	}
}

/*
 *	Cria um tecto com textura com base no ponto do canto inf. esq. e canto sup. dir.
 */
void criaHorizontal(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
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

void criaHorizontalTexturaUnica(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
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

void criaHorizontalDefinida(GLfloat x0, GLfloat y0, GLfloat z0, GLfloat x1, GLfloat y1, GLfloat z1)
{
	GLfloat step = 1;
	if (x0 < x1)
	{
		if (z0 < z1){
	        glNormal3f(0,1,0);	
			for (GLfloat z = z0; z < z1; z += step)
				for (GLfloat x = x0; x < x1; x += step)
					criaHorizontal(x, y0, z, min(x+step, x1), y0, min(z+step, z1));
		}
		else
		{
		    glNormal3f(0,-1,0);	
			for (GLfloat z = z0; z > z1; z -= step)
				for (GLfloat x = x0; x < x1; x += step)
					criaHorizontal(x, y0, z, min(x+step, x1), y0, max(z-step, z1));
		}
	}
	else
	{
		if (z0 < z1){
		    glNormal3f(0,-1,0);	
			for (GLfloat z = z0; z < z1; z += step)
				for (GLfloat x = x0; x > x1; x -= step)
					criaHorizontal(x, y0, z, max(x-step, x1), y0, min(z+step, z1));
		}
		else{
		    glNormal3f(0,1,0);	
			for (GLfloat z = z0; z > z1; z -= step)
				for (GLfloat x = x0; x > x1; x -= step)
					criaHorizontal(x, y0, z, max(x-step, x1), y0, max(z-step, z1));
		}
	}
}

/*
 *	Desenha a arma do jogador
 */
void criaArma()
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[8]);
	if(obsP[0]>sala1[0] && obsP[0]<sala1[2]){
	    if(obsP[2]>sala2[1]-0.1)
	        iluminaSala(1);
	    else
	        iluminaSala(3);
	    iluminaSala(2);
	}
	else
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
void criaCaixa(GLfloat x, GLfloat y, GLfloat z, GLfloat angulo)
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
 *	Cria o alvo a ser abatido
 */
void criaAlvo(GLfloat* alvo)
{
    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);
    iluminaSala(alvo[5]);
	glPushMatrix();
		glTranslatef(alvo[0], alvo[1]+0.02, alvo[2]);
	    glRotatef(alvo[3], 0, 1, 0);
	    glRotatef(-alvo[4], 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D,texture[13]);
	    criaParedeTexturaUnica(-lAlvo/2, 0, 0, lAlvo/2, aAlvo, 0);
		glBindTexture(GL_TEXTURE_2D,texture[3]);
	    criaParedeTexturaUnica(lAlvo/2, 0, -0.02, -lAlvo/2, aAlvo, -0.02);
	    criaParedeTexturaUnica(lAlvo/2, 0, 0, lAlvo/2, aAlvo, -0.02);
	    criaParedeTexturaUnica(-lAlvo/2, 0, -0.02, -lAlvo/2, aAlvo, 0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void furoTiro(GLfloat x, GLfloat y, GLfloat z, GLfloat ang, GLfloat ang2)
{
	glDisable(GL_TEXTURE_2D);
    glColor4f(0.3, 0.3, 0.3, 1);
	glPushMatrix();
		glTranslatef(x, y, z);
	    glRotatef(ang2, 1, 0, 0);
	    glRotatef(ang, 0, 1, 0);
	    glutSolidCone(0.015, 0.001, 20, 20);
	glPopMatrix();
    glColor4f(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
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
	
	// Sala 1
	iluminaSala(1);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParedeDefinida(sala1[0], 0, sala1[3], sala12[0], 1.25, sala1[3]);
	criaParedeDefinida(sala12[2], 0, sala1[3], sala1[2], 1.25, sala1[3]);
	criaParedeDefinida(sala1[0], 1.25, sala1[3], sala1[2], alturaSala12, sala1[3]);
	criaParedeDefinida(sala3[0], 0, sala1[1], sala1[0], alturaSala12, sala1[1]);
	criaParedeDefinida(sala1[0], 0, sala1[1], sala1[0], alturaSala12, sala1[3]);
	criaParedeDefinida(sala1[2], 0, sala1[3], sala1[2], alturaSala12, sala1[1]);
	glBindTexture(GL_TEXTURE_2D,texture[11]);
	criaHorizontalDefinida(sala1[0], alturaSala12, sala1[1], sala1[2], alturaSala12, sala1[3]);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    criaHorizontalDefinida(sala1[2], 0, sala1[1], sala1[0], 0, sala1[3]);
	apagaLuzes();
	
	// Sala 1 <-> Sala 2
	iluminaSala(1);
	iluminaSala(2);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParedeDefinida(sala12[0], 0, sala12[1], sala12[0], 1.25, sala12[3]);
	criaParedeDefinida(sala12[2], 0, sala12[3], sala12[2], 1.25, sala12[1]);
	criaHorizontalDefinida(sala12[0], 1.25, sala12[1], sala12[2], 1.25, sala12[3]);
	// Vidro
	if(text_vidro)
	    glBindTexture(GL_TEXTURE_2D,texture[5]);
	else
	    glBindTexture(GL_TEXTURE_2D,texture[10]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1,1,1,0.5);
    criaParedeTexturaUnica(sala12[2], 0, sala12[1], sala12[0], 1.25, sala12[1]);
	// Aluminio
	glBindTexture(GL_TEXTURE_2D,texture[6]);
	glDisable(GL_BLEND);
    criaParedeDefinida(sala12[2], 1.2, sala12[1]-0.005, sala12[0], 1.25, sala12[1]-0.005);
    criaParedeDefinida(sala12[2], 0, sala12[1]-0.005, sala12[0], 0.05, sala12[1]-0.005);
    criaParedeDefinida(sala12[2], 0, sala12[1]-0.005, sala12[2]-0.05, 1.25, sala12[1]-0.005);
    criaParedeDefinida(sala12[0]+0.05, 0, sala12[1]-0.005, sala12[0], 1.25, sala12[1]-0.005);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0,0,0,1);
    criaParedeDefinida(sala12[0]+0.055, 0.05, sala12[1], sala12[0]+0.05, 1.25, sala12[1]-0.005);
    criaParedeDefinida(sala12[2]-0.05, 0.05, sala12[1]-0.005, sala12[2]-0.055, 1.25, sala12[1]);
    criaHorizontalDefinida(sala12[0], 1.2, sala12[1], sala12[2], 1.2, sala12[1]-0.005);
    criaHorizontalDefinida(sala12[2], 0.05, sala12[1], sala12[0], 0.05, sala12[1]-0.005);
    glColor4f(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    criaHorizontalDefinida(sala1[2], 0, sala2[1]+0.1, sala1[0], 0, sala2[1]);
	apagaLuzes();
	
	// Sala 2
	iluminaSala(2);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParedeDefinida(sala12[0], 0, sala2[1], sala2[0], 1.25, sala2[1]);
	criaParedeDefinida(sala2[2], 0, sala2[1], sala12[2], 1.25, sala2[1]);
	criaParedeDefinida(sala2[2], 1.25, sala2[1], sala2[0], alturaSala12, sala2[1]);
	criaParedeDefinida(sala2[0], 0, sala2[1], sala2[0], alturaSala12, sala2[3]);
	criaParedeDefinida(sala2[2], 1.25, sala2[3], sala2[2], alturaSala12, sala2[1]);
	criaParedeDefinida(sala2[2], 0, sala23[1], sala2[2], 1.25, sala2[1]);
	criaParedeDefinida(sala2[2], 0, sala2[3], sala2[2], 1.25, sala23[3]);
	glBindTexture(GL_TEXTURE_2D,texture[11]);
	criaHorizontalDefinida(sala2[0], alturaSala12, sala2[1], sala2[0]+2, alturaSala12, sala2[3]);
	apagaLuzes();
	
	// Sala 2 <-> Sala 3
	iluminaSala(2);
	iluminaSala(3);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	criaParedeDefinida(sala23[2], 0, sala23[1], sala23[0], 1.25, sala23[1]);
	criaParedeDefinida(sala23[0], 0, sala23[3], sala23[2], 1.25, sala23[3]);
	criaHorizontalDefinida(sala23[0], 1.25, sala23[1], sala23[2], 1.25, sala23[3]);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParedeDefinida(sala1[0]+2, alturaSala12, sala2[1], sala1[0]+2, alturaEdificio+1.5, sala2[3]);
	criaParedeDefinida(sala2[0], 0, sala2[3], sala3[0], alturaEdificio+1.5, sala2[3]);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	criaHorizontalDefinida(sala3[0], 0, sala2[0]-0.1, sala2[0], 0, sala3[3]);
	apagaLuzes();
	
	// Sala 3
	iluminaSala(3);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	criaParedeDefinida(sala3[0], 1.25, sala3[1], sala3[0], alturaSala12, sala3[3]);
	criaParedeDefinida(sala3[2], alturaSala12, sala1[1], sala1[0], alturaEdificio+1.5, sala1[1]);
	criaParedeDefinida(sala3[0], 0, sala3[1], sala3[0], 1.25, sala23[1]);
	criaParedeDefinida(sala3[0], 0, sala23[3], sala3[0], 1.25, sala3[3]);
	criaParedeDefinida(sala1[0]+2, alturaSala12, sala1[1], sala1[0]+2, alturaEdificio+1.5, sala2[1]);
	criaParedeDefinida(sala3[2], 0, sala3[1], sala3[0], alturaSala12, sala3[1]);
	criaParedeDefinida(sala3[0], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[3]);
	criaParedeDefinida(sala3[2], 0, sala3[3], sala3[2], alturaEdificio+1.5, sala3[1]);
	glBindTexture(GL_TEXTURE_2D,texture[12]);
	criaHorizontalDefinida(sala1[0], alturaEdificio+1.5, sala1[1], sala3[2], alturaEdificio+1.5, sala3[3]);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	criaHorizontalDefinida(sala3[2], 0, sala1[1], sala3[0], 0, sala3[3]);
	
	// -------interior sala 3-----------
    glBindTexture(GL_TEXTURE_2D, texture[2]);
	
	//paredes do meio
	criaParedeDefinida(sala3[0]+2, 0, sala3[1]-2, sala3[2]-2, alturaSala12, sala3[1]-2);
	criaParedeDefinida(sala3[2]-2, 0, sala3[1]-2-0.1, sala3[0]+2, alturaSala12, sala3[1]-2-0.1);
	criaParedeDefinida(sala3[2]-2, 0, sala3[1]-2, sala3[2]-2, alturaSala12, sala3[1]-2-0.1);
	criaParedeDefinida(sala3[0]+2, 0, sala3[1]-2-0.1, sala3[0]+2, alturaSala12, sala3[1]-2);
	criaParedeDefinida(sala3[0]+5, 0, sala3[1]-2.1, sala3[0]+5, alturaSala12, sala3[1]-4);
	criaParedeDefinida(sala3[0]+5-0.1, 0, sala3[1]-4, sala3[0]+5-0.1, alturaSala12, sala3[1]-2.1);
	criaParedeDefinida(sala3[0]+5, 0, sala3[1]-4, sala3[0]+5-0.1, alturaSala12, sala3[1]-4);
	
	//paredes e vidro do canto
	criaParedeDefinida(sala3[2]-2, 0, sala3[3]+2, sala3[2]-2, alturaSala12, sala3[3]);
	criaParedeDefinida(sala3[2]-2.1, 0, sala3[3], sala3[2]-2.1, alturaSala12, sala3[3]+2);
	criaParedeDefinida(sala3[2]-2.1, 0, sala3[3]+2, sala3[2]-2, alturaSala12, sala3[3]+2);
	criaParedeDefinida(sala3[2]-2, alturaSala12-0.1, sala3[3]+2, sala3[2]-1.1, alturaSala12, sala3[3]+2);
	criaParedeDefinida(sala3[2]-2, 0, sala3[3]+2, sala3[2]-1.1, 0.1, sala3[3]+2);
	criaHorizontalDefinida(sala3[2]-2, alturaSala12-0.1, sala3[3]+2, sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.8);
	criaHorizontalDefinida(sala3[2]-1.1, 0.1, sala3[3]+2, sala3[2]-2, 0.1, sala3[3]+1.8);
	criaParedeDefinida(sala3[2]-1.1, 0, sala3[3]+2, sala3[2]-1, alturaSala12, sala3[3]+2);
	criaParedeDefinida(sala3[2]-1.1, 0, sala3[3]+1.8, sala3[2]-1.1, alturaSala12, sala3[3]+2);
	criaParedeDefinida(sala3[2]-1, 0, sala3[3]+2, sala3[2]-1, alturaSala12, sala3[3]+1.8);
	
	criaParedeDefinida(sala3[2]-1, 0, sala3[3]+1.8, sala3[2]-1.1, alturaSala12, sala3[3]+1.8);
	criaHorizontalDefinida(sala3[2]-1.1, alturaSala12-0.1, sala3[3]+2, sala3[2]-2, alturaSala12-0.1, sala3[3]+1.8);
	criaHorizontalDefinida(sala3[2]-2, 0.1, sala3[3]+2, sala3[2]-1.1, 0.1, sala3[3]+1.8);
	criaParedeDefinida(sala3[2]-1.1, 0, sala3[3]+1.8, sala3[2]-2, 0.1, sala3[3]+1.8);
	criaParedeDefinida(sala3[2]-1.1, alturaSala12-0.1, sala3[3]+1.8, sala3[2]-2, alturaSala12, sala3[3]+1.8);

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
	glDisable(GL_LIGHT3);
	
	for (GLint i=0; i<furosTam; i++)
	{
		furoTiro(furos[i][0], furos[i][1], furos[i][2], furos[i][3], furos[i][4]);
	}
	
	glDisable(GL_TEXTURE_2D);
}

/*
 *	Desenha o mapa
 */
void mapa()
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	
	// Geral
	criaHorizontal(sala3[2]+0.1, -0.1, sala1[1]+0.1, sala1[0]-0.1, -0.1, sala3[3]-0.1); // Chão
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	criaHorizontal(sala1[2], 0, sala1[1], sala1[0], 0, sala1[3]); // Sala 1
	criaHorizontal(sala12[2], 1.25, sala12[1], sala12[0], 1.25, sala12[3]); // Sala 1 <-> Sala 2
	criaHorizontal(sala2[2], 0, sala2[1], sala2[0], 0, sala2[3]); // Sala 2
	criaHorizontal(sala23[2], 1.25, sala23[1], sala23[0], 1.25, sala23[3]); // Sala 2 <-> Sala 3
	criaHorizontal(sala3[2], 0, sala3[1], sala3[0], 0, sala3[3]); // Sala 3
	
	// Pormenores
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	criaHorizontal(sala3[2]-2, 0.1, sala3[3]+2.1, sala3[2]-2.1, 0.1, sala3[3]); // Parede vidro 1
	criaHorizontal(sala3[2]-1.1, 0.1, sala3[3]+2.1, sala3[2]-2.1, 0.1, sala3[3]+1.8); // Parede vidro 2
	criaHorizontal(sala3[2]-2, 0.1, sala3[1]-2, sala3[0]+2, 0.1, sala3[1]-2.1); // Parede em T 1
	criaHorizontal(sala3[0]+5, 0.1, sala3[1]-2.1, sala3[0]+4.9, 0.1, sala3[1]-4); // Parede em T 2
	
	glDisable(GL_TEXTURE_2D);
}

/*
 *	Trata da definição das luzes
 */
void iluminacao(GLfloat lookLant[])
{
    glLightfv(GL_LIGHT0, GL_POSITION, localPos1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, localCor);
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    glLightfv(GL_LIGHT1, GL_POSITION, localPos2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, localCor);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, localDif );
    glLightfv(GL_LIGHT1, GL_SPECULAR, localSpec );
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    glLightfv(GL_LIGHT2, GL_POSITION, localPos3);
    glLightfv(GL_LIGHT2, GL_AMBIENT, localCor);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, localDif );
    glLightfv(GL_LIGHT2, GL_SPECULAR, localSpec );
    glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, localAttLin);
    glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, localAttQua);
    
    glLightfv(GL_LIGHT3, GL_DIFFUSE, localDif );
    glLightf (GL_LIGHT3, GL_SPOT_EXPONENT , concentracao);
    glLightf (GL_LIGHT3, GL_SPOT_CUTOFF, ang);
    
	if(ctrlLant==1){
	    glEnable(GL_LIGHT3);
    
        GLfloat ponto[4] = {obsP[0], obsP[1], obsP[2], 1.0};
	
	    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lookLant );
        glLightfv(GL_LIGHT3, GL_POSITION, ponto );
    }
	else
	    glDisable(GL_LIGHT3);

    glShadeModel(GL_SMOOTH);
    apagaLuzes();
}

/*
 *	Preenche o cenário com os vários objectos
 */
void cenario(GLint view)
{
	// Alvos
	for (GLint i=0; i<numAlvos; i++)
		criaAlvo(alvos[i]);
	apagaLuzes();
	// Sala 1
	iluminaSala(1, view);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	criaCaixa(1, 0.2, 1.5, 30);
	criaCaixa(0.55, 0.2, 1.7, 10);
	criaCaixa(0.7, 0.6, 1.5, 55);
	criaCaixa(-0.2, 0.2, -0.6, 5);
	criaCaixa(-1.2, 0.2, -1, 20);
	criaCaixa(-1.5, 0.2, 1.5, 45);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glColor4f(1,1,0.6,1);
	criaHorizontal(sala1[0]+2.1, alturaSala12-0.05, sala1[1]-0.35, sala1[0]+2.5, alturaSala12-0.05, sala1[1]-0.7);
	glColor4f(1,1,1,1);
	apagaLuzes();
	
	//Luz sala 2
	iluminaSala(2, view);
	glColor4f(1,1,0.6,1);
	criaHorizontalDefinida(sala2[0]+1, alturaSala12-0.05, sala2[1]-1.35, sala2[0]+1.3, alturaSala12-0.05, sala2[1]-1.7);
	glColor4f(1,1,1,1);
	apagaLuzes();
	
	// Sala 2
	iluminaSala(2, view);
	iluminaSala(3, view);
	
	criaCaixa(-1.2, 0.2, -3.95, 5);
	criaCaixa(-1.7, 0.2, -3.95, -10);
	glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT,GL_AMBIENT, goldAmbient);
        glMaterialf(GL_FRONT,GL_SHININESS, goldShininess);
        glMaterialfv(GL_FRONT,GL_SPECULAR, goldSpecular);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, goldDiffuse);
		glTranslatef(-1.3, 0.45, -3.9);
		glutSolidSphere(0.05, 100, 100);
		glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
	glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT,GL_AMBIENT, rubyAmbient);
        glMaterialf(GL_FRONT,GL_SHININESS, rubyShininess);
        glMaterialfv(GL_FRONT,GL_SPECULAR, rubySpecular);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, rubyDiffuse);
		glTranslatef(-1.2, 0.425, -3.98);
		glutSolidSphere(0.025, 100, 100);
		glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
	
	
	apagaLuzes();
	
	// Sala 3
	iluminaSala(3, view);
	
	criaCaixa(10, 0.2, -3.8, 30);
	criaCaixa(9.55, 0.2, -4, 10);
	criaCaixa(9.7, 0.6, -3.8, 55);
	
	criaCaixa(6.6, 0.2, -0.5, 0);
	criaCaixa(6.1, 0.2, -0.7, 10);
	criaCaixa(6.3, 0.6, -0.5, 55);
	criaCaixa(6.4, 1, -0.45, 70);
	
	criaCaixa(1.5, alturaSala12+0.3, -0.5, 40);
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,1,0.6,1);
	criaHorizontalDefinida(sala3[0]+2.5, alturaEdificio+1.5-0.05, sala3[1]-2.5, sala3[0]+3, alturaEdificio+1.5-0.05, sala3[1]-4);
	glColor4f(0.3,0.3,0.3,1);
	criaHorizontalDefinida(sala3[0]+5.5, alturaEdificio+1.5-0.05, sala3[1]-2.5, sala3[0]+6, alturaEdificio+1.5-0.05, sala3[1]-4);
	glEnable(GL_TEXTURE_2D);
	
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
	GLfloat lookLant[] = {obsL[0]-obsP[0], obsL[1]-obsP[1] ,obsL[2]-obsP[2], 0};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1]+0.04*sin(passo), obsP[2], obsL[0], obsL[1], obsL[2], 0, 1, 0);
	
	// Objectos do cenário
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	iluminacao(lookLant);
	cenario(PERSPECTIVE);
	
	// Minimapa
	glViewport (wExtra/2, hExtra/2, wScreen/6, wScreen/6);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho (-xC*2.5, xC*2.5, -yC*2.5, yC*2.5, -zC*2.5, zC*2.5);
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
void keyPress(unsigned char key, GLint x, GLint y)
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
			if (fundidas[key-'1'] == false)
				lampadas[key-'1'] = !lampadas[key-'1'];
			break;
		
		case 'c':
		case 'C':
			obsP[1] -= 0.3;
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
		
		case 'l':
        case 'L':
            ctrlLant+=1;
            ctrlLant%=2;
            break;
		
		case 27: // Esq
			exit(0);
			break;
	}
}

/*
 *	Controlo para as teclas com letras quando são largadas
 */
void keyUp(unsigned char key, GLint x, GLint y)
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
		
		case 'c':
		case 'C':
			obsP[1] += 0.3;
			break;
	}
}

/*
 *	Timer para a animação do alvo a ser derrubado
 */
void derrubaAlvo(GLint value)
{
	alvos[value][4] += 5;
	if (alvos[value][4] < 90)
		glutTimerFunc(msec, derrubaAlvo, value);
}

/*
 *	Verifica se o tiro acertou em algum alvo e fá-lo rodar em caso afirmativo
 */
bool acertaAlvo(GLfloat* bala)
{
	for (GLint i=0; i<numAlvos; i++)
	{
		if (alvos[i][4] == 0) // Se está OK
		{
			if (alvos[i][3] == 0 && obsP[2] > alvos[i][2] && alvos[i][0]-lAlvo/2 <= bala[0] && alvos[i][0]+lAlvo/2 >= bala[0] && alvos[i][1] <= bala[1] && alvos[i][1]+aAlvo >= bala[1] && alvos[i][2] >= bala[2] && alvos[i][2]-0.1 <= bala[2]); // Se está virado para Sul
			else if (alvos[i][3] == 90 && obsP[0] > alvos[i][0] && alvos[i][2]-lAlvo/2 <= bala[2] && alvos[i][2]+lAlvo/2 >= bala[2] && alvos[i][1] <= bala[1] && alvos[i][1]+aAlvo >= bala[1] && alvos[i][0] >= bala[0] && alvos[i][0]-0.1 <= bala[0]); // Se está virado para Este
			else
				continue;
			glutTimerFunc(msec, derrubaAlvo, i);
			alvosOK--;
			return true;
		}
	}
	return false;
}

/*
 *	Verifica se o tiro acertou em algum candeeiro e fá-lo estragar-se em caso afirmativo
 */
bool acertaCandeeiro(GLfloat* bala)
{
	// Sala 2
	if (bala[0] >= sala2[0]+1 && bala[0] <= sala2[0]+1.3 && bala[1] >= alturaSala12-0.05 && bala[2] <= sala2[1]-1.35 && bala[2] >= sala2[1]-1.7)
	{
		lampadas[1] = false;
		fundidas[1] = true;
		return true;
	}
	// Sala 3   (sala3[0]+2.5, alturaEdificio+1.5-0.05, sala3[1]-2.5, sala3[0]+3, alturaEdificio+1.5-0.05, sala3[1]-4);
	else if (bala[0] >= sala3[0]+2.5 && bala[0] <= sala3[0]+3 && bala[1] > alturaEdificio+1.5-0.05 && bala[2] <= sala3[1]-2.5 && bala[2] >= sala3[1]-4)
	{
		lampadas[2] = false;
		fundidas[2] = true;
		return true;
	}
	// Não encontrou
	return false;
}

/*
 *	Descobre onde é que o tiro acertou e faz qualquer coisa aí
 */
void tiro(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat bala[] = {obsP[0], obsP[1], obsP[2]};
	GLfloat alvo[] = {x, y, z};
	GLfloat mov[] = {(alvo[0]-bala[0])/10, (alvo[1]-bala[1])/10, (alvo[2]-bala[2])/10};
	while (1)
	{
		GLfloat ang = 0;
		GLfloat ang2 = 0;
		
		// Alvos
		if (acertaAlvo(bala))
			break;
		// Candeeiros
		if (acertaCandeeiro(bala))
			break;
		
		// Paredes exteriores
		else if (bala[0] >= sala3[2]) // Parede Este
		{
			bala[0] = sala3[2]-0.001;
			ang = 90;
		}
		else if (bala[0] <= sala1[0]) // Parede Oeste
		{
			bala[0] = sala1[0]+0.001;
			ang = 90;
		}
		else if (bala[2] >= sala1[1]) // Parede Sul
		{
			bala[2] = sala1[1]-0.001;
			ang = 0;
		}
		else if (bala[2] <= sala3[3]) // Parede Norte
		{
			bala[2] = sala3[3]+0.001;
			ang = 0;
		}
		else if (bala[1] <= 0) // Chão
		{
			bala[1] = 0.001;
			ang2 = 90;
		}
		else if (bala[1] >= alturaEdificio+1.5) // Tecto
		{
			bala[1] = alturaEdificio+1.5-0.001;
			ang2 = 90;
		}
		
		// Sala 2
		else if (bala[2] >= sala12[3] && bala[0] <= sala2[2] && bala[1] <= alturaSala12) // Parede de ligação entre salas 1 e 2
		{
			if (bala[0] >= sala12[0] && bala[0] <= sala12[2] && bala[1] >= 0 && bala[1] <= 1.25) // Porta de vidro
			{
				text_vidro=false;
				break;
			}
			bala[2] = sala12[3]-0.001;
		}
		else if (bala[0] >= sala2[2] && obsP[0] <= sala2[2] && bala[0] <= sala3[0] && bala[1] <= alturaSala12 && !(bala[2] <= sala23[1] && bala[2] >= sala23[3] && bala[1] <= 1.25)) // Parede de ligação entre salas 2 e 3
		{
			bala[0] = sala2[2]-0.001;
			ang = 90;
		}
		else if (bala[1] >= alturaSala12 && bala[0] <= 0)
		{
			if (bala[1]-alturaSala12 < -bala[0]) // Tecto mais baixo
			{
				bala[1] = alturaSala12-0.001;
				ang2 = 90;
			}
			else // Parede mais alta
			{
				bala[0] = 0.001;
				ang = 90;
			}
		}
		
		// Sala 3
		else if (bala[0] <= sala3[0] && obsP[0] >= sala3[0] && bala[0] >= sala2[2] && bala[1] <= alturaSala12 && !(bala[2] <= sala23[1] && bala[2] >= sala23[3] && bala[1] <= 1.25)) // Parede de ligação entre salas 2 e 3
		{
			bala[0] = sala3[0]+0.001;
			ang = 90;
		}
		// Parede de blocos de vidro
		else if (bala[0] <= sala3[2]-2 && obsP[0] >= sala3[2]-2 && bala[0] >= sala3[2]-2.1 && bala[1] <= alturaSala12 && bala[2] < sala3[3]+1.8) // Este
		{
			bala[0] = sala3[2]-2+0.001;
			ang = 90;
		}
		else if (bala[0] >= sala3[2]-2.1 && obsP[0] <= sala3[2]-2 && bala[0] <= sala3[2]-2 && bala[1] <= alturaSala12 && bala[2] < sala3[3]+2.1) // Oeste
		{
			bala[0] = sala3[2]-2.1-0.001;
			ang = 90;
		}
		else if (bala[0] >= sala3[2]-2.1 && bala[0] <= sala3[2]-1 && bala[1] <= alturaSala12 && bala[2] <= sala3[3]+2.1 && bala[2] >= sala3[3]+2) // Sul
		{
			bala[2] = sala3[3]+2+0.001;
			ang = 0;
		}
		else if (bala[0] >= sala3[2]-1.1 && bala[0] <= sala3[2]-1 && bala[1] <= alturaSala12 && bala[2] <= sala3[3]+2 && bala[2] >= sala3[3]+1.8) // Ponta
		{
			bala[0] = sala3[2]-1+0.001;
			ang = 90;
		}
		// Parede em T
		else if (bala[2] <= sala3[1]-2 && obsP[2] >= sala3[1]-2 && bala[2] >= sala3[1]-2.1 && bala[1] <= alturaSala12 && bala[0] >= sala3[0]+2 && bala[0] <= sala3[2]-2) // z=0
		{
			bala[2] = sala3[1]-2+0.001;
			ang = 0;
		}
		else if (bala[2] >= sala3[1]-2.1 && obsP[2] <= sala3[1]-2.1 && bala[2] <= sala3[1]-2 && bala[1] <= alturaSala12 && bala[0] >= sala3[0]+2 && bala[0] <= sala3[2]-2) // z=-0.1
		{
			bala[2] = sala3[1]-2.1-0.001;
			ang = 0;
		}
		else if (bala[2] >= sala3[1]-4 && obsP[2] <= sala3[1]-4 && bala[2] <= sala3[1]-3.9 && bala[1] <= alturaSala12 && bala[0] >= sala3[0]+4.9 && bala[0] <= sala3[0]+5) // z=-2
		{
			bala[2] = sala3[1]-4-0.001;
			ang = 0;
		}
		else if (bala[0] >= sala3[0]+2 && obsP[0] <= sala3[0]+2 && bala[0] <= sala3[0]+2.1 && bala[1] <= alturaSala12 && bala[2] >= sala3[1]-2.1 && bala[2] <= sala3[1]-2) // x=4.1
		{
			bala[0] = sala3[0]+2-0.001;
			ang = 90;
		}
		else if (bala[0] <= sala3[2]-2 && obsP[0] >= sala3[2]-2 && bala[0] >= sala3[2]-2.1 && bala[1] <= alturaSala12 && bala[2] >= sala3[1]-2.1 && bala[2] <= sala3[1]-2) // x=9
		{
			bala[0] = sala3[2]-2+0.001;
			ang = 90;
		}
		else if (bala[0] >= sala3[0]+4.9 && obsP[0] <= sala3[0]+4.9 && bala[0] <= sala3[0]+5 && bala[1] <= alturaSala12 && bala[2] >= sala3[1]-4 && bala[2] <= sala3[1]-2.1) // x=7
		{
			bala[0] = sala3[0]+4.9-0.001;
			ang = 90;
		}
		else if (bala[0] <= sala3[0]+5 && obsP[0] >= sala3[0]+5 && bala[0] >= sala3[0]+4.9 && bala[1] <= alturaSala12 && bala[2] >= sala3[1]-4 && bala[2] <= sala3[1]-2.1) // x=7.1
		{
			bala[0] = sala3[0]+5+0.001;
			ang = 90;
		}
		
		// Ainda não acertou em nada
		else
		{
			for (GLint i=0; i<3; i++)
				bala[i] += mov[i];
			continue;
		}
		for (GLint i=0; i<3; i++)
			furos[furosPos][i] = bala[i];
		furos[furosPos][3] = ang;
		furos[furosPos][4] = ang2;
		furosPos = (furosPos + 1) % furosMax;
		if (furosTam < furosMax)
			furosTam++;
		break;
	}
}

/*
 *	Controlo dos cliques de rato, para disparar
 */
void mouseClick(GLint button, GLint state, GLint x, GLint y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		recoil = 0.12;
		tiro(cos(anguloH-3.14/2)+obsP[0], obsP[1]+anguloV+0.04*sin(passo), sin(anguloH-3.14/2)+obsP[2]);
	}
}

/*
 *	Controlo da câmara com o rato
 */
void mouseMovement(GLint x, GLint y) {
	GLfloat limite = 3.14/2;
	GLint diffx = x - wScreen/2;
	GLint diffy = y - hScreen/2;
	anguloV -= (GLfloat) diffy * velCamara;
	anguloH += (GLfloat) diffx * velCamara;
	if (anguloV > limite)
		anguloV = limite;
	if (anguloV < -limite)
		anguloV = -limite;
}

/*
 *	Detecta colisões segundo o eixo dos x
 */
GLfloat colisoesX(GLfloat x, GLfloat z)
{
	// Global
	if (x < sala2[0]+0.15) // Parede Oeste
		return obsP[0];
	if (x > sala3[2]-0.15) // Parede Este
		return obsP[0];
	
	// Sala 2
	if (x > sala2[2]-0.15 && x < sala3[0] && !(z < sala23[1]-0.15 && z > sala23[3]+0.15) && obsP[0] < x) // Parede Este
		return obsP[0];
	// Alvo + caixas
	if (x < alvos[0][0]+0.15 && x > alvos[0][0]-0.1 && z < -3.5 && obsP[0] > x)
		return obsP[0];
	
	// Sala 3
	if (x < sala3[0]+0.15 && x > sala2[2] && !(z < sala23[1]-0.15 && z > sala23[3]+0.15) && obsP[0] > x)
		return obsP[0];
	// Parede de blocos de vidro
	if (x > sala3[2]-2.1-0.15 && x < sala3[2]-2 && z < sala3[3]+2.1+0.15 && obsP[0] < x)
		if (sala3[2]-x >= -(sala3[3]-z))
			return obsP[0];
	if (x > sala3[2]-2.1 && x < sala3[2]-2+0.15 && z < sala3[3]+2.1+0.15 && obsP[0] > x)
		if (sala3[2]-x+0.1 >= -(sala3[3]-z))
			return obsP[0];
	if (x > sala3[2]-1 && x < sala3[2]-1+0.15 && z < sala3[3]+2.1+0.15 && z > sala3[3]+1.8-0.15 && obsP[0] > x)
		return obsP[0];
	// Parede em T
	if (x > sala3[0]+4.9-0.15 && x < sala3[0]+5 && z < sala3[1]-2.1 && z > sala3[1]-4-0.15 && obsP[0] < x)
		return obsP[0];
	if (x > sala3[0]+4.9 && x < sala3[0]+5+0.15 && z < sala3[1]-2.1 && z > sala3[1]-4-0.15 && obsP[0] > x)
		return obsP[0];
	if (x > sala3[0]+2-0.15 && x < sala3[0]+2 && z < sala3[1]-2+0.15 && z > sala3[1]-2.1-0.15 && obsP[0] < x)
		return obsP[0];
	if (x > sala3[2]-2 && x < sala3[2]-2+0.15 && z < sala3[1]-2+0.15 && z > sala3[1]-2.1-0.15 && obsP[0] > x)
		return obsP[0];
	// Caixas
	if (x > 5.7 && x < 5.8 && z > -1.2 && z < 0 && obsP[0] < x)
		return obsP[0];
		
	return x;
}

/*
 *	Detecta colisões segundo o eixo dos z
 */
GLfloat colisoesZ(GLfloat x, GLfloat z)
{
	// Global
	if (z > sala3[1]-0.15) // Parede Sul
		return obsP[2];
	if (z < sala3[3]+0.15) // Parede Norte
		return obsP[2];
	
	// Sala 2
	if (z > sala2[1]-0.15 && x < sala2[2]+0.15) // Parede Sul
		return obsP[2];
	// Alvo + caixas
	if (x < alvos[0][0]+0.15 && z < -3.5 && z > -3.6 && obsP[2] > z)
		return obsP[2];
	
	// Sala 2<->3
	if (x > sala2[2]-0.15 && x < sala3[0] && ((z > sala23[1]-0.15 && obsP[2] < z) || (z < sala23[3]+0.15 && obsP[2] > z))) // Porta
		return obsP[2];
	
	// Sala 3
	// Parede de blocos de vidro
	if (x > sala3[2]-2.1-0.15 && x < sala3[2]-1.1+0.15 && z < sala3[3]+2.1+0.15 && z > sala3[3]+1.8 && obsP[2] > z)
		if (sala3[2]-x <= -(sala3[3]-z))
			return obsP[2];
	if (x > sala3[2]-2.1 && x < sala3[2]-1.1+0.15 && z > sala3[3]+1.8-0.15 && z < sala3[3]+2.1 && obsP[2] < z)
		return obsP[2];
	// Parede em T
	if (x > sala3[0]+4.9-0.15 && x < sala3[0]+5+0.15 && z < sala3[1]-4 && z > sala3[1]-4-0.15 && obsP[2] < z)
		return obsP[2];
	if (x > sala3[0]+2-0.15 && x < sala3[2]-2+0.15 && z < sala3[1]-2+0.15 && z > sala3[1]-2.1 && obsP[2] > z)
		return obsP[2];
	if (x > sala3[0]+2-0.15 && x < sala3[2]-2+0.15 && z < sala3[1]-2 && z > sala3[1]-2.1-0.15 && obsP[2] < z)
		return obsP[2];
	// Alvo
	if (x > sala3[2]-1.1 && z < sala3[3]+1.95 && obsP[2] > z)
		return obsP[2];
	// Caixas
	if (x > 5.7 && x < sala3[0]+5 && z > -1.2 && z < -1.1 && obsP[2] < z)
		return obsP[2];
		
	return z;
}

/*
 *	Timer
 */
void Timer(GLint value)
{
	GLint anda = 0;
	
	// Movimento
	GLfloat novoObsP[] = {obsP[0], obsP[1], obsP[2]};
	if (frente)
	{
		novoObsP[2] -= vel * cos(anguloH) * factorResize;
		novoObsP[0] += vel * sin(anguloH) * factorResize;
		anda = 1;
	}
	if (atras)
	{
		novoObsP[2] += vel * cos(anguloH) * factorResize;
		novoObsP[0] -= vel * sin(anguloH) * factorResize;
		anda = 1;
	}
	if (esquerda)
	{
		novoObsP[2] += vel * cos(anguloH + 3.14/2) * factorResize;
		novoObsP[0] -= vel * sin(anguloH + 3.14/2) * factorResize;
		anda = 1;
	}
	if (direita)
	{
		novoObsP[2] -= vel * cos(anguloH + 3.14/2) * factorResize;
		novoObsP[0] += vel * sin(anguloH + 3.14/2) * factorResize;
		anda = 1;
	}
	if (anda > 0)
	{
		passo += vel * 5 * factorResize;
		if (passo > 3.14)
			passo -= 3.14;
	}
	obsP[0] = colisoesX(novoObsP[0], novoObsP[2]);
	obsP[2] = colisoesZ(novoObsP[0], novoObsP[2]);
	
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
