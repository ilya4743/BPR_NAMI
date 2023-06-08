#pragma once

#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <list>

#include "obstacle.h"
#include "occupancy_grid.h"

namespace bg = boost::geometry;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::polygon<point> polygon;
class Placer {
   private:
    inline void Bresenham(int x1, int y1, int x2, int y2, OccupancyGrid& grid) {
        // если препятствие слишком маленькое, с 1 ячейку, то просто закрасим её
        if (x1 == x2 || y1 == y2) {
            // запоминаем какую ячейку закрасили, чтобы потом при очередном запросе быстро можно было очистить сетку занятости
            // не пересоздавая её полность
            occupancyCell.push_back(grid.width * y1 + x1);
            grid.data[grid.width * y1 + x1] = 100;
            return;
        }
        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;
        while (x1 != x2 || y1 != y2) {
            int error2 = error * 2;
            if (error2 > -deltaY) {
                error -= deltaY;
                x1 += signX;
            }
            if (error2 < deltaX) {
                error += deltaX;
                y1 += signY;
            }
            grid.data[grid.width * y1 + x1] = 100;
            // запоминаем какую ячейку закрасили, чтобы потом при очередном запросе быстро можно было очистить сетку занятости
            // не пересоздавая её полность
            occupancyCell.push_back(grid.width * y1 + x1);
        }
    }
    // закрашенные ячейки
    std::list<int> occupancyCell;

   public:
    inline void PlaceObstacleOnGrid(OccupancyGrid& grid, const BQuadrAngle& obstacle) {
        float width = (grid.width - 1) * grid.resolution;
        float height = (grid.height - 1) * grid.resolution;
        std::vector<Eigen::Vector4f> points = obstacle.GetVertexes();

        polygon poly;
        poly.outer().reserve(points.size());
        for (auto it = points.begin(); it != points.end(); ++it)
            poly.outer().push_back({(*it)(0), (*it)(2)});
        poly.outer().push_back(poly.outer()[0]);

        box box{{0, 0}, {width, height - grid.resolution}};

        // Находим пересечение сетки занятости и препятствия
        if (boost::geometry::intersects(box, poly)) {
            std::vector<polygon> output;
            boost::geometry::intersection(box, poly, output);
            std::vector<Eigen::Vector4f> out;

            if (output.size() > 0) {
                out.reserve(output[0].outer().size());
                for (int i = 0; i < output[0].outer().size(); i++)
                    out.push_back(Eigen::Vector4f{bg::get<0>(output[0].outer()[i]), 0, bg::get<1>(output[0].outer()[i]), 1});
                for (int i = 1; i < out.size(); i++) {
                    int x1 = grid.getI(out[i - 1]);
                    int y1 = grid.getJ(out[i - 1]);

                    int x2 = grid.getI(out[i]);
                    int y2 = grid.getJ(out[i]);
                    // закрашиваем
                    Bresenham(x1, y1, x2, y2, grid);
                }
            } else {
                for (int i = 1; i < points.size(); i++) {
                    int x1 = grid.getI(points[i - 1]);
                    int y1 = grid.getJ(points[i - 1]);

                    int x2 = grid.getI(points[i]);
                    int y2 = grid.getJ(points[i]);
                    // закрашиваем
                    Bresenham(x1, y1, x2, y2, grid);
                }
            }
        }
    }

    // очищаем сетку занятости, используя occupancyCell, чтобы не пересоздавать заново
    inline void clearGrid(OccupancyGrid& grid) {
        for (auto it = occupancyCell.cbegin(); it != occupancyCell.end(); ++it)
            grid.data[*it] = 0;
        occupancyCell.clear();
    }
};