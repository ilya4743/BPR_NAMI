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
    /*if(isSmoothing)
    {

    list<Ogre::Vector3> msg;

    for(auto it = shortest_path.begin();it!=shortest_path.end();++it)
        msg.push_back(Ogre::Vector3(distance.matrix[*it].x, 0 , distance.matrix[*it].z));
    msg.push_front(*(--msg.end()));
    msg.pop_back();
    if(barriers.size()==0)
    {
        msg.clear();
        msg.push_back(Ogre::Vector3(0, 0,0));
        auto u=shortest_path.end();--u;--u;
        msg.push_back(Ogre::Vector3(distance.matrix[(*u)].x, 0, distance.matrix[(*u)].z));
        list<vertex_descriptor> s;
        s.push_back(*(++u));
        s.push_back(*(--u));
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

        Ogre::Vector3 tmp=*it;
        unsigned long tmpshort=*shortIT;

        while(it!=itend)
        {
            for(auto it1=barriers.begin();it1!=barriers.end();++it1)
            {
                if(!(*it1)->isIntersection(Ogre::Vector3(x1, y1, 0),Ogre::Vector3(x2,y2,0)))
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
        return msg;
    }
    else*/
    {
    list<Ogre::Vector3> msg1;
    for (auto it=shortest_path.begin();it != shortest_path.end(); ++it)
        msg1.push_back(Ogre::Vector3(distance.matrix[*it].x,0,distance.matrix[*it].z));
    msg1.pop_back();
    return msg1;
    }
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

    list<Barrier*>::iterator itGP;
    bool partial_path=false;
    if(DEBUG_OUTPUT==1) //если дебаг включен
    {
        for(auto it= barriers.begin(); it!=barriers.end();)
        {

            //if(!(*it)->hasPoint(goal_point, *this))    //если точка конечного маршрута не попала на препятствие
            //{
                if((*it)->init(*(dynamic_cast<DMQuadrangle*>(distance)))==-2)
                    it=barriers.erase(it);
                else
                    ++it;
            //}
            //else                                //если точка конечного маршрута попала на препятствие
            //{
            //    itGP=it;
            //    ++it;
            //    partial_path=true;
            //}
        }
/*
        if(partial_path)                    //если маршрут неполный (точка конечного маршрута попала на препятствие)
        {
            auto it= short1.begin();
            auto itEnd=short1.end();
            while((*itGP)->hasVertex(*it,*this))
            {
                short1.erase(it);
                ++it;
            }
        }
        list<vertex_descriptor> smoothing_path=short1;*/

        GameMapPrinter::print_game_map(*this);

        PrinterBQuadrAngle printerBQuadrAngle;
        for(auto it= barriers.begin(); it!=barriers.end();++it)
            printerBQuadrAngle.drawCube(*dynamic_cast<BQuadrAngle*>(*it),*adapter, *dynamic_cast<DMQuadrangle*>(distance),*graph);
        list<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        short_path=create_msg(*distance,short1);

        GameMapPrinter::print_way(*this, short1);
        for(auto it=short_path.begin();it!=short_path.end();++it)
        {
            (*it)=car.matrix4*(*it);
        }
    }
    else    //если дебаг выключен
    {
        for(auto it= barriers.begin(); it!=barriers.end();++it)
            if(!(*it)->hasPoint(goal_point, *this))    //если точка конечного маршрута не попала на препятствие
            {
                (*it)->init(*(dynamic_cast<DMQuadrangle*>(distance)));
            }
            else                                //если точка конечного маршрута попала на препятствие
            {
                itGP=it;
                //++it;
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

        //Ogre::Vector3 b(5,5,5);
        //Ogre::Matrix3 m;
        //car.rotation.ToRotationMatrix(m);


        //b=b*car.rotation;

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
