#include "gamemap.h"
#include <boost/graph/adjacency_list.hpp>
#include <QtDebug>
#include<QTime>

GameMap::GameMap()
{
}

GameMap::GameMap(float width_coord, float height_coord, float step, int center,
                 const Car &car,
                 const Ogre::Vector3 &goal_p):
    width_coord(width_coord),height_coord(height_coord),step(step),
    num_vertices_width(width_coord/step), num_vertices_height(height_coord/step),
    goal_point(goal_p),start(center),goal(),car(car)
{
}

GameMap::~GameMap()
{
    //for(auto it= barriers.begin(); it!=barriers.end(); ++it)
        //delete *it;
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
