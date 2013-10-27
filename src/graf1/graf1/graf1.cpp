// graf1.cpp: определяет точку входа для консольного приложения.
// Лабораторная работа №4 Компьютерная Графика Мельник А. В. ДА-82
//

#include "stdafx.h"

#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "gl/glut.h"
#include <math.h>
#include "gl/glaux.h"

#pragma comment(lib, "glaux.lib")

//-----------------------------------------------------------------------

//Горячая ESC клавиша для выхода из программы
#define ESCAPE '\033'

//-----------------------------------------------------------------------

bool fl = true;
float cx=40,cy=0,cz=20,t=0,t_l=0.5;
GLfloat V[8][3]; // Для x и y и z
GLfloat C[8][3]; // Для R, G и B
float scale=1,rot=0,movex=0;
float startx=0,starty=0,startz=0;
int i, ShadeModel=GL_SMOOTH;	

GLint Width = 600, Height = 600;

GLfloat lightZeroPosition[4] = {5.0, 5.0, 5.0, 1.0};
GLfloat lightSpotDirection[4] = {5.0, 5.0, 5.0, 1.0}; 

GLfloat lightAmbientColor[4] = {0.3, 0.3, 0.3, 1.0};  
GLfloat lightDiffuseColor[4] = {0.5, 0.5, 0.5, 0.8};  
GLfloat lightSpecularColor[4] ={0.6, 0.6, 0.6, 0.5};  

GLfloat m[16]={1.0, 0.0, 0.0, 0.0,
			   0.0, 1.0, 0.0, 0.0,
			   0.0, 0.0, 1.0, 0.0,
			   0.0, 0.0, 0.0, 1.0,};

GLfloat mat_dif[] = {0.3, 0.3, 0.3, 1.0};
GLfloat mat_amb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_spec[]= {0.7, 0.5, 0.3, 0.7};
GLfloat mat_emis[]= {0.1, 0.1, 0.1, 0.5};
GLfloat shininess = 0.6 * 128;

//Определяем текстуру
GLuint texture[1];
GLUquadricObj *quadObj;	


GLvoid LoadGLTextures()
{
// Загрузка картинки
	AUX_RGBImageRec *texture1;
	texture1 = auxDIBImageLoad(L"text.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
// Создаем текстуру
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0,
					GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}

void init(void)
{
	LoadGLTextures();
	
	//Управление режимом интерполяции
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable( GL_LIGHTING);
 	glEnable(GL_NORMALIZE);
	glMaterialfv (GL_FRONT_AND_BACK,GL_AMBIENT, mat_amb);
	glMaterialfv (GL_FRONT_AND_BACK,GL_DIFFUSE, mat_dif);
	glMaterialfv (GL_FRONT_AND_BACK,GL_SPECULAR, mat_spec);
	glMaterialfv (GL_FRONT_AND_BACK,GL_EMISSION, mat_emis);
	glMaterialf  (GL_FRONT,GL_SHININESS, shininess);

}

void Display(void)
{
	glClearColor(0, 120, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();  
	glShadeModel (ShadeModel);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lightAmbientColor);

	glDisable(GL_LIGHT0);
	glTranslated(lightZeroPosition[0],lightZeroPosition[2],lightZeroPosition[1]);
	glColor3ub(255,10,10);
	glutSolidSphere(3,20,20);
	glTranslated(-lightZeroPosition[0],-lightZeroPosition[2],-lightZeroPosition[1]);

	//Тип света
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR,lightSpecularColor);
	//Позиция света
	glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,0.0005);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 110);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightSpotDirection);

	glEnable(GL_LIGHT0); 

	//Позиция камеры	 
	gluLookAt(startx+cx+40,starty+cy+15,startz+cz+20,  20,5,10,  0,1,0);
	
	
	
	//Трансформируем
	for(i=0;i<15;i++)
		m[i]=0; 

	//Scale
	m[0]=scale; m[5]=scale; m[10]=scale; 
	//Move
	m[13]=movex;
	glMultMatrixf(m);

	//Поворот
	m[0]=1; m[5]=m[10]=cos(rot*3.1416/180);  
	m[6]=sin(rot*3.1416/180); m[9]=-m[6];
	m[12]=m[13]=m[14]=0;
	glMultMatrixf(m);
	
	//Рисуем плоскость
	glBegin(GL_QUADS);
		glColor3ub(255,215,0);
		glVertex3f(-200,0,-200);
		glVertex3f(-200,0,200);
		glVertex3f(200,0,200);
		glVertex3f(200,0,-200);
	glEnd(); 
			
	//Подключаем текстуру
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTranslated(10,5,10);
	glColor4ub(255,255,255,1);
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	
	auxSolidDodecahedron(5);
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);	//Отключаем
	gluDeleteQuadric(quadObj);

	glColor4ub(255,0,163,1);
	glTranslated(20,0,0);

	//Текстурирование
	glColor4f(1,1,1,1);
	glTranslated(-10,0,10);
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);

	if(fl == true)
	{
		V[0][0] = 2; V[0][1] = -2; V[0][2] = 0;
		V[1][0] = 2; V[1][1] = 2; V[1][2] = 0;
		V[2][0] = 0; V[2][1] = 0; V[2][2] = 2.5;
		V[3][0] = -2; V[3][1] = 2; V[3][2] = 0;
		V[4][0] = -2; V[4][1] = -2; V[4][2] = 0;
		V[5][0] = 0; V[5][1] = 0; V[5][2] = -2.5;
		V[6][0] = 2; V[6][1] = -2; V[6][2] = 0;
		V[7][0] = 2; V[7][1] = 2; V[7][2] = 0;
		C[0][0] = 1.0; C[0][1] = 0.0; C[0][2] = 0.0;
		C[1][0] = 1.0; C[1][1] = 0.0; C[1][2] = 0.0;
		C[2][0] = 0.0; C[2][1] = 0.0; C[2][2] = 1.0;
		C[3][0] = 1.0; C[3][1] = 1.0; C[3][2] = 0.0;
		C[4][0] = 1.0; C[4][1] = 0.0; C[4][2] = 0.0;
		C[5][0] = 0.0; C[5][1] = 0.0; C[5][2] = 1.0;
		C[6][0] = 1.0; C[6][1] = 0.0; C[6][2] = 0.0;
		C[7][0] = 0.0; C[7][1] = 0.0; C[7][2] = 1.0;
		glVertexPointer(3, GL_FLOAT, 0, V);
		glColorPointer(3, GL_FLOAT, 0, C);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	//-----------------------------------------------
	
	if(fl == false)
	{
		glColor3ub(150,100,0);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(2,-2,0);
			glVertex3f(2,2,0);
			glVertex3f(0,0,2.84);
			glVertex3f(-2,2,0);
			glVertex3f(-2,-2,0);
			glVertex3f(0,0,-2.84);
			glVertex3f(2,-2,0);
			glVertex3f(2,2,0);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0,0,-2.84);
			glVertex3f(2,-2,0);
			glVertex3f(2,2,0);
			glVertex3f(0,0,2.84);
		glEnd();

		float x1=2, y1=2, x2=-2,y2=2, x3=-2, y3=-2, x4=2, y4=-2, z5=5+2.84, z6=5-2.84;
		float z1=5, z2=5, z3=5, z4=5;
		float LZP=lightZeroPosition[0]-20;float LZP2=lightZeroPosition[2]-20;
		float LZP1=lightZeroPosition[1];
		float X1=-z1*(x1-LZP)/(z1-LZP1)+x1,
		Y1=-z1*(y1-LZP2)/(z1-LZP1)+y1,
		X2=-z2*(x2-LZP)/(z2-LZP1)+x2,
		Y2=-z2*(y2-LZP2)/(z2-LZP1)+y2,
		X3=-z3*(x3-LZP)/(z3-LZP1)+x3,
		Y3=-z3*(y3-LZP2)/(z3-LZP1)+y3,
		X4=-z4*(x4-LZP)/(z4-LZP1)+x4,
		Y4=-z4*(y4-LZP2)/(z4-LZP1)+y4,
		X5=-z5*(0-LZP)/(z5-LZP1),
		Y5=-z5*(0-LZP2)/(z5-LZP1),
		X6=-z6*(0-LZP)/(z6-LZP1),
		Y6=-z6*(0-LZP2)/(z6-LZP1);

		glColor3ub(0,0,0);
		glTranslated(0,-4.9,0);

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(X1,0,Y1);
			glVertex3f(X2,0,Y2);
			glVertex3f(X4,0,Y4);
			glVertex3f(X3,0,Y3);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(X1,0,Y1);
			glVertex3f(X5,0,Y5);
			glVertex3f(X6,0,Y6);
			glVertex3f(X3,0,Y3);
		glEnd();	
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(X2,0,Y2);
			glVertex3f(X5,0,Y5);
			glVertex3f(X6,0,Y6);
			glVertex3f(X4,0,Y4);
		glEnd();
	}
		
	//-----------------------------------------------

	glFlush();
	glutSwapBuffers();
}

void Reshape(GLint w, GLint h)
{
    glViewport(0, 0, w, h);
 	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30,1,3,2000);
    glMatrixMode(GL_MODELVIEW);
}

void Keyboard( unsigned char key, int x, int y )
{
   float stepx=3,stepy=3,stepz=3;
	switch (key) {
      case ESCAPE:
         exit(0);
         break;
	  case '7':scale/=1.5;
		  printf("7");
		  glutPostRedisplay();
		       break;
	  case '9':scale*=1.5;
		  printf("9");
		       break;
	  case '4':rot-=5;
		  printf("4");
		  glutPostRedisplay();
		       break;
	  case '6':rot+=5;
		  printf("6");
		  glutPostRedisplay();
		       break;
	  case '1':movex-=5;
		  printf("1");
		  glutPostRedisplay();
		       break;
	  case '3':movex+=5;
		  printf("3");
		  glutPostRedisplay();
		       break;
	  case '0':scale=1,rot=0,movex=0;
		  startx=0,starty=0,startz=0;
		  printf("0");
		  glutPostRedisplay();
		       break;
	  case '-':					//change shade model
		  if(ShadeModel==GL_SMOOTH) 
			  ShadeModel=GL_FLAT;  
		  else                
			  ShadeModel=GL_SMOOTH;
		  printf("-");
		  glutPostRedisplay();
		       break;
	  case 'w':
		  starty=starty+stepy;
	    glutPostRedisplay();
			break;
	  case 's':
		  starty=starty-stepy;
		glutPostRedisplay();
		break;
	  case 'a':
		  startx=startx-stepx;
	    glutPostRedisplay();
		break;
	  case 'd':
		 startx=startx+stepx;
		glutPostRedisplay();
		break;
	  case 'f':
		 if(fl == true) fl = false;
		 else fl = true;
		glutPostRedisplay();
		break;
   }
}

void idlef()
{
	static int busy=1;
	if(busy==1)
	{
		busy=0;
		t+=0.001;/*
		cx=70*cos(t*2*3.147);
		cz=70*sin(t*2*3.147);
		cy=10;*/

		glutPostRedisplay();
		busy=1;
	}
}

void timerf(int value)
{
//		t+=0.01;
		lightZeroPosition[0]=3*cos(-t*0.5*3.147)+20;
		lightZeroPosition[1]=10;
		lightZeroPosition[2]=3*sin(-t*0.5*3.147)+20;

		glutPostRedisplay();
		glutTimerFunc(50,timerf,1);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GL_DOUBLE);
    glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);
    glutCreateWindow("Лабораторная работа номер 4. Мельник А. В. ДА-82");
	init();
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
	
	//----------------------
	glutIdleFunc(idlef);
	glutTimerFunc(50,timerf,1);
	//-----------------------

    glutMainLoop();

	return 0;
}
