#include "theta.h"
Theta::~Theta()
{
}

bool Theta::lineOfSight(int i1, int j1, int i2, int j2, const Map &map, bool cutcorners)
{
    int dx = i2 - i1;
    int dy = j2 - j1;
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


Node Theta::resetParent(Node cur, Node* parent, const Map &map, const EnvironmentOptions &options )
{
    if(parent == NULL)
            return cur;
    if (close.find(cur.i * map.getMapHeight() + cur.j) != close.end())
        return cur;
    if(parent->parent != NULL && lineOfSight(cur.i,cur.j,parent->parent->i,parent->parent->j,map,options.cutcorners)) {
        auto tmp = open.find(cur);
        double len = computeHFromCellToCell(cur.i, cur.j, parent->parent->i, parent->parent->j, options);
        if (tmp == open.end()) {
            cur.H = computeHFromCellToCell(cur.i, cur.i, map.getMapGoalI(), map.getMapGoalJ(), options);
            cur.g = parent->parent->g + len;
            cur.F = cur.g + cur.H;
            cur.parent = parent->parent;
            open.insert(cur);
        } else {
            double val;
            val = parent->parent->g + len;
            if (val <= (tmp)->g) {
                cur.H = tmp->H;
                cur.parent = parent->parent;
                cur.g = val;
                cur.F = val + cur.H;
                open.erase(tmp);
                open.insert(cur);
            }
        }
        return cur;
    } else {
        int dx = abs(cur.i - parent->i);
        int dy = abs(cur.j - parent->j);
        if (dx + dy > 2) {
            return cur;
        }
        auto tmp = open.find(cur);
        if (tmp == open.end()) {
            cur.H = computeHFromCellToCell(cur.i, cur.i, map.getMapGoalI(), map.getMapGoalJ(), options);
            if (dx && dy)
                cur.g = (*parent).g + sqrt(2);
            else
                cur.g = (*parent).g + 1;
            cur.F = cur.g + cur.H;
            cur.parent = parent;
            open.insert(cur);
        } else {
            double val;
            if (dx && dy)
                val = (*parent).g + sqrt(2);
            else
                val = (*parent).g + 1;
            if (val < (tmp)->g) {
                cur.H = tmp->H;
                cur.parent = parent;
                cur.g = val;
                cur.F = val + cur.H;
                open.erase(tmp);
                open.insert(cur);
            }
        }
    }
}

void Theta::makeSecondaryPath()
{
    //need to implement
}

void Theta::makePrimaryPath(Node curNode)
{
    //need to implement
}
