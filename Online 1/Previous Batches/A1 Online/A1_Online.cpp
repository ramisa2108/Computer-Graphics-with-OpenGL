#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double cube_length;

double red_angle, blue_angle, green_angle, yellow_angle, white_angle;

struct point
{
	double x,y,z;
	point(double xx=0.0, double yy=0.0, double zz=0.0)
	{
        x = xx;
        y = yy;
        z = zz;
	}

};

point wheel_center;

void drawAxes()
{
	if(drawaxes==1)
	{
		glBegin(GL_LINES);{
			glColor3f(1.0, 0.0, 0.0);
		
			glVertex3f( 100,0,0);
			glVertex3f(0,0,0);

			glColor3f(0.0, 1.0, 0.0);
		
			glVertex3f(0,0,0);
			glVertex3f(0, 100,0);

			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 100);

			glVertex3f(0,0,0);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-20;i<=20;i++){


				//lines parallel to Y-axis
				glVertex3f(i*10, -200, 0);
				glVertex3f(i*10,  200, 0);

				//lines parallel to X-axis
				glVertex3f(-200, i*10, 0);
				glVertex3f( 200, i*10, 0);
			}
		}glEnd();
	}
}
void drawSquare(double a)
{

	glBegin(GL_QUADS);{
        glVertex3f( a,a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawSS()
{
	
	glColor3f(0.5, 0.5, 0.5);
	drawSquare(cube_length);

	// red one
	glPushMatrix();
	{
		glColor3f(1.0,0.0, 0.0);
		
		glTranslated(cube_length, 0, 0);
		glRotated(red_angle, 0, 1, 0);

		glBegin(GL_QUADS);{
			glVertex3f(-2.0 * cube_length, cube_length, 0);
			glVertex3f(-2.0 * cube_length, -cube_length, 0);
			glVertex3f(0, -cube_length, 0);
			glVertex3f(0, cube_length, 0);
		}glEnd();
	}
	glPopMatrix();

	// blue one

	glPushMatrix();
	{
		glColor3f(0.0, 0.0, 1.0);
		glTranslated(0.0, cube_length, 0);
		glRotated(blue_angle, 1, 0, 0);
		glBegin(GL_QUADS);{
			glVertex3f(-cube_length, 2 * cube_length, 0);
			glVertex3f(-cube_length, 0, 0);
			glVertex3f(cube_length, 0, 0);
			glVertex3f(cube_length, 2*cube_length, 0);
		}glEnd();
	}
	glPopMatrix();

	// green one

	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 0.0);
		glTranslated(-cube_length, 0.0, 0.0);
		glRotated(green_angle, 0, 1, 0);
		glBegin(GL_QUADS);{
			glVertex3f(-2.0 * cube_length, cube_length, 0);
			glVertex3f(-2.0 * cube_length, -cube_length, 0);
			glVertex3f(0, -cube_length, 0);
			glVertex3f(0, cube_length, 0);
		}glEnd();

	}
	glPopMatrix();

	// // yellow one
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);
		glTranslated(0.0, -cube_length, 0);
		glRotated(yellow_angle, 1, 0, 0);
		glBegin(GL_QUADS);{
			glVertex3f(-cube_length, 2 * cube_length, 0);
			glVertex3f(-cube_length, 0, 0);
			glVertex3f(cube_length, 0, 0);
			glVertex3f(cube_length, 2*cube_length, 0);
		}glEnd();

		// white one
		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			glTranslated(0.0, 2 * cube_length, 0);
			glRotated(white_angle, 1, 0, 0);
			glBegin(GL_QUADS);{
				glVertex3f(-cube_length, 2 * cube_length, 0);
				glVertex3f(-cube_length, 0, 0);
				glVertex3f(cube_length, 0, 0);
				glVertex3f(cube_length, 2*cube_length, 0);
			}glEnd();
		}
		glPopMatrix();
		
	}
	glPopMatrix();




}

void keyboardListener(unsigned char key, int x,int y){


	switch(key){

		case '2':
			red_angle = std::fmin(180.0, red_angle+30.0);
			break;

		case '1':
			red_angle = std::fmax(90.0, red_angle-30.0);
			break;

		case '4':
			blue_angle = std::fmax(0, blue_angle - 30.0);
			break;
		case '3':
			blue_angle = std::fmin(90.0, blue_angle + 30.0);
			break;
		
		case '6':
			green_angle = std::fmax(0.0, green_angle-30.0);
			
			break;

		case '5':
			green_angle = std::fmin(90.0, green_angle+30.0);
			
			break;
		
		case '8':

			yellow_angle = std::fmin(180, yellow_angle + 30.0);
			break;
		
		case '7':
			yellow_angle = std::fmax(90, yellow_angle - 30.0);
			break;

		case '0':
			white_angle = std::fmin(360, white_angle + 30.0);
			break;

		case '9':
			white_angle = std::fmax(270, white_angle-30.0);
			break;


		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){

		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
            break;
		case GLUT_KEY_PAGE_DOWN:
            break;
		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    drawSS();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){

	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	angle=0;
    cameraHeight=40.0;
	cameraAngle=0.21;

	// cube initialization
	cube_length = 40.0;
	red_angle = blue_angle = green_angle = yellow_angle = 90.0;
	white_angle = 270;



	


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
