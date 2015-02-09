#include <iostream>
#include <cstring>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

using namespace std;

class Ball
{
	private:
		GLfloat _X,_Y,_rad;
		GLfloat _VelX,_VelY;
		GLint _mass;
		GLint _color[5];
	public:
		GLfloat getX()
		{
			return _X;
		}
		void setX(GLfloat X)
		{
			_X=X;
			return;
		}
		GLfloat getY()
		{
			return _Y;
		}
		void setY(GLfloat Y)
		{
			_Y=Y;
			return;
		}
		GLfloat getRad()
		{
			return _rad;
		}
		void setRad(GLfloat rad)
		{
			_rad=rad;
			return;
		}
		GLfloat getVelX()
		{
			return _VelX;
		}
		void setVelX(GLfloat VelX)
		{
			_VelX=VelX;
			return;
		}
		GLfloat getVelY()
		{
			return _VelY;
		}
		void setVelY(GLfloat VelY)
		{
			_VelY=VelY;
			return;
		}
		GLint getColor()
		{
			int i;
			for(i=0;i<=4;i++)
				if(_color[i]==1)
					return i;
			return i;
		}
		void setColor(int i)
		{
			_color[i]=1;
			return;
		}
		GLint getMass()
		{
			return _mass;
		}
		void setMass(GLint mass)
		{
			_mass=mass;
			return;
		}
};
Ball ball[12];

void initRendering();
void startGame();
void drawBoard(GLfloat x);
void drawBall(GLfloat r);
void update(GLint value);
void drawMarker();
void showDirection(GLfloat x,GLfloat y);
void drawCoin(GLint i);
void handleKeypress1(unsigned char key, GLint x, GLint y);
void handleKeypress2(GLint key, GLint x, GLint y);
void handleMouseclick(GLint button, GLint state, GLint x, GLint y);
void handleResize(GLint width, GLint height);
void scoredec(int value);

GLfloat power=-0.6f,markX=0.0f,markY=0.0f,theta=0.0,Bpadding=0.05f,Wpadding=0.05f;
GLint flag=1,first=0,score=30,toPocket=0;

GLint main(GLint argc,char **argv)
{
	glutInit(&argc,argv);
	glutTimerFunc(1000,scoredec,0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	GLint w = glutGet(GLUT_SCREEN_WIDTH);
	GLint h = glutGet(GLUT_SCREEN_HEIGHT);
	GLint windowWidth=w*2/3;
	GLint windowHeight=h*2/3;
	glutInitWindowSize(600,540);
	glutInitWindowPosition((w-windowWidth)/2,(h-windowHeight)/2);
	glutCreateWindow("Ball collision");
	initRendering();
	glutDisplayFunc(startGame);
	glutKeyboardFunc(handleKeypress1);
	glutReshapeFunc(handleResize);
	glutIdleFunc(startGame);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	if(flag==3)
		glutTimerFunc(25, update, 0);

	glutMainLoop();
	return 0;
}

void scoredec(int value){
	score = score-1;
	glutTimerFunc(1000,scoredec,0);
	

}
void initRendering()
{
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Setting up Black and White Coins
	GLfloat angle=0.0f;
	for(GLint i=1;i<=8;i++)
	{
		glPushMatrix();
		ball[i].setX(0.1*cos(angle));
		ball[i].setY(0.1*sin(angle));
		ball[i].setVelX(0);
		ball[i].setVelY(0);
		ball[i].setMass(5);
		glTranslatef(ball[i].getX(),ball[i].getY(),0.0f);
		ball[i].setRad(0.02f);
		drawBall(ball[i].getRad());
		glPopMatrix();
		angle+=PI/4;
	}
	// Red Coin
	ball[9].setX(0.0f);
	ball[9].setY(0.0f);
	ball[9].setVelX(0);
	ball[9].setVelY(0);
	ball[9].setRad(0.02f);
	ball[9].setColor(2);
	ball[9].setMass(5);

	// Striker
	ball[10].setX(0.0f);
	ball[10].setY(-0.38f);
	ball[10].setVelX(0);
	ball[10].setVelY(0);
	ball[10].setRad(0.03f);
	ball[10].setColor(1);
	ball[10].setMass(15);
}
void startGame()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f);

	// Outter Rectangle
	glColor3f(0.211f,0.211f,0.211f);
	drawBoard(0.6f);

	// Inner Rectangle
	glColor3f(1.0f,0.8f,0.45f);
	drawBoard(0.55f);

	// Holes
	for(GLint i=0;i<4;i++)
	{
		glPushMatrix();
		switch(i)
		{
			case 0:
				glTranslatef( 0.515f,  0.515f, 0.0f);
				break;
			case 1:
				glTranslatef(-0.515f, -0.515f, 0.0f);
				break;
			case 2:
				glTranslatef( 0.515f, -0.515f, 0.0f);
				break;
			case 3:
				glTranslatef(-0.515f,  0.515f, 0.0f);
				break;
		}
		glColor3f(0.0f,0.0f,0.0f);
		drawBall(0.035f);
		glPopMatrix();
	}

	// Left and Right Rectangle bars
	for(GLint i=1;i<3;i++)
	{
		glPushMatrix();
		glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_LINES);
		glVertex2f(pow(-1,i)*0.4f,pow(-1,i)*0.35f);
		glVertex2f(pow(-1,i)*0.36f,pow(-1,i)*0.35f);
		glVertex2f(pow(-1,i)*0.36f,pow(-1,i)*0.35f);
		glVertex2f(pow(-1,i)*0.36f,pow(-1,i+1)*0.35f);
		glVertex2f(pow(-1,i)*0.36f,pow(-1,i+1)*0.35f);
		glVertex2f(pow(-1,i)*0.4f,pow(-1,i+1)*0.35f);
		glVertex2f(pow(-1,i)*0.4f,pow(-1,i+1)*0.35f);
		glVertex2f(pow(-1,i)*0.4f,pow(-1,i)*0.35f);
		glEnd();
		glPopMatrix();

		// Red Circles
		for(GLint j=1;j<3;j++)
		{
			glPushMatrix();
			switch(j)
			{
				case 1:
					glTranslatef(pow(-1,i)*0.38f,pow(-1,i)*0.35f, 0.0f);
					break;
				case 2:
					glTranslatef(pow(-1,i)*0.38f,pow(-1,i+1)*0.35f, 0.0f);
					break;
			}
			glColor3f(0.49f,0.003f,0.094f);
			drawBall(0.02f);
			glPopMatrix();
		}
	}

	// Upper and Lower Rectangle Bars
	for(GLint i=1;i<3;i++)
	{
		glPushMatrix();
		glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_LINES);
		glVertex2f(pow(-1,i)*0.35f,pow(-1,i+1)*0.4f);
		glVertex2f(pow(-1,i)*0.35f,pow(-1,i+1)*0.36f);
		glVertex2f(pow(-1,i)*0.35f,pow(-1,i+1)*0.36f);
		glVertex2f(pow(-1,i+1)*0.35f,pow(-1,i+1)*0.36f);
		glVertex2f(pow(-1,i+1)*0.35f,pow(-1,i+1)*0.36f);
		glVertex2f(pow(-1,i+1)*0.35f,pow(-1,i+1)*0.4f);
		glVertex2f(pow(-1,i+1)*0.35f,pow(-1,i+1)*0.4f);
		glVertex2f(pow(-1,i)*0.35f,pow(-1,i+1)*0.4f);
		glEnd();
		glPopMatrix();

		// Red Circles
		for(GLint j=1;j<3;j++)
		{
			glPushMatrix();
			switch(j)
			{
				case 1:
					glTranslatef(pow(-1,i)*0.35f,pow(-1,i+1)*0.38f, 0.0f);
					break;
				case 2:
					glTranslatef(pow(-1,i+1)*0.35f,pow(-1,i+1)*0.38f, 0.0f);
					break;
			}
			glColor3f(0.49f,0.003f,0.094f);
			drawBall(0.02f);
			glPopMatrix();
		}
	}

	// Center Circle
	glColor3f(0.49f,0.003f,0.094f);
	glBegin(GL_LINE_LOOP);
	GLint i=0;
	for(i=0;i<360;i++) 
	{
		glVertex2f(0.1*cos(DEG2RAD(i)),0.1*sin(DEG2RAD(i)));
	}
	glEnd();

	// Draw Tray
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex2f(0.76f,0.1f);
	glVertex2f(0.84f,0.1f);
	glVertex2f(0.84f,0.1f);
	glVertex2f(0.84f,0.4f);
	glVertex2f(0.76f,0.1f);
	glVertex2f(0.76f,0.4f);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0.76f,-0.4f);
	glVertex2f(0.84f,-0.4f);
	glVertex2f(0.84f,-0.4f);
	glVertex2f(0.84f,-0.1f);
	glVertex2f(0.76f,-0.4f);
	glVertex2f(0.76f,-0.1f);
	glEnd();
	glPopMatrix();

	// Draw Coins
	for(GLint i=1;i<=10;i++)
	{
		drawCoin(i);
	}

	// Draw  Cross Marker
	if(flag<3)
		drawMarker();

	// Show direction
	if(flag<3)
	{
		glColor3f(0.0f,0.0f,0.0f);
		showDirection(markX,markY);
	}

	// Show Power
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex2f(-0.85f,-0.6f);
	glVertex2f(-0.75f,-0.6f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex2f(-0.75f, 0.6f);
	glVertex2f(-0.85f, 0.6f);
	glEnd();
	glPopMatrix();

	// Show Power Marker
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex2f(-0.9f,power);
	glVertex2f(-0.7f,power);
	glEnd();

	// Score Display
	for(i=1;i<3;i++)
	{
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		std::string str,s;
		void * font = GLUT_BITMAP_TIMES_ROMAN_24;
		switch(i)
		{
			case 1:
				str = "Score :";
				s = std::to_string(score);
				glRasterPos2f(0.5f, 0.8f);
				for (i = 0; i != 6; i++) 
				{                   
					char c = str[i];                  
					glutBitmapCharacter(font, c);       
				}                                               
				for (i = 0; i != 2; i++)
				{       
					char c = s[i];  
					glutBitmapCharacter(font, c);
				}
			case 2:
				str = "To be Pocketed : ";
				if(toPocket==3)
					s="White";
				else if(toPocket==4)
					s="Black";
				glRasterPos2f(-0.55f, 0.8f);
				for (i = 0; i != 17; i++) 
				{                   
					char c = str[i];                  
					glutBitmapCharacter(font, c);       
				}   
				if(toPocket!=0)
				{
					for (i = 0; i != 5; i++)
					{       
						char c = s[i];  
						glutBitmapCharacter(font, c);
					}
				}
			case 3:
				font = GLUT_BITMAP_9_BY_15;
				str = "White";
				glRasterPos2f(0.75f, 0.0f);
				for (i = 0; i != 5; i++) 
				{                   
					char c = str[i];                  
					glutBitmapCharacter(font, c);       
				}
			case 4:
				font = GLUT_BITMAP_9_BY_15;
				str = "Black";
				glRasterPos2f(0.75f, -0.5f);
				for (i = 0; i != 5; i++) 
				{                   
					char c = str[i];                  
					glutBitmapCharacter(font, c);       
				}

		}
		glPopMatrix();
	}
	glutSwapBuffers();
}
void drawCoin(GLint i)
{
	glPushMatrix();
	glTranslatef(ball[i].getX(),ball[i].getY(),0.0f);
	if(i<9)
	{
		if(i%2==1)			// Black Coin
		{
			glColor3f(0.14f,0.07f,0.07f);
			drawBall(ball[i].getRad());
			glColor3f(0.467f,0.333f,0.325f);
			drawBall(ball[i].getRad()-0.01f);
			ball[i].setColor(4);
		}
		else				// White Coin
		{
			//glColor3f(0.831f,0.737f,0.372f);
			glColor3f(0.658f,0.454f,0.011f);
			drawBall(ball[i].getRad());
			glColor3f(0.839f,0.627f,0.168f);
			drawBall(ball[i].getRad()-0.01f);
			ball[i].setColor(3);
		}
	}
	else if(i==9)			// Red Coin
	{
		glColor3f(0.96f,0.10f,0.10f);
		drawBall(ball[i].getRad());
		glColor3f(0.49f,0.003f,0.094f);
		drawBall(ball[i].getRad()-0.01f);
	}
	else					// Striker
	{
		glColor3f(0.11f,0.035f,0.58f);
		drawBall(ball[i].getRad());
		glColor3f(0.47f,0.4f,0.929f);
		drawBall(ball[i].getRad()-0.01f);
	}
	glPopMatrix();
}
void update(GLint value) 
{
	if(flag==4)
	{
		// Check Coin in Hole
		for(GLint i=1;i<=10;i++)
		{
			GLfloat distance=0.035+ball[i].getRad();
			GLfloat X=0.515,Y=0.515;
			if((sqrt(( X-ball[i].getX())*( X-ball[i].getX())+(( Y-ball[i].getY())*( Y-ball[i].getY())))<distance
						|| sqrt((-X-ball[i].getX())*(-X-ball[i].getX())+((-Y-ball[i].getY())*(-Y-ball[i].getY())))<distance
						|| sqrt((-X-ball[i].getX())*(-X-ball[i].getX())+(( Y-ball[i].getY())*( Y-ball[i].getY())))<distance
						|| sqrt(( X-ball[i].getX())*( X-ball[i].getX())+((-Y-ball[i].getY())*(-Y-ball[i].getY())))<distance)
					&& sqrt((ball[i].getVelX()*ball[i].getVelX())+(ball[i].getVelY()*ball[i].getVelY()))<=0.02f)
			{
				ball[i].setVelX(0.0f);
				ball[i].setVelY(0.0f);
				ball[i].setX(0.8f);
				if(ball[i].getColor()==3)
				{
					ball[i].setY(0.1f+Wpadding);
					Wpadding += 0.05f;
				}
				else if(ball[i].getColor()==4)
				{
					ball[i].setY(-0.4f+Bpadding);
					Bpadding += 0.05f;
				}
				else
				{
					ball[i].setX(0.7f);
					ball[i].setY(-0.04f);
				}

				// Score Update
				if(first==0)
				{
					if(ball[i].getColor()!=2 && ball[i].getColor()!=1)      // First pocketed coin should not be Red
					{
						first=1;
						toPocket=ball[i].getColor();
						score+=10;
					}
					else if(ball[i].getColor()==2)
						score+=50;
					else
						score-=5;
				}
				else
				{
					if(ball[i].getColor()==toPocket || ball[i].getColor()==2)
					{
						if(ball[i].getColor()!=2)
							score+=10;
						else
							score+=50;
					}
					else
						score-=5;
				}
			}
		}


		// Check for Restart
		GLint cnt=0;
		for(GLint i=1;i<=10;i++)
		{
			if(ball[i].getVelX()==0 && ball[i].getVelY()==0)
				cnt++;
		}
		if(cnt==10)
		{
			flag=1;
			power=-0.6f;
			ball[10].setX(0.0f);
			ball[10].setY(-0.38f);
			markX=0.0f;
			markY=0.0f;
			drawCoin(10);
			startGame();
		}

		// Handle ball collisions with box
		for(GLint i=1;i<=10;i++)
		{
			if(ball[i].getX() + ball[i].getRad() >= 0.54f || ball[i].getX() - ball[i].getRad() <= -0.54f)
				ball[i].setVelX(ball[i].getVelX()*-1);
			if(ball[i].getY() + ball[i].getRad() >= 0.54f || ball[i].getY() - ball[i].getRad() <= -0.54f)
				ball[i].setVelY(ball[i].getVelY()*-1);
		}
		// Coin to Coin Collision
		for(GLint i=1;i<=10;i++)
		{
			for(GLint j=i+1;j<=10;j++)
			{
				GLfloat b = ball[i].getY() - ball[j].getY();
				GLfloat a = ball[i].getX() - ball[j].getX();
				GLfloat sumrad = ball[i].getRad() + ball[j].getRad() ;
				GLfloat sqrrad = sumrad * sumrad ;
				GLfloat distsqr = (a * a) + (b * b);
				if( distsqr <= sqrrad )
				{

					GLfloat collision[2];
					collision[1]=b;
					collision[0]=a;
					GLfloat unor[2];   
					GLfloat u1[2]; 
					GLfloat u2[2]; 
					GLfloat utang[2]; 
					GLfloat distance = sqrt((a*a) + (b*b)) ;
					unor[1] = collision[1]/distance;
					unor[0]=collision[0]/distance;  
					u1[1]=ball[i].getVelY();
					u1[0]=ball[i].getVelX(); 
					u2[1]=ball[j].getVelY();
					u2[0]=ball[j].getVelX(); 
					utang[1]=unor[0];
					utang[0]=-1*unor[1]; 
					GLfloat u1t = utang[0] * u1[0] + utang[1] * u1[1];
					GLfloat u1n = unor[0] * u1[0] + unor[1] * u1[1];
					GLfloat u2t = utang[0] * u2[0] + utang[1] * u2[1];	
					GLfloat u2n = unor[0] * u2[0] + unor[1] * u2[1];
					GLfloat v2n = (u2n * (ball[j].getMass() - ball[i].getMass())+ (2 * ball[i].getMass() * u1n))/(ball[i].getMass() + ball[j].getMass());
					GLfloat v1n = (u1n * (ball[i].getMass() - ball[j].getMass())+ (2 * ball[j].getMass() * u2n))/(ball[i].getMass() + ball[j].getMass());
					GLfloat v1nf[2]; 
					GLfloat v2nf[2]; 
					GLfloat v2tf[2]; 
					GLfloat v2[2];
					GLfloat v1[2]; 
					v1nf[1] = v1n * unor[1];
					v1nf[0] = v1n * unor[0]; 
					GLfloat v1tf[2]; 
					v1tf[1] = u1t * utang[1];
					v1tf[0] = u1t * utang[0]; 
					v2nf[1] = v2n * unor[1];
					v2nf[0] = v2n * unor[0]; 
					v2tf[1] = u2t * utang[1];
					v2tf[0] = u2t * utang[0]; 
					v1[1] = v1nf[1] + v1tf[1];
					v1[0] = v1nf[0] + v1tf[0];  
					v2[1] = v2nf[1] + v2tf[1];
					v2[0] = v2nf[0] + v2tf[0]; 

					// Update position of ball
					ball[i].setVelX(v1[0]*0.98);
					ball[i].setVelY(v1[1]*0.98);
					ball[j].setVelX(v2[0]*0.98);
					ball[j].setVelY(v2[1]*0.98);

					ball[i].setX(ball[i].getX()+ball[i].getVelX());
					ball[i].setY(ball[i].getY()+ball[i].getVelY());
					ball[j].setX(ball[j].getX()+ball[j].getVelX());
					ball[j].setY(ball[j].getY()+ball[j].getVelY());

					// Collision with Wall
					if(ball[i].getX() + ball[i].getRad() >= 0.54f || ball[i].getX() - ball[i].getRad() <= -0.54f)
						ball[i].setVelX(ball[i].getVelX()*-1);
					if(ball[i].getY() + ball[i].getRad() >= 0.54f || ball[i].getY() - ball[i].getRad() <= -0.54f)
						ball[i].setVelY(ball[i].getVelY()*-1);
					if(ball[j].getX() + ball[j].getRad() >= 0.54f || ball[j].getX() - ball[j].getRad() <= -0.54f)
						ball[j].setVelX(ball[j].getVelX()*-1);
					if(ball[j].getY() + ball[j].getRad() >= 0.54f || ball[j].getY() - ball[j].getRad() <= -0.54f)
						ball[j].setVelY(ball[j].getVelY()*-1);;

				}
			}
			// Update position of ball
			ball[i].setVelX(ball[i].getVelX()*0.98);
			ball[i].setVelY(ball[i].getVelY()*0.98);
			ball[i].setX(ball[i].getX()+ball[i].getVelX());
			ball[i].setY(ball[i].getY()+ball[i].getVelY());
			if(sqrt((ball[i].getVelX()*ball[i].getVelX())+(ball[i].getVelY()*ball[i].getVelY()))<=0.0001)
			{
				ball[i].setVelX(0);
				ball[i].setVelY(0);
			}
		}
		glutTimerFunc(25, update, 0);
	}
}
void drawBoard(GLfloat x)
{
	glBegin(GL_QUADS);
	glVertex2f(-x,-x);
	glVertex2f( x,-x);
	glVertex2f( x, x);
	glVertex2f(-x, x);
	glEnd();
}
void showDirection(GLfloat x,GLfloat y)
{
	glBegin(GL_LINES);
	glVertex2f(ball[10].getX(), ball[10].getY());
	glVertex2f(x, y);
	glEnd();
}
void drawMarker() 
{
	glPushMatrix();
	glTranslatef(markX, markY, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-0.01f,-0.01f);
	glVertex2f( 0.01f, 0.01f);
	glVertex2f(-0.01f, 0.01f);
	glVertex2f( 0.01f,-0.01f);
	glEnd();
	glPopMatrix();
}
void drawBall(GLfloat r)
{
	glBegin(GL_TRIANGLE_FAN);
	GLint i=0;
	for(i=0;i<360;i++)
		glVertex2f(r*cos(DEG2RAD(i)),r*sin(DEG2RAD(i)));
	glEnd();
}
void handleKeypress1(unsigned char key, GLint x, GLint y) 
{
	if (key == 27)  // escape key is pressed
		exit(0);     
	if (key == 13)  // Enter key is pressed
	{
		if(flag==3)
		{
			ball[10].setVelX((power+0.6)*cos(theta)*0.05);
			ball[10].setVelY((power+0.6)*sin(theta)*0.05);
			glutTimerFunc(25, update, 0);
			flag++;
		}
		else
			flag++;
	}
}
void handleKeypress2(GLint key, GLint x, GLint y) 
{
	if (key == GLUT_KEY_LEFT)
	{
		if(flag==1 && ball[10].getX()>=-0.34)
			ball[10].setX(ball[10].getX()-0.01);
		else if(flag==2 && markX>=-0.55)
			markX -=0.01;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		if(flag==1 && ball[10].getX()<=0.34)
			ball[10].setX(ball[10].getX()+0.01);
		else if(flag==2 && markX<=0.55)
			markX +=0.01;
	}
	else if (key == GLUT_KEY_UP)
	{
		if(flag==2 && markY<=0.55)
			markY +=0.01;
		if(flag==3 && power<=0.59f)
			power+=0.01;
	} 	
	else if (key == GLUT_KEY_DOWN)
	{
		if(flag==2 && markY>=-0.55)
			markY -=0.01;
		if(flag==3 && power>=-0.6)
			power-=0.01;	
	}
	if(markY>ball[10].getY())
	{
		if(markX!=ball[10].getX())
		{
			theta=atan((markY-ball[10].getY())/(markX-ball[10].getX()));
			if(theta<0)
				theta+=PI;
		}
		else
			theta=PI/2;
	}
	else
	{
		if(markX!=ball[10].getX())
		{
			theta=atan((ball[10].getY()-markY)/(ball[10].getX()-markX));
			if(theta>0)
				theta+=PI;
		}
		else
			theta=PI/2;
	}
}
void handleMouseclick(GLint button, GLint state, GLint x, GLint y) 
{
	if(state==GLUT_DOWN)
	{
		GLint w = glutGet(GLUT_SCREEN_WIDTH);
		GLint h = glutGet(GLUT_SCREEN_HEIGHT);
		double x1 = x / (double) 1366 * 2 + -1;
		double y1 = (1 - y / (double) 768) * 2 + -1;
		//cout<<"x= "<<x1<<"y= "<<y1<<endl;
		cout<<"w= "<<w<<"h= "<<h<<endl;
		if(flag==2)
		{
			markX = x1;
			markY = y1;
			if(markY>ball[10].getY())
			{
				if(markX!=ball[10].getX())
				{
					theta=atan((markY-ball[10].getY())/(markX-ball[10].getX()));
					if(theta<0)
						theta+=PI;
				}
				else
					theta=PI/2;
			}
			else
			{
				if(markX!=ball[10].getX())
				{
					theta=atan((ball[10].getY()-markY)/(ball[10].getX()-markX));
					if(theta>0)
						theta+=PI;
				}
				else
					theta=PI/2;
			}
		}
		else if(flag==3)
		{
			if(y1>=0.59)
				power=0.59;
			else if(y1<=-0.6)
				power=-0.6;
			else
				power=y1;
		}	
	}
}
void handleResize(GLint w, GLint h) 
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
