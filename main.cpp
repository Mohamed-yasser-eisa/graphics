//#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include "imageloader.h"
#include <math.h>


static int RightArm = 0, RightArmX=0 ,RightForeArm = 0,LeftArm = 0, LeftArmX = 0,LeftForeArm = 0,RightLeg = 0,RightLegX = 0,
RightTibiaX = 0,LeftLeg = 0,LeftLegX = 0 , LeftTibiaX = 0,LeftFootZ = 0,fingerbase1=0,fingerbase2=0,fingerbase3=0,fingerbase4=0,fingerbase5=0,fingerbase6=0,
fingerUp1=0,fingerUp2=0,fingerUp3=0,fingerUp4=0,fingerUp5=0,fingerUp6=0;


int moving, startx, starty;

float light_ambient[] = {4.0, 4.0, 1.0, 1.0};
float light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
float light_specular[] = {1.0, 1.0, 1.0, 1.0};
float light_position[] = {0.0, 1.0, 4.0, 1.0};

int poses[29][13]={ {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {0,0,0,0,0,0,0,10,-10,0,-10,-10,0},
                   {0,0,0,0,0,0,0,20,-20,0,-20,-20,0 },
                   {0,0,0,0,0,0,0,30,-30,0,-30,-30,0 },
                   {0,0,0,0,0,0,0,40,-40,0,-40,-40,0 },
                   {0,0,0,0,0,0,0,30,-30,0,-30,-30,0 },
                   {0,0,0,0,0,0,0,20,-20,0,-20,-20,0 },
                   {0,0,0,0,0,0,0,10,-10,0,-10,-10,0 },
                   {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {0,0,0,0,0,0,0,-10,-10,0,10,-10,0 },
                   {0,0,0,0,0,0,0,-20,-20,0,20,-20,0 },
                   {0,0,0,0,0,0,0,-30,-30,0,30,-30,0 },
                   {0,0,0,0,0,0,0,-40,-40,0,40,-40,0 },
                   {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {0,0,0,0,0,0,0,0,0,0,0,0,0 },
                   {20,0,0,-20,0,0,10,0,0,-10,0,0,0 },
                   {40,0,0,-40,0,0,20,0,0,-20,0,0,0 },
                   {60,0,0,-60,0,0,30,0,0,-30,0,0,0 },
                   {80,0,0,-80,0,0,40,0,0,-40,0,0,0 },
                   {100,0,0,-100,0,0,50,0,0,-50,0,0,0 },
                   {120,0,0,-120,0,0,60,0,0,-60,0,0,0 },
                   {100,0,0,-100,0,0,50,0,0,-50,0,0,0 },
                   {80,0,0,-80,0,0,40,0,0,-40,0,0,0 },
                   {60,0,0,-60,0,0,30,0,0,-30,0,0,0 },
                   {40,0,0,-40,0,0,20,0,0,-20,0,0,0 },
                   {20,0,0,-20,0,0,10,0,0,-10,0,0,0 }};

GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
double thetaR=0;
double thetaL=0;
double  thetadown=0;
double  thetaup=0;
static float speed=-0.01;
double eye[] = { 0, 0, -15 };
double center[] = { 0, 0, -1 };
double up[] = { 0, 1, 0 };
double direction[3];
double crossProduct_result[3];

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
      GLuint textureId;
      glGenTextures(1, &textureId); //Make room for our texture
      glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
      //Map the image to the texture
      glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                               0,                            //0 for now
                               GL_RGB,                       //Format OpenGL uses for image
                               image->width, image->height,  //Width and height
                               0,                            //The border of the image
                               GL_RGB, //GL_RGB, because pixels are stored in RGB format
                               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                                 //as unsigned numbers
                               image->pixels);               //The actual pixel data
      return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
GLuint _textureId1; //The id of the texture

void crossProduct(double a[], double b[], double c[])
{

	c[0] = a[1] * b[2] - a[2] * b[1];

	c[1] = a[2] * b[0] - a[0] * b[2];

	c[2] = a[0] * b[1] - a[1] * b[0];

}

void normalize(double a[])
{

	double norm;

	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];

	norm = sqrt(norm);

	a[0] /= norm;

	a[1] /= norm;

	a[2] /= norm;

}
void rotatePoint(double a[], double theta, double p[])
{
	double temp[3];

	temp[0] = p[0];

	temp[1] = p[1];

	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];

	temp[1] = a[2] * p[0] - a[0] * p[2];

	temp[2] = -a[1] * p[0] + a[0] * p[1];



	temp[0] *= sin(theta);

	temp[1] *= sin(theta);

	temp[2] *= sin(theta);



	temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);

	temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);

	temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);



	temp[0] += cos(theta)*p[0];

	temp[1] += cos(theta)*p[1];

	temp[2] += cos(theta)*p[2];



	p[0] = temp[0];

	p[1] = temp[1];

	p[2] = temp[2];



}

void Left()
{

    thetaL-=0.001;
    rotatePoint(up,thetaL,eye);

}
void Right()

{
	thetaR+=0.001;
    rotatePoint(up,thetaR,eye);
}
void Up()
{

	crossProduct(up,center,crossProduct_result);
	normalize(crossProduct_result);



    thetaup-=0.001;

	rotatePoint(crossProduct_result,thetaup,up);
	rotatePoint(crossProduct_result,thetaup,eye);


}



void Down()

{
    crossProduct(up,center,crossProduct_result);
	normalize(crossProduct_result);

    thetadown+=0.001;
	rotatePoint(crossProduct_result,thetadown,up);
	rotatePoint(crossProduct_result,thetadown,eye);

}


void moveForward()

{
    direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	eye[0]    -= direction[0] * speed;
	eye[1]    -= direction[1] * speed;
	eye[2]    -= direction[2] * speed;

	center[0] -= direction[0] * speed;
	center[1] -= direction[1] * speed;
	center[2] -= direction[2] * speed;

}



void moveBack()

{

    direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	eye[0]    += direction[0] * speed;
	eye[1]    += direction[1] * speed;
	eye[2]    += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;

}


void setPoses(int framNum)
{
   RightArm=poses[framNum][0];
   RightArmX= poses[framNum][1];
   RightForeArm=poses[framNum][2];
   LeftArm=poses[framNum][3];
   LeftArmX=poses[framNum][4];
   LeftForeArm=poses[framNum][5];
   RightLeg=poses[framNum][6];
   RightLegX=poses[framNum][7];
   RightTibiaX=poses[framNum][8];
   LeftLeg=poses[framNum][9];
   LeftLegX=poses[framNum][10];
   LeftTibiaX=poses[framNum][11];
   LeftFootZ=poses[framNum][12];
}

static int f =0;
void timer(int value)
{
    f=f%29;
    setPoses(f);
    f++;
    glutPostRedisplay();
    glutTimerFunc(150,timer,0);
}



void init(void)

{
    

    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);

 Image* image = loadBMP("floor.bmp");
      	_textureId = loadTexture(image);
      	delete image;
          
}


void display(void)

{

	//glClear(GL_COLOR_BUFFER_BIT );
   	glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_FLAT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

   // glPushMatrix();
	//glTranslatef(0, 0, -1);
	//floor
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBegin(GL_QUADS);

	glNormal3f(0.0,-1.0,0.0);
	glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-7.0,-3.7,2);
        glTexCoord2f(5.0f,  0.0f);
        glVertex3f(7.0,-3.7,2);
        glTexCoord2f(5.0f,  20.0f);
        glVertex3f(10.0,-3.7,-7);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-10.0,-3.7,-7);
        glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

    
    
    
    glPushMatrix();

   glRotatef(angle2, 1.0, 0.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0);


// Drawing Trunk

   glPushMatrix();

   glTranslatef(0.0,1.0,0.0);
   glScalef (2.0,3.0, 0.6); //S1
   glutWireCube(1.0);
   //glutSolidCube(1.0);
   glPopMatrix();

// Drawing the Head
   glPushMatrix();

   glTranslatef(0.0,3.0,0.0);
   glutWireSphere(0.5,30,30);

   glPopMatrix();


glPushMatrix();

   // Drawing Right Arm
   glTranslatef(1.1,2.4,0.0);
   glRotatef ((GLfloat)RightArm, 0.0, 0.0, 1.0); // RightArm
   glRotatef ((GLfloat)RightArmX, 0.0, 1.0, 0.0);
   glTranslatef(-1.1,-2.4,0.0);

   glPushMatrix();

   glTranslatef(1.2,1.8,0);
   glScalef(0.2,1.2,0.3);
   glutWireCube(1.0);
   //glutSolidCube(1.0);
   glPopMatrix();

   // Drawing Right ForeArm

   glTranslatef(1.2,1.2,0.0);
   glRotatef ((GLfloat) RightForeArm, 1.0, 0.0, 0.0); //RightForeArm
   glTranslatef(-1.2,-1.2,0.0);

   glPushMatrix();

   glTranslatef(1.2,0.6,0);
   glScalef(0.2,1.2,0.3);
   glutWireCube(1.0);
  //glutSolidCube(1.0);
   glPopMatrix();

   // Drawing Finger1

glPushMatrix();

    glTranslatef(1.3,0.0,0.1);
    glRotatef ((GLfloat) fingerbase1, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);

  // Drawing base1

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip1
    glTranslatef(0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp1, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();

  //Drawing Finger2

glPushMatrix();

    glTranslatef(1.3,0.0,-0.1);
    glRotatef ((GLfloat) fingerbase2, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);

  // Drawing base2

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip2
    glTranslatef(0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp2, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();

//Drawing Finger3

glPushMatrix();

    glTranslatef(1.15,0.0,-0.05);
    glRotatef ((GLfloat) fingerbase3, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);

  // Drawing base3

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
       glutWireCube(1.0);
       //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip3
    glTranslatef(0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp3, 0.0, 0.0, 1.0); //finger1
    glTranslatef(-0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
       //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();

glPopMatrix();


glPushMatrix();

   // Drawing Left Arm
   glTranslatef(-1.1,2.4,0.0);
   glRotatef ((GLfloat)LeftArm, 0.0, 0.0, 1.0); // LeftArm
   glRotatef ((GLfloat)LeftArmX, 0.0, 1.0, 0.0);
   glTranslatef(1.1,-2.4,0.0);

   glPushMatrix();

   glTranslatef(-1.2,1.8,0);
   glScalef(0.2,1.2,0.3);
   glutWireCube(1.0);
   //glutSolidCube(1.0);
   glPopMatrix();

   // Drawing Left ForeArm

   glTranslatef(-1.2,1.2,0.0);
   glRotatef ((GLfloat) LeftForeArm, 1.0, 0.0, 0.0); //LeftForeArm
   glTranslatef(1.2,-1.2,0.0);

   glPushMatrix();

   glTranslatef(-1.2,0.6,0);
   glScalef(0.2,1.2,0.3);
   glutWireCube(1.0);
   //glutSolidCube(1.0);

   glPopMatrix();


     // Drawing Finger4

glPushMatrix();

    glTranslatef(-1.3,0.0,0.1);
    glRotatef ((GLfloat) fingerbase4, 0.0, 0.0, 1.0); //finger1
    glTranslatef(0.025,-0.1,0.0);

  // Drawing base4

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip4
    glTranslatef(-0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp4, 0.0, 0.0, 1.0); //finger1
    glTranslatef(0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();

  //Drawing Finger5

glPushMatrix();

    glTranslatef(-1.3,0.0,-0.1);
    glRotatef ((GLfloat) fingerbase5, 0.0, 0.0, 1.0); //finger5
    glTranslatef(0.025,-0.1,0.0);

  // Drawing base5

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip5
    glTranslatef(-0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp5, 0.0, 0.0, 1.0); //finger5
    glTranslatef(0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();

//Drawing Finger6

glPushMatrix();

    glTranslatef(-1.1,0.0,-0.05);
    glRotatef ((GLfloat) fingerbase6, 0.0, 0.0, 1.0); //finger6
    glTranslatef(-0.025,-0.1,0.0);

  // Drawing base6

    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

  // Drawing the tip6
    glTranslatef(-0.025,-0.1,0);
    glRotatef ((GLfloat) fingerUp6, 0.0, 0.0, 1.0); //finger6
    glTranslatef(0.025,-0.1,0.0);


    glPushMatrix();
        glScalef(0.05,0.2,0.1);
        glutWireCube(1.0);
        //glutSolidCube(1.0);
    glPopMatrix();

glPopMatrix();


glPopMatrix();

glPushMatrix();

// Drawing left Leg

   glTranslatef(-0.75,-0.50,0.0);

   glRotatef ((GLfloat) LeftLeg, 0.0, 0.0, 1.0); //LeftLeg
   glRotatef ((GLfloat) LeftLegX, 1.0, 0.0, 0.0); //LeftLeg

   glTranslatef(0.75,0.50,0.0);

   glPushMatrix();

   glTranslatef(-0.75,-1.5,0);
   glScalef(0.5,2.0,0.6);
   glutWireCube(1.0);
   //glutSolidCube(1.0);

   glPopMatrix();


// Drawing (Left tibia)

   glTranslatef(-0.75,-2.5,0.0);
   glRotatef ((GLfloat) LeftTibiaX, 1.0, 0.0, 0.0); //LeftTibiaX
   glTranslatef(0.75,2.5,0.0);


   glPushMatrix();
   glTranslatef(-0.75,-3,0);
   glScalef(0.5,1.0,0.6);
   glutWireCube(1.0);
   //glutSolidCube(1.0);

   glPopMatrix();


// Drawing Left Foot

   glPushMatrix();

   glTranslatef(-0.75,-3.5,0);
   glScalef(0.7,0.3,1.2);
   glutSolidCube(1.0);

   glPopMatrix();

glPopMatrix();


glPushMatrix();

// Drawing Right Leg1

   glTranslatef(0.75,-0.50,0.0);

   glRotatef ((GLfloat) RightLeg, 0.0, 0.0, 1.0); //RightLeg
   glRotatef ((GLfloat) RightLegX, 1.0, 0.0, 0.0); //RightLeg

   glTranslatef(-0.75,0.50,0.0);

   glPushMatrix();

   glTranslatef(0.75,-1.5,0);
   glScalef(0.5,2.0,0.6);
   glutWireCube(1.0);
  //glutSolidCube(1.0);
   glPopMatrix();


// Drawing (Right tibia)


   glTranslatef(0.75,-2.5,0.0);
   glRotatef ((GLfloat) RightTibiaX, 1.0, 0.0, 0.0); //RightTibiaX
   glTranslatef(-0.75,2.5,0.0);

   glPushMatrix();

   glTranslatef(0.75,-2.25,0);
   glTranslatef(0,-0.75,0);
   glScalef(0.5,1.0,0.6);
   glutWireCube(1.0);
   //glutSolidCube(1.0);

   glPopMatrix();


// Drawing Right Foot

   glPushMatrix();

   glTranslatef(0.75,-3.5,0);
   glScalef(0.7,0.3,1.2);
   glutSolidCube(1.0);

   glPopMatrix();

glPopMatrix();

glPopMatrix();

   glPopMatrix();


	glutSwapBuffers();

}



void specialKeys(int key, int x, int y)

{

	switch (key)

	{

	case GLUT_KEY_LEFT: Left(); break;

	case GLUT_KEY_RIGHT: Right(); break;

	case GLUT_KEY_UP:    Up(); break;

	case GLUT_KEY_DOWN:  Down(); break;

	}



	glutPostRedisplay();

}



void keyboard(unsigned char key, int x, int y)

{
	switch (key)

	{
	case '5':

		moveForward();

		glutPostRedisplay();

		break;

	case '2':

		moveBack();

		glutPostRedisplay();

		break;
	case 'a':
    if (RightArm<=90)
    {
      RightArm = (RightArm + 5) % 360;
      glutPostRedisplay();
    }
      break;

   case 'A':
       if(RightArm>=5)
       {
          RightArm = (RightArm - 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 's':

    if (LeftArm >= -90)
    {
        LeftArm = (LeftArm - 5) % 360;
        glutPostRedisplay();
    }
      break;

   case 'S':

        if (LeftArm <=0 )
        {
            LeftArm = (LeftArm + 5) % 360;
            glutPostRedisplay();
        }
      break;


   case 'd':
       if(RightForeArm<=100)
       {
           RightForeArm = (RightForeArm + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'D':
       if (RightForeArm>=0)
       {
           RightForeArm = (RightForeArm - 5) % 360;
           glutPostRedisplay();
       }
      break;

   case 'z':
       if (LeftForeArm<=0)
       {
           LeftForeArm = (LeftForeArm + 5) % 360;
            glutPostRedisplay();
       }

      break;

   case 'Z':
       if (LeftForeArm>=-100)
       {
          LeftForeArm = (LeftForeArm - 5) % 360;
          glutPostRedisplay();
       }
      break;


   case 'x':
      if(LeftLeg<=5)
       {
           LeftLeg = (LeftLeg + 5) %360 ;
            glutPostRedisplay();
       }
      break;
   case 'X':
      if (LeftLeg>=-90)
       {
           LeftLeg =(LeftLeg - 5) % 360;
           glutPostRedisplay();
       }
      break;

   case 'c':
       if(RightLeg<=90)
       {
          RightLeg = (RightLeg + 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 'C':
       if (RightLeg>=0)
       {

          RightLeg =(RightLeg - 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 'q':
       if(LeftTibiaX<=70)
       {
          LeftTibiaX = (LeftTibiaX + 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 'Q':
       if(LeftTibiaX>=0)
       {
          LeftTibiaX = (LeftTibiaX - 5) % 360;
          glutPostRedisplay();
       }
      break;


    case 'w':
      if(RightTibiaX<=70)
       {
           RightTibiaX = (RightTibiaX + 5) %360 ;
            glutPostRedisplay();
       }
      break;
   case 'W':
      if (RightTibiaX>=0)
       {
           RightTibiaX = (RightTibiaX - 5) % 360;
           glutPostRedisplay();
       }

      break;

  case 'e':
       if(LeftLegX<=90)
       {
          LeftLegX = (LeftLegX + 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 'E':
       if(LeftLegX>=-90)
       {
          LeftLegX = (LeftLegX - 5) % 360;
          glutPostRedisplay();
       }
      break;


     case 'r':
       if(RightLegX<=90)
       {
          RightLegX = (RightLegX + 5) % 360;
          glutPostRedisplay();
       }
      break;

   case 'R':
       if(RightLegX>=-90)
       {
          RightLegX = (RightLegX - 5) % 360;
          glutPostRedisplay();
       }
      break;


      case 'f':
          RightArmX = (RightArmX + 5) % 360;
          glutPostRedisplay();
      break;

     case 'F':
         RightArmX = (RightArmX - 5) % 360;
          glutPostRedisplay();
      break;

      case 'v':
          LeftArmX = (LeftArmX + 5) % 360;
          glutPostRedisplay();
      break;

     case 'V':
         LeftArmX = (LeftArmX - 5) % 360;
          glutPostRedisplay();
      break;


      //finger 1
   case 'p':
       if(fingerbase1<=45)
       {
           fingerbase1 = (fingerbase1 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'P':
       if (fingerbase1>=-45)
       {
           fingerbase1 = (fingerbase1 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'o':
       if (fingerUp1<=45)
       {
            fingerUp1 = (fingerUp1 + 5) % 360;
            glutPostRedisplay();
       }

      break;

   case 'O':
       if (fingerUp1>=-45)
       {
          fingerUp1 = (fingerUp1 - 5) % 360;
          glutPostRedisplay();
       }
      break;


      //finger 2
   case 'i':
       if(fingerbase2<=45)
       {
           fingerbase2 = (fingerbase2 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'I':
       if (fingerbase2>=-45)
       {
           fingerbase2 = (fingerbase2 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'u':
       if (fingerUp2<=45)
       {
            fingerUp2 = (fingerUp2 + 5) % 360;
            glutPostRedisplay();
       }

      break;

   case 'U':
       if (fingerUp2>=-45)
       {
          fingerUp2 = (fingerUp2 - 5) % 360;
          glutPostRedisplay();
       }
      break;


      //finger 3
   case 'l':
       if(fingerbase3<=45)
       {
           fingerbase3 = (fingerbase3 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'L':
       if (fingerbase3>=-45)
       {
           fingerbase3 = (fingerbase3 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'k':
       if (fingerUp3<=45)
       {
            fingerUp3 = (fingerUp3 + 5) % 360;
            glutPostRedisplay();
       }

      break;

    case 'K':
       if (fingerUp3>=-45)
       {
          fingerUp3 = (fingerUp3 - 5) % 360;
          glutPostRedisplay();
       }
      break;

      //finger 4
   case 'b':
       if(fingerbase4<=45)
       {
           fingerbase4 = (fingerbase4 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'B':
       if (fingerbase4>=-45)
       {
           fingerbase4 = (fingerbase4 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'n':
       if (fingerUp4<=45)
       {
            fingerUp4 = (fingerUp4 + 5) % 360;
            glutPostRedisplay();
       }

      break;

   case 'N':
       if (fingerUp4>=-45)
       {
          fingerUp4 = (fingerUp4 - 5) % 360;
          glutPostRedisplay();
       }
      break;


      //finger 5
   case 'm':
       if(fingerbase5<=45)
       {
           fingerbase5 = (fingerbase5 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'M':
       if (fingerbase5>=-45)
       {
           fingerbase5 = (fingerbase5 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'g':
       if (fingerUp5<=45)
       {
            fingerUp5 = (fingerUp5 + 5) % 360;
            glutPostRedisplay();
       }

      break;

   case 'G':
       if (fingerUp5>=-45)
       {
          fingerUp5= (fingerUp5 - 5) % 360;
          glutPostRedisplay();
       }
      break;


      //finger 6

   case 'h':
       if(fingerbase6<=45)
       {
           fingerbase6 = (fingerbase6 + 5) %360 ;
            glutPostRedisplay();
       }

      break;
   case 'H':
       if (fingerbase6>=-45)
       {
           fingerbase6 = (fingerbase6 - 5) % 360;
           glutPostRedisplay();
       }

      break;


    case 'j':
       if (fingerUp6<=45)
       {
            fingerUp6 = (fingerUp6 + 5) % 360;
            glutPostRedisplay();
       }

      break;

    case 'J':
       if (fingerUp6>=-45)
       {
          fingerUp6 = (fingerUp6 - 5) % 360;
          glutPostRedisplay();
       }
      break;

	default:

		break;

	}

}

static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}


static void motion(int x, int y)
{
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}


int main(int argc, char **argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("body");

	init();

	glutMouseFunc(mouse);

    glutMotionFunc(motion);

	glutDisplayFunc(display);

    glutSpecialFunc(specialKeys);

	glutKeyboardFunc(keyboard);

	glutTimerFunc(0,timer,0);


	glutMainLoop();

	return 0;
}
