#include<GL/glut.h>
#include<math.h>
#include<iostream>
#include<vector>

using namespace std;

#define pi acos(0.0) * 2.0
#define sq2 1.0 / sqrt(2)
#define inf 1e18
#define AMB 0
#define DIFF 1
#define SPEC 2
#define REC_REFLECTION 3
#define eps 1e-8

int recursions;

class Vector3D {

public:
    double x, y, z;
    Vector3D()
    {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
    Vector3D(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3D operator +(Vector3D const &v)
    {
        return Vector3D(this->x+v.x, this->y+v.y, this->z+v.z);
    }
    Vector3D operator -(Vector3D const &v)
    {
        return Vector3D(this->x-v.x, this->y-v.y, this->z-v.z);
    }
    Vector3D operator -()
    {
        return Vector3D(-this->x, -this->y, -this->z);
    }
    Vector3D multiply(double a)
    {
        return Vector3D(a*this->x, a*this->y, a*this->z);
    }
    double dot(Vector3D const &v)
    {
        return (this->x * v.x + this->y * v.y + this->z * v.z);
    }
    Vector3D cross(Vector3D const &v)
    {
        return Vector3D(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
    }
    double square()
    {
        return (this->x * this->x + this->y * this->y + this->z * this->z);
    }
    void normalize()
    {
        double n = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        this->x /= n;
        this->y /= n;
        this->z /= n;
    }
    friend ostream& operator<<(ostream& os, const Vector3D& v);
    
};

class Color{

public:
    double r, g, b;

    Color()
    {
        this->r = 0.0;
        this->g = 0.0;
        this->b = 0.0;
    }
    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color operator *(double d)
    {
        return Color(this->r * d, this->g * d, this->b * d);
    }
    Color operator *(Color const &c)
    {
        return Color(this->r * c.r, this->g * c.g, this->b * c.b);
    }
    Color operator +(Color const &c)
    {
        return Color(this->r + c.r, this->g + c.g, this->b + c.b);
    }
    Color operator -(Color const &c)
    {
        return Color(this->r - c.r, this->g - c.g, this->b - c.b);
    }
    void fixRange()
    {
        this->r = std::min(this->r, 1.0);
        this->r = std::max(this->r, 0.0);
        this->g = std::min(this->g, 1.0);
        this->g = std::max(this->g, 0.0);
        this->b = std::min(this->b, 1.0);
        this->b = std::max(this->b, 0.0);
    }
    friend ostream& operator<<(ostream& os, const Color& c);
    
    
};

class QuadraticCoefficients{

public:
    double A, B, C, D, E, F, G, H, I, J;

    QuadraticCoefficients()
    {
        this->A = this->B = this->C = this->D = this->E =
        this->F = this->G = this->H = this->I = this->J = 0.0;
    }
    QuadraticCoefficients(double A, double B, double C, double D, double E,
                            double F, double G, double H, double I, double J)
    {
        this->A = A, this->B = B, this->C = C, this->D = D, this->E = E;
        this->F = F, this->G = G,this->H = H, this->I = I, this->J = J;
    }
    friend ostream& operator<<(ostream& os, const QuadraticCoefficients& q);
    
};

class Ray{

public:
    Vector3D start;
    Vector3D dir;

    Ray()
    {

    }
    Ray(Vector3D start, Vector3D dir)
    {
        this->start = start;
        this->dir = dir;
        this->normalize();
    }

    void normalize()
    {
        this->dir.normalize();
    }   
    friend ostream& operator<<(ostream& os, const Ray& r);

};



class PointLight{

public:
    Vector3D lightPos;
    Color color;

    PointLight()
    {

    }
    PointLight(Vector3D lightPos){
        this->lightPos = lightPos;
    }
    void setColor(double r, double g, double b)
    {
        this->color = Color(r, g, b);
    }
    void draw()
    {
        glPushMatrix();
        {

            glTranslatef(this->lightPos.x, this->lightPos.y, this->lightPos.z);
            double radius = 0.5;

            Vector3D points[100][100];
            int i,j;
            double h,r;
            int stacks = 20, slices=24;
            //generate points
            for(i=0;i<=stacks;i++)
            {
                h=radius*sin(((double)i/(double)stacks)*(pi/2));
                r=radius*cos(((double)i/(double)stacks)*(pi/2));
                for(j=0;j<=slices;j++)
                {
                    points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                    points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                    points[i][j].z=h;
                }
            }
            //draw quads using generated points
            glColor3f(this->color.r, this->color.g, this->color.b);

            for(i=0;i<stacks;i++)
            {
                for(j=0;j<slices;j++)
                {
                    glBegin(GL_QUADS);{
                        //upper hemisphere
                        glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                        glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                        //lower hemisphere
                        glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                        glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                    }glEnd();
                }
            }

        }
        glPopMatrix();
    }
    friend ostream& operator<<(ostream& os, const PointLight& p);
    
};

class SpotLight{

public:
    PointLight pointLight;
    Vector3D lightDirection;
    double cutoffAngle;

    SpotLight()
    {

    }
    SpotLight(PointLight pointLight, Vector3D lightDirection, double cutoffAngle)
    {
        this->pointLight = pointLight;
        this->lightDirection = lightDirection;
        this->cutoffAngle = cutoffAngle;
    }
    bool exceedsCutoff(Ray &ray)
    {
        double dot = this->lightDirection.dot(ray.dir);
        dot /= sqrt(this->lightDirection.square() * ray.dir.square()); 
        double theta = acos(abs(dot));

        return (theta > (this->cutoffAngle * pi / 180.0));
    }
    void draw()
    {
        this->pointLight.draw();
    }
    friend ostream& operator<<(ostream& os, const SpotLight& s);
    
};




class Object {

public:
    Vector3D referencePoint;
    double height, width, length;
    Color color;
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component

    Object()
    {

    }
    Object(Vector3D referencePoint)
    {
        this->referencePoint = referencePoint;
    }
    Object(Vector3D referencePoint, double length, double width, double height)
    {
        this->referencePoint = referencePoint;
        this->height = height;
        this->width = width;
        this->length = length;
    }
    void setColor(double r, double g, double b)
    {
        this->color = Color(r, g, b);
    }
    
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setCoefficients(double ambient, double diffuse, double specular, double reflection)
    {
        this->coEfficients[0] = ambient;
        this->coEfficients[1] = diffuse;
        this->coEfficients[2] = specular;
        this->coEfficients[3] = reflection;
    }
    virtual void draw()
    {
        cout << "base draw function" << endl;
    }
    
    virtual Color getColorAtPoint(Vector3D &point)
    {
        cout << "base getColorAtPoint function" << endl;
        return this->color;
    }
    virtual double getT(Ray &ray)
    {
        cout << "base getT function" << endl;
        return -1.0;
    }
    virtual Vector3D getNormal(Ray &ray, Vector3D &intersectionPoint)
    {
        cout << "base getNormal function" << endl;
        return Vector3D(0, 0, 0);
    }
    double intersect(Ray &ray, Color &color, int level);
    void getColorByPhongsModel(double tCurrent, Ray &ray, Color &color, Vector3D &intersectionPoint,
                                Color &intersectionColor, Vector3D &normal);
    
    friend ostream& operator<<(ostream& os, const Object& o);
    

};

vector<Object *> objects;
vector<PointLight *> pointLights;
vector<SpotLight *> spotLights;

void Object::getColorByPhongsModel(double tCurrent, Ray &ray, Color &finalColor, Vector3D &intersectionPoint,
                                Color &intersectionColor, Vector3D &normal)
{

    // do lighting and reflection
    finalColor = (intersectionColor * (this->coEfficients[AMB]));
    finalColor.fixRange();
    
    for(PointLight *pl: pointLights)
    {
        Ray rayL = Ray(pl->lightPos, intersectionPoint - pl->lightPos);
        rayL.normalize();
        
        // check if the ray intersects with any object
        double tmin = inf;
        for(Object *o: objects)
        {
            double tnow = o->intersect(rayL, color, 0);
            if(tnow > 0.0 && (tmin - tnow)  > eps)
            {
                tmin = tnow;
            }
        }

        if((tCurrent - tmin) > eps)
        {
            continue;
        }

        // calculate the lighting by phong model
        double lambertValue = normal.dot(-rayL.dir);
        Ray rayR = Ray(intersectionPoint, normal.multiply(2.0 * lambertValue) + rayL.dir);
        rayR.normalize();
        double phongValue = pow(rayR.dir.dot(-ray.dir), this->shine);

        finalColor = finalColor + pl->color * this->coEfficients[DIFF] * max(lambertValue, 0.0) * intersectionColor;
        finalColor.fixRange();
        finalColor = finalColor + pl->color * this->coEfficients[SPEC] * max(phongValue, 0.0) ;
        finalColor.fixRange();
    }

    for(SpotLight *spl: spotLights)
    {
        Ray rayL = Ray(spl->pointLight.lightPos, intersectionPoint - spl->pointLight.lightPos);
        rayL.normalize();

        if(spl->exceedsCutoff(rayL))
        {
            continue;
        }
        
        // check if the ray intersects with any object
        double tmin = inf;
        for(Object *o: objects)
        {
            double tnow = o->intersect(rayL, color, 0);
            if(tnow > 0.0 && (tmin - tnow)  > eps)
            {
                tmin = tnow;
            }
        }

        if((tCurrent - tmin) > eps)
        {
            continue;
        }

        // calculate the lighting by phong model
        double lambertValue = normal.dot(-rayL.dir);
        Ray rayR = Ray(intersectionPoint, normal.multiply(2.0 * lambertValue) + rayL.dir);
        rayR.normalize();
        double phongValue = pow(rayR.dir.dot(-ray.dir), this->shine);

        finalColor = finalColor + spl->pointLight.color * this->coEfficients[DIFF] * max(lambertValue, 0.0) * intersectionColor;
        finalColor.fixRange();
        finalColor = finalColor + spl->pointLight.color * this->coEfficients[SPEC] * max(phongValue, 0.0) ;
        finalColor.fixRange();
    }
    return ;
}

double Object::intersect(Ray &ray, Color &color, int level=0)
{
    ray.normalize();

    double t = getT(ray);

    if(level == 0 || t < 0) return t;

    // do lighting and reflection
    Vector3D intersectionPoint = ray.start + ray.dir.multiply(t);
    Color intersectionColor = this->getColorAtPoint(intersectionPoint);
    
    // calculate normal
    Vector3D normal = this->getNormal(ray, intersectionPoint);

    getColorByPhongsModel(t, ray, color, intersectionPoint, intersectionColor, normal);
    // recursive reflection
    if(level >= recursions) return t;

    Ray reflected = Ray(intersectionPoint, normal.multiply(2.0 * normal.dot(-ray.dir)) + ray.dir);
    // slightly forward for avoiding self intersection
    reflected.start = reflected.start + reflected.dir.multiply(0.00001);
    reflected.normalize();

    double tmin = inf;
    int objnum = -1, idx = 0;
    Color reflectedColor;
    for(Object *obj: objects)
    {
        double tt = obj->intersect(reflected, reflectedColor, 0);
        if(tt >= 0.0 && (tmin-tt) > eps)
        {
            tmin = tt;
            objnum = idx;
            
        }
        idx++;
    }

    if(abs(tmin - inf) > eps)
    {
        objects[objnum]->intersect(reflected, reflectedColor, level + 1);
        color = color + reflectedColor * this->coEfficients[REC_REFLECTION];  
        color.fixRange();          
    }
    
    return t;
}

class Triangle: public Object {


public:
    Vector3D point1, point2, point3;

    Triangle()
    {

    }
    Triangle(Vector3D point1, Vector3D point2, Vector3D point3)
    {
        this->point1 = point1;
        this->point2 = point2;
        this->point3 = point3;
    }
    Color getColorAtPoint(Vector3D &point)
    {
        return this->color;
    }
    void draw()
    {
        glPushMatrix();
        {
            glColor3f(this->color.r, this->color.g, this->color.b);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(point1.x, point1.y, point1.z);
                glVertex3f(point2.x, point2.y, point2.z);
                glVertex3f(point3.x, point3.y, point3.z);
            }
            glEnd();
        }
        glPopMatrix();
    }
    double getT(Ray &ray)
    {
        double detA = (point1.x - point2.x) * ((point1.y - point3.y) * ray.dir.z - (point1.z-point3.z) * ray.dir.y);
        detA -= (point1.y - point2.y) * ((point1.x - point3.x) * ray.dir.z - (point1.z - point3.z) * ray.dir.x);
        detA += (point1.z - point2.z) * ((point1.x - point3.x) * ray.dir.y - (point1.y - point3.y) * ray.dir.x);

        if(abs(detA) < eps)
        {
            return -1.0;
        }

        double detBeta = (point1.x - ray.start.x) * ((point1.y - point3.y) * ray.dir.z - (point1.z-point3.z) * ray.dir.y);
        detBeta -= (point1.y - ray.start.y) * ((point1.x - point3.x) * ray.dir.z - (point1.z - point3.z) * ray.dir.x);
        detBeta += (point1.z - ray.start.z) * ((point1.x - point3.x) * ray.dir.y - (point1.y - point3.y) * ray.dir.x);

        double detGamma = (point1.x - point2.x) * ((point1.y - ray.start.y) * ray.dir.z - (point1.z-ray.start.z) * ray.dir.y);
        detGamma -= (point1.y - point2.y) * ((point1.x - ray.start.x) * ray.dir.z - (point1.z - ray.start.z) * ray.dir.x);
        detGamma += (point1.z - point2.z) * ((point1.x - ray.start.x) * ray.dir.y - (point1.y - ray.start.y) * ray.dir.x);

        double detT = (point1.x - point2.x) * ((point1.y - point3.y) * (point1.z - ray.start.z) - (point1.z-point3.z) * (point1.y - ray.start.y));
        detT -= (point1.y - point2.y) * ((point1.x - point3.x) * (point1.z - ray.start.z) - (point1.z - point3.z) * (point1.x - ray.start.x));
        detT += (point1.z - point2.z) * ((point1.x - point3.x) * (point1.y - ray.start.y) - (point1.y - point3.y) * (point1.x - ray.start.x));

        double beta = detBeta / detA;
        double gamma = detGamma / detA;
        double t = detT / detA;

        if(beta < 0.0 || (beta-1.0) > eps || gamma < 0.0 || (gamma - 1.0) > eps || t < 0.0 || (beta+gamma-1.0)>eps)
        {
            return -1.0;
        }

        return t;
    }
    Vector3D getNormal(Ray &ray, Vector3D &intersectionPoint)
    {
        Vector3D normal = (this->point1 - this->point2).cross(this->point3 - this->point2);
        normal.normalize();

        if(ray.dir.dot(normal) > 0.0)
        {
            normal = normal.multiply(-1.0);
        }
        return normal;
    }

    friend ostream& operator<<(ostream& os, const Triangle& t);
   

};



class Sphere: public Object {


public:
    Sphere(Vector3D center, double radius)
    {
        this->referencePoint = center;
        this->length = radius;
        this->width = radius;
        this->height = radius;
    }
    Color getColorAtPoint(Vector3D &point)
    {
        return this->color;
    }

    void draw()
    {
        glPushMatrix();
        {

            glTranslatef(this->referencePoint.x, this->referencePoint.y, this->referencePoint.z);
            double radius = this->length;

            Vector3D points[100][100];
            int i,j;
            double h,r;
            int stacks = 20, slices=24;
            //generate points
            for(i=0;i<=stacks;i++)
            {
                h=radius*sin(((double)i/(double)stacks)*(pi/2));
                r=radius*cos(((double)i/(double)stacks)*(pi/2));
                for(j=0;j<=slices;j++)
                {
                    points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                    points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                    points[i][j].z=h;
                }
            }
            //draw quads using generated points
            glColor3f(this->color.r, this->color.g, this->color.b);

            for(i=0;i<stacks;i++)
            {
                for(j=0;j<slices;j++)
                {
                    glBegin(GL_QUADS);{
                        //upper hemisphere
                        glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                        glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                        //lower hemisphere
                        glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                        glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                    }glEnd();
                }
            }

        }
        glPopMatrix();
    }

    double getT(Ray &ray)
    {
        Vector3D r0 = ray.start - this->referencePoint;
        ray.normalize();

        double tp = -r0.dot(ray.dir);
        if(tp < 0.0)
        {
            return -1.0;
        }
        double d2 = r0.square() - tp * tp;
        
        if((d2 - this->length * this->length) > eps)
        {
            return -1.0;
        }
        double tprime = sqrt(this->length * this->length - d2);
        
        double t1 = tp + tprime;
        double t2 = tp - tprime;

        double t = -1.0;
        
        if((r0.square() - this->length * this->length) > eps)
        {
            if(t2 >= 0.0)
            {
                t = t2;
            }
        }
        else 
        {
            if(t1 >= 0.0) 
            {
                t = t1;
            }  
        }

        return t;

    }
    Vector3D getNormal(Ray &ray, Vector3D &intersectionPoint)
    {
        Vector3D normal = intersectionPoint - this->referencePoint;
        normal.normalize();

        if((ray.start - this->referencePoint).square() - this->length * this->length > eps)
        {
            normal = normal.multiply(-1.0);
        }
        return normal;

    }
    
    friend ostream& operator<<(ostream& os, const Sphere& s);
    


};



class General: public Object {


public:
    QuadraticCoefficients coeff;
    Vector3D cubeReferencePoint;

    General()
    {

    }
    General(QuadraticCoefficients coefficients, Vector3D cubeReferencePoint,
            double length, double width, double height)
    {
        this->cubeReferencePoint = cubeReferencePoint;
        this->length = length;
        this->width = width;
        this->height = height;
        this->coeff = coefficients;
       
    }
    Color getColorAtPoint(Vector3D &point)
    {
        return this->color;
    }
    void draw(){

    }
    bool isInsideCube(Vector3D &point)
    {
        if(length > 0.0 && ((this->cubeReferencePoint.x - point.x) > eps  || (point.x - this->cubeReferencePoint.x - this->length) > eps ))
        {
            return false;
        }
        if(width > 0.0 && ((this->cubeReferencePoint.y - point.y) > eps || (point.y -  this->cubeReferencePoint.y - this->width)  > eps))
        {
            return false;
        }
        if(height > 0.0 && ((this->cubeReferencePoint.z - point.z) > eps  || (point.z  - this->cubeReferencePoint.z - this->height) > eps))
        {
            return false;
        }
        return true;
        
    }
    double getT(Ray &ray)
    {
        double a, b, c;
        a = coeff.A  * ray.dir.x * ray.dir.x + coeff.B * ray.dir.y * ray.dir.y + coeff.C * ray.dir.z * ray.dir.z;
        a += coeff.D * ray.dir.x * ray.dir.y + coeff.E * ray.dir.x * ray.dir.z + coeff.F * ray.dir.y * ray.dir.z;

        b = 2.0 * (coeff.A * ray.dir.x * ray.start.x + coeff.B * ray.dir.y * ray.start.y + coeff.C * ray.dir.z * ray.start.z);
        b += coeff.D * (ray.start.y * ray.dir.x + ray.start.x * ray.dir.y);
        b += coeff.E * (ray.start.x * ray.dir.z + ray.start.z * ray.dir.x);
        b += coeff.F * (ray.start.y * ray.dir.z + ray.start.z * ray.dir.y);
        b += coeff.G * ray.dir.x + coeff.H * ray.dir.y + coeff.I * ray.dir.z;

        c = coeff.A * ray.start.x * ray.start.x + coeff.B * ray.start.y * ray.start.y + coeff.C * ray.start.z * ray.start.z;
        c += coeff.D * ray.start.x * ray.start.y + coeff.E * ray.start.x * ray.start.z + coeff.F * ray.start.y * ray.start.z;
        c += coeff.G * ray.start.x + coeff.H * ray.start.y + coeff.I * ray.start.z;
        c += coeff.J;


        

        double t = -1.0;
        if(a == 0.0 && b == 0.0)
        {
            return -1.0;
        }
        else if(a == 0.0)
        {
            double t = -c / b;
            Vector3D intersection = ray.start + ray.dir.multiply(t);
            if(!isInsideCube(intersection))
            {
                return -1.0;
            }
        }
        else 
        {
            double d = b*b - 4.0*a*c;
            if (d < 0.0)
            {
                return -1.0;
            }
            else if(d == 0.0)
            {
                double t = -b / (2.0 * a);
                Vector3D intersection = ray.start + ray.dir.multiply(t);
                if(!isInsideCube(intersection))
                {    
                    return -1.0;
                }
            }
            else 
            {
                double t1 = (-b - sqrt(d)) / (2.0 * a);
                double t2 = (-b + sqrt(d)) / (2.0 * a);
                if(t1 > 0.0)
                {
                    Vector3D intersection = ray.start + ray.dir.multiply(t1);
                    if(isInsideCube(intersection))
                    {
                        t = t1;
                    }
                    else t1 = -1.0;
                    
                }
                if(t1 < 0.0 && t2 > 0.0)
                {
                    Vector3D intersection = ray.start + ray.dir.multiply(t2);
                    if(isInsideCube(intersection))
                    {
                        t = t2;
                    }
                    
                }
                
            }

        }

        return t;

    }
    Vector3D getNormal(Ray &ray, Vector3D &intersectionPoint)
    {
        Vector3D normal;
        normal.x = 2.0 * coeff.A * intersectionPoint.x + coeff.D * intersectionPoint.y + coeff.E * intersectionPoint.z + coeff.G;
        normal.y = 2.0 * coeff.B * intersectionPoint.y + coeff.D * intersectionPoint.x + coeff.F * intersectionPoint.z + coeff.H;
        normal.z = 2.0 * coeff.C * intersectionPoint.z + coeff.E * intersectionPoint.x + coeff.F * intersectionPoint.y + coeff.I;

        normal.normalize();

        if((ray.start - this->referencePoint).dot(normal) > 0.0)
        {
            normal = normal.multiply(-1.0);
        }
        return normal;
    }
    
    friend ostream& operator<<(ostream& os, const General& g);
    


};




class Floor: public Object {


public:

    double floorWidth, tileWidth;
    Color color2;

    Floor(double floorWidth, double tileWidth)
    {
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        this->referencePoint = Vector3D(-floorWidth/2.0, -floorWidth/2.0, 0.0);
        this->length = floorWidth;
        
        this->color = Color(1.0, 1.0, 1.0);
        this->color2 = Color(0.0, 0.0, 0.0);
    }

    Color getColorAtPoint(Vector3D &point)
    {
        int i = (point.x - this->referencePoint.x) / tileWidth;
        int j = (point.y - this->referencePoint.y) / tileWidth;
        if((i+j) % 2 == 0) return this->color;
        else return this->color2;    
    }

    bool outside(Vector3D &intersectionPoint)
    {
        return (intersectionPoint.x < this->referencePoint.x || intersectionPoint.x > (this->referencePoint.x + this->floorWidth)) 
        || (intersectionPoint.y < this->referencePoint.y || intersectionPoint.y > (this->referencePoint.y + this->floorWidth));
    }

    void draw()
    {
        glPushMatrix();
        {
            int n = this->floorWidth / this->tileWidth;
            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    double x = this->referencePoint.x + tileWidth * i;
                    double y = this->referencePoint.y + tileWidth * j;

                    if((i+j) % 2 == 0)
                    {
                        glColor3f(color.r, color.g, color.b);
                    }
                    else
                    {
                        glColor3f(color2.r, color2.g, color2.b);
                    }

                    glBegin(GL_QUADS); {
                        glVertex3f(x, y, 0.0);
                        glVertex3f(x + tileWidth, y, 0.0);
                        glVertex3f(x + tileWidth, y + tileWidth, 0.0);
                        glVertex3f(x, y + tileWidth, 0.0);
                    }
                    glEnd();
                }
            }

        }
        glPopMatrix();
    }
    double getT(Ray &ray)
    {
        Vector3D normal = Vector3D(0.0, 0.0, 1.0);
        if(ray.dir.dot(normal) == 0.0) 
        {
            return -1.0;
        }

        double t = -ray.start.dot(normal) / ray.dir.dot(normal);
        Vector3D intersectionPoint = ray.start + ray.dir.multiply(t);
        if(this->outside(intersectionPoint))
        {
            return -1.0;
        }
        return t;
    }
    Vector3D getNormal(Ray &ray, Vector3D &intersectionPoint)
    {
        Vector3D normal = Vector3D(0.0, 0.0, 1.0);
        if(ray.start.z < 0.0)
            normal.z = -1.0;
        return normal;
    }
    
    friend ostream& operator<<(ostream& os, const Floor& f);
    
};




// ------------------------------- output friend functions -------------------------------
ostream & operator << (ostream &out, const Vector3D &v)
{
    out << "Vector3D (" << v.x << ' ' <<  v.y << ' ' << v.z << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Color &c)
{
    out << "Color (" << c.r << ' ' <<  c.g << ' ' << c.b << ")\n" ;
    return out;
}


ostream & operator << (ostream &out, const QuadraticCoefficients &q)
{
    out << "QuadraticCoefficients (" << q.A << ' ' <<  q.B << ' ' << q.C << ' ' << 
    q.D << ' ' << q.E << ' ' << q.F << ' ' << q.G << ' ' << q.H << ' ' << q.I << ' ' << q.J << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Ray &r)
{
    out << "Ray ( start: " << r.start << " , dir: " << r.dir << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Object &o)
{
    out << "Object (" << o.referencePoint << " " << o.color << ")\n" ;
    out << "length " << o.length << ' ' << " width " << o.width << " height " << o.height << "\n";
    out << "coefficients (" << o.coEfficients[0] << ' ' << o.coEfficients[1] << ' ' << o.coEfficients[2] << ' ' << o.coEfficients[3] << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Sphere &s)
{
    out << "Sphere (" << s.referencePoint << " " << s.color << ' ' << s.length << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Triangle &t)
{
    out << "General (" << t.point1 << " " << t.point2 << ' ' << t.point3 << ' ' << t.color << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const General &g)
{
    out << "General (" << g.referencePoint << " " << g.color << ' ' << 
    g.coEfficients[0] << ' ' << g.coEfficients[1] << ' ' << g.coEfficients[2] << 
    ' ' << g.coEfficients[3] << ' ' << g.coeff << ")\n" ;
    return out;
}
ostream & operator << (ostream &out, const Floor &f)
{
    out << "Floor (" << f.referencePoint << " " << f.color << ' ' << f.color2 << ' ' << f.floorWidth << ' ' << f.tileWidth << ")\n" ;
    return out;
}

