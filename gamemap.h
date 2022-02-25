#ifndef GAMEMAP_H
#define GAMEMAP_H
#include"locationmap.h"
#include"mygraph.h"
#include"barrier.h"
#include"point.h"
#include"mylog.h"

using namespace std;

class Barrier;

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
    //ширина авто
    float width_auto;
    //высота авто
    float height_auto;

    GameMap();
    GameMap(float width_coord, float height_coord, float step, int center, float width_auto, float height_auto, Point goal_p);
    MyGraph* graph;
    DistanceMatrix* distance;
    list<Barrier*> barriers;
    list<Point> short_path;
    list<Point> create_msg(const DistanceMatrix& locations, const list<vertex_descriptor>& shortest_path);
    void print_game_map();
    void print_vertex_map();
    void print_way(const DistanceMatrix* distance, const list<vertex_descriptor>& shortest_path);
    int GetJ(float i);
    int GetI(float j);
    void init(IDistanceMatrix&distance,IGraph&graph);
    void doo(const int DEBUG_OUTPUT);
    void printToFile(ofstream& out);
    ~GameMap();
};

#endif // GAMEMAP_H
