#ifndef BARRIER_H
#define BARRIER_H
#include"point.h"
#include"mygraph.h"
#include "gamemap.h"

template<class BarrierT>

class GameMap;
class IBarrier
{
public:
    void virtual init(MyGraph& graph, float step,GameMap&g)=0;
};

class Barrier:public IBarrier, public Point
{
public:
    Barrier();
    Barrier(float x, float y, float w, float h);
    Barrier(const Barrier& o);
    void init(MyGraph& graph, float step, GameMap&g)override;
    void print(GameMap&g);
    float width;
    float height;
    Point center;
    Point left_top;
    Point right_bottom;
};

#endif // BARRIER_H
