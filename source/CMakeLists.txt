cmake_minimum_required(VERSION 2.8)
project(ASearch)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Wextra")
set(SOURCE_FILES
        config.h
        environmentoptions.cpp
        ilogger.h
        jp_search.cpp
        mission.cpp
        searchresult.h
        xmllogger.cpp
        astar.cpp
        dijkstra.cpp
        environmentoptions.h
        isearch.cpp
        jp_search.h
        map.cpp
        mission.h
        theta.cpp
        xmllogger.h
        astar.h
        anya.cpp
        anya.h
        config.cpp
        dijkstra.h
        gl_const.h
        isearch.h
        map.h
        node.h
        theta.h
        tinyxml2.cpp
        tinyxml2.h
        main.cpp
        )
add_executable(ASearch ${SOURCE_FILES})