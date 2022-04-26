#include "locationmap.h"
#include <iomanip>

using namespace std;

IDistanceMatrix::~IDistanceMatrix(){}

void DistanceMatrix::init(){}

void DistanceMatrix::print(){}

int DistanceMatrix::GetI(float j){}
int DistanceMatrix::GetJ(float i){}

DistanceMatrix::~DistanceMatrix()
{
    matrix.clear();
}

DMQuadrangle::DMQuadrangle():width(0),height(0),center(0),step(0)
{
}

DMQuadrangle::DMQuadrangle(int width, int height, int center, float step):width(width),height(height),center(center),step(step)
{
}

DMQuadrangle::DMQuadrangle(const DMQuadrangle &o):width(o.width),height(o.height), center(o.center), step(o.step)
{
}

void DMQuadrangle::init()
{
    matrix.resize(width*height);
    float x= step, y=0;
    for (int i = center / width; i >= 0; i--)
    {
        for (int j = center % width; j >= 0; j--)
            matrix[i * width + j] = { x-=step,-y };
        x = step;
        y-=step;
    }
    x = -step;
    y = 0;
    for (int i = center / width; i < height; i++)
    {
        for (int j = center % width; j < width; j++)
            matrix[i * width + j] = { x+=step,-y };
        x = -step;
        y+=step;
    }y = 0;
    for (int i = center/ width; i >= 0; i--)
    {
        for (int j = center % width; j < width; j++)
            matrix[i * width + j] = { x+=step,y };
        x = -step;
        y+=step;
    }
    x = step;
    y = 0;
    for (int i = center/ width; i <height; i++)
    {
        for (int j = center%width; j >=0; j--)
            matrix[i * width + j] = { x-=step,y };
        x = step;
        y-=step;
    }
}

void DMQuadrangle::print()
{
    cout << "locations:\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            cout <<'{'<< setw(5) << matrix[i*width+j].x<<','<<setw(5) << matrix[i * width + j].y<<'}'<<' ';
        cout << endl;
    }
}

DMQuadrangle::~DMQuadrangle()
{
    matrix.clear();
    //cout<<"Destructor DMQuadrangle";
};

int DMQuadrangle::GetI(float j)
{
    return (center - int(j / step) * width) / width;
}

int DMQuadrangle::GetJ(float i)
{
    return (center + int(i/step)) % width;
}
