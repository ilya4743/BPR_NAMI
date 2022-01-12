#ifndef BARRIER_H
#define BARRIER_H
#include"point.h"
#include"mygraph.h"
#include "gamemap.h"

class GameMap;
class IBarrier
{
public:
    int virtual init(MyGraph& graph, float step,GameMap&g)=0;
    void virtual print(GameMap&g)=0;
    virtual ~IBarrier()=0;
    virtual bool hasPoint(Point p, GameMap&g)=0;
    virtual bool hasVertex(int v, GameMap&g)=0;
};

class Barrier:public IBarrier, public Point
{
public:
    Barrier();
    Barrier(float x, float y);
    Barrier(const Barrier &o);
    int init(MyGraph& graph, float step, GameMap&g)override;
    void print(GameMap&g)override;
    ~Barrier();
    bool hasPoint(Point p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
};

class BQuadrAngle: public Barrier
{
public:
    BQuadrAngle();
    BQuadrAngle(float x, float y, float w, float h);
    BQuadrAngle(const BQuadrAngle& o);
    int init(MyGraph& graph, float step, GameMap&g)override;
    void print(GameMap&g)override;
    bool hasPoint(Point p, GameMap&g)override;
    bool hasVertex(int v, GameMap&g)override;
    ~BQuadrAngle();
    float width;
    float height;
    Point center;
    Point left_top;
    Point right_bottom;
};

#endif // BARRIER_H
