#include "mygraph.h"
#include "point.h"

using namespace std;

IGraph::~IGraph()
{

}

MyGraph::MyGraph()
{
    this->adj_list=NULL;
}

MyGraph::MyGraph(int num_vertices)
{
    this->adj_list=new my_graph(num_vertices);
}

MyGraph::MyGraph(MyGraph&o)
{
    this->adj_list=o.adj_list;
}

void MyGraph::init(int width, int height)
{
    for (int i = 1; i < height; i++)
        for (int j = 0; j < width; j++)
            add_edge(((i - 1) * width) + j, i * width + j, *this->adj_list);

    for (int i = 0; i < height; i++)
        for (int j = 1; j < width; j++)
            add_edge(i * width + j - 1, i * width + j, *this->adj_list);

    for (int i = 0; i < height - 1; i++)
        for (int j = 0; j < width - 1; j++)
            add_edge(i * width + j, i * width + (width + 1) + j, *this->adj_list);

    for (int i = height - 1; i > 0; i--)
        for (int j = width - 2; j >= 0; j--)
            add_edge(i * width + j, i * width - (width - 1) + j, *this->adj_list);
}

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
public:
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    distance_heuristic(LocMap l, Vertex goal) : m_location(l), m_goal(goal) {}
    CostType operator()(Vertex u)
    {
        CostType dx = m_location[m_goal].x - m_location[u].x;
        CostType dy = m_location[m_goal].y - m_location[u].y;
        float f=sqrt(dx * dx + dy * dy);
        return ::sqrt(dx * dx + dy * dy);
    }
private:
    LocMap m_location;
    Vertex m_goal;
};

struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal) : m_goal(goal) {}
    template <class Graph>
    void examine_vertex(Vertex u, Graph& g) {
        if (u == m_goal)
            throw found_goal();
        //cout<<u;
    }
    template <class Graph>
    void finish_vertex(Vertex u, Graph& g)
    {
        //cout<<u<<endl;
    }
private:
    Vertex m_goal;
};

void MyGraph::print_vertexes()
{
    cout << "Vertex:\n";
    for (vertexPair vi = vertices(*this->adj_list); vi.first != vi.second; ++vi.first)
        cout << *vi.first << endl;
}

void MyGraph::print_edges()
{
    cout << "Edges:\n";
    for (edgePair ei = edges(*this->adj_list); ei.first != ei.second; ++ei.first)
        cout << *ei.first << endl;
}

void MyGraph::print_to_graphviz()
{
    ofstream f("graph.dot");
    write_graphviz(f, *this->adj_list);
    f.close();
}

vector<vertex_descriptor> MyGraph::search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Point>& distance )
{

    vector<vertex_descriptor> p(num_vertices(*this->adj_list));
    vector<float> d(num_vertices(*this->adj_list));
    try {
        // call astar named parameter interface
        astar_search_tree(*this->adj_list, start, distance_heuristic<my_graph, float, vector<Point>>(distance, goal),
            predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, *this->adj_list))).
            distance_map(make_iterator_property_map(d.begin(), get(vertex_index, *this->adj_list))).
            visitor(astar_goal_visitor<vertex_descriptor>(goal)));
    }
    catch (found_goal fg) { // found a path to the goal
        list<vertex_descriptor> shortest_path;
        for (vertex_descriptor v = goal;; v = p[v]) {
            shortest_path.push_front(v);
            if (p[v] == v)
                break;
        }
        list<vertex_descriptor>::iterator spi = shortest_path.begin();
        vector<vertex_descriptor> shortest_path1;
        shortest_path1.reserve(shortest_path.size());
        for (++spi; spi != shortest_path.end(); ++spi)
            shortest_path1.push_back(*spi);

        shortest_path1.push_back(start);
        return shortest_path1;
    }
    list<vertex_descriptor> shortest_path;
            for (vertex_descriptor v = goal;; v = p[v]) {
                shortest_path.push_front(v);
                if (p[v] == v)
                    break;
            }
            list<vertex_descriptor>::iterator spi = shortest_path.begin();
            vector<vertex_descriptor> shortest_path1;
            shortest_path1.reserve(shortest_path.size());
            for (++spi; spi != shortest_path.end(); ++spi)
                shortest_path1.push_back(*spi);

            shortest_path1.push_back(start);
    throw MyException("Path not found!\0",PathNotFound);
    cout << "Didn't find a path from " << start << "to" << goal << "!" << endl;
}

MyGraph::~MyGraph()
{
    adj_list->clear();
    delete adj_list;
}
