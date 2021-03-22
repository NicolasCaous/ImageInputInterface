printf "cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project( ImageInputInterface )
find_package( OpenCV REQUIRED PATHS /home/nicolas/Documents/opencv/compiled_static NO_DEFAULT_PATH)

MESSAGE(\${OpenCV_LIBS})

add_library( ImageInputInterface SHARED src/UnityInterface.cpp)
target_link_libraries( ImageInputInterface \${OpenCV_LIBS} )" > CMakeLists.txt

rm -rf build
mkdir build
cd build
cmake ..
make

# cp libImageInputInterface.so /home/nicolas/Unity-2018.1.0b8/Projects/NicolasCaous-pongfake-622c4cd378cb/Assets/Plugins/
