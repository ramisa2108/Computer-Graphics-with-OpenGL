#include<stdlib.h>
#include<math.h>
#include<vector>
#include<fstream>
#include<string>



#include "classes.hpp"
#include "bitmap_image.hpp"


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


Object *obj;

Vector3D pos, u, r, l;

int captures;
extern int recursions;
int pixelWidth, pixelHeight;

int windowWidth = 500, windowHeight = 500;
double viewAngle = 80.0;



extern vector<Object *> objects;
extern vector<PointLight *> pointLights;
extern vector<SpotLight *> spotLights;


void loadData(){

    ifstream scene("scene.txt");

    int totalObjects;


    double r, g, b,ambient, diffuse, specular, reflection;
    int shine;

    Object *tempObject;

    if(scene.is_open())
    {

        scene >> recursions;
        scene >> pixelWidth;
        pixelHeight = pixelWidth;
        
        scene >> totalObjects;

        for(int i=0;i<totalObjects;i++)
        {

            string name;
            scene >> name;
            if(name == "sphere")
            {
                // sphere parameters: center and radius

                Vector3D center;
                double radius;
                scene >> center.x >> center.y >> center.z;
                scene >> radius;
                tempObject = new Sphere(center, radius);

            }
            else if(name == "triangle")
            {
                // triangle parameters: 3 points (vertices)

                Vector3D point1, point2, point3;
                scene >> point1.x >> point1.y >> point1.z;
                scene >> point2.x >> point2.y >> point2.z;
                scene >> point3.x >> point3.y >> point3.z;

                tempObject = new Triangle(point1, point2, point3);
            }
            else if(name == "general")
            {
                // general parameters: quadratic coefficients, reference point, length, width, height

                Vector3D referencePoint;
                double length, width, height;
                QuadraticCoefficients coeff;
                scene >> coeff.A >> coeff.B >> coeff.C >> coeff.D >> coeff.E;
                scene >> coeff.F >> coeff.G >> coeff.H >> coeff.I >> coeff.J;
                scene >> referencePoint.x >> referencePoint.y >> referencePoint.z
                 >> length >> width >> height;
                tempObject = new General(coeff, referencePoint, length, width, height);
            }

            // common object parameters: color, ambient, diffuse, specular, reflection, shine

            scene >> r >> g >> b;
            scene >> ambient >> diffuse >> specular >> reflection;
            scene >> shine;

            
            tempObject->setColor(r, g, b);
            tempObject->setCoefficients(ambient, diffuse, specular, reflection);
            tempObject->setShine(shine);

            objects.push_back(tempObject);

            
        }

        int totalPointLights;
        scene >> totalPointLights;
        PointLight *tempPointLight;

        for(int i=0;i<totalPointLights;i++)
        {

            // pointlight parameters: position and color

            Vector3D position;
            scene >> position.x >> position.y >> position.z;
            scene >> r >> g >> b;
            tempPointLight = new PointLight(position);
            tempPointLight->setColor(r, g, b);

            pointLights.push_back(tempPointLight);

        }

        
        int totalSpotLights;
        scene >> totalSpotLights;
        SpotLight *tempSpotLight;

        for(int i=0;i<totalSpotLights;i++)
        {
            // spotlight parameters: pointlight, direction and angle

            Vector3D position, direction;
            double cutoffAngle;
            scene >> position.x >> position.y >> position.z;
            scene >> r >> g >> b;
            scene >> direction.x >> direction.y >> direction.z;
            scene >> cutoffAngle;

            tempPointLight = new PointLight(position);
            tempPointLight->setColor(r, g, b);

            tempSpotLight = new SpotLight(*tempPointLight, direction, cutoffAngle);
            spotLights.push_back(tempSpotLight);
        }

        // floor parameters: floor width and tile width
        tempObject = new Floor(1000.0, 20.0);
        
        // set coefficients and shine randomly
        tempObject->setCoefficients(0.4, 0.2, 0.2, 0.2);
        tempObject->setShine(10);
        // tempObject->setCoefficients(0.1, 0.8, 0.5, 0.1);
        // tempObject->setShine(10);

        // push the floor in objects list
        objects.push_back(tempObject);

        scene.close();

    }
    else
    {
        cout << "Can't open scene.txt" << endl;
    }

    

}

void capture()
{
    bitmap_image *image;
    image = new bitmap_image(pixelWidth, pixelHeight);

    for(int j=0;j<pixelHeight;j++)
    {
        for(int i=0;i<pixelWidth;i++)
            image->set_pixel(j, i, 0, 0, 0);
    }

    double planeDistance = ((double) windowHeight / 2.0) / tan(viewAngle / 2.0 * pi / 180.0);
    Vector3D topLeft = pos + l.multiply(planeDistance) - r.multiply((double) windowWidth/2.0) 
                           + u.multiply((double) windowHeight / 2.0);

    double du = (double) windowWidth / (double) pixelWidth;
    double dv = (double) windowHeight / (double) pixelHeight;

    // choose middle of the grid cell
    topLeft = topLeft + r.multiply(du / 2.0) - u.multiply(dv / 2.0);

    cout << "Output_1" << captures <<  ".bmp" << endl;
    // capture objects for each pixel
    for(int j=0;j<pixelHeight;j++)
    {
        for(int i=0;i<pixelWidth;i++)
        {
            Vector3D currentPixel;
            currentPixel = topLeft + r.multiply(du * j) - u.multiply(dv * i);
            Ray ray(pos, currentPixel - pos);
            ray.normalize();

            
            double tmin = inf;
            Color color(0, 0, 0);
            int idx=0, objnum = -1;

            for(Object *obj: objects)
            {
                double t = obj->intersect(ray, color, 0);

                
                if(t >= 0.0 && (tmin-t) > eps)
                {
                    tmin = t;
                    objnum = idx;
                    
                }
                idx++;
                
            }
            if(abs(tmin - inf) > eps)
            {
                objects[objnum]->intersect(ray, color, 1);
                image->set_pixel(j, i, 255 * color.r, 255 * color.g, 255 * color.b);
                
            }
            
            
        }

    }
    image->save_image("Output_1" + to_string(captures) + ".bmp");
    delete image;
    
    cout << "------------------------------------------" << endl;
    
}

void clear_Vectors()
{
    cout << "Objects cleared" << endl;
    for(int i = 0; i < objects.size(); i++)
    {
        delete[] objects[i];
    }
    for(int i = 0; i < pointLights.size(); i++)
    {
        delete[] pointLights[i];
    }
    for(int i = 0; i < spotLights.size(); i++)
    {
        delete[] spotLights[i];
    }
    
    objects.clear();
    pointLights.clear();
    spotLights.clear();
    return ;

}


void drawAxes()
{
	if(drawaxes==1)
	{
		glBegin(GL_LINES);{
            glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glColor3f(0.0, 0.0, 1.0);
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


void drawSS()
{
    for(Object *obj: objects){
        obj->draw();
    }
    for(PointLight *point: pointLights)
    {
        point->draw();
    }
    for(SpotLight *spot: spotLights)
    {
        spot->draw();
    }
}


void keyboardListener(unsigned char key, int x,int y){

    double A = 1.0 * pi / 180.0;

    switch(key){
        case '0':
            captures++;
            capture();
            break;

		case '1':
            // u constant
            
            l = l.multiply(cos(A)) + u.cross(l).multiply(sin(A));
            r = r.multiply(cos(A)) + u.cross(r).multiply(sin(A));
            
            
            break;

        case '2':
            // u constant
            
            l = l.multiply(cos(-A)) + u.cross(l).multiply(sin(-A));
            r = r.multiply(cos(-A)) + u.cross(r).multiply(sin(-A));

            break;

        case '3':
            // r constant
            
            u = u.multiply(cos(A)) + r.cross(u).multiply(sin(A));
            l = l.multiply(cos(A)) + r.cross(l).multiply(sin(A));
            break;

        case '4':
            // r constant
            
            u = u.multiply(cos(-A)) + r.cross(u).multiply(sin(-A));
            l = l.multiply(cos(-A)) + r.cross(l).multiply(sin(-A));
            break;


        case '5':
            // l constant
            
            r = r.multiply(cos(A)) + l.cross(r).multiply(sin(A));
            u = u.multiply(cos(A)) + l.cross(u).multiply(sin(A));

            break;
        case '6':
            // l constant
            
            r = r.multiply(cos(-A)) + l.cross(r).multiply(sin(-A));
            u = u.multiply(cos(-A)) + l.cross(u).multiply(sin(-A));

            break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:
            pos = pos - l;
			break;
		case GLUT_KEY_UP:
            pos = pos + l;
			break;

		case GLUT_KEY_RIGHT:
			pos = pos + r;
			break;
		case GLUT_KEY_LEFT:
            pos = pos - r;
			break;

		case GLUT_KEY_PAGE_UP:
            pos = pos + u;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos = pos - u;
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
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


    // start drawing
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
	cameraAngle=1.32;


	// initialize pos of camera and directions

	pos = Vector3D(115.0, 115.0, 50.0);
	u = Vector3D(0.0, 0.0, 1.0);
	r = Vector3D(-1.0/sq2, 1.0/sq2, 0.0);
	l = Vector3D(-1.0/sq2, -1.0/sq2, 0.0);

    
	loadData();

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
    atexit(clear_Vectors);
	
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
