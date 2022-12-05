#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "locationmap.h"
#include "mygraph.h"
#include "barrier.h"
#include "car.h"
#include <OgreVector.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

typedef bg::model::d2::point_xy<float> point2d;
namespace bg = boost::geometry;
using namespace std;

class Barrier;
class BQuadrAngle;

/** @brief Класс игрового поля
 *  @details
 *  <p>Игровое поле представляет из себя матрицу расстояний (<b>distance</b>) наложенную на граф (<b>graph</b>), см. рис. ниже
 *  <p><img src="pic/GameMap1.svg" alt="Игровое поле" height="425px" width="450px" >
*/

class GameMap
{
public:

    /// Ширина покрытия сетки графа
    float width_coord;

    /// Высота покрытия сетки графа
    float height_coord;

    /// Шаг вершин графа
    float step;

    /// Количество вершин графа по ширине
    int num_vertices_width;

    /// Количество вершин графа по высоте
    int num_vertices_height;

    /// Точка конечного маршрута
    Ogre::Vector3 goal_point;

    /// Вершина графа откуда стартует авто
    vertex_descriptor start;

    /// Вершина графа куда едем
    vertex_descriptor goal;

    /// Автомобиль
    Car car;

    /// Список препятствий
    vector<BQuadrAngle> barriers;

    /// Кратчайший путь
    vector<Ogre::Vector3> short_path;

    /// Формирование сообщения под отправку
    vector<Ogre::Vector3> create_msg(vector<vertex_descriptor>& shortest_path);

    /// Конструктор по умолчанию
    GameMap();

    /// Конструктор с параметрами
    GameMap(float width_coord, float height_coord, float step, int center,
            const Car &car,
            const Ogre::Vector3 &goal_p);

    /// Инициализация карты
    void init();


    void doo(const int DEBUG_OUTPUT);

    /// Деструктор
    ~GameMap();
};

class GameMapPrinter
{
public:
    /// Печать игрового поля в консоль
    static void print_game_map(const DMQuadrangle &map);

    /// Печать вершин игрового поля в консоль
    static void print_vertex_map(const GameMap &map);

    /// Печать пути
    static void print_way(const DMQuadrangle& dm, const vector<vertex_descriptor>& shortest_path);
};

#endif // GAMEMAP_H
