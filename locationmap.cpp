#include "locationmap.h"
#include <iomanip>

using namespace std;

IDistanceMatrix::~IDistanceMatrix(){}

void DistanceMatrix::init(){}

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
       float x= step, z=0;
       for (int i = center / width; i >= 0; i--)
       {
           for (int j = center % width; j >= 0; j--)
               matrix[i * width + j] = { x-=step,0,-z };
           x = step;
           z-=step;
       }
       x = -step;
       z = 0;
       for (int i = center / width; i < height; i++)
       {
           for (int j = center % width; j < width; j++)
               matrix[i * width + j] = { x+=step,0,-z};
           x = -step;
           z+=step;
       }z = 0;
       for (int i = center/ width; i >= 0; i--)
       {
           for (int j = center % width; j < width; j++)
               matrix[i * width + j] = { x+=step,0,z };
           x = -step;
           z+=step;
       }
       x = step;
       z = 0;
       for (int i = center/ width; i <height; i++)
       {
           for (int j = center%width; j >=0; j--)
               matrix[i * width + j] = { x-=step,0,z};
           x = step;
           z-=step;
       }
}

DMQuadrangle::~DMQuadrangle()
{
    matrix.clear();
    //cout<<"Destructor DMQuadrangle";
};

template<class DistanceMatrix>
void DistanceMatrixPrinter::print(const DistanceMatrix& distance)
{
    cout << "locations:\n";
    for (int i = 0; i < distance.height; i++)
    {
        for (int j = 0; j < distance.width; j++)
            cout <<'{'<< setw(5) << distance.matrix[i*distance.width+j].x<<','<<setw(5) << distance.matrix[i * distance.width + j].y<<'}'<<' ';
        cout << endl;
    }
}
