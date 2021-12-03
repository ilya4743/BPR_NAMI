#ifndef GAMEMAP_H
#define GAMEMAP_H
#include<locationmap.h>
#include<mygraph.h>
#include<barrier.h>
#include<point.h>
using namespace std;

class Barrier;

template<class bar>
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
    vector<bar> barriers;
    vector<Point> short_path;
    vector<Point> create_msg(const vector<Point>& locations, const vector<vertex_descriptor>& shortest_path);
    void print_game_map();
    void print_vertex_map();
    void print_way(const DMQuadrangle* distance, const vector<vertex_descriptor>& shortest_path);
    int GetJ(float i);
    int GetI(float j);
    void init(IDistanceMatrix&distance,IGraph&graph);
    void doo();
};

#endif // GAMEMAP_H
