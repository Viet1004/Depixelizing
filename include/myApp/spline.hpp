#ifndef SPLINE_HPP
#define SPLINE_H

#include <vector>
#include <tuple>
#include <iostream>
#include "graph.hpp"
#include "../../src/simple-svg.hpp"

class Spline{
    std::vector<std::vector<FPoint>> mainOutlines;
    public:
        Spline(svg::Document, std::vector<std::vector<FPoint>>);
        void getSpline();
};

void printSpline(svg::Document& doc, std::vector<FPoint> matrix, Color_RGB color);
void drawImage_(svg::Document &doc, std::vector<std::vector<std::pair<FPoint, Color_RGB>>> points);

#endif