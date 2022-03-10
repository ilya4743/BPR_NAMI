#include "gamemap.h"
#include <boost/graph/adjacency_list.hpp>
#include <QtDebug>

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
    start = this->center;

    this->width_auto=width_auto;
    this->height_auto=height_auto;
    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    distance->init();

    //если точка конечного маршрута вышла за пределы сетки графа по x
    if (!(distance->matrix[0].x<=goal_point.x && distance->matrix[num_vertices_width-1].x>=goal_point.x))
        if(goal_point.x>0)
            goal_point.x=distance->matrix[num_vertices_width-1].x;
        else
            goal_point.x=distance->matrix[0].x;

    //если точка конечного маршрута вышла за пределы сетки графа по y
    if (!(distance->matrix[0].y>=goal_point.y && distance->matrix[distance->matrix.size()-1].y<=goal_point.y))
        if(goal_point.y>0)
            goal_point.y=distance->matrix[0].y;
        else
            goal_point.y=distance->matrix[distance->matrix.size()-1].y;

    //вершина графа куда едем
    goal = GetI(goal_point.y)*num_vertices_width + GetJ(goal_point.x);

    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

void GameMap::print_way(const DistanceMatrix* distance, const list<vertex_descriptor>& shortest_path)
{
    cout<<"\033[s";
    for (auto it=shortest_path.begin(); it != shortest_path.end(); ++it)
    {
        int k=GetJ(distance->matrix[(*it)].x)+1;
        int d=GetI(distance->matrix[(*it)].y)+1;
        cout<<"\033["<<d<<';'<<k<<"H\033[0;31;40m*\033[0;0m";
    }
    cout<<"\033[u\n";
}


list<Point> GameMap::create_msg(const DistanceMatrix& distance, list<vertex_descriptor>& shortest_path)
{
    int o=1;
    if(o==1)
    {

    list<Point> msg;

    for(auto it = shortest_path.begin();it!=shortest_path.end();++it)
        msg.push_back(distance.matrix[*it]);

    if(barriers.size()==0)
    {
        msg.clear();
        msg.push_back(Point(0, 0));
        auto u=shortest_path.end();--u;
        msg.push_back(distance.matrix[(*u)]);
        list<vertex_descriptor> s;
        s.push_back(*shortest_path.begin());
        s.push_back(*u);
        shortest_path.clear();
        shortest_path=s;
    }
    else
    if(msg.size()>2)
    {
        auto it = msg.begin();
        auto itend= msg.end();--itend;
        auto shortIT=shortest_path.begin();
        float x1, y1, x2, y2;
        x1=(*it).x;
        y1=(*it).y;
        ++shortIT;
        ++it;
        x2=(*it).x;
        y2=(*it).y;
        int count=0;

        Point tmp=*it;
        unsigned long tmpshort=*shortIT;

        while(it!=itend)
        {
            for(auto it1=barriers.begin();it1!=barriers.end();++it1)
            {
                if(!(*it1)->isIntersection(Point(x1,y1),Point(x2,y2)))
                {
                    ++count;
                    if(count==barriers.size())
                    {
                        tmp=*it;
                        tmpshort=*shortIT;
                        it=msg.erase(it);
                        shortIT=shortest_path.erase(shortIT);
                        x2=(*it).x;
                        y2=(*it).y;
                    }
                }
                else
                {
                    if(tmp.x!=(*it).x && tmp.y!=(*it).y)
                    {
                        *it=tmp;
                        *shortIT=tmpshort;
                    }
                    x1=(*it).x;
                    y1=(*it).y;
                    ++it;
                    ++shortIT;
                    x2=(*it).x;
                    y2=(*it).y;
                    break;
                }
            }
            count=0;

        }
    }
    else
    {

    }
    return msg;
    }
    else
    {
    list<Point> msg1;

    msg1.push_back(Point(distance.matrix[(*shortest_path.begin())].x,distance.matrix[(*shortest_path.begin())].y));


    auto it = shortest_path.begin();
    ++it;
    for (;it != shortest_path.end(); ++it)
    {
        Point p;
        p.x=distance.matrix[*it].x- distance.matrix[*it-1].x;
        p.y=distance.matrix[*it].y-distance.matrix[*it-1].y;
        msg1.push_back((p));
    }
    return msg1;
    }
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
    //distance->init();
    graph->init(num_vertices_width, num_vertices_height);
    list<Barrier*>::iterator itGP;
    bool partial_path=false;
    if(DEBUG_OUTPUT==1) //если дебаг включен
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
        list<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);

        if(partial_path)                    //если маршрут неполный (точка конечного маршрута попала на препятствие)
        {
            auto it= short1.begin();
            auto itEnd=short1.end();
            //while(it!=itEnd)
            while((*itGP)->hasVertex(*it,*this))
            {

                short1.erase(it);
                ++it;
            }
        }
        list<vertex_descriptor> smoothing_path=short1;
        short_path=create_msg(*distance,short1);

        print_game_map();


        for(auto it= barriers.begin(); it!=barriers.end();++it)
            (*it)->print(*this);
        print_way(distance,short1);
    }
    else    //если дебаг выключен
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
        list<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        if(partial_path)                    //если маршрут неполный (точка конечного маршрута попала на препятствие)
        {
            auto itShort1=short1.end();
            --itShort1;--itShort1;
            while((*itGP)->hasVertex(*itShort1, *this))
            {
                short1.erase(itShort1);
                --itShort1;
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

#include<QTime>

void GameMap::printToFile(ofstream& out)
{
    //QString str=QTime::currentTime().toString("HH:mm:ss");
    //out<<str.toStdString();
    out<<width_coord<<endl<<height_coord<<endl<<step<<endl<<start<<endl;
    out<<width_auto<<endl<<height_auto<<endl;
    out<<goal_point<<barriers.size();
    for(auto it= barriers.begin(); it!=barriers.end();++it)
        out<<*(*it);
    out<<endl<<"Data Path\n";
    for(auto it=short_path.begin();it!=short_path.end();++it)
        out<<*it;
}
