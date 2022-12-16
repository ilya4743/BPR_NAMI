#include "node3d.h"
#include "algorithm.h"
#include "helper.h"
#include <smoother.h>
#include <vector>
#include "dynamicvoronoi.h"
#include <OGRE/OgreVector3.h>
#include "occupancygrid.h"

using namespace std;
using namespace HybridAStar;

class HybridAstarAlgo
{
    private:
        DynamicVoronoi* voronoiDiagram;
        OccupancyGrid grid;
        CollisionDetection cd;
        vector<int8_t> ggrid;
        float* dubinsLookup;
        Smoother smoother;
    public:
    HybridAstarAlgo()
    {
        dubinsLookup= new float [Constants::headings * Constants::headings * Constants::dubinsWidth * Constants::dubinsWidth];   
        //voronoiDiagram=new DynamicVoronoi;   
    }
    ~HybridAstarAlgo()
    {        
        //delete voronoiDiagram;
        delete [] dubinsLookup;
    }
std::vector<Ogre::Vector3> searchHybridAStar(float x1, float y1, float t1, float x2, float y2, float t2, const OccupancyGrid& occurancy);
};