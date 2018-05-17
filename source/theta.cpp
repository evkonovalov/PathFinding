#include "theta.h"
Theta::~Theta()
{
}

bool Theta::lineOfSight(int i1, int j1, int i2, int j2, const Map &map, bool cutcorners)
{
    int dx = i1 - i2;
    int dy = j1 - j2;
    int error = 0;
    int diry = 1;
    int dirx = 1;
    if(dy < 0){
        dy = -dy;
        diry = -1;
    }
    if(dx < 0){
        dx = -dx;
        dirx = -1;
    }
    if(dx >= dy) {
        int de = dy;
        int j = j1;
        for(int i = i1; i <= i2; i+= dirx) {
            if (!map.CellOnGrid(i, j) || map.CellIsObstacle(i, j))
                return false;
            error = error + de;
            if (2 * error >= dx) {
                j = j + diry;
                error = error - dx;
            }
        }
        return true;
    } else {
        int de = dx;
        int i = i1;
        for(int j = j1; j <= j2; j+= diry) {
            if (!map.CellOnGrid(i, j) || map.CellIsObstacle(i, j))
                return false;
            error = error + de;
            if (2 * error >= dy) {
                i = i + dirx;
                error = error - dy;
            }
        }
        return true;
    }
}


Node Theta::resetParent(Node current, Node* parent, const Map &map, const EnvironmentOptions &options )
{
    //need to implement
    return current;
}

void Theta::makeSecondaryPath()
{
    //need to implement
}

void Theta::makePrimaryPath(Node curNode)
{
    //need to implement
}
