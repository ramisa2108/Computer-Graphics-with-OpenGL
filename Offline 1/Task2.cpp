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

double wheel_radius;
double rotation_angle;
double forward_angle;


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

            // x axis : red
            glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

            // y axis : green
            glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

            // z axis : blue
            glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		
        }glEnd();
	}
}


void drawGrid()
{
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i=-20;i<=20;i++){


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


void drawCyllinder(double radius, double height, int segments)
{
	//  parameters:
	//		- segments: number of rectangles that makes up the surface of the cyllinder 

    struct point points[100];
    
    // generate points
    for(int i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }

    // draw quads using generated points
    for(int i=0;i<segments;i++)
    {
        // create shading effect
		double shade;
        if(i<segments/2)
			shade=2*(double)i/(double)segments;
        else 
			shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade, 0, 0);

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i+1].x, points[i+1].y, height/2);
            glVertex3f(points[i].x, points[i].y, height/2);
			glVertex3f(points[i].x, points[i].y, -height/2);
			glVertex3f(points[i+1].x, points[i+1].y, -height/2);
        }
        glEnd();
    }
}

void drawSS()
{
    // translate to wheel position
    glTranslatef(wheel_center.x, wheel_center.y, wheel_center.z);

    // rotate to proper angles about x, y and z axis
    glRotatef(90, 1, 0, 0);
    glRotatef(rotation_angle, 0, 1, 0);
    glRotatef(forward_angle, 0, 0, 1);

    // draw the wheel body
    glPushMatrix();
    {
        drawCyllinder(wheel_radius, 5, 24);
    }
    glPopMatrix();

    // draw the wheel axes
    glColor3f(0.7, 0, 0);

	// horizontal axis
    glPushMatrix();
    {

        glBegin(GL_QUADS);{
            glVertex3f( wheel_radius, 0, 2.5);
            glVertex3f( wheel_radius, 0, -2.5);
            glVertex3f(-wheel_radius, 0, -2.5);
            glVertex3f(-wheel_radius, 0, 2.5);
        }glEnd();

    }
    glPopMatrix();

    // vertical axis
	glPushMatrix();
    {
        glBegin(GL_QUADS);{
            glVertex3f( 0, wheel_radius, 2.5);
            glVertex3f( 0, wheel_radius, -2.5);
            glVertex3f(0, -wheel_radius, -2.5);
            glVertex3f(0, -wheel_radius, 2.5);
        }glEnd();
    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y){

    double A = pi / 20;

	switch(key){

		case '1':	// show / hide grid
            drawgrid=1-drawgrid;
            break;

        case 'w':	// move forward

            forward_angle += 30;
            if(forward_angle >= 360.0) forward_angle -= 360.0;

            wheel_center.x -= wheel_radius * pi / 6.0  * cos(rotation_angle * pi / 180);
            wheel_center.y -= wheel_radius * pi / 6.0  * sin(rotation_angle * pi / 180);
            break;

        case 's':	// move backward

            forward_angle -= 30;
            if(forward_angle < 0.0) forward_angle += 360.0;

            wheel_center.x += wheel_radius * pi / 6.0 * cos(rotation_angle * pi / 180);
            wheel_center.y += wheel_radius * pi / 6.0 * sin(rotation_angle * pi / 180);
            break;

        case 'a':	// rotate left

            rotation_angle += A * 180 / pi;
            if(rotation_angle < 0.0) rotation_angle += 360.0;
            break;

        case 'd':	// rotate right

            rotation_angle -= A * 180 / pi;
            if(rotation_angle >= 360.0) rotation_angle -= 360.0;
            break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){

	switch(key){

		case GLUT_KEY_DOWN:		// move down
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// move up
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:	// rotate right
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:		// rotate left
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

void mouseListener(int button, int state, int x, int y){
	// x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:

			// show or hide axes
			if(state == GLUT_DOWN){
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
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

	//  gluLookAt parameters:
	//      - where is the camera (viewer)?
	//      - where is the camera looking?
	//      - which direction is the camera's UP direction?
    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight, 0,0,0, 0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	//add objects to draw
	drawGrid();
    drawAxes();
	drawSS();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	glutPostRedisplay();
}

void init(){

	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	angle=0;
    cameraHeight=40.0;
	cameraAngle=1.32;


	// initialize wheel position
	wheel_radius = 15.0;
	rotation_angle = 0.0;
	forward_angle = 0.0;
	wheel_center = point(21.0, 120.0, wheel_radius);



	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	// give PERSPECTIVE parameters
    //      - field of view in the Y (vertically)
	//      - aspect ratio = field x / field y
	//      - near distance
	//      - far distance
	gluPerspective(80,	1,	1,	1000.0);

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
