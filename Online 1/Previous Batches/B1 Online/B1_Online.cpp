
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))
#define sq2 1.0 / sqrt(2)

int drawgrid;
int drawaxes;
double angle;


double R1, R2, R3;
double angle2, angle3;

struct point
{
	double x,y,z;
	point(double xx=0.0, double yy=0.0, double zz=0.0)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	struct point add(struct point p)
	{
        return point(x+p.x, y+p.y, z+p.z);
	}
    struct point multiply(double a)
    {
        return point(a*x, a*y, a*z);
    }

};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
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
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawSS()
{
    glRotated(45, 0, 0, 1);
    glColor3f(1, 1, 1);
    drawCircle(R1, 30);
    glBegin(GL_LINES);
    {
        glVertex3f(0, R1, 0);
        glVertex3f(0, -R1, 0);

        glVertex3f(R1, 0, 0);
        glVertex3f(-R1, 0, 0);
        
    }
    glEnd();

    glPushMatrix();
    {
        double x = (R1+R2) * cos((angle2 + 90.0) * pi / 180);
        double y = (R1+R2) * sin((angle2 + 90.0) * pi / 180);
        glRotated(angle2, 0, 0, 1);
        
        glTranslated(x, y, 0);
        glRotated(2 * angle2, 0, 0, 1);
        
        glBegin(GL_LINES);
        {
            glVertex3f(0, R2, 0);
            glVertex3f(0, -R2, 0);

            glVertex3f(R2, 0, 0);
            glVertex3f(-R2, 0, 0);
        
        }
        glEnd();
        
        drawCircle(R2, 30);

        glPushMatrix();
        {
            double x2 = (R2+R3) * cos((angle3 + 90.0) * pi / 180);
            double y2 = (R2+R3) * sin((angle3 + 90.0) * pi / 180);
            glRotated(angle3, 0, 0, 1);
            
            glTranslated(x2, y2, 0);
            glRotated(2 * angle3, 0, 0, 1);
            
            glBegin(GL_LINES);
            {
                glVertex3f(0, R3, 0);
                glVertex3f(0, -R3, 0);

                glVertex3f(R3, 0, 0);
                glVertex3f(-R3, 0, 0);
            
            }
            glEnd();
            
            drawCircle(R3, 30);
        }
        glPopMatrix();
        
        
    }
    glPopMatrix();
    
}

void keyboardListener(unsigned char key, int x,int y){

    switch(key){

        case '1':
            angle2 += 3.0;
            break;
        case '2':
            angle2 -= 3.0;
            break;
		case '3':
            angle3 += 3.0;
            break;
        case '4':
            angle3 -= 3.0;
            break;
        default:

			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:
            break;
		case GLUT_KEY_UP:
            break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
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
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

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

    R1 = 20.0;
    R2 = 10.0;
    R3 = 5.0;

    angle2 = 0.0;
    angle3 = 0.0;
    

    
	// initialize pos of camera and directions

	
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
	//aspect ratio that determines the field of view in the X direction (horizontally) : aspect ratio = field x / field y
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
