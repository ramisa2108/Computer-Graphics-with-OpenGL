#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<string>
#include<vector>
#include<algorithm>
#include "bitmap_image.hpp"

using namespace std;

#define pi acos(-1)
#define inf 1e18
#define eps 1e-8

class Vector{

public:

    double x, y, z;

    Vector(double x=0.0, double y=0.0, double z=0.0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void normalize()
    {
        double n = sqrt(x*x + y*y + z*z);
        x /= n, y /= n, z /= n;
    }
    Vector operator *(double c)
    {
        return Vector(c*x, c*y, c*z);
    }
    Vector operator +(Vector v)
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    Vector operator -(Vector v)
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    Vector operator -()
    {
        return Vector(-x, -y, -z);
    }
    double dotProduct(Vector v)
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vector crossProduct(Vector v)
    {
        Vector result;
        result.x = y * v.z - z * v.y;
        result.y = z * v.x - x * v.z;
        result.z = x * v.y - y * v.x;
        return result;
    }
    Vector rodriguesFormula(double angle, Vector axis)
    {
        angle = angle * pi / 180.0;
        Vector v1 = *this * (cos(angle));
        Vector v2 = axis * ((1-cos(angle)) * this->dotProduct(axis));
        Vector v3 = axis.crossProduct(*this) * (sin(angle));
        return v1 + v2 + v3;

    }
    void print(ofstream &out)
    {
        out << x << ' ' << y << ' ' << z << endl;
    }
    void print()
    {
        cout << x << ' ' << y << ' ' << z << endl;
    }
};

class Triangle{

public:
    Vector points[3];
    int colors[3];

    Triangle(Vector v1, Vector v2, Vector v3)
    {
        points[0] = v1, points[1] = v2, points[2] = v3;
        for(int i=0;i<3;i++)
        {
            colors[i] = rand() % 256;
        }

    }

    pair<Vector, Vector> findIntersectingPoints(double y)
    {
        pair<Vector, Vector>intersections;

        intersections.first = Vector(inf, y, inf);      // left intersecting point (min x)
        intersections.second = Vector(-inf, y, -inf);   // right intersecting point (max x)

        for(int i=0;i<3;i++)
        {
            int j = (i+1) % 3;
            if(abs(points[i].y - points[j].y) < eps)
            {
                if(abs(points[i].y - y) < eps)
                {
                    if(points[i].x < intersections.first.x)
                    {
                        intersections.first = Vector(points[i].x, y, points[i].z);
                    }
                    if(points[i].x > intersections.second.x)
                    {
                        intersections.second = Vector(points[i].x, y, points[i].z);

                    }

                    if(points[j].x < intersections.first.x)
                    {
                        intersections.first = Vector(points[j].x, y, points[j].z);
                    }
                    if(points[j].x > intersections.second.x)
                    {
                        intersections.second = Vector(points[j].x, y, points[j].z);

                    }

                }
                continue;
            }
            double x = points[i].x + (y - points[i].y) * (points[i].x - points[j].x) / (points[i].y - points[j].y);
            double z = points[i].z + (y - points[i].y) * (points[i].z - points[j].z) / (points[i].y - points[j].y);

            if(x >= min(points[i].x, points[j].x) && x <= max(points[i].x, points[j].x))
            {
                if (x < intersections.first.x)
                {
                    intersections.first = Vector(x, y, z);
                }
                if (x > intersections.second.x)
                {
                    intersections.second = Vector(x, y, z);
                }

            }
        }
        return intersections;
    }

    void print()
    {
        for(int i=0;i<3;i++) points[i].print();
        cout << "colors: " << colors[0] << ' ' << colors[1] << ' ' << colors[2] << endl;
    }

};

class Matrix
{
public:

    int rows, cols;
    double **values;

    Matrix(int rows=4, int cols=4)
    {
        this->rows = rows;
        this->cols = cols;
        this->values = new double*[rows];

        for(int i=0;i<rows;i++)
        {
            this->values[i] = new double[cols];
        }

        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++) this->values[i][j] = 0.0;
        }

    }

    Matrix(Vector v)
    {
        this->rows = 4;
        this->cols = 1;
        this->values = new double*[4];
        for(int i=0;i<4;i++) this->values[i] = new double[1];

        values[0][0] = v.x, values[1][0] = v.y, values[2][0] = v.z, values[3][0] = 1.0;

    }
    void setRow(int i, Vector v)
    {
        values[i][0] = v.x, values[i][1] = v.y, values[i][2] = v.z;
    }
    void setCol(int i, Vector v)
    {
        values[0][i] = v.x, values[1][i] = v.y, values[2][i] =v.z;
    }
    void setIdentityMatrix()
    {
        if(rows != cols)
        {
            cout << "Identity Matrix has to be square!" << endl;
            return ;
        }
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(i==j) values[i][j] = 1.0;
                else values[i][j] = 0.0;
            }
        }
    }
    void setTranslationMatrix(Vector v)
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(i==j) values[i][j] = 1.0;
                else values[i][j] = 0.0;
            }

        }
        values[0][3] = v.x, values[1][3] = v.y , values[2][3] = v.z;
    }
    void setScaleMatrix(Vector v)
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                if(i==j) values[i][j] = 1.0;
                else values[i][j] = 0.0;
            }

        }
        values[0][0] = v.x, values[1][1] = v.y, values[2][2] = v.z;
    }
    void setRotationMatrix(double angle, Vector axis)
    {
        axis.normalize();
        Vector c1, c2, c3;
        c1 = Vector(1.0, 0.0, 0.0).rodriguesFormula(angle, axis);
        c2 = Vector(0.0, 1.0, 0.0).rodriguesFormula(angle, axis);
        c3 = Vector(0.0, 0.0, 1.0).rodriguesFormula(angle, axis);

        setCol(0, c1);
        setCol(1, c2);
        setCol(2, c3);
        values[3][3] = 1.0;
        return ;

    }
    void operator =(struct Matrix m)
    {
        if(m.cols != cols || m.rows != rows)
        {
            cout << "Can't copy Matrix of dims (" << m.rows << ", " << m.cols << ") into dims (" << rows << ", " << cols << ")" << endl;
            return ;
        }
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
                values[i][j] = m.values[i][j];
        }
    }
    struct Matrix multiply(struct Matrix m, bool normalize=false)
    {
        if(cols != m.rows)
        {
            cout << "Matrix dimensions don't match for multiplication (" << rows << ", " << cols << ") and (" << m.rows << ", " << m.cols << ")" << endl;
            return Matrix();
        }

        Matrix result(rows, m.cols);

        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<m.cols;j++)
            {
                for(int k=0;k<cols;k++)
                    result.values[i][j] += values[i][k] * m.values[k][j];
            }
        }
        if(normalize && m.cols == 1)
        {
            for(int i=0;i<result.rows-1;i++) result.values[i][0] /= result.values[result.rows-1][0];
        }
        return result;
    }



    void print()
    {
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++) cout << values[i][j] <<  ' ';
            cout << endl;
        }

        return ;
    }
};

class StackElement{
public:

    Matrix elem;
    StackElement *prev;
    StackElement()
    {
        this->prev = NULL;
    }
    StackElement(Matrix elem)
    {
        this->elem = elem;
        this->prev = NULL;
    }
    StackElement(Matrix elem, StackElement *prev)
    {
        this->elem = elem;
        this->prev = prev;
    }

    void setPrev(StackElement *s)
    {
        prev = s;
    }
    void copyElement(StackElement *s)
    {
        elem = s->elem;
    }
    void print()
    {
        elem.print();
    }
    void printAll()
    {
        cout << "Stack :\n========================\n";
        StackElement *s = prev;
        print();
        while(s != NULL)
        {
            s->print();
            s = s->prev ;
        }
        cout << "========================\n" ;
    }

};



StackElement *top = new StackElement();

Vector transformVector(Vector v, Matrix m)
{
    Matrix mv(v);
    Matrix res = m.multiply(mv, true);
    if(res.rows != 4 || res.cols != 1)
    {
        cout << "Problem in multiplication" << endl;
    }
    Vector transp = Vector(res.values[0][0], res.values[1][0], res.values[2][0]);
    return transp;
}


int main()
{

    srand(time(0));

    ifstream scene("scene.txt");
    ifstream config("config.txt");

    ofstream stage1, stage2, stage3, zbuffer;
    stage1.open("stage1.txt");
    stage2.open("stage2.txt");
    stage3.open("stage3.txt");
    zbuffer.open("z_buffer.txt");

    Vector eye, look, up;
    double fovX, fovY, aspectRatio, near, far;

    string command;
    Vector v1, v2, v3, t, a, s;
    double angle;

    Vector l, r, u;

    Matrix identity;
    identity.setIdentityMatrix();

    top->elem = identity;

    stage1 << fixed << setprecision(7);
    stage2 << fixed << setprecision(7);
    stage3 << fixed << setprecision(7);
    zbuffer << fixed << setprecision(6);




    if (scene.is_open())
    {
        scene >> eye.x >> eye.y >> eye.z;
        scene >> look.x >> look.y >> look.z;
        scene >> up.x >> up.y >> up.z;
        scene >> fovY >> aspectRatio >> near >> far;


        // set view transformation
        l = look - eye;
        l.normalize();

        r = l.crossProduct(up);
        r.normalize();

        u = r.crossProduct(l);
        u.normalize();

        Matrix T, R;
        T.setIdentityMatrix();
        T.setCol(3, -eye);

        R.setIdentityMatrix();
        R.setRow(0, r);
        R.setRow(1, u);
        R.setRow(2, -l);

        Matrix V = R.multiply(T);

        // set projection transformation
        fovX = fovY * aspectRatio;
        double pt, pr;
        Matrix P;
        pt = near * tan(fovY * pi / 180.0 / 2.0);
        pr = near * tan(fovX * pi/ 180.0 / 2.0);

        P.values[0][0] = near / pr;
        P.values[1][1] = near / pt;
        P.values[2][2] = -(far + near) / (far - near), 
        P.values[2][3] = -(2.0 * far * near)/(far - near);
        P.values[3][2] = -1.0;

        while(!scene.eof())
        {
            scene >> command;
            if(command == "triangle")
            {
                // transform the triangle
                scene >> v1.x >> v1.y >> v1.z;
                scene >> v2.x >> v2.y >> v2.z;
                scene >> v3.x >> v3.y >> v3.z;

                // modeling transformation
                v1 = transformVector(v1, top->elem);
                v2 = transformVector(v2, top->elem);
                v3 = transformVector(v3, top->elem);

                v1.print(stage1); v2.print(stage1); v3.print(stage1);
                stage1 << endl;

                // view transformation
                v1 = transformVector(v1, V);
                v2 = transformVector(v2, V);
                v3 = transformVector(v3, V);

                v1.print(stage2);  v2.print(stage2);  v3.print(stage2);
                stage2 << endl;

                // projection transformation
                v1 = transformVector(v1, P);
                v2 = transformVector(v2, P);
                v3 = transformVector(v3, P);

                v1.print(stage3);  v2.print(stage3);  v3.print(stage3);
                stage3 << endl;



            }
            else if(command == "translate")
            {
                // translation
                scene >> t.x >> t.y >> t.z;
                Matrix translation;
                translation.setTranslationMatrix(t);
                top->elem = top->elem.multiply(translation);

            }
            else if(command == "scale")
            {
                // scale
                scene >> s.x >> s.y >> s.z;
                Matrix scale;
                scale.setScaleMatrix(s);
                top->elem = top->elem.multiply(scale);
            }
            else if(command == "rotate")
            {
                // rotation
                scene >> angle >> a.x >> a.y >> a.z;

                Matrix rotation;
                rotation.setRotationMatrix(angle, a);
                top->elem = top->elem.multiply(rotation);
            }
            else if(command == "push")
            {
                // push Matrix
                StackElement *s;
                if(top == NULL)
                {
                    s = new StackElement(identity);
                }
                else
                {
                    s = new StackElement(top->elem, top);
                }
                top = s;

            }
            else if(command == "pop")
            {
                // pop Matrix
                if(top == NULL)
                {
                    cout << "nothing to pop" << endl;
                }
                else
                {
                    top = top->prev;
                }

            }
            else if(command == "end")
            {
                break;
            }

        }

    }
    else
    {
        cout << "Can't open scene.txt" << endl;
        return 0;
    }

    stage3.close();
    stage2.close();
    stage1.close();


    int screen_width, screen_height;
    double xmin, xmax, ymin, ymax, zmin, zmax, dx, dy, topY, bottomY, rightX, leftX;

    double **zBuffer;
    vector<Triangle>allTriangles;


    ifstream stage3_in("stage3.txt");

    if(stage3_in.is_open())
    {

        while(!stage3_in.eof())
        {
            stage3_in >> v1.x >> v1.y >> v1.z;
            stage3_in >> v2.x >> v2.y >> v2.z;
            stage3_in >> v3.x >> v3.y >> v3.z;
            stage3_in.ignore();
            Triangle triangle(v1, v2, v3);
            allTriangles.push_back(triangle);

        }
    }
    else
    {
        cout << "Can't read stage3.txt" << endl;
        return 0;
    }


    if(config.is_open())
    {
        config >> screen_width >> screen_height;
        config >> xmin;
        xmax = xmin * -1.0;
        config >> ymin;
        ymax = ymin * -1.0;
        config >> zmin >> zmax;

        dx = (xmax - xmin) / screen_width;
        dy = (ymax - ymin) / screen_height;

        topY = ymax - dy / 2.0;
        bottomY = ymin + dy / 2.0;
        leftX = xmin + dx / 2.0;
        rightX = xmax - dx / 2.0;

        bitmap_image *image;
        image = new bitmap_image(screen_width, screen_height);

        for(int i=0;i<screen_width;i++)
        {
            for(int j=0;j<screen_height;j++)
                image->set_pixel(i,j,0, 0, 0);
        }

        zBuffer = new double*[screen_width];

        for(int i=0;i<screen_width;i++)
        {
            zBuffer[i] = new double[screen_height];
            for(int j=0;j<screen_height;j++)
            {
                zBuffer[i][j] = zmax;
            }
        }

        for(Triangle triangle: allTriangles)
        {

            double maxY = max({triangle.points[0].y, triangle.points[1].y, triangle.points[2].y});
            if(topY - maxY < eps) maxY = topY;
            else
            {
                int row_num = round ((ymax - maxY) / dy);
                maxY = topY - row_num * dy;
            }

            double minY = min({triangle.points[0].y, triangle.points[1].y, triangle.points[2].y});
            if(minY - bottomY < eps) minY = bottomY;
            else
            {
                int row_num = round((minY - ymin) / dy);
                minY = bottomY + row_num * dy;
            }

            for(double y=minY; (y-maxY) < eps; y+=dy)
            {

                pair<Vector, Vector> intersections = triangle.findIntersectingPoints(y);

                double minX = intersections.first.x;
                double maxX = intersections.second.x;

                if(minX-leftX < eps) minX = leftX;
                else
                {
                    int col_num = ((minX - xmin) / dx);
                    minX = leftX + col_num * dx;
                }

                if(rightX - maxX < eps) maxX = rightX;
                else
                {
                    int col_num = ((xmax - maxX) / dx);
                    maxX = rightX - col_num * dx;
                }

                for(double x=minX; (x - maxX) < eps; x += dx)
                {


                    double z;
                    if(intersections.second.x != intersections.first.x)
                        z = intersections.first.z + (x - intersections.first.x) *
                            (intersections.second.z - intersections.first.z) /
                            (intersections.second.x - intersections.first.x);
                    else z = intersections.first.z;


                    int i = ((x - xmin) / dx);
                    int j = ((ymax - y) / dy);


                    if(z-zmin > eps && zBuffer[i][j] - z > eps)
                    {
                        zBuffer[i][j] = z;
                        image->set_pixel(i,j,triangle.colors[0], triangle.colors[1], triangle.colors[2]);
                    }

                }
            }


        }

        for(int j=0;j<screen_height;j++)
        {
            for(int i=0;i<screen_width;i++)
            {
                if(abs(zBuffer[i][j] - zmax) > eps)
                    zbuffer << zBuffer[i][j] << "\t";

            }
            zbuffer << endl;
        }

        image->save_image("out.bmp");

        delete image;




    }
    else
    {
        cout << "Can't open config.txt" << endl;
        return 0;
    }


    zbuffer.close();
    config.close();
    scene.close();

    for(int i=0;i<screen_width;i++)
    {
           delete zBuffer[i];
    }
    delete zBuffer;



}




