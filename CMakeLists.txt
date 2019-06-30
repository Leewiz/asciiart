cmake_minimum_required(VERSION 3.1)
# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "-lncurses")
SET(OpenCV_DIR /usr/local/Cellar/opencv/4.1.0_2/lib/cmake/opencv4)

project( asciiart )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( asciiart main.cpp )
target_link_libraries( asciiart ${OpenCV_LIBS} )