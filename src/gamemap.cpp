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
    graph=new MyGraph(num_vertices_width*num_vertices_height);
}

#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

vector<Ogre::Vector3> GameMap::create_msg(const DistanceMatrix& distance, vector<vertex_descriptor>& shortest_path)
{
    boost::geometry::model::linestring<point2d> m;
    boost::geometry::model::multi_point<point2d> m1;
    vector<Ogre::Vector3>msg;
    m.reserve(shortest_path.size()-1);
    msg.reserve(shortest_path.size()-1);
    for (auto it=shortest_path.begin();it != shortest_path.end()-1; ++it)
        m.push_back(point2d(distance.matrix[*it].x,distance.matrix[*it].z));
    boost::geometry::line_interpolate(m,step,m1);

    vector<float> y,x;
    y.reserve(shortest_path.size()-1);
    x.reserve(shortest_path.size()-1);
    for(int i=0;i<m1.size();++i)
    {
        x.push_back(m1[i].x());
        y.push_back(m1[i].y());
    }

    //boost::math::interpolators::cardinal_cubic_b_spline<float> spline(x.data(), x.size(), 0 /* start time */, 10);
    //for(float i=0; i<y[y.size()-1];i+=1)
    //    msg.push_back(Ogre::Vector3(spline(i),0,i));

    //for (auto it=m1.begin();it != m1.end(); ++it)
    //    msg.push_back(Ogre::Vector3(it->x(),0,it->y()));
    for (auto it=m.begin();it != m.end(); ++it)
        msg.push_back(Ogre::Vector3(it->x(),0,it->y()));
    return msg;
}

void GameMap::init()
{
    distance->init();
    graph->init(num_vertices_width,num_vertices_height);
    point2d _goal(goal_point.x, goal_point.z);
    polygon box{{{distance->getLDP()},{distance->getLUP()},{distance->getRUP()},{distance->getRDP()},{distance->getLDP()}}};
    if (!bg::covered_by(_goal, box))
    {
        bg::model::linestring<point2d> seg{{0,0},{_goal.x(),_goal.y()}};
        vector<point2d>  output ;
        boost::geometry::intersection(seg, box, output);
        goal_point.x=output[0].x();
        goal_point.z=output[0].y();
    }
    goal = distance->GetI(goal_point.z)*num_vertices_width + distance->GetJ(goal_point.x);
}

void GameMap::doo(const int DEBUG_OUTPUT)
{
    init();     

    if(DEBUG_OUTPUT==1) //если дебаг включен
    {
        GameMapPrinter::print_game_map(*distance);
        PrinterBQuadrAngle printerBQuadrAngle;

        for(auto it= barriers.begin(); it!=barriers.end();++it)
        {
            vector<Ogre::Vector3> clipping;
            (*it)->init(*distance,clipping);
            printerBQuadrAngle.drawCube(clipping,*distance,*graph);
        }

        vector<vertex_descriptor> short1=graph->search(start,goal,distance->matrix);
        short_path=create_msg(*distance,short1);

        GameMapPrinter::print_way(*distance, short1);
        for(auto it=short_path.begin();it!=short_path.end();++it)        
            *it=car.rotation*(*it);
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

void GameMapPrinter::print_game_map(const DMQuadrangle& dm)
{
    for (int i = 0; i < dm.height; i++)
    {
        for (int j = 0; j < dm.width; j++)
            cout << '*';
        cout << endl;
    }
    cout << endl;
}

void GameMapPrinter::print_way(const DMQuadrangle& dm,const vector<vertex_descriptor>& shortest_path)
{
    cout<<"\033[s";
    for (auto it=shortest_path.begin(); it != shortest_path.end(); ++it)
    {
        int k=dm.GetJ(dm.matrix[(*it)].x)+1;
        int d=dm.GetI(dm.matrix[(*it)].z)+1;
        cout<<"\033["<<d<<';'<<k<<"H\033[0;31;40m*\033[0;0m";
    }
    cout<<"\033[u\n";
}
