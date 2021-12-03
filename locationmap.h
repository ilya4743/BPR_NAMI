#ifndef LOCATIONMAP_H
#define LOCATIONMAP_H
#include<iostream>
#include"point.h"
#include<vector>

using namespace std;

class IDistanceMatrix
{
public:
    void virtual init()=0;
    void virtual print()=0;
};

class DMQuadrangle: public IDistanceMatrix
{
    int width;
    int height;
    int center;
    float step;
public:
    vector<Point> matrix;
    DMQuadrangle();
    DMQuadrangle(int width, int height, int center, float step);
    DMQuadrangle(const DMQuadrangle&o);
    void init() override;
    void print()override;
};

#endif // LOCATIONMAP_H
