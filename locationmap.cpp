#include "locationmap.h"
#include <iomanip>

using namespace std;

DMQuadrangle::DMQuadrangle()
{
    this->width=0;
    this->height=0;
    this->center=0;
    this->step=0;
}

DMQuadrangle::DMQuadrangle(int width, int height, int center, float step)
{
    this->width=width;
    this->height=height;
    this->center=center;
    this->step=step;
}

DMQuadrangle::DMQuadrangle(const DMQuadrangle&o)
{
    this->width=o.width;
    this->height=o.height;
    this->center=o.center;
    this->step=o.step;
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
