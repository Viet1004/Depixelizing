cmake_minimum_required(VERSION 3.16.3)
project(DEPIXELIZER)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( ${PROJECT_NAME} ./src/main.cpp ./src/image.cpp ./src/graph.cpp ./src/spline.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )


enable_testing()

add_executable(tester ./tests/tester.cpp ./src/image.cpp ./src/graph.cpp)
add_test(Tester tester)
target_link_libraries( tester ${OpenCV_LIBS} )
