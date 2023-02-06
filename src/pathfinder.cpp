#include "pathfinder.h"

void PathFinder::UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
    float theta, float speed, size_t n, std::map<uint64_t,Ogre::Matrix4>&& objects)
{
    grid.resize(width, height, resolution);
    goal.x=x;
    goal.y=y;
    goal.z=theta;
    auto itCar=objects.find(0);
    objects.erase(itCar);
    Ogre::Matrix4 mat4=(*itCar).second;
    //auto qua=(*itCar).second.extractQuaternion();
    Ogre::Vector3 pos1(-width/2, mat4.getTrans().y, -height/2);
    pos1=mat4*pos1;
    Ogre::Matrix4 mat41(mat4);
    mat41.setTrans(pos1);
    //mat4=mat41.inverse()*mat4;
      
    car=std::make_unique<Car>(mat4, speed);
    obstacles.reserve(n);
    for(auto it=objects.begin(); it!=objects.end(); ++it)
    {
        auto obstacle=std::make_unique<BQuadrAngle>(mat41.inverse()*(*it).second);
        placer.placeObstacleOnGrid(grid, *(obstacle.get()));
        obstacles.push_back(std::move(obstacle));
    }

    // for(int i=grid.height-1; i>=0; i--)
    // {
    //     for (int j=grid.width-1; j>=0; j--)
    //     {
    //         if(grid.data[i*grid.width+j]==100)
    //             std::cout<<1;
    //          else cout<<0;
    //     }
    //     std::cout<<endl;
    // }      
}

std::vector<Ogre::Vector3> PathFinder::Find()
{
    return hybrid_astar.searchHybridAStar(0,0, (*car).rotation,0, 398, 1.57, grid);
}

void PathFinder::Clear() noexcept
{
    obstacles.clear();
    car.reset();
}
