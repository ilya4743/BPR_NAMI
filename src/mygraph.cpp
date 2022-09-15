#include "mygraph.h"

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
    Weight_Map weight_map = get(edge_weight, *this->adj_list);

    for (int i = 1; i < height; i++)
        for (int j = 0; j < width; j++)
            weight_map[add_edge(((i - 1) * width) + j, i * width + j, *this->adj_list).first]=1;
        
    for (int i = 0; i < height; i++)
        for (int j = 1; j < width; j++)
            weight_map[add_edge(i * width + j - 1, i * width + j, *this->adj_list).first]=1;
        

    for (int i = 0; i < height - 1; i++)
        for (int j = 0; j < width - 1; j++)
            weight_map[add_edge(i * width + j, i * width + (width + 1) + j, *this->adj_list).first]=1.1;

    for (int i = height - 1; i > 0; i--)
        for (int j = width - 2; j >= 0; j--)
            weight_map[add_edge(i * width + j, i * width - (width - 1) + j, *this->adj_list).first]=1.1;
}

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class euclidean_heuristic : public astar_heuristic<Graph, CostType>
{
public:
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    euclidean_heuristic(LocMap l, Vertex goal) : m_location(l), m_goal(goal) {
    }
    CostType operator()(Vertex u)
    {
        CostType dx = m_location[m_goal].x - m_location[u].x;
        CostType dy = m_location[m_goal].z - m_location[u].z;
        float a=sqrt(dx * dx + dy * dy);
        return sqrt(dx * dx + dy * dy);
    }
private:
    LocMap m_location;
    Vertex m_goal;
};

//diagonal_distance_heuristic
template <class Graph, class CostType, class LocMap>
class diagonal_heuristic : public astar_heuristic<Graph, CostType>
{
public:
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    diagonal_heuristic(LocMap l, Vertex goal) : m_location(l), m_goal(goal) {}
    CostType operator()(Vertex u)
    {
        CostType dx = abs(m_location[m_goal].x - m_location[u].x);
        CostType dz = abs(m_location[m_goal].z - m_location[u].z);
        float D=1;
        float D2=1;
        float m=min(dx,dz);
        float g=D*(dx+dz)+(D2-2*D)*m;
        return g;
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
    }
    template <class Graph>
    void finish_vertex(Vertex u, Graph& g)
    {
        //cout<<u<<endl;
    }
private:
    Vertex m_goal;
};

std::vector<vertex_descriptor> MyGraph::search(const vertex_descriptor start,const vertex_descriptor goal,const vector<Ogre::Vector3>& distance )
{
    vector<vertex_descriptor> p(num_vertices(*this->adj_list));
    vector<float> d(num_vertices(*this->adj_list));
    try {
        // call astar named parameter interface
        astar_search_tree(*this->adj_list, start, euclidean_heuristic<my_graph, float, vector<Ogre::Vector3>>(distance, goal),
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
        shortest_path.pop_front();
        shortest_path.push_back(start);
        vector<vertex_descriptor> path{ std::make_move_iterator(std::begin(shortest_path)),
                    std::make_move_iterator(std::end(shortest_path)) };
        return path;
    }
    throw MyException("Path not found!\0",PathNotFound);
    cout << "Didn't find a path from " << start << "to" << goal << "!" << endl;
}

MyGraph::~MyGraph()
{
    adj_list->clear();
    delete adj_list;
}

template<class Graph>
void MyGraphPrinter::print_vertexes(const Graph& graph)
{
    cout << "Vertex:\n";
    for (vertexPair vi = vertices(graph.adj_list); vi.first != vi.second; ++vi.first)
        cout << *vi.first << endl;
}

template<class Graph>
void print_edges(const Graph& graph)
{
    cout << "Edges:\n";
    for (edgePair ei = edges(graph.adj_list); ei.first != ei.second; ++ei.first)
        cout << *ei.first << endl;
}

template<class Graph>
void print_to_graphviz(const Graph& graph, const string& filename)
{
    ofstream f(filename);
    write_graphviz(f, graph.adj_list);
    f.close();
}
