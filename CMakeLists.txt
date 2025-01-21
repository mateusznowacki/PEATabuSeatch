cmake_minimum_required(VERSION 3.29)
project(PEATabuSearch)

set(CMAKE_CXX_STANDARD 23)
MATH(EXPR stack_size "16 * 1024 * 1024")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,16777216")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEATabuSearch main.cpp
        utils/ConsolePrinter.cpp
        utils/FileController.cpp
        dto/DataDto.cpp
        dto/DataDto.h
        dto/ConfigDataDto.cpp
        dto/ConfigDataDto.h
        service/Graph.cpp
        dto/DataDto.cpp
        dto/DataDto.h
        utils/ConsolePrinter.cpp
        dto/ResultDataDto.cpp
        dto/ResultDataDto.h
        service/TabuSearch.cpp
        service/TabuSearch.h
        service/RandomSolver.h
           service/TabuSearch.cpp
        service/TabuSearch.cpp
        service/RandomSolver.cpp
)
