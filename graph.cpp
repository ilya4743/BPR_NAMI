#include <boost/graph/astar_search.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/random.hpp>
#include <boost/graph/random.hpp>
#include <iosfwd>
#include <windows.h>

using namespace boost; 
using namespace std;

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
        //cout << u << '(' << (dx * dx + dy * dy) <<')' << endl;
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
    }
private:
    Vertex m_goal;
};

//вес ребра
typedef boost::property<boost::edge_weight_t, int> weight;
//граф
typedef adjacency_list<vecS, vecS, undirectedS, boost::no_property, weight> my_graph;
//итератор дуг
typedef boost::graph_traits<my_graph>::edge_iterator edge_iterator;
//итератор вершин
typedef boost::graph_traits<my_graph>::vertex_iterator vertex_iterator;
//карта весов рёбер
typedef boost::property_map<my_graph, boost::edge_weight_t>::type Weight_Map;
//пара итераторов для ребра
typedef pair<edge_iterator, edge_iterator> edgePair;
//пара итераторов для вершины
typedef pair<vertex_iterator, vertex_iterator> vertexPair;
//дескриптор вершин
typedef typename graph_traits<my_graph>::vertex_descriptor vertex_descriptor;
//дескриптор рёбер
typedef typename graph_traits<my_graph>::edge_descriptor  edge_descriptor;

struct location
{
    float x,y; // lat, long
};

const int width = 50;
const int height = 30;

void print_vertexes(const my_graph& g)
{
    cout << "Vertex:\n";
    for (vertexPair vi = vertices(g); vi.first != vi.second; ++vi.first)
        cout << *vi.first << endl;
}

void print_edges(const my_graph &g)
{
    cout << "Edges:\n";
    for (edgePair ei = edges(g); ei.first != ei.second; ++ei.first)
        cout << *ei.first << endl;
}

void print_in_to_graphviz(const my_graph& g)
{
    ofstream f("graph.dot");
    boost::write_graphviz(f, g);
    f.close();
}

struct point
{
    int x, y;
    point(int x, int y)
    {
        this->x=x;
        this->y=y;
    }
};

struct figure
{
    vector<point> points;
};

void print_game_map()
{
    for (int i = 0; i < height; i++)
    { 
        for (int j = 0; j < width; j++)
            cout << '*';
        cout << endl;
    }
}

void print_here(int x, int y)
{
    DWORD dw;
    COORD here;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
    {
        printf("Invalid handle");
    }
    here.X = x;
    here.Y = y;
    WriteConsoleOutputCharacter (hStdOut, L" ", 1, here, &dw);
}

void color_change(int x, int y, WORD w)
{
    DWORD dw;
    COORD here;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
    {
        printf("Invalid handle");
    }
    here.X = x;
    here.Y = y;

    WORD wColors[1], wColor;
    wColors[0] = w;
    WriteConsoleOutputAttribute(hStdOut, wColors, 1, here, &dw);
}

void print_game_figure(const figure&f1,  my_graph& g)
{
    for (int i = f1.points[0].y; i <= f1.points[3].y; i++)
        for (int j = f1.points[0].x; j <= f1.points[3].x; j++)
        {
            print_here(i,j);
            color_change(i, j, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        }

    //соседние по горизонтали
    for(int i= f1.points[0].y; i <= f1.points[3].y+1; i++)
    { 
        if (i == 0||i==width)continue;
        for (int j = f1.points[0].x; j <= f1.points[3].x; j++)
        {
            int u = j * width + i - 1;
            int v = j * width + i;
            remove_edge(u, v, g);
        }
    }

    //соседние по вертикале
    for (int i = f1.points[0].y; i <= f1.points[3].y; i++)
        for (int j = f1.points[0].x; j <= f1.points[3].x+1; j++)
        {
            if (j == 0 || j == height)continue;
            int u2 = (j - 1) * width + i;
            int v2 = (j)*width + i;
            remove_edge(u2, v2, g);
        }

    //главная диагональ
    for (int i = f1.points[0].y; i <= f1.points[3].y+1; i++)
    { 
        if (i == 0 || i == width)continue;
        for (int j = f1.points[0].x; j <= f1.points[3].x+1; j++)
        {
            if (j == 0 || j == height)continue;
            int u1 = (j-1) * width + i-1;
            int v1 = (j) * width + i;
            remove_edge(u1, v1, g);
        }
    }  
    //побочная диагональ
    for (int i = f1.points[3].y+1; i >= f1.points[0].y; i--)
    {
        if (i == width)continue;
        for (int j = f1.points[3].x+1; j >= f1.points[0].x; j--)
        {
            if (j == height||j==0)continue;
            int u1 = (j) * width + i-1;
            int v1 = (j-1) * width + i;
            remove_edge(u1, v1, g);
        }
    }
}

void print_way(const vector<location>&locations, const vector<vertex_descriptor>&shortest_path)
{
    for (int i = 0; i<shortest_path.size(); i++)
        color_change(locations[shortest_path[i]].y, locations[shortest_path[i]].x, FOREGROUND_RED);
}

int main()
{
    vector<location> locations(width* height);    
    my_graph test(width * height);
    figure f1;
    f1.points.push_back(point(3, 5));
    f1.points.push_back(point(3, 8));
    f1.points.push_back(point(7, 5));
    f1.points.push_back(point(7, 8));
    figure f2;
    f2.points.push_back(point(9, 30));
    f2.points.push_back(point(9, 38));
    f2.points.push_back(point(19, 30));
    f2.points.push_back(point(19, 38));
    figure f3;
    f3.points.push_back(point(12, 15));
    f3.points.push_back(point(12, 20));
    f3.points.push_back(point(20, 15));
    f3.points.push_back(point(20, 20));
    figure f4;
    f4.points.push_back(point(22, 7));
    f4.points.push_back(point(22, 40));
    f4.points.push_back(point(27, 7));
    f4.points.push_back(point(27, 40));
    figure f5;
    f5.points.push_back(point(1, 33));
    f5.points.push_back(point(1, 48));
    f5.points.push_back(point(5, 33));
    f5.points.push_back(point(5, 48));
    print_game_map();    

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            locations[i*width+j].x = i;
            locations[i * width + j].y = j;
        }

    for (int i = 1; i < height; i++)
        for (int j = 0; j < width; j++)
            add_edge(((i-1) * width)+j, i * width + j, test);

    for (int i = 0; i < height; i++)
        for (int j = 1; j < width; j++)
            add_edge(i * width +j-1, i * width +j, test);

    for(int i=0;i< height-1;i++)
        for(int j=0;j< width-1;j++)
            add_edge(i * width +j, i * width + (width+1) + j, test);
    
    for (int i = height-1; i > 0; i--)
        for (int j = width-2; j >= 0; j--)
            add_edge(i * width + j, i * width - (width-1) + j, test);

    print_game_figure(f1,test);    
    print_game_figure(f2, test);
    print_game_figure(f3, test);
    print_game_figure(f4, test);
    print_game_figure(f5, test);

    for (vertexPair vit = vertices(test); vit.first != vit.second; ++vit.first)
    {
        cout << *vit.first;
        for (int j = 0; j < width-1; j++)
            cout << setw(5) <<*(++vit.first);
        cout << endl;
    }

     // pick random start/goal
    boost::mt19937 gen(std::time(0));
    vertex_descriptor start = 0;// random_vertex(test, gen);
    vertex_descriptor goal = 1499;// random_vertex(test, gen);

    cout << "Start vertex: " << start << endl;
    cout << "Goal vertex: " <<goal << endl;

    vector<vertex_descriptor> p(num_vertices(test));
    vector<int> d(num_vertices(test));
    try {
        // call astar named parameter interface
        astar_search_tree(test, start, distance_heuristic<my_graph, int, vector<location>>(locations, goal),
            predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, test))).
            distance_map(make_iterator_property_map(d.begin(), get(vertex_index, test))).
            visitor(astar_goal_visitor<vertex_descriptor>(goal)));
    }
    catch (found_goal fg) { // found a path to the goal
        list<vertex_descriptor> shortest_path;
        for (vertex_descriptor v = goal;; v = p[v]) {
            shortest_path.push_front(v);
            if (p[v] == v)
                break;
        }
        cout << "Shortest path from " << start << " to " << goal << ": ";
        list<vertex_descriptor>::iterator spi = shortest_path.begin();
        cout << start;
        vector<vertex_descriptor> shortest_path1;
        int k = 0;
        for (++spi; spi != shortest_path.end(); ++spi)
        { 
            cout << " -> " << *spi;
            shortest_path1.push_back(*spi);
        }
        shortest_path1.push_back(start);
        print_way(locations, shortest_path1);
        cout << endl << "Total travel time: " << d[goal] << endl;
        return 0;
    }
    cout << "Didn't find a path from " << start << "to"<< goal << "!" << endl;
    return EXIT_SUCCESS;
}