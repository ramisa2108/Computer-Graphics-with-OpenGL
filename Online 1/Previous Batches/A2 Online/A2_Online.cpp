
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))
#define sq2 1.0 / sqrt(2)

int drawgrid;
int drawaxes;
double angle;
double cameraHeight;
double cameraAngle;
double angle_diff;
double tilt_angle;


double cube_length;

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
	struct point goAlong(struct point dir, double unit)
	{
		return point(x + dir.x * unit, y + dir.y * unit, z + dir.z * unit);
	}
	point operator-(const point &right) const {
		return {x - right.x, y - right.y, z - right.z};
	}
	void normalize()
	{
		double sum = sqrt(x*x+y*y+z*z);
	}

};

point pos, u, r, l;
point red_dir, blue_dir, green_dir, yellow_dir, white_dir;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			//glVertex3f(0,0,0);

			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			//glVertex3f(0,0,0);

			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
			//glVertex3f(0,0,0);

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

void drawSquare(double a)
{


	glBegin(GL_QUADS);{
        glVertex3f( a,a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawPolygon(double a, int n)
{
    double theta = pi * 2.0 / n;
	point points[n];
	glRotated(angle, 0, 0, 1);
	
    glBegin(GL_LINES);
    {
        for(int i=0;i<n;i++)
        {
            points[i] = point(a * cos(theta * i), a * sin(theta * i), 0.0);
			if(i > 0)
			{
				glColor3f(1.0, 1.0, 0.0);
				glVertex3f(points[i-1].x, points[i-1].y, points[i-1].z);
				glVertex3f(points[i].x, points[i].y, points[i].z);
				
			}

		
				
        }
		glVertex3f(points[n-1].x, points[n-1].y, points[n-1].z);
		glVertex3f(points[0].x, points[0].y, points[0].z);

				

    }
    glEnd();

	for(int i=0;i<n;i++)
	{
		glPushMatrix();
		{
			glRotated(360 * i / n, 0, 0, 1);
			glTranslated(a, 0, 0);
			glRotated(tilt_angle, 0, -1, 0);
			glTranslated(0, 0, -30.0);

			glColor3f(1, 0, 0);
			drawSquare(5);

			glBegin(GL_LINES);
			{
				glColor3f(1,1,1);
				glVertex3f(0,0,30);
				glVertex3f(5, 0, 0);

				glVertex3f(0,0,30);
				glVertex3f(-5, 0, 0);
				
			}
			glEnd();
		}
		glPopMatrix();
	}

	
}
void drawSS()
{
	
	glRotatef(angle, 0, 0, 1);
    drawPolygon(50, 6);
    
}

void keyboardListener(unsigned char key, int x,int y){

    double A = pi / 20;

    switch(key){

		case '1':
			angle_diff = std::fmin(angle_diff + 0.2, 1.0);
			tilt_angle = std::fmin(tilt_angle+18, 90);
			break;
		case '2':
			
			angle_diff = std::fmax(0.0, angle_diff-0.2);
			tilt_angle = std::fmax(0, tilt_angle - 18);
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
	angle+=angle_diff;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	drawgrid=0;
	drawaxes=1;
	angle=0;
	angle_diff = 0;
	tilt_angle = 0;
	cameraHeight=115.0;
	cameraAngle=7.0;

    
	cube_length = 70.0;
    
	

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
