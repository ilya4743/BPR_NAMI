#include "placer.h"

// inline void Placer::bresenham(int x1, int y1, int x2, int y2, OccupancyGrid& grid)
// {
//     const int deltaX = abs(x2 - x1);
//     const int deltaY = abs(y2 - y1);
//     const int signX = x1 < x2 ? 1 : -1;
//     const int signY = y1 < y2 ? 1 : -1;
//     int error = deltaX - deltaY;
//     while(x1 != x2 || y1 != y2)
//     {
//         int error2 = error * 2;
//         if(error2 > -deltaY)
//         {
//             error -= deltaY;
//             x1 += signX;
//         }
//         if(error2 < deltaX)
//         {
//             error += deltaX;
//             y1 += signY;
//         }
//         grid.data[grid.width*y1+x1]=100;
//         occupancyCell.push_back(grid.width*y1+x1);
//     }
// }

// inline void Placer::placeObstacleOnGrid(OccupancyGrid& OccupancyGrid, const BQuadrAngle & obstacle)
// {

// };