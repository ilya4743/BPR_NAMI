#ifndef GAMEMAP_H
#define GAMEMAP_H
#include<locationmap.h>
#include<mygraph.h>

#include<barrier.h>
#include<point.h>
using namespace std;

class GameMap;
class IBarrier
{
public:
    void virtual init(MyGraph& graph, float step,GameMap&g)=0;
    void virtual print(GameMap&g)=0;
};

class Barrier:public IBarrier, public Point
{
public:
    Barrier();
    Barrier(float x, float y, float w, float h);
    Barrier(const Barrier& o);
    void init(MyGraph& graph, float step, GameMap&g)override;
    void print(GameMap&g)override;
    float width;
    float height;
    Point center;
    Point left_top;
    Point right_bottom;
};

class GameMap
{
public:
    //шаг вершин графа
        float step;
        //ширина покрытия сетки графа
        float width_coord;
        //высота покрытия сетки графа
        float height_coord;
        //количество вершин графа по ширине
        int num_vertices_width;
        //количество вершин графа по высоте
        int num_vertices_height;
        //начало отсчета системы координат (наша машина)
        int center;
        //точка конечного маршрута
        Point goal_point;
        //вершина графа откуда стартует авто
        vertex_descriptor start;
        //вершина графа куда едем
    int goal;

    GameMap();
    GameMap(Point goal_p,vector<Barrier>& barriers);
    MyGraph* graph;
    DMQuadrangle* distance;
    vector<Barrier> barriers;
    vector<Point> short_path;
    vector<Point> create_msg(const vector<Point>& locations, const vector<vertex_descriptor>& shortest_path);
    void print_game_map();
    void print_vertex_map();
    void print_way(const DMQuadrangle* distance, const vector<vertex_descriptor>& shortest_path);
    int GetJ(float i)
    {
        return (center + int(i/step)) % num_vertices_width;
    }

    int GetI(float j)
    {
            return (center - int(j / step) * num_vertices_width) / num_vertices_width;
    }
    void init(IDistanceMatrix&distance,IGraph&graph)
    {
        distance.init();
        graph.init(num_vertices_width,num_vertices_height);
    }
    void doo()
    {
        distance->init();
        graph->init(num_vertices_width, num_vertices_height);
        for(int i=0; i<barriers.size();i++)
            barriers[i].init(*graph,step,*this);
        print_game_map();
        for(int i=0; i<barriers.size();i++)
        {
            barriers[i].print(*this);
        }
        print_way(distance,graph->search(start,goal,distance));
        short_path=create_msg(distance->matrix,graph->search(start,goal,distance));

    }
};

#endif // GAMEMAP_H
