#ifndef GAMEMAP_H
#define GAMEMAP_H
#include"locationmap.h"
#include"mygraph.h"
#include"barrier.h"
#include"point.h"
#include"mylog.h"

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
    /// Шаг вершин графа
    float step;

    /// Ширина покрытия сетки графа
    float width_coord;

    /// Высота покрытия сетки графа
    float height_coord;

    /// Количество вершин графа по ширине
    int num_vertices_width;

    /// Количество вершин графа по высоте
    int num_vertices_height;

    /// Начало отсчета системы координат (наша машина)
    int center;

    /// Точка конечного маршрута
    Point goal_point;

    /// Вершина графа откуда стартует авто
    vertex_descriptor start;

    /// Вершина графа куда едем
    int goal;

    /// Ширина авто
    float width_auto;

    /// Высота авто
    float height_auto;

    /// Конструктор по умолчанию
    GameMap();

    /// Конструктор с параметрами
    GameMap(float width_coord, float height_coord, float step, int center, float width_auto, float height_auto, Point goal_p, bool isSmoothing);

    /// Навигационный граф
    MyGraph* graph;

    /// Матрица расстояний навигационного графа
    DistanceMatrix* distance;

    /// Список препятствий
    list<Barrier*> barriers;

    /// Кратчайший путь
    list<Point> short_path;

    /// Конструктор с параметрами
    bool isSmoothing;

    /// Формирование сообщения под отправку
    list<Point> create_msg(const DistanceMatrix& locations, list<vertex_descriptor>& shortest_path);

    /// Печать игрового поля в консоль
    void print_game_map();

    /// Печать вершин игрового поля в консоль
    void print_vertex_map();

    /// Печать пути
    void print_way(const DistanceMatrix* distance, const list<vertex_descriptor>& shortest_path);

    int GetJ(float i);
    int GetI(float j);
    void init(IDistanceMatrix&distance,IGraph&graph);
    void doo(const int DEBUG_OUTPUT);

    /// Печать в файл
    void printToFile(ofstream& out);

    /// Деструктор
    ~GameMap();
};

#endif // GAMEMAP_H
