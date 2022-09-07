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


double length;



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

point wheel_center,body_center;


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


void drawCyllinder(double radius, double height, int segments)
{
    int i;
    struct point points[100];
    double shade;

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }

    //draw quads using generated points
    for(i=0;i<segments;i++)
    {
        // create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
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

void drawWheel(point wheel_center)
{
    // translate to position
	glPushMatrix();
	{
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
	glPopMatrix();
	
    

	
}

void drawSquare(double a, double h)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,h);
		glVertex3f( a,-a,h);
		glVertex3f(-a,-a,h);
		glVertex3f(-a, a,h);
	}glEnd();
}

void drawSS()
{
    //glRotated(rotation_angle, 0, 0, 1);
    glPushMatrix();
    {
        glTranslated(length/2.0, length/2.0, 0);
        //glRotated(rotation_angle, 0, 0, 1);
        drawWheel(wheel_center);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslated(length/2.0, -length/2.0, 0);
        //glRotated(rotation_angle, 0, 0, 1);
        drawWheel(wheel_center);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-length/2.0, length/2.0, 0);
        //glRotated(rotation_angle, 0, 0, 1);
        drawWheel(wheel_center);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslated(-length/2.0, -length/2.0, 0);
        //glRotated(rotation_angle, 0, 0, 1);
        drawWheel(wheel_center);
    }
    glPopMatrix();

    // draw body
    glPushMatrix();
    {
        glColor3f(0.7, 0.7, 0.7);
        glTranslated(wheel_center.x, wheel_center.y, wheel_center.z);
        //glRotated(rotation_angle, 0, 0, 1);
        drawSquare(length / 2.0 - 2.5, -2.5);
        drawSquare(length / 2.0 - 2.5 ,5);

        
    }
    glPopMatrix();


    glPushMatrix();
	{
		glTranslated(wheel_center.x, wheel_center.y, wheel_center.z);
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		{
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 10);
			glVertex3f(0, 0, 10);
			double dirx = -10 * wheel_radius * pi / 6.0  * cos(rotation_angle * pi / 180);
            double diry = -10 * wheel_radius * pi / 6.0  * sin(rotation_angle * pi / 180);
			glVertex3f(dirx, diry, 10);

		}
		glEnd();
	}
	glPopMatrix();

    

    



    
}
void keyboardListener(unsigned char key, int x,int y){

    double A = pi / 20;

	switch(key){

		case '1':
            drawgrid=1-drawgrid;
            break;

        case 'w':

            forward_angle += 30;
            if(forward_angle >= 360.0) forward_angle -= 360.0;

            wheel_center.x -= wheel_radius * pi / 6.0  * cos(rotation_angle * pi / 180);
            wheel_center.y -= wheel_radius * pi / 6.0  * sin(rotation_angle * pi / 180);

            break;

        case 's':
            forward_angle -= 30;
            if(forward_angle < 0.0) forward_angle += 360.0;

            wheel_center.x += wheel_radius * pi / 6.0 * cos(rotation_angle * pi / 180);
            wheel_center.y += wheel_radius * pi / 6.0 * sin(rotation_angle * pi / 180);


            break;

        case 'a':
            rotation_angle += A * 180 / pi;
            if(rotation_angle < 0.0) rotation_angle += 360.0;
            break;

        case 'd':
            rotation_angle -= A * 180 / pi;
            if(rotation_angle >= 360.0) rotation_angle -= 360.0;
            break;

        case 'c':
            wheel_radius = std::fmax(wheel_radius - 1.0, 5.0);
            length = std::fmax(length - 1.0, 40.0);
            break;

        case 'x':
            wheel_radius = std::fmin(wheel_radius + 1.0, 10.0);
            length = std::fmin(length + 1.0, 50.0);
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
	drawgrid=1;
	drawaxes=0;
	angle=0;
    cameraHeight=64.0;
	cameraAngle=1.44;


	// initialize wheel position
	wheel_radius = 10.0;
	rotation_angle = 0.0;
	forward_angle = 0.0;
	wheel_center = point(21.0, 120.0, wheel_radius);
    
    length = 50.0;


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
