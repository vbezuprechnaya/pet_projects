cmake_minimum_required(VERSION 3.22)
project(SEIR)

set(CMAKE_CXX_STANDARD 17)

add_executable(SEIR main.cpp Model.cpp Model.h State.cpp State.h Optimizer.cpp Optimizer.h Matrix.cpp Matrix.h)
