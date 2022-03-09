#include "../include/myApp/graph.hpp"

Graph::Graph(){
    image = nullptr;
//    untreated_node.clear();
    graph.clear();
}


Graph::Graph(Image& IMage){
    this->image = &IMage;
    height = image->get_height();
    width = image->get_width();
//    std::cout << "Hello: " << graph.size() << std::endl; 
    for(size_t i = 0; i < height; i++){
        std::vector<Node> temp;
        for(size_t j = 0; j < width; j++){
            temp.push_back(Node()); 
        }
        graph.push_back(temp);
    }
}

Node Graph::get_node(size_t i, size_t j){
    return graph[i][j];
}

bool Graph::edge_exist(size_t i, size_t j, Direction dir){
    
    size_t first = i + direction[dir][0];
    size_t second = j + direction[dir][1];
    std::tuple<double, double, double> origin = image->get_RGB(i,j);
//    std::cout << dir << std::endl;
//    std::cout << first << "    " << second << std::endl;
    std::tuple<double, double, double> dest = image->get_RGB(first,second);
    
    return (abs(std::get<0>(origin) - std::get<0>(dest)) < 48. \
        && abs(std::get<1>(origin) - std::get<1>(dest)) < 7. \
        && abs(std::get<2>(origin) - std::get<2>(dest)) < 6. );
        
}

std::tuple<double, double, double> Graph::get_color(size_t i, size_t j){
    return image->get_RGB(i,j);        
}

/*
Graph::~Graph(){
    delete image;
    graph.clear();
    untreated_node.clear();
}
*/

void Graph::add_neighbor(size_t i, size_t j, Direction dir){
    if(std::find(graph[i][j].neighbors.begin(), graph[i][j].neighbors.end(), dir ) == graph[i][j].neighbors.end()){
        graph[i][j].neighbors.push_back(dir);
        graph[i][j].valence += 1;
    }
}

void Graph::connect_block(size_t i, size_t j){
    add_neighbor(i, j, RIGHT);
    add_neighbor(i, j, BOTTOM);
    add_neighbor(i ,j+1 ,LEFT);
    add_neighbor(i ,j+1 ,BOTTOM);
    add_neighbor(i+1, j+1, TOP);
    add_neighbor(i+1 ,j+1 ,LEFT);
    add_neighbor(i+1, j, TOP);
    add_neighbor(i+1, j, RIGHT);        
}

unsigned int Graph::get_valence(size_t i, size_t j){
    return graph[i][j].valence;
}

std::vector<Direction> Graph::get_neighbors(size_t i,size_t j){
    return graph[i][j].neighbors;
}


void Graph::simple_link(){
    // This method is for creating an graph by the difference between each pixel.
    // For each pixel (i,j) we first compare if the (2 * 2) block is fully connected. If so, the block is connected in circle graph
    // Otherwise, it connected by their similarity
    for(size_t i = 0; i < height - 1; i++){
        for(size_t j = 0; j < width -1; j++){
            // Determine a block 2*2 by all of the node inside and the edges.
            bool cross_1 = edge_exist(i ,j ,BOTTOM_RIGHT);
            bool cross_2 = edge_exist(i ,j+1 ,BOTTOM_LEFT);
            bool up = edge_exist(i, j, RIGHT);
            bool right = edge_exist(i, j+1, BOTTOM);
            bool down = edge_exist(i+1, j+1, LEFT);
            bool left = edge_exist(i+1, j, TOP);
//            std::cout << "Inside simple_link" << cross_1 << cross_2 << up << right << down << left << std::endl;
            if (cross_1 && cross_2){
                // If there exists two crosses
                if (up || right || down || left){
                    connect_block(i,j);
                }
                else{
                    std::cout << "untreated_node: " << i << j << std::endl;
                    untreated_node.push(std::make_pair(i,j));
                }             
            }
        
            if (cross_1 && !cross_2){
                // If cross_1 (top_left to bottom_right) exists but not cross_2 (top_right to bottom_left)
                if ((up && down) || (left && right) || (up && left) || (down && right)){
                    connect_block(i,j);
                }else{
                    add_neighbor(i,j,BOTTOM_RIGHT);
                    add_neighbor(i+1,j+1,TOP_LEFT);
                    if(up){
                        add_neighbor(i,j,RIGHT);
                        add_neighbor(i,j+1,LEFT);
                        add_neighbor(i,j+1, BOTTOM);
                        add_neighbor(i+1,j+1, TOP);
                        continue;
                    }
                    if(down){
                        add_neighbor(i+1,j,RIGHT);
                        add_neighbor(i+1,j+1,LEFT);
                        add_neighbor(i+1,j,TOP);
                        add_neighbor(i,j,BOTTOM);
                        continue;
                    }
                    if(left){
                        add_neighbor(i+1,j,TOP);
                        add_neighbor(i,j,BOTTOM);
                        add_neighbor(i+1,j, RIGHT);
                        add_neighbor(i+1,j+1, LEFT);
                        continue;
                    }
                    if(right){
                        add_neighbor(i,j+1,BOTTOM);
                        add_neighbor(i+1,j+1,TOP);
                        add_neighbor(i,j, RIGHT);
                        add_neighbor(i,j+1, LEFT);
                        continue;
                    }
                }
            }
        
            if (!cross_1 && cross_2){
                // inverse of the previous case
                if ((up && down) || (left && right) || (up && right) || (down && left)){
                    connect_block(i,j);
                }else{
                    add_neighbor(i,j+1,BOTTOM_LEFT);
                    add_neighbor(i+1,j,TOP_RIGHT);
                    if(up){
                        add_neighbor(i,j,RIGHT);
                        add_neighbor(i,j+1,LEFT);
                        add_neighbor(i,j,BOTTOM);
                        add_neighbor(i+1,j, TOP);
                        continue;
                    }
                    if(down){
                        add_neighbor(i+1,j,RIGHT);
                        add_neighbor(i+1,j+1,LEFT);
                        add_neighbor(i,j+1,BOTTOM);
                        add_neighbor(i+1,j+1, TOP);
                        continue;
                    }
                    if(left){
                        add_neighbor(i+1,j,TOP);
                        add_neighbor(i,j,BOTTOM);
                        add_neighbor(i,j,RIGHT);
                        add_neighbor(i,j+1,LEFT);
                        continue;
                    }
                    if(right){
                        add_neighbor(i,j+1,BOTTOM);
                        add_neighbor(i+1,j+1,TOP);
                        add_neighbor(i+1,j, RIGHT);
                        add_neighbor(i+1,j+1, LEFT);
                        continue;
                    }
                }
            }
        
            if(!cross_1 && !cross_2){
                // if none of the cross existes
                if(up && down && left && right){
                    connect_block(i,j);
                }else{
                    if(up){
                        add_neighbor(i,j,RIGHT);
                        add_neighbor(i,j+1,LEFT);
                    }
                    if(down){
                        add_neighbor(i+1,j,RIGHT);
                        add_neighbor(i+1,j+1,LEFT);
                    }
                    if(left){
                        add_neighbor(i+1,j,TOP);
                        add_neighbor(i,j,BOTTOM);
                    }
                    if(right){
                        add_neighbor(i,j+1,BOTTOM);
                        add_neighbor(i+1,j+1,TOP);
                    }    
                }
            }
        
        }
    }
}

bool insideBounds(size_t x, size_t y, size_t row_st, size_t row_end, size_t col_st, size_t col_end)
{
	return (x>=row_st && x <= row_end && y >= col_st && y <= col_end);
}

int Graph::curves_heuristic(size_t i, size_t j){
    // This function implement the curves_heuristic algo in the paper
    // for investigate each cross (i,j) -> (i+i,j+1) = (i,j) + BOTTOM_RIGHT
    //                            (i,j+1) -> (i+1,j) = (i,j+1) + BOTTOM_LEFT
    int weight_cross_1 = 0;
    int weight_cross_2 = 0;
    Direction dir;
    size_t next_x;
    size_t next_y;

    if (get_valence(i,j) == 1){
        next_x = i;
        next_y = j;
        dir = get_neighbors(i,j)[0];
        while(true){ 
            std::cout << "Inside curves_heuristic 00"  << std::endl;
  
            next_x += direction[dir][0];
            next_y += direction[dir][1];
            if (get_valence(next_x, next_y) != 2) break;
            weight_cross_1++;
            if(dir == (get_neighbors(next_x, next_y)[0]+4)%8){
                dir = get_neighbors(next_x, next_y)[1];
            }
            else{
                dir = get_neighbors(next_x, next_y)[0];
            }
        }    
    }

    if (get_valence(i+1,j+1) == 1){
        next_x = i+1;
        next_y = j+1;
        dir = get_neighbors(i+1,j+1)[0];
        while(true){  
            std::cout << "Inside curves_heuristic 11" << std::endl;
   
            next_x += direction[dir][0];
            next_y += direction[dir][1];
            if (get_valence(next_x, next_y) != 2) break;
            weight_cross_1++;
            if(dir == (get_neighbors(next_x, next_y)[0]+4)%8){
                dir = get_neighbors(next_x, next_y)[1];
            }
            else{
                dir = get_neighbors(next_x, next_y)[0];
            }
        }    
    }
    if (get_valence(i,j+1) == 1){
        next_x = i;
        next_y = j+1;
        dir = get_neighbors(i,j+1)[0];
        while(true){   
            std::cout << "Inside curves_heuristic 01"  << std::endl;
            std::cout << next_x << next_y << std::endl;
            std::cout << i << j << std::endl;
//            std::cout << "Inside curve_heuristic" << std::endl;
            next_x += direction[dir][0];
            next_y += direction[dir][1];
            if (get_valence(next_x, next_y) != 2) break;
            weight_cross_2++;
            if(dir == (get_neighbors(next_x, next_y)[0]+4)%8){
                dir = get_neighbors(next_x, next_y)[1];
            }
            else{
                dir = get_neighbors(next_x, next_y)[0];
            }
        }    
    }

    if (get_valence(i+1,j) == 1){
        next_x = i+1;
        next_y = j;
        dir = get_neighbors(i+1,j)[0];
        while(true){   
            std::cout << "Inside curves_heuristic 10"  << std::endl;
            
            next_x += direction[dir][0];
            next_y += direction[dir][1];
            if (get_valence(next_x, next_y) != 2) break;
            weight_cross_2++;
            if(dir == (get_neighbors(next_x, next_y)[0]+4)%8){
                dir = get_neighbors(next_x, next_y)[1];
            }
            else{
                dir = get_neighbors(next_x, next_y)[0];
            }
        }    
    }
    std::cout << "curves_heuristic score: " << weight_cross_1 - weight_cross_2 << std::endl;
    return weight_cross_1 - weight_cross_2;
}

int Graph::sparse_pixels_heuristic(size_t i, size_t j){
    int labels[8][8] = {0};
    std::stack<std::pair<size_t,size_t>> st;
//    std::cout << i << j << std::endl;
    st.push(std::make_pair(i,j));
    st.push(std::make_pair(i+1,j+1));
    labels[3][3] = 1;
    labels[4][4] = 1;
    while(!st.empty()){
        std::cout << "Inside sparse_pixels_heuristic " << st.size() << std::endl;
        std::pair<size_t,size_t> point = st.top();
        size_t p = point.first;
        size_t q = point.second;
        size_t x = p;
        size_t y = q;        
        st.pop();
        std::vector<Direction> dirs = graph[p][q].neighbors;
        for (Direction dir: dirs){
            x = p + direction[dir][0];
            y = q + direction[dir][1];
            if (!insideBounds(x,y,0,height-1,0,width-1)) continue;
            if (!insideBounds(x+3,y+3,i,i+7,j,j+7)) continue;            
            if (labels[3+x-i][3+y-j] == 0){
                st.push(std::make_pair(x,y));
  
                labels[3+x-i][3+y-j] = 1;
            }else continue;
            
        } 
    }
    int labels_2[8][8] = {0};

    st.push(std::make_pair(i+1,j));
    st.push(std::make_pair(i,j+1));
    labels_2[3][4] = 1;
    labels_2[4][3] = 1;
    while(!st.empty()){
        std::cout << "Inside sparse_pixels_heuristic 2" << st.size() << std::endl;

        std::pair<size_t,size_t> point = st.top();
        size_t p = point.first;
        size_t q = point.second;
        size_t x = p;
        size_t y = q;        
        st.pop();
        std::vector<Direction> dirs = graph[p][q].neighbors;
        for (Direction dir: dirs){
            x = p + direction[dir][0];
            y = q + direction[dir][1];
            if (!insideBounds(x,y,0,height-1,0,width-1)) {
                continue;
            }
            if (!insideBounds(x+3,y+3,i,i+7,j,j+7)){
            continue;      
            }             
            if (labels_2[3+x-i][3+y-j] == 0){
                std::cout << x << y << std::endl ;
                st.push(std::make_pair(x,y));

                labels_2[3+x-i][3+y-j] = 1;
            }else continue;
            
        } 
    }
    int componentA = 0, componentB = 0;
	for(int h = 0 ; h < 8 ; h++) for(int k = 0 ; k < 8; k++)
	{
		if(labels[h][k] == 1) componentA++;
		if(labels_2[h][k] == 1) componentB++;
	}
    std::cout << "sparse_pixels_heuristic score: " << componentB << componentA << std::endl;
    return componentB - componentA;
}
int Graph::islands_heuristic(size_t i, size_t j){
    int weight1 = 0;
    int weight2 = 0;
    if (get_valence(i,j) == 0 || get_valence(i+1,j+1) == 0){
        
        weight1 = 5;
    }
    if (get_valence(i+1,j) == 0 || get_valence(i,j+1) == 0){
        weight2 = 5;
    }
    std::cout << "islands_heuristic score: " << weight1 - weight2 << std::endl;
    return weight1 - weight2;
}
    
void Graph::planarize(){
    simple_link();
    while(!untreated_node.empty()){
        IntPoint point = untreated_node.top();
        size_t x = point.first;
        size_t y = point.second;
        untreated_node.pop();
        int indicator = curves_heuristic(x,y) + sparse_pixels_heuristic(x,y) + islands_heuristic(x,y);
        if(indicator > 0){
            add_neighbor(x,y,BOTTOM_RIGHT);
            add_neighbor(x+1,y+1, TOP_LEFT);
        }
        if(indicator < 0){
            add_neighbor(x,y+1, BOTTOM_LEFT);
            add_neighbor(x+1,y, TOP_RIGHT);
        }
    }
}

bool is_in(Direction dir, std::vector<Direction> dirs){
    std::vector<Direction>::iterator d;
    d = std::find(dirs.begin(), dirs.end(), dir);
    if (d == dirs.end()) return false;
    return true;
}

void Graph::add_voronoi_edge(size_t i, size_t j){
    float fi = (float)i;
    float fj = (float)j;
    FPoint str_point;
    FPoint end_point;
    // Top left corner
    if (!insideBounds(i,j, 1, height, 0, width - 1) || !insideBounds(i,j, 0, height - 1, 1, width ) ){
        str_point = std::make_pair(fi, fj);
        end_point = std::make_pair(fi, fj);
        
    }else{
        if(std::find(graph[i][j].neighbors.begin(), graph[i][j].neighbors.end(), TOP_LEFT) != graph[i][j].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP_LEFT, std::make_pair(std::make_pair(fi+0.25,fj-0.25),std::make_pair(fi-0.25,fj+0.25))));    
            str_point = std::make_pair(fi-0.25,fj+0.25);
            end_point = std::make_pair(fi+0.25,fj-0.25);
        }
        else if(std::find(graph[i-1][j].neighbors.begin(), graph[i-1][j].neighbors.end(), BOTTOM_LEFT) != graph[i-1][j].neighbors.end()){
            str_point = std::make_pair(fi+0.25,fj+0.25);
            end_point = std::make_pair(fi+0.25,fj+0.25);
        }
        else{
            str_point = std::make_pair(fi,fj);
            end_point = std::make_pair(fi,fj);
        }
    }
    // Top right corner
    if (!insideBounds(i,j, 1, height , 0, width - 1) || !insideBounds(i,j+1, 0, height - 1, 0, width - 1) ){
        graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP, std::make_pair(str_point,std::make_pair(fi, fj+1.))));    
        str_point = std::make_pair(fi, fj+1.);
    }else{
        if(std::find(graph[i][j].neighbors.begin(), graph[i][j].neighbors.end(), TOP_RIGHT) != graph[i][j].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP, std::make_pair(str_point,std::make_pair(fi-0.25, fj+0.75))));    
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP_RIGHT, std::make_pair(std::make_pair(fi-0.25,fj+0.75),std::make_pair(fi+0.25,fj+1.25))));    
            str_point = std::make_pair(fi+0.25,fj+1.25);
        }
        else if(std::find(graph[i-1][j].neighbors.begin(), graph[i-1][j].neighbors.end(), BOTTOM_RIGHT) != graph[i-1][j].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP, std::make_pair(str_point,std::make_pair(fi+0.25, fj+0.75))));    
            str_point = std::make_pair(fi+0.25,fj+0.75);
        }
        else{
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(TOP, std::make_pair(str_point,std::make_pair(fi, fj+1.))));    
            str_point = std::make_pair(fi,fj+1.);
        }
    }
    // Bottom right corner
    if (!insideBounds(i+1,j, 0, height - 1, 0, width - 1) || !insideBounds(i,j+1, 0, height - 1, 0, width - 1) ){
        graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(str_point,std::make_pair(fi+1., fj+1.))));    
        str_point = std::make_pair(fi+1., fj+1.);
    }else{
        if(std::find(graph[i][j].neighbors.begin(), graph[i][j].neighbors.end(), BOTTOM_RIGHT) != graph[i][j].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(str_point,std::make_pair(fi+0.75, fj+1.25))));    
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM_RIGHT, std::make_pair(std::make_pair(fi+0.75,fj+1.25),std::make_pair(fi+1.25,fj+0.75))));    
            str_point = std::make_pair(fi+1.25,fj+0.75);
        }
        else if(std::find(graph[i][j+1].neighbors.begin(), graph[i][j+1].neighbors.end(), BOTTOM_LEFT) != graph[i][j+1].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(str_point,std::make_pair(fi+0.75, fj+0.75))));    
            str_point = std::make_pair(fi+0.75,fj+0.75);
        }
        else{
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(str_point,std::make_pair(fi+1., fj+1.))));    
            str_point = std::make_pair(fi+1.,fj+1.);
        }
    }
    // Bottom left corner
    if (!insideBounds(i+1,j, 0, height - 1, 0, width - 1) || !insideBounds(i,j, 0, height - 1, 1, width )){
        graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(str_point,std::make_pair(fi, fj+1.))));    
        str_point = std::make_pair(fi+1., fj);
    }else{
        if(std::find(graph[i][j].neighbors.begin(), graph[i][j].neighbors.end(), BOTTOM_LEFT) != graph[i][j].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(str_point,std::make_pair(fi+1.25, fj+0.25))));    
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM_LEFT, std::make_pair(std::make_pair(fi+1.25,fj+0.25),std::make_pair(fi+0.75,fj-0.25))));    
            str_point = std::make_pair(fi+0.75,fj-0.25);
        }
        else if(std::find(graph[i][j-1].neighbors.begin(), graph[i][j-1].neighbors.end(), BOTTOM_RIGHT) != graph[i][j-1].neighbors.end()){
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(str_point,std::make_pair(fi+0.75, fj+0.25))));    
            str_point = std::make_pair(fi+0.75, fj+0.25);
        }
        else{
            graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(str_point,std::make_pair(fi, fj+1.))));    
            str_point = std::make_pair(fi+1.,fj);
        }
    }
    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(LEFT, std::make_pair(str_point,end_point)));    

}

void Graph::voronoi_formation(){
    float fheight = (float)height;
    float fwidth = (float)width;
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            add_voronoi_edge(i,j);
/*            if ( i==0 && j == 0){
                if (is_in(BOTTOM_LEFT, graph[i][j].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(0.75, 1.25))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM_RIGHT, std::make_pair(std::make_pair(0.75, 1.25),std::make_pair(1.25, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1.25, 0.75),std::make_pair(1., 0.))));            
                }
                else if(is_in(BOTTOM_RIGHT, graph[i][j+1].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(0.75, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(0.75, 0.75),std::make_pair(1., 0.))));            
                }
                else{
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(1., 1.))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1., 1.),std::make_pair(1., 0.))));            
                }
            }
            if ( i==0 && j == width-1){
                if (is_in(BOTTOM_RIGHT, graph[i][j].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(LEFT, std::make_pair(std::make_pair(0., fwidth - 1.),std::make_pair(0.75, 1.25))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM_RIGHT, std::make_pair(std::make_pair(0.75, 1.25),std::make_pair(1.25, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1.25, 0.75),std::make_pair(1., 0.))));            
                }
                else if(is_in(BOTTOM_RIGHT, graph[i][j+1].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(0.75, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(0.75, 0.75),std::make_pair(1., 0.))));            
                }
                else{
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(1., 1.))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1., 1.),std::make_pair(1., 0.))));            
                }
            }
            if ( i== height - 1 && j == width-1){
                if (is_in(TOP_LEFT, graph[i][j].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(LEFT, std::make_pair(std::make_pair(0., 1.),std::make_pair(0.75, 1.25))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM_RIGHT, std::make_pair(std::make_pair(0.75, 1.25),std::make_pair(1.25, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1.25, 0.75),std::make_pair(1., 0.))));            
                }
                else if(is_in(BOTTOM_RIGHT, graph[i][j+1].neighbors)){
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(0.75, 0.75))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(0.75, 0.75),std::make_pair(1., 0.))));            
                }
                else{
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(RIGHT, std::make_pair(std::make_pair(0., 1.),std::make_pair(1., 1.))));
                    graph[i][j].edge_Voronoi.insert(std::pair<Direction, fEdge>(BOTTOM, std::make_pair(std::make_pair(1., 1.),std::make_pair(1., 0.))));            
                }
            }
*/            
        }
    }
}
/*
Node::Node(IntPoint position)
    : position(position)
    , valence(0)
    {neighbors.clear();
    }


IntPoint Node::get_position(){
    return position;
}

void Node::add_neighbor(Direction dir){
    for(Direction direction: neighbors){
        if ( dir == direction ){
            std::cout << "inside if " << std::endl;
            return;
        } 
    }
    valence += 1;
    std::cout << "Inside add_neighbor " << valence << std::endl;
    neighbors.push_back(dir);
}

unsigned int Node::get_valence(){
    return valence;
}

std::vector<Direction> Node::get_neighbors(){
    std::vector<Direction> res(neighbors.size());
    for (size_t i = 0; i < neighbors.size(); i++){
        res[i] = neighbors[i];
    }
    return res;
}
*/
/*
Node& Node::operator=(Node& node){
    valence = node.get_valence();
    position = node.get_position();
    std::vector<Direction> temp = node.get_neighbors();
    neighbors.clear();
    for (size_t i = 0; i < valence; i++){
        neighbors.push_back(temp[i]);
    }
}
*/