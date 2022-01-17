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
    this->width_auto=0;
    this->height_auto=0;
    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

GameMap::GameMap(float width_coord, float height_coord, float step, int center, float width_auto, float height_auto, Point goal_p)
{
    this->step = step;
    //ширина покрытия сетки графа
    this->width_coord=width_coord;
    //высота покрытия сетки графа
    this->height_coord=height_coord;
    //количество вершин графа по ширине
    num_vertices_width = width_coord/step;
    //количество вершин графа по высоте
    num_vertices_height = height_coord/step;
    //начало отсчета системы координат (наша машина)
    this->center = center;
    this->goal_point=goal_p;
    //this->barriers=barriers;
    start = this->center;
    //вершина графа куда едем
    goal = GetI(goal_point.y)*num_vertices_width + GetJ(goal_point.x);
    this->width_auto=width_auto;
    this->height_auto=height_auto;
    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

void GameMap::print_way(const DistanceMatrix* distance, const vector<vertex_descriptor>& shortest_path)
{
    cout<<"\033[s";
    for (unsigned int i = 0; i < shortest_path.size(); i++)
    {
        int k=GetJ(distance->matrix[shortest_path[i]].x)+1;
        int d=GetI(distance->matrix[shortest_path[i]].y)+1;
        cout<<"\033["<<d<<';'<<k<<"H\033[0;31;40m*\033[0;0m";
    }
    cout<<"\033[u\n";

    /*cout << "\n\nCoord path:\n";
    cout<<distance->matrix[shortest_path[0]].x<<'\t'<<distance->matrix[shortest_path[0]].y<<endl;
    for (unsigned int i = 1; i < shortest_path.size()-1; i++)
    {
        cout << distance->matrix[shortest_path[i]].x- distance->matrix[shortest_path[i-1]].x<<'\t';
        cout<< distance->matrix[shortest_path[i]].y-distance->matrix[shortest_path[i-1]].y<<endl;
    }*/
}

vector<Point> GameMap::create_msg(const DistanceMatrix& distance, const vector<vertex_descriptor>& shortest_path)
{
    vector<Point> msg;
    msg.reserve(shortest_path.size());
    msg.push_back(Point(distance.matrix[shortest_path[0]].x,distance.matrix[shortest_path[0]].y));
    for (unsigned int i = 1; i < shortest_path.size()-1; i++)
    {
        Point p;
        p.x=distance.matrix[shortest_path[i]].x- distance.matrix[shortest_path[i-1]].x;
        p.y=distance.matrix[shortest_path[i]].y-distance.matrix[shortest_path[i-1]].y;
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

void GameMap::doo(const int DEBUG_OUTPUT)
{
    distance->init();
    graph->init(num_vertices_width, num_vertices_height);
    list<Barrier*>::iterator itGP;
    bool partial_path=false;
    if(DEBUG_OUTPUT==1) //если дебаг выключен
    {
        for(auto it= barriers.begin(); it!=barriers.end();)
        {
            if(!(*it)->hasPoint(goal_point, *this))    //если точка конечного маршрута не попала на препятствие
            {
                if((*it)->init(*graph,step,*this)==-2)
                    it=barriers.erase(it);
                else
                    ++it;
            }
            else                                //если точка конечного маршрута попала на препятствие
            {
                itGP=it;
                ++it;
                partial_path=true;
            }
}
        vector<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        if(partial_path)                    //если маршрут неполный (точка конечного маршрута попала на препятствие)
        {
            int i=short1.size()-2;
            while((*itGP)->hasVertex(short1[i], *this))
            {
                short1.erase(short1.begin()+i);
                i--;
            }
        }

        short_path=create_msg(*distance,short1);

        print_game_map();


        for(auto it= barriers.begin(); it!=barriers.end();++it)
            (*it)->print(*this);
        print_way(distance,short1);
    }
    else    //если дебаг включен
    {
        for(auto it= barriers.begin(); it!=barriers.end();++it)
            if(!(*it)->hasPoint(goal_point, *this))    //если точка конечного маршрута не попала на препятствие
            {
                (*it)->init(*graph,step,*this);
            }
            else                                //если точка конечного маршрута попала на препятствие
            {
                itGP=it;
                ++it;
                partial_path=true;
            }
        vector<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        if(partial_path)                    //если маршрут неполный (точка конечного маршрута попала на препятствие)
        {
            int i=short1.size()-2;
            while((*itGP)->hasVertex(short1[i], *this))
            {
                short1.erase(short1.begin()+i);
                i--;
            }
        }
        short_path=create_msg(*distance,short1);
    }
}

GameMap::~GameMap()
{
    delete this->distance;
    delete this->graph;
    for(auto it= barriers.begin(); it!=barriers.end(); ++it)
        delete(*it);
    this->barriers.clear();
}

void GameMap::printToFile(ofstream& out)
{
    out<<width_coord<<endl<<height_coord<<endl<<step<<endl<<start<<endl;
    out<<width_auto<<endl<<height_auto<<endl<<barriers.size();
    for(auto it= barriers.begin(); it!=barriers.end();++it)
        out<<*(*it);
    out<<endl;
}
