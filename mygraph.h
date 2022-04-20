#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/astar_search.hpp>
#include"locationmap.h"
#include "point.h"
#include"myexception.h"

using namespace boost;

/// Вес ребра
typedef boost::property<boost::edge_weight_t, float> weight;
/// Граф (список смежности)
typedef adjacency_list<vecS, vecS, undirectedS, boost::no_property, weight> my_graph;
/// Итератор дуг
typedef boost::graph_traits<my_graph>::edge_iterator edge_iterator;
/// Итератор вершин
typedef boost::graph_traits<my_graph>::vertex_iterator vertex_iterator;
/// Карта весов рёбер
typedef boost::property_map<my_graph, boost::edge_weight_t>::type Weight_Map;
/// Пара итераторов для ребра
typedef std::pair<edge_iterator, edge_iterator> edgePair;
/// Пара итераторов для вершины
typedef std::pair<vertex_iterator, vertex_iterator> vertexPair;
/// Дескриптор вершин
typedef typename graph_traits<my_graph>::vertex_descriptor vertex_descriptor;
/// Дескриптор рёбер
typedef typename graph_traits<my_graph>::edge_descriptor  edge_descriptor;

/// Интерфейс графа
class IGraph
{
public:
    /// @brief Поиск путь на графе
    /// @param start вершина графа, отвечающая за начало пути
    /// @param goal вершина графа, отвечающая за конец пути
    /// @param distance матрица расстояний
    /// @return вектор вершин графа, через которые пролегает путь
    /// @throw -1 путь не найден
    std::list<vertex_descriptor> virtual search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Point>& distance)=0;

    /// @brief Инициализация связей графа
    /// @param width количество вершин графа по ширине
    /// @param height количество вершин графа по высоте
    void virtual init(int width, int height)=0;

    /// Виртуальный деструктор
    virtual ~IGraph()=0;
};

/** @brief Класс для построения неориентированного прямоугольного графа
 *   @details
 *   <p>Значение <b>width</b> и <b>height</b> неориентированного прямоугольного графа задают количество вершин по ширине и высоте соответственно.
 *   <br>Рёбра графа связывают смежные пары вершин по горизонтали, вертикали, главной и побочной диагонали.
 *   <br>На рисунке изображена схема неориентированного прямоугольного графа
 *   <p><img src="pic/GQuadrAngle1.svg" alt="Схема графа" height="400px" width="400px" />
*/
class MyGraph:public IGraph
{
public:
    /// Граф (список смежности)
    my_graph* adj_list;

    /// Конструктор по умолчанию
    MyGraph();

    /// Конструктор с параметрами
    MyGraph(int num_vertices);

    /// Конструктор копирования
    MyGraph(MyGraph&o);

    ~MyGraph();

    void init(int width, int height)override;
    std::list<vertex_descriptor> search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Point>& distance)override;

    /// Печать номеров вершин в консоль
    void print_vertexes();

    /// Печать дуг в консоль
    void print_edges();

    /// Печать графа в graphviz
    void print_to_graphviz();
};

#endif // MYGRAPH_H
