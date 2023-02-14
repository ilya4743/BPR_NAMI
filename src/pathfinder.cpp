#include "pathfinder.h"

void PathFinder::UpdateData(float width, float height, float resolution, uint32_t center, float x, float y,
    float theta, float speed, size_t n, std::map<uint64_t,TransformMatrix>&& objects)
{
    grid.resize(width, height, resolution);
    auto itCar=objects.find(0);
    objects.erase(itCar);
    TransformMatrix mat4=(*itCar).second;
    auto qua=ExtractQuaternion(mat4);
    auto pos=ExtractPosition(mat4);

    Vector4 pos1{-width/2, Y(pos), -height/2, 1};
    pos1=mat4*pos1;
    TransformMatrix mat41(mat4);
    SetPosition(pos1, mat41);
    mat4=inverse(mat41)*mat4;

    auto pos_local=ExtractPosition(mat4);
    rotation=qua;
    start=pos_local;

    car=std::make_unique<Car>(mat4, speed);
    X(goal)=x+X(pos_local);
    Y(goal)=theta+Z(pos_local);
    Z(goal)=theta;
    obstacles.reserve(n);
    placer.clearGrid(grid);
    for(auto it=objects.begin(); it!=objects.end(); ++it)
    {
        auto obstacle=std::make_unique<BQuadrAngle>(inverse(mat41)*(*it).second);
        placer.placeObstacleOnGrid(grid, *(obstacle.get()));
        obstacles.push_back(std::move(obstacle));
    }
    system("clear");
    for(int i=grid.height-1; i>=0; i--)
    {
        for (int j=grid.width-1; j>=0; j--)
        {
            if(grid.data[i*grid.width+j]==100)
                std::cout<<1;
             else std::cout<<0;
        }
        std::cout<<std::endl;
    }      
}

std::vector<Vector3> PathFinder::Find()
{
    return hybrid_astar.searchHybridAStar(X(start),Z(start), rotation, X(goal), Y(goal), 1.57, grid);
}

void PathFinder::Clear() noexcept
{
    obstacles.clear();
    car.reset();
}
