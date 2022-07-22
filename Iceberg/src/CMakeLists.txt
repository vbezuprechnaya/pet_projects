cmake_minimum_required(VERSION 3.10)
project(ice)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES main.cpp model.cpp model.h plane.cpp plane.h state.cpp state.h visual.cpp visual.h Calculator.cpp
        MyMatrix.cpp MyMatrix.h face.h MyVector.h MyVector.cpp)

add_executable(ice  ${PROJECT_SOURCES})

target_link_libraries(ice  -lglut -lGLU -lGL)
