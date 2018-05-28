#ifndef MAP_H
#define    MAP_H

#include <iostream>
#include "gl_const.h"
#include <sstream>
#include <string>
#include <algorithm>
#include "tinyxml2.h"

class Map {
    private:
        int height, width;
        int start_i, start_j;
        int goal_i, goal_j;
        double cellSize;
        int **Grid;

    public:
        Map();

        Map(const Map &orig);

        ~Map();

        bool getMap(const char *FileName);

        bool CellIsTraversable(int i, int j) const;

        bool CellOnGrid(int i, int j) const;

        bool CellIsObstacle(int i, int j) const;

        int getValue(int i, int j) const;

        int getMapHeight() const;

        int getMapWidth() const;

        double getCellSize() const;

        int getMapStartJ() const;

        int getMapStartI() const;

        int getMapGoalI() const;

        int getMapGoalJ() const;
};

#endif

