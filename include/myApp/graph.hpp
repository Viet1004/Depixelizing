#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <stack>
#include <map>
#include <memory>
#include "image.hpp"

using FPoint = std::pair<float,float>;
using fEdge = std::pair<FPoint, FPoint>;
using IntPoint = std::pair<size_t,size_t>;

enum Direction {
    TOP_LEFT = 0,
    TOP = 1,
    TOP_RIGHT = 2,
    RIGHT = 3,    
    BOTTOM_RIGHT = 4,
    BOTTOM = 5,
    BOTTOM_LEFT = 6,
    LEFT = 7
};

const int direction[8][2] =
{
	{-1,-1},
	{-1,0},
	{-1,1},
	{0,1},
	{1,1},
	{1,0},
	{1,-1},
	{0,-1}
};



struct Node{
//    IntPoint position;
    std::vector<Direction> neighbors;
//    std::tuple<double, double double> YUV;
    signed int valence = 0;
    double weight = 0;
    std::map<Direction, std::pair<FPoint, FPoint>> edge_Voronoi; 
/*    public:
        Node(IntPoint position);
        IntPoint get_position();
        double get_weight(Direction);
        unsigned int get_valence();
        std::vector<Direction> get_neighbors();
//        Node& operator=(Node& node);
//        Node& operator=(Node&);
        void add_neighbor(Direction dir);
//        bool edge_exist(Direction dir);        
*/
};

class Graph{
    Image* image;
    std::vector<std::vector<Node>> graph;
    size_t height;
    size_t width;
    std::stack<IntPoint> untreated_node;
    bool edge_exist(size_t i, size_t j, Direction dir);
    std::tuple<double, double, double> get_color(size_t i, size_t j);
    void connect_block(size_t i, size_t j);
    void add_neighbor(size_t i, size_t j, Direction dir);
    int curves_heuristic(size_t i, size_t j);
	int sparse_pixels_heuristic(size_t i, size_t j);
	int islands_heuristic(size_t i, size_t j);
    void add_voronoi_edge(size_t i, size_t j);
//    void set_weight(size_t i, size_t j);
    public:
        Graph();
        Node get_node(size_t i, size_t j);
        size_t get_height(){
            return height;
        }
        size_t get_width(){
            return width;
        }
        unsigned int get_valence(size_t i,size_t j);
        std::vector<Direction> get_neighbors(size_t i,size_t j);
        Graph(Image& image);
//        ~Graph();
        void simple_link();

        void planarize();
        void voronoi_formation();
};

/*
class Curve{
    std::vector<IntPoint> curve;
    std::pair<IntPoint, Direction> firstPoint;
    std::pair<IntPoint, Direction> lastPoint;
    public:
        Curve();
        
}
*/
#endif
