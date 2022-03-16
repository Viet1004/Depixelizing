#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/myApp/graph.hpp"
#include "../include/myApp/image.hpp"
#include "../include/myApp/spline.hpp"
#include "simple-svg.hpp"

//#include "../include/myApp/voronoi.hpp"
//#include "../include/myApp/spline.hpp"

//using namespace std;
//using namespace cv;

#define draw(x, y) svg::Point(IMAGE_SCALE * x, IMAGE_SCALE * y)

unsigned IMAGE_SCALE = 10;

void drawPolygon(svg::Document &doc, std::vector<std::pair<float,float> > hull, const std::tuple<int,int,int>& c)
{
	svg::Polygon polygon(svg::Color(std::get<0>(c), std::get<1>(c), std::get<2>(c)));
	for (const auto& point : hull) polygon << draw(point.first, point.second);
	doc << polygon;
}

void drawImage(svg::Document &doc, Graph& graph, Image& image)
{
	//Draw Voronoi Diagrams
	
	for(int x = 0 ; x < graph.get_height(); x++){
	    for(int y = 0 ; y < graph.get_width(); y++)
	    {
		auto map = graph.get_node(x,y).edge_Voronoi;
        std::vector<FPoint> hull;
//        std::cout << "(x,y): " << x << " " << y << std::endl;
        for (int i = 0; i< 8; i++){
            if (map.find((Direction)i) != map.end()){
//                std::cout << "i is " << i << std::endl;
                hull.push_back(map[(Direction)i].first);
//                std::cout << map[(Direction)i].first.first << " " << map[(Direction)i].first.second << std::endl;
            }
        }
//        std::cout << std::get<0>(image.get_RGB(x,y)) << " " << std::get<1>(image.get_RGB(x,y)) << " " << std::get<2>(image.get_RGB(x,y)) << std::endl ;
        
		//Fill Polygon
		drawPolygon(doc, hull, image.get_RGB(x,y));
	    }
    }

}

int main(int argc, char** argv)
{
/*	if(argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " <<image_path>> <<output_path>>\n";
		return 1;
	}
	std::string output_path = std::string(argv[2]);
	*/
//   	Mat image;//taking an image matrix//
//   	image = imread("/home/viet1004/MAP586/MAP586/Projects/depixelize/test/snow.bmp");//loading an image//
//	imshow("test",image);
//	std::string input_path = std::string(argv[1]);
//	Image mat_image(input_path);
	Image mat_image("../tests/Images/2x2.bmp");
    Graph graph(mat_image);
//   	Node node = graph.get_node(1,1);
//	graph.simple_link();
    graph.planarize();
    graph.voronoi_formation();
    

/*    std::vector<Direction> neighbor11 = graph.get_neighbors(0,0);
    std::vector<Direction>::iterator d; 
    d = std::find(neighbor11.begin(), neighbor11.end(), RIGHT);
    if (d == neighbor11.end()){
        std::cout << neighbor11[0] << neighbor11[1] << std::endl;
    }
*/
	svg::Dimensions dimensions(IMAGE_SCALE * graph.get_width(), IMAGE_SCALE * graph.get_height());
	svg::Document doc("testingsnow1.svg", svg::Layout(dimensions, svg::Layout::TopLeft));
    
	std::vector<FPoint> matrix = {std::make_pair(0,0),std::make_pair(1,1),std::make_pair(0,2)};
	Color_RGB color = std::make_tuple(125,125,255);

	std::vector<std::vector<FPoint>> mainOutLines = graph.getMainOutline();
//	printSpline(doc, matrix, color);
//	drawImage(doc, graph, mat_image);
	
	std::vector<std::pair<FPoint, Color_RGB>> points = {std::make_pair(std::make_pair(0,0), std::make_tuple(225,125,255)),
														std::make_pair(std::make_pair(1,1), std::make_tuple(125,225,255)),
														std::make_pair(std::make_pair(0,2), std::make_tuple(125,0,0)),
														std::make_pair(std::make_pair(2,2), std::make_tuple(125,0,255))};
	drawImage_(doc, points);
	doc.save();

	return 0;
}