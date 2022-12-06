#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "barrier.h"
#include "car.h"
#include <OgreVector.h>

using namespace std;

class GameMap
{
public:

    /// Ширина покрытия сетки графа
    float width_coord;

    /// Высота покрытия сетки графа
    float height_coord;

    /// Шаг вершин графа
    float step;

    /// Точка конечного маршрута
    Ogre::Vector3 goal_point;

    /// Автомобиль
    Car car;

    /// Список препятствий
    vector<BQuadrAngle> barriers;

    /// Конструктор по умолчанию
    GameMap();

    /// Конструктор с параметрами
    GameMap(float width_coord, float height_coord, float step,
            const Car &car,
            const Ogre::Vector3 &goal_p);

    /// Деструктор
    ~GameMap();
};

#endif // GAMEMAP_H
