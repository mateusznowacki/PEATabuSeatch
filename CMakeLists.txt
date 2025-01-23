cmake_minimum_required(VERSION 3.29)
project(PEATabuSearch)

set(CMAKE_CXX_STANDARD 23)
MATH(EXPR stack_size "16 * 1024 * 1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,16777216")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEATabuSearch
        service/main.cpp
        utils/ConsolePrinter.cpp
        utils/FileController.cpp
        dto/ConfigDataDto.cpp
        dto/ResultDataDto.cpp
        dto/DataDto.cpp
        service/pea12/NearestNeighbour.cpp
        service/pea12/RandomSolver.cpp
        service/TabuSearch.cpp
        service/AntColonyOptimization.cpp
)
