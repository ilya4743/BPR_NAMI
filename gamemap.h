#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "locationmap.h"
#include "mygraph.h"
#include "barrier.h"
#include "point.h"
#include "mylog.h"
#include "car.h"
using namespace std;

class Barrier;

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
    Point goal_point;

    /// Вершина графа откуда стартует авто
    vertex_descriptor start;

    /// Вершина графа куда едем
    vertex_descriptor goal;

    /// Автомобиль
    Car car;

    /// Навигационный граф
    MyGraph* graph;

    /// Матрица расстояний навигационного графа
    DistanceMatrix* distance;

    /// Список препятствий
    list<Barrier*> barriers;

    /// Кратчайший путь
    list<Point> short_path;

    bool isSmoothing;

    IDistanceMatrixAdapter* adapter;

    /// Формирование сообщения под отправку
    list<Point> create_msg(const DistanceMatrix& locations, list<vertex_descriptor>& shortest_path);

    /// Конструктор по умолчанию
    GameMap();

    /// Конструктор с параметрами
    GameMap(float width_coord, float height_coord, float step, int center, float width_auto, float height_auto, Point goal_p, bool isSmoothing);

    /// Инициализация карты
    void init();


    void doo(const int DEBUG_OUTPUT);

    /// Деструктор
    ~GameMap();

    friend ostream& operator <<(ostream &out, const GameMap &map);
    friend istream& operator >>(istream &in, GameMap &map);
    friend QDataStream& operator <<(QDataStream &out, const GameMap &map);
    friend QDataStream& operator >>(QDataStream &in, GameMap &map);
};

class GameMapPrinter
{
public:
    /// Печать игрового поля в консоль
    static void print_game_map(const GameMap &map);

    /// Печать вершин игрового поля в консоль
    static void print_vertex_map(const GameMap &map);

    /// Печать пути
    static void print_way(const GameMap& distance, const list<vertex_descriptor>& shortest_path);

    /// Печать в файл
    static void printToFile(const GameMap& map, ofstream& out);
};

#endif // GAMEMAP_H
