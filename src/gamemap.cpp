#include "gamemap.h"
#include <boost/graph/adjacency_list.hpp>
#include <QtDebug>
#include<QTime>

GameMap::GameMap()
{
}

GameMap::GameMap(float width_coord, float height_coord, float step, int center,
                 const Car &car,
                 const Ogre::Vector3 &goal_p,
                 bool isSmoothing):
    width_coord(width_coord),height_coord(height_coord),step(step),
    num_vertices_width(width_coord/step), num_vertices_height(height_coord/step),
    goal_point(goal_p),start(center),goal(),car(car),
    isSmoothing(isSmoothing)
{
    distance=new DMQuadrangle(num_vertices_width,num_vertices_height,center,step);
    adapter=new DistanceMatrixAdapter(dynamic_cast<DMQuadrangle*>(distance));
    //вершина графа куда едем
    goal = adapter->GetI(goal_p.z)*num_vertices_width + adapter->GetJ(goal_p.x);
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

void GameMapPrinter::print_way(const GameMap& map,const list<vertex_descriptor>& shortest_path)
{
    cout<<"\033[s";
    for (auto it=shortest_path.begin(); it != shortest_path.end(); ++it)
    {
        int k=map.adapter->GetJ(map.distance->matrix[(*it)].x)+1;
        int d=map.adapter->GetI(map.distance->matrix[(*it)].z)+1;
        cout<<"\033["<<d<<';'<<k<<"H\033[0;31;40m*\033[0;0m";
    }
    cout<<"\033[u\n";
}

list<Ogre::Vector3> GameMap::create_msg(const DistanceMatrix& distance, list<vertex_descriptor>& shortest_path)
{
    list<Ogre::Vector3> msg1;
    for (auto it=shortest_path.begin();it != shortest_path.end(); ++it)
        msg1.push_back(Ogre::Vector3(distance.matrix[*it].x,0,distance.matrix[*it].z));
    msg1.pop_back();
    return msg1;
}

void GameMapPrinter::print_game_map(const GameMap& map)
{
    for (int i = 0; i < map.num_vertices_height; i++)
    {
        for (int j = 0; j < map.num_vertices_width; j++)
            cout << '*';
        cout << endl;
    }
    cout << endl;
}

void GameMap::init()
{
    distance->init();
    graph->init(num_vertices_width,num_vertices_height);
}


void GameMap::doo(const int DEBUG_OUTPUT)
{
    init();
    //goal_point=car.rotation*goal_point;
    //если точка конечного маршрута вышла за пределы сетки графа по x
    if (!(distance->matrix[0].x<=goal_point.x && distance->matrix[num_vertices_width-1].x>=goal_point.x))
        if(goal_point.x>0)
            goal_point.x=distance->matrix[num_vertices_width-1].x;
        else
            goal_point.x=distance->matrix[0].x;

    //если точка конечного маршрута вышла за пределы сетки графа по y
    if (!(distance->matrix[0].z>=goal_point.z && distance->matrix[distance->matrix.size()-1].z<=goal_point.z))
        if(goal_point.z>0)
            goal_point.z=distance->matrix[0].z;
        else
            goal_point.z=distance->matrix[distance->matrix.size()-1].z;

    if(DEBUG_OUTPUT==1) //если дебаг включен
    {
        GameMapPrinter::print_game_map(*this);
        PrinterBQuadrAngle printerBQuadrAngle;

        for(auto it= barriers.begin(); it!=barriers.end();++it)
        {
            vector<Ogre::Vector3> clipping;
            (*it)->init(*(dynamic_cast<DMQuadrangle*>(distance)),clipping);
            printerBQuadrAngle.drawCube(clipping,*dynamic_cast<BQuadrAngle*>(*it),*adapter, *dynamic_cast<DMQuadrangle*>(distance),*graph);
        }

        list<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        short_path=create_msg(*distance,short1);

        GameMapPrinter::print_way(*this, short1);
        for(auto it=short_path.begin();it!=short_path.end();++it)
            (*it)=car.rotation*(*it);
    }
    else    //если дебаг выключен
    {

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

void GameMapPrinter::printToFile(const GameMap& map, ofstream& out)
{
    out<<map.width_coord<<endl<<map.height_coord<<endl<<map.step<<endl<<map.start<<endl;
    out<<map.car<<endl;
    out<<map.goal_point<<map.barriers.size();
    for(auto it= map.barriers.begin(); it!=map.barriers.end();++it)
        out<<*(*it);
    out<<endl<<"Data Path\n";
    for(auto it=map.short_path.begin();it!=map.short_path.end();++it)
        out<<*it;
}

ostream& operator <<(ostream &out, const GameMap &map)
{
    out<<map.width_coord<<endl<<map.height_coord<<endl<<map.step<<endl;
    out<<map.goal_point<<endl<<map.start<<endl<<map.goal<<endl;
    out<<map.car<<map.barriers.size()<<endl;
    for(auto it=map.barriers.begin(); it!=map.barriers.end(); ++it)
        out<<*it;
    return out;
}

istream& operator >>(istream &in, GameMap &map)
{
    int barrier_size;
    in>>map.width_coord>>map.height_coord>>map.step;
    in>>map.goal_point.x>>map.goal_point.y>>map.goal_point.z;
    in>>map.start>>map.goal;
    in>>map.car>>barrier_size;
    for(int i=0; i<barrier_size; i++)
    {
        BQuadrAngle barrier;
        in>>barrier;
        map.barriers.push_back(&barrier);
     }
    return in;
}

QDataStream& operator <<(QDataStream &out, const GameMap &map)
{

    return out;
}

QDataStream& operator >>(QDataStream &in, GameMap &map)
{
    return in;
}
