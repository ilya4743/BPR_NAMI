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
    virtual ~IDistanceMatrix()=0;
};

class DistanceMatrix:public IDistanceMatrix
{
public:
    vector<Point> matrix;
    void init()override;
    void print()override;
     ~DistanceMatrix()override;
};

class DMQuadrangle: public DistanceMatrix
{
    int width;
    int height;
    int center;
    float step;
public:
    DMQuadrangle();
    DMQuadrangle(int width, int height, int center, float step);
    DMQuadrangle(const DMQuadrangle&o);
    void init() override;
    void print()override;
    ~DMQuadrangle()override;
};

#endif // LOCATIONMAP_H
