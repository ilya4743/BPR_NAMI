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
typedef pair<edge_iterator, edge_iterator> edgePair;
//пара итераторов для вершины
typedef pair<vertex_iterator, vertex_iterator> vertexPair;
//дескриптор вершин
typedef typename graph_traits<my_graph>::vertex_descriptor vertex_descriptor;
//дескриптор рёбер
typedef typename graph_traits<my_graph>::edge_descriptor  edge_descriptor;

struct location
{
    float x, y; // lat, long
};

struct point
{
    float x, y;
    point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};
int GetI(float j);
int GetJ(float i);

//шаг вершин графа
const float step = 0.25;
//ширина покрытия сетки графа 
const float width_coord=16;
//высота покрытия сетки графа
const float height_coord=5.2;
//количество вершин графа по ширине
const int width = width_coord/step;
//количество вершин графа по высоте
const int height = height_coord/step;
//начало отсчета системы координат (наша машина)
const int center = (width / 2) + ((height - 1) * width);
//точка конечного маршрута
const point goal_point(-0.4, 2);
//вершина графа откуда стартует авто
const vertex_descriptor start = center;
//вершина графа куда едем
const vertex_descriptor goal = GetI(goal_point.y)*width + GetJ(goal_point.x);

void print_vertexes(const my_graph& g)
{
    cout << "Vertex:\n";
    for (vertexPair vi = vertices(g); vi.first != vi.second; ++vi.first)
        cout << *vi.first << endl;
}

void print_vertex_map(const my_graph& g)
{
    cout << endl << "vertex map: \n";
    for (vertexPair vit = vertices(g); vit.first != vit.second; ++vit.first)
    {
        cout << *vit.first;
        for (int j = 0; j < width - 1; j++)
            cout << setw(5) << *(++vit.first);
        cout << endl;
    }
    cout << endl;
}

void print_edges(const my_graph& g)
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
    cout << endl;
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
    WriteConsoleOutputCharacter(hStdOut, L" ", 1, here, &dw);
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

int GetJ(float i)
{
    return (center + int(i/step)) % width;
}

int GetI(float j)
{
        return (center - int(j / step) * width) / width;
}

void print_game_figure( figure& f1, my_graph& g)
{
    if (abs(f1.points[0].x) > abs(int(f1.points[0].x / step) * step))
    {
        f1.points[0].x = int(f1.points[0].x / step) * step;
        if (f1.points[0].x > 0)
            f1.points[0].x += step;
        else
            f1.points[0].x -= step;
    }

    if (abs(f1.points[3].x) > abs(int(f1.points[3].x / step) * step))
    {
        f1.points[3].x = int(f1.points[3].x / step) * step;
        if (f1.points[3].x > 0)
            f1.points[3].x += step;
        else
            f1.points[3].x -= step;
    }

    if (abs(f1.points[0].y) > abs(int(f1.points[0].y / step) * step))
    {
        f1.points[0].y = int(f1.points[0].y / step) * step;
        if (f1.points[0].y > 0)
            f1.points[0].y += step;
        else
            f1.points[0].y -= step;
    }

    if (abs(f1.points[3].y) > abs(int(f1.points[3].y / step) * step))
    {
        f1.points[3].y = int(f1.points[3].y / step) * step;
        if (f1.points[3].y > 0)
            f1.points[3].y += step;
        else
            f1.points[3].y -= step;
    }

    for (int i = GetI(f1.points[0].y); i <= GetI(f1.points[3].y); i++)
        for (int j = GetJ(f1.points[0].x); j <= GetJ(f1.points[3].x); j++)
        {
            print_here(j, i);
            color_change(j, i, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        }

    //соседние по горизонтали
    for (int i = GetI(f1.points[0].y); i <= GetI(f1.points[3].y); i++)
    {
        for (int j = GetJ(f1.points[0].x); j <= GetJ(f1.points[3].x) + 1; j++)
        {   
            if (j == 0 || j == width)continue;

            int u = i * width + j - 1;
            int v = i * width + j;
            remove_edge(u, v, g);
        }
    }

    //соседние по вертикале
    for (int i = GetI(f1.points[0].y); i <= GetI(f1.points[3].y) + 1; i++)
    {             
        if (i == 0 || i == height)continue;

        for (int j = GetJ(f1.points[0].x); j <= GetJ(f1.points[3].x); j++)
        {
            int u2 = (i - 1) * width + j;
            int v2 = (i)*width + j;
            remove_edge(u2, v2, g);
        }
    }

    //главная диагональ
    for (int i = GetI(f1.points[0].y); i <= GetI(f1.points[3].y) + 1; i++)
    {
        if (i == 0 || i == height)continue;
        for (int j = GetJ(f1.points[0].x); j <= GetJ(f1.points[3].x) + 1; j++)
        {
            if (j == 0 || j == width)continue;
            int u1 = (i - 1) * width + j - 1;
            int v1 = (i)*width + j;
            remove_edge(u1, v1, g);
        }
    }
    //побочная диагональ
    for (int i = GetI(f1.points[3].y) + 1; i >= GetI(f1.points[0].y); i--)
    {
        if (i == height || i==0)continue;
        for (int j = GetJ(f1.points[3].x) + 1; j >= GetJ(f1.points[0].x); j--)
        {
            if (j == width || j == 0)continue;
            int u1 = (i)*width + j - 1;
            int v1 = (i - 1) * width + j;
            remove_edge(u1, v1, g);
        }
    }
}

void print_way(const vector<location>& locations, const vector<vertex_descriptor>& shortest_path)
{
    for (int i = 0; i < shortest_path.size(); i++)
        color_change(GetJ(locations[shortest_path[i]].x), GetI(locations[shortest_path[i]].y), FOREGROUND_RED);
   cout << "\n\nCoord path:\n";
    for (int i = 0; i < shortest_path.size()-1; i++)
        cout << locations[shortest_path[i]].x << '\t' << locations[shortest_path[i]].y<<endl;
  
}

void init_location_map(vector<location>& locations)
{
    float x= step, y=0;
    for (int i = center / width; i >= 0; i--)
    {
        for (int j = center % width; j >= 0; j--)
            locations[i * width + j] = { x-=step,-y };
        x = step;
        y-=step;
    }
    x = -step;
    y = 0;
    for (int i = center / width; i < height; i++)
    {
        for (int j = center % width; j < width; j++)
            locations[i * width + j] = { x+=step,-y };
        x = -step;
        y+=step;
    }y = 0;
    for (int i = center/ width; i >= 0; i--)
    {
        for (int j = center % width; j < width; j++)
            locations[i * width + j] = { x+=step,y };
        x = -step;
        y+=step;
    }
    x = step;
    y = 0;
    for (int i = center/ width; i <height; i++)
    {
        for (int j = center%width; j >=0; j--)
            locations[i * width + j] = { x-=step,y };
        x = step;
        y-=step;
    }
}

void print_location_map(vector<location>&locations)
{
    cout << "locations:\n";
    for (int i = 0; i < height; i++)
    { 
        for (int j = 0; j < width; j++)
            cout <<'{'<< setw(5) << locations[i*width+j].x<<','<<setw(5) << locations[i * width + j].y<<'}'<<' ';
        cout << endl;
    }
}

void init_graph(my_graph& test)
{
    for (int i = 1; i < height; i++)
        for (int j = 0; j < width; j++)
            add_edge(((i - 1) * width) + j, i * width + j, test);

    for (int i = 0; i < height; i++)
        for (int j = 1; j < width; j++)
            add_edge(i * width + j - 1, i * width + j, test);

    for (int i = 0; i < height - 1; i++)
        for (int j = 0; j < width - 1; j++)
            add_edge(i * width + j, i * width + (width + 1) + j, test);

    for (int i = height - 1; i > 0; i--)
        for (int j = width - 2; j >= 0; j--)
            add_edge(i * width + j, i * width - (width - 1) + j, test);
}

int main()
{
    vector<location> locations(width * height);
    my_graph test(width * height);
    figure f1, f2;

    f1.points.push_back(point(-0.3, 0.75));
    f1.points.push_back(point(0, 0));
    f1.points.push_back(point(-1, -1));
    f1.points.push_back(point(0.3, 0.5));
    f2.points.push_back(point(-0.7, 1.7));
    f2.points.push_back(point(-0.7, 1.2));
    f2.points.push_back(point(-0.7, 1.2));
    f2.points.push_back(point(-0.3, 1.5));

    init_location_map(locations);
    init_graph(test);     
    
    print_game_map();
    //print_location_map(locations);   
    //print_vertex_map(test);

    print_game_figure(f1, test);
    print_game_figure(f2, test);

    cout << "Start coord: {"<<setw(5)<<0<<", " << setw(5) <<0<<"}\tStart vertex: " << start << endl;
    cout << " Goal coord: {" << setw(5) << goal_point.x << ", " << setw(5) << goal_point.y<<"}\tGoal vertex: " << goal << endl;

    vector<vertex_descriptor> p(num_vertices(test));
    vector<float> d(num_vertices(test));
    try {
        // call astar named parameter interface
        astar_search_tree(test, start, distance_heuristic<my_graph, float, vector<location>>(locations, goal),
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
        cout << "\nShortest path from " << start << " to " << goal << ": ";
        list<vertex_descriptor>::iterator spi = shortest_path.begin();
        cout << start;
        vector<vertex_descriptor> shortest_path1;
        for (++spi; spi != shortest_path.end(); ++spi)
        {
            cout << " -> " << *spi;
            shortest_path1.push_back(*spi);
        }
        shortest_path1.push_back(start);
        print_way(locations, shortest_path1);
        return 0;
    }
    cout << "Didn't find a path from " << start << "to" << goal << "!" << endl;
    return EXIT_SUCCESS;
}