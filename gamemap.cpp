#include "gamemap.h"
#include <boost/graph/adjacency_list.hpp>

GameMap::GameMap()
{
    step = 0.1;
    //ширина покрытия сетки графа
    width_coord=16.1;
    //высота покрытия сетки графа
    height_coord=4.5;
    //количество вершин графа по ширине
    num_vertices_width = width_coord/step;
    //количество вершин графа по высоте
    num_vertices_height = height_coord/step;
    //начало отсчета системы координат (наша машина)
    center = (num_vertices_width / 2) + (num_vertices_height/2);
    //точка конечного маршрута
    goal_point.x=-0.4;
    goal_point.y=2;

    //вершина графа откуда стартует авто
    start = center;
    //вершина графа куда едем
    goal = GetI(goal_point.y)*num_vertices_width + GetJ(goal_point.x);

    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

GameMap::GameMap(Point goal_p,vector<Barrier>& barriers)
{
    step = 0.1;
    //ширина покрытия сетки графа
    width_coord=16.1;
    //высота покрытия сетки графа
    height_coord=4.5;
    //количество вершин графа по ширине
    num_vertices_width = width_coord/step;
    //количество вершин графа по высоте
    num_vertices_height = height_coord/step;
    //начало отсчета системы координат (наша машина)
    center = (num_vertices_width / 2) + (num_vertices_height/2)*num_vertices_width;
    this->goal_point=goal_p;
    this->barriers=barriers;
    start = center;
    //вершина графа куда едем
    goal = GetI(goal_point.y)*num_vertices_width + GetJ(goal_point.x);

    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

void GameMap::print_way(const DMQuadrangle* distance, const vector<vertex_descriptor>& shortest_path)
{
    cout<<"\033[s";
    for (int i = 0; i < shortest_path.size(); i++)
    {
        int k=GetJ(distance->matrix[shortest_path[i]].x)+1;
        int d=GetI(distance->matrix[shortest_path[i]].y)+1;
        cout<<"\033["<<d<<';'<<k<<"H\033[0;31;40m*\033[0;0m";
    }
    cout<<"\033[u";
    cout << "\n\nCoord path:\n";
    cout<<distance->matrix[shortest_path[0]].x<<'\t'<<distance->matrix[shortest_path[0]].y<<endl;
    for (int i = 1; i < shortest_path.size()-1; i++)
    {
        cout << distance->matrix[shortest_path[i]].x- distance->matrix[shortest_path[i-1]].x<<'\t';
        cout<< distance->matrix[shortest_path[i]].y-distance->matrix[shortest_path[i-1]].y<<endl;
    }/**/
}

vector<Point> GameMap::create_msg(const vector<Point>& locations, const vector<vertex_descriptor>& shortest_path)
{
    vector<Point> msg;
    msg.reserve(shortest_path.size());
    msg.push_back(Point(locations[shortest_path[0]].x,locations[shortest_path[0]].y));
    for (int i = 1; i < shortest_path.size()-1; i++)
    {
        Point p;
        p.x=locations[shortest_path[i]].x- locations[shortest_path[i-1]].x;
        p.y=locations[shortest_path[i]].y-locations[shortest_path[i-1]].y;
        msg.push_back((p));
    }
    return msg;
}

void GameMap::print_game_map()
{
    for (int i = 0; i < num_vertices_height; i++)
    {
        for (int j = 0; j < num_vertices_width; j++)
            cout << '*';
        cout << endl;
    }
    cout << endl;
}

int GameMap::GetJ(float i)
{
    return (center + int(i/step)) % num_vertices_width;
}

int GameMap::GetI(float j)
{
        return (center - int(j / step) * num_vertices_width) / num_vertices_width;
}

void GameMap::init(IDistanceMatrix&distance,IGraph&graph)
{
    distance.init();
    graph.init(num_vertices_width,num_vertices_height);
}

void GameMap::doo()
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
