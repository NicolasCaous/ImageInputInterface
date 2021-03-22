rm CMakeLists.txt
printf "cmake_minimum_required(VERSION 2.8)
project( ImageInputInterface )
find_package( OpenCV REQUIRED PATHS /home/nicolas/Documents/opencv/compiled_static NO_DEFAULT_PATH)
add_executable( ImageInputInterface src/DEBUG.cpp )
target_link_libraries( ImageInputInterface \${OpenCV_LIBS} )" > CMakeLists.txt

rm -rf build
mkdir build
cd build
cmake ..
make
