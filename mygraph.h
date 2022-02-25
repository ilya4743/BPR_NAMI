#ifndef MYGRAPH_H
#define MYGRAPH_H
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/astar_search.hpp>
#include"locationmap.h"
#include "point.h"
#include"myexception.h"

using namespace boost;

//вес ребра
typedef boost::property<boost::edge_weight_t, float> weight;
//граф
typedef adjacency_list<vecS, vecS, undirectedS, boost::no_property, weight> my_graph;
//итератор дуг
typedef boost::graph_traits<my_graph>::edge_iterator edge_iterator;
//итератор вершин
typedef boost::graph_traits<my_graph>::vertex_iterator vertex_iterator;
//карта весов рёбер
typedef boost::property_map<my_graph, boost::edge_weight_t>::type Weight_Map;
//пара итераторов для ребра
typedef std::pair<edge_iterator, edge_iterator> edgePair;
//пара итераторов для вершины
typedef std::pair<vertex_iterator, vertex_iterator> vertexPair;
//дескриптор вершин
typedef typename graph_traits<my_graph>::vertex_descriptor vertex_descriptor;
//дескриптор рёбер
typedef typename graph_traits<my_graph>::edge_descriptor  edge_descriptor;

class IGraph
{
public:
    std::list<vertex_descriptor> virtual search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Point>& distance)=0;
    void virtual init(int width, int height)=0;
    virtual ~IGraph()=0;
};

class MyGraph:public IGraph
{
public:
    my_graph* adj_list;

    MyGraph();
    MyGraph(int num_vertices);
    MyGraph(MyGraph&o);
    void init(int width, int height)override;
    void print_vertexes();
    void print_edges();
    void print_to_graphviz();
    std::list<vertex_descriptor> search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Point>& distance)override;
    ~MyGraph();
};

#endif // MYGRAPH_H
