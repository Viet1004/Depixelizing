#include "../include/myApp/spline.hpp"

#define draw(x, y) svg::Point(IMAGE_SCALE * x, IMAGE_SCALE * y)

inline float evalBspline(float x1, float x2, float x3, float t) { return x1 + x2*t + x3*t*t; }

std::vector<std::vector<float>> getSpline(std::vector<FPoint > points) // For 3 points
{
	//assert(points.size() == 3);
	//Basis Matrix for quadratic uniform b-spline
	float B[3][3] = {{1, 1, 0},{ -2, 2, 0},{1, -2, 1}};

	// 3x2 vector matrix initialized with 0
	std::vector<std::vector<float>> a(3,std::vector<float>(2,0));

	// multiply it with B-splines basis matrix
	for(int i = 0 ; i < 3; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			for(int k = 0 ; k < 3; k ++) 
			{
				if(j == 0) a[i][j] += 0.5*B[i][k]*points[k].first;
				else a[i][j] += 0.5*B[i][k]*points[k].second;
			}
		}
	}
	return a;
}

void printSpline(svg::Document& doc, std::vector<FPoint> matrix_, Color_RGB color){
    const int IMAGE_SCALE = 10;
	//T is extroplated a little for intersecting pieces
	std::vector<std::vector<float>> matrix = getSpline(matrix_);
	// parameter t that will generate all the points
	float t = -0.1f;

	// increment t by steps
	float step = 0.01f;

	// store x,y after multiplying with 1, t and t^2
	float xcor, ycor;

	svg::Polyline poly_line(svg::Stroke(1, svg::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color))));
	for(;t<=1.1f; t+=step) poly_line << draw(evalBspline(matrix[0][0],matrix[1][0],matrix[2][0],t), evalBspline(matrix[0][1],matrix[1][1],matrix[2][1],t));
	doc << poly_line;

}

void drawImage_(svg::Document &doc, std::vector<std::vector<std::pair<FPoint, Color_RGB>>> outlines)
{
	//Draw Voronoi Diagrams
	for(auto points: outlines){
		for(int i = 0; i < points.size() - 2; i++)
			printSpline(doc, {points[i].first,points[(i+1)%points.size()].first,points[(i+2)%points.size()].first}, points[i+1].second);
	}
	

}

