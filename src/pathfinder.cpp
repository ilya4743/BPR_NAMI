#include "pathfinder.h"
#include "constants.h"

void PathFinder::UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
    float theta, float speed, size_t n, std::map<uint64_t,Eigen::Matrix4f>&& objects)
{
    grid.resize(width, height, resolution);
    auto itCar=objects.find(0);
    objects.erase(itCar);
    Eigen::Matrix4f mat4=(*itCar).second;
    Eigen::Quaternionf qua(mat4.block<3, 3>(0, 0));
    Eigen::Vector3f pos = mat4.block<3, 1>(0, 3);
    Eigen::Vector4f pos1{-width/2, pos(1), -height/2, 1};
    pos1=mat4*pos1;
    Eigen::Matrix4f mat41(mat4);
    
    mat41.block<3, 1>(0, 3) = pos1.head<3>() / pos1.w();
    mat4=mat41.inverse()*mat4;

    auto pos_local=mat4.block<3, 1>(0, 3);
    rotation=qua;
    start=pos_local;

    car=std::make_unique<Car>(mat4, speed);
    goal(0)=x+pos_local(0);
    goal(1)=theta+pos_local(2);
    goal(2)=theta;
    obstacles.reserve(n);
    placer.clearGrid(grid);
    for(auto it=objects.begin(); it!=objects.end(); ++it)
    {
        auto obstacle=std::make_unique<BQuadrAngle>(mat41.inverse()*(*it).second);
        placer.placeObstacleOnGrid(grid, *(obstacle.get()));
        obstacles.push_back(std::move(obstacle));
    }
    //std::cout<<qua.a[0]<<'\t'<<qua.a[1]<<'\t'<<qua.a[2]<<'\t'<<qua.a[3]<<'\n';
    // system("clear");
    // for(int i=grid.height-1; i>=0; i--)
    // {
    //     for (int j=grid.width-1; j>=0; j--)
    //     {
    //         if(grid.data[i*grid.width+j]==100)
    //             std::cout<<1;
    //          else std::cout<<0;
    //     }
    //     std::cout<<std::endl;
    // }      
}

std::vector<Eigen::Vector3f> PathFinder::Find()
{
    return hybrid_astar.searchHybridAStar(start(0),start(2), rotation, goal(0), goal(1), 1.57, grid);
}

void PathFinder::Clear() noexcept
{
    obstacles.clear();
    car.reset();
}
