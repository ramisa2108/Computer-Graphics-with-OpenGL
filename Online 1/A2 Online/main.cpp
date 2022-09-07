
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))
#define sq2 1.0 / sqrt(2)

int drawgrid;
int drawaxes;
double angle;


double cube_length;
double cube_ratio;
double sphere_radius;
double total_length;

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

point pos, u, r, l;

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

void drawSquare(double a)
{

	glBegin(GL_QUADS);{
        glVertex3f( a,a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawCyllinder(double radius, double height, int segments, int fraction=1)
{
    int i;
    struct point points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi/fraction);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi/fraction);
    }

    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i+1].x,points[i+1].y,height);
            glVertex3f(points[i].x,points[i].y,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}
void drawHalfSphere(double radius,int slices,int stacks, int fraction=1)
{
    struct point points[100][100];
	int i,j;
	double h,r;

	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi/fraction);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi/fraction);
			points[i][j].z=h;
		}
	}

	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawSphereFrame(double displacement, double radius)
{
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, displacement);
        drawHalfSphere(radius,24,20, 4);

    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, -displacement);
        glRotatef(90, 0, 1, 0);
        drawHalfSphere(radius,24,20, 4);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, -displacement);
        glRotatef(180, 0, 1, 0);
        drawHalfSphere(radius,24,20,4);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, displacement);
        glRotatef(270, 0, 1, 0);
        drawHalfSphere(radius,24,20,4);
    }

    glPopMatrix();
}

void drawCyllindersFrame(double displacement, double radius)
{
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, -displacement);
        drawCyllinder(radius, displacement * 2, 20, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, -displacement);
        glRotatef(90, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 20, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, displacement);
        glRotatef(180, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 20, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(displacement, displacement, displacement);
        glRotatef(270, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 20, 4);
    }
    glPopMatrix();

}
void drawSS()
{

    cube_length = total_length * cube_ratio;
    sphere_radius = total_length - cube_length;
    // draw 1/8th spheres
    glPushMatrix();
    {
        glColor3f(1.0,0.0,0.0);

        // upper hemisphere
        drawSphereFrame(cube_length, sphere_radius);

        glRotatef(180, 1, 0, 0);

        // lower hemisphere
        drawSphereFrame(cube_length, sphere_radius);

    }
    glPopMatrix();

    // draw 6 full squares
    glPushMatrix();
    {
        glColor3f(1.0, 1.0, 1.0);

        glPushMatrix();
        {
            glTranslatef(0, 0, cube_length + sphere_radius);
            drawSquare(cube_length);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, 0, -(cube_length + sphere_radius));
            drawSquare(cube_length);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(cube_length + sphere_radius, 0, 0);
            glRotatef(90, 0, 1, 0);
            drawSquare(cube_length);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-(cube_length + sphere_radius), 0, 0);
            glRotatef(90, 0, 1, 0);
            drawSquare(cube_length);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, cube_length + sphere_radius, 0);
            glRotatef(90, 1, 0, 0);
            drawSquare(cube_length);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, -(cube_length + sphere_radius), 0);
            glRotatef(90, 1, 0, 0);
            drawSquare(cube_length);
        }
        glPopMatrix();

    }
    glPopMatrix();


    // draw 1/4th of cyllinders
    glPushMatrix();
    {
        glColor3f(0.0, 1.0, 0.0);

        // front frame
        drawCyllindersFrame(cube_length, sphere_radius);

        // back frame
        glPushMatrix();
        {
            glRotatef(180, 0, 0, 1);
            drawCyllindersFrame(cube_length, sphere_radius);
        }
        glPopMatrix();

        // horizontal frames
        glPushMatrix();
        {
            glRotatef(90, 1, 0, 0);

            // upper horizontal
            drawCyllindersFrame(cube_length, sphere_radius);

            // lower horizontal
            glPushMatrix();
            {
                glRotatef(180, 0, 0, 1);
                drawCyllindersFrame(cube_length, sphere_radius);
            }
            glPopMatrix();

        }
        glPopMatrix();

    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y){

    double A = pi / 20;

    switch(key){

		case '1': // look left
            // u constant     
            l = l.multiply(cos(-A)).add(r.multiply(sin(-A)));
            r = r.multiply(cos(A)).add(l.multiply(sin(A)));
            break;

        case '2': // look right
            // u constant
            l = l.multiply(cos(A)).add(r.multiply(sin(A)));
            r = r.multiply(cos(-A)).add(l.multiply(sin(-A)));
            break;

        case '3': // look up
            // r constant
            u = u.multiply(cos(-A)).add(l.multiply(sin(-A)));
            l = l.multiply(cos(A)).add(u.multiply(sin(A)));
            break;

        case '4': // look down
            // r constant
            u = u.multiply(cos(A)).add(l.multiply(sin(A)));
            l = l.multiply(cos(-A)).add(u.multiply(sin(-A)));
            break;


        case '5': // tilt clockwise
            // l constant
            r = r.multiply(cos(-A)).add(u.multiply(sin(-A)));
			u = u.multiply(cos(A)).add(r.multiply(sin(A)));

            break;
        case '6': // tilt counterclockwise
            // l constant
            r = r.multiply(cos(A)).add(u.multiply(sin(A)));
			u = u.multiply(cos(-A)).add(r.multiply(sin(-A)));

            break;
        case '7': // look up left

            // look up
            u = u.multiply(cos(-A)).add(l.multiply(sin(-A)));
            l = l.multiply(cos(A)).add(u.multiply(sin(A)));
            // look left
            l = l.multiply(cos(-A)).add(r.multiply(sin(-A)));
            r = r.multiply(cos(A)).add(l.multiply(sin(A)));
            
            break;
        
        case '8': // look up right
            // look up
            u = u.multiply(cos(-A)).add(l.multiply(sin(-A)));
            l = l.multiply(cos(A)).add(u.multiply(sin(A)));
            // look right
            l = l.multiply(cos(A)).add(r.multiply(sin(A)));
            r = r.multiply(cos(-A)).add(l.multiply(sin(-A)));
            
            break;
        
        case '9': // look down left
            // look down
            u = u.multiply(cos(A)).add(l.multiply(sin(A)));
            l = l.multiply(cos(-A)).add(u.multiply(sin(-A)));
            // look left
            l = l.multiply(cos(-A)).add(r.multiply(sin(-A)));
            r = r.multiply(cos(A)).add(l.multiply(sin(A)));
            
            break;

        case '0': // look down right
            // look down
            u = u.multiply(cos(A)).add(l.multiply(sin(A)));
            l = l.multiply(cos(-A)).add(u.multiply(sin(-A)));
            // look right
            l = l.multiply(cos(A)).add(r.multiply(sin(A)));
            r = r.multiply(cos(-A)).add(l.multiply(sin(-A)));
            
            break;

        case 'U':
            total_length += 1.0;
            break;

        case 'D':
            total_length = std::fmax(total_length - 1.0, 0.0);
            break;

		default:

			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:
            pos = pos.add(l.multiply(-2));
            
			break;
		case GLUT_KEY_UP:
            pos = pos.add(l.multiply(2));
            break;

		case GLUT_KEY_RIGHT:
			pos = pos.add(r.multiply(2));
			break;
		case GLUT_KEY_LEFT:
            pos = pos.add(r.multiply(-2));
			break;

		case GLUT_KEY_PAGE_UP:
            pos = pos.add(u.multiply(2));
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos = pos.add(u.multiply(-2));
            break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
            cube_ratio = std::fmax(0.0, cube_ratio - 0.1);
            break;
		case GLUT_KEY_END:
            cube_ratio = std::fmin(1.0, cube_ratio + 0.1);
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
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);

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
    total_length = 30.0;
    cube_ratio = 2.0 / 3.0;
	cube_length = total_length * cube_ratio;
    sphere_radius = total_length - cube_length;
	
    
	// initialize pos of camera and directions

	pos = point(100.0, 100.0, 0.0);
	u = point(0.0, 0.0, 1.0);
	r = point(-1.0/sq2, 1.0/sq2, 0.0);
	l = point(-1.0/sq2, -1.0/sq2, 0.0);

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
