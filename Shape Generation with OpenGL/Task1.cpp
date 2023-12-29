
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#define sq2 1.0 / sqrt(2)

int drawgrid;
int drawaxes;
double angle;

const double CUBE_LENTH = 20.0;
const double SPHERE_RADIUS = 10.0;
const double TOTAL_LENGTH = CUBE_LENTH + SPHERE_RADIUS;     // maximum size of any shape

double cube_length;
double sphere_radius;

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
    struct point cross(struct point v)
    {
        return point(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

};

point pos, u, r, l;

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
			
            for(int i=-8;i<=8;i++){

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
    // draws a square on the xy plane with (0,0,0) at center
	glBegin(GL_QUADS);{
        glVertex3f( a,a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawCyllinder(double radius, double height, int segments, int fraction=1)
{

    //  parameters:
    //      - segments : number of rectangles that makes up the surface of the cyllinder 
    //      - fraction : fraction of the full (360 degree) radius of the cyllinder to draw

    struct point points[100];

    // generate points
    for(int i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi/fraction);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi/fraction);
    }

    // draw quads using generated points
    for(int i=0;i<segments;i++)
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
    //  draws the upper half of a sphere
    //  parameters:
    //      - slices : number of segments that makes up one 2D circle 
    //      - stacks : number of 2D circles used to make the 3D sphere
    //      - fraction : fraction of the half sphere to draw

    struct point points[100][100];
	
	//generate points
	for(int i=0;i<=stacks;i++)
	{
        // radius and height of the 2D circle
		double h=radius*sin(((double)i/(double)stacks)*(2.0 * pi));
		double r=radius*cos(((double)i/(double)stacks)*(2.0 * pi));

		for(int j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi/fraction);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi/fraction);
			points[i][j].z=h;
		}
	}

	//draw quads using generated points
	for(int i=0;i<stacks;i++)
	{
        for(int j=0;j<slices;j++)
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

void drawSpheresFrame(double displacement, double radius)
{
    // draws 4 parts of a half sphere with space in between for the cube
    // displacement : length of the cube
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, displacement);
        drawHalfSphere(radius,50,50, 4);

    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, -displacement);
        glRotatef(90, 0, 1, 0);
        drawHalfSphere(radius,50,50, 4);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, -displacement);
        glRotatef(180, 0, 1, 0);
        drawHalfSphere(radius,50,50,4);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, displacement);
        glRotatef(270, 0, 1, 0);
        drawHalfSphere(radius,50,50,4);
    }

    glPopMatrix();
}

void drawCyllindersFrame(double displacement, double radius)
{

    // draw 4 parts of a cyllinder with space in between for the cube
    // displacement : length of the cube
    glPushMatrix();
    {
        glTranslatef(displacement, displacement, -displacement);
        drawCyllinder(radius, displacement * 2, 50, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, -displacement);
        glRotatef(90, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 50, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-displacement, displacement, displacement);
        glRotatef(180, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 50, 4);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(displacement, displacement, displacement);
        glRotatef(270, 0, 1, 0);
        drawCyllinder(radius, displacement * 2, 50, 4);
    }
    glPopMatrix();

}
void drawSS()
{

    // draw 1/8th spheres
    glPushMatrix();
    {
        glColor3f(1.0,0.0,0.0);

        // upper halfsphere frame
        drawSpheresFrame(cube_length, sphere_radius);

        glRotatef(180, 1, 0, 0);

        // lower halfsphere frame
        drawSpheresFrame(cube_length, sphere_radius);

    }
    glPopMatrix();

    // draw 6 full squares (sides of the cube)
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

		case '1':   // rotate / look left (axis : u)
            
            l = l.multiply(cos(A)).add(u.cross(l).multiply(sin(A)));
            r = r.multiply(cos(A)).add(u.cross(r).multiply(sin(A)));
            break;

        case '2':   // rotate / look right (axis : u)
            
            l = l.multiply(cos(-A)).add(u.cross(l).multiply(sin(-A)));
            r = r.multiply(cos(-A)).add(u.cross(r).multiply(sin(-A)));

            break;

        case '3':   // look up (axis : r)

            u = u.multiply(cos(A)).add(r.cross(u).multiply(sin(A)));
            l = l.multiply(cos(A)).add(r.cross(l).multiply(sin(A)));
            break;

        case '4':   // look down (axis : r)
            
            u = u.multiply(cos(-A)).add(r.cross(u).multiply(sin(-A)));
            l = l.multiply(cos(-A)).add(r.cross(l).multiply(sin(-A)));
            break;


        case '5':   // tilt clockwise (axis : l)
            
            r = r.multiply(cos(A)).add(l.cross(r).multiply(sin(A)));
            u = u.multiply(cos(A)).add(l.cross(u).multiply(sin(A)));
            break;

        case '6':   // tilt counterclockwise (axis : l)
            
            r = r.multiply(cos(-A)).add(l.cross(r).multiply(sin(-A)));
            u = u.multiply(cos(-A)).add(l.cross(u).multiply(sin(-A)));
            break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){

    double delta = 2.0;
	
    switch(key){

		case GLUT_KEY_UP:           // move forward
            pos = pos.add(l.multiply(delta));
			break;

        case GLUT_KEY_DOWN:         // move backward
            pos = pos.add(l.multiply(-delta));
			break;
		
		case GLUT_KEY_RIGHT:        // move right
			pos = pos.add(r.multiply(delta));
			break;
		
        case GLUT_KEY_LEFT:         // move left
            pos = pos.add(r.multiply(-delta));
			break;

		case GLUT_KEY_PAGE_UP:      // move up
            pos = pos.add(u.multiply(delta));
			break;

		case GLUT_KEY_PAGE_DOWN:    // move down
            pos = pos.add(u.multiply(-delta));
            break;

		case GLUT_KEY_HOME:         // cube to sphere

            // increase the sphere radius and decrease cube length
            sphere_radius = std::min(TOTAL_LENGTH, sphere_radius + delta);
            cube_length = std::max(0.0, cube_length - delta);
			break;
		case GLUT_KEY_END:          // sphere to cube

            // increase the cube length and decrease sphere radius
            cube_length = std::min(TOTAL_LENGTH, cube_length + delta);
            sphere_radius = std::max(0.0, sphere_radius - delta);
			break;
        
        case GLUT_KEY_INSERT:
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
	//  load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//  gluLookAt parameters:
	//      - where is the camera (viewer)?
	//      - where is the camera looking?
	//      - which direction is the camera's UP direction?
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	// add objects to draw
	drawAxes();
	drawGrid();
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

	drawgrid=0;
	drawaxes=1;
	angle=0;
	sphere_radius = SPHERE_RADIUS;
	cube_length = CUBE_LENTH;

	// initialize positions and direction vectors for the camera

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
