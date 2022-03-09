#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/myApp/graph.hpp"
#include "../include/myApp/image.hpp"

int main(){
    Image mat_image("../tests/Images/2x2.bmp");
    Graph graph(mat_image);
    graph.simple_link();

    if(graph.get_valence(0,0) != 2 || graph.get_valence(0,1) != 2 || graph.get_valence(1,0) != 2 || graph.get_valence(1,1) != 0){
        return 1;
    }
    std::vector<Direction> neighbor11 = graph.get_neighbors(0,0);
    std::vector<Direction> neighbor21 = graph.get_neighbors(1,0);
    
    std::vector<Direction>::iterator d,d1,d2; 
    d = std::find(neighbor21.begin(), neighbor21.end(), TOP);
    d1 = std::find(neighbor21.begin(), neighbor21.end(), TOP_RIGHT);
    d2 = std::find(neighbor21.begin(), neighbor21.end(), RIGHT);
    if (d == neighbor21.end() || d1 == neighbor21.end() || d2 != neighbor21.end()){
        return 1;
    }
    d = std::find(neighbor11.begin(), neighbor11.end(), BOTTOM);
    d1 = std::find(neighbor11.begin(), neighbor11.end(), TOP_RIGHT);
    d2 = std::find(neighbor11.begin(), neighbor11.end(), RIGHT);
    if (d == neighbor11.end() || d1 != neighbor11.end() || d2 == neighbor11.end()){
        return 1;
    }

    return 0;
}