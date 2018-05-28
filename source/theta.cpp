#include "theta.h"

Theta::~Theta() {
}

bool Theta::lineOfSight(int i1, int j1, int i2, int j2, const Map &map, std::list<Node> *passed) {
    int lasti = -1;
    int lastj = -1;
    int dx = i2 - i1;
    int dy = j2 - j1;
    int error = 0;
    int diry = 1;
    int dirx = 1;
    if (dy < 0) {
        dy = -dy;
        diry = -1;
    }
    if (dx < 0) {
        dx = -dx;
        dirx = -1;
    }
    if (dx >= dy) {
        int de = dy;
        int j = j1;
        lastj = j1;
        for (int i = i1; i != i2; i += dirx) {
            if (!map.CellOnGrid(i, j) || map.CellIsObstacle(i, j))
                return false;
            if (lasti != -1) {
                if (!checkSquize(lasti, lastj, i, j, map))
                    return false;
            }
            if (passed != nullptr) {
                passed->push_back(Node(i, j, true));
            }
            error = error + de;
            if (2 * error >= dx) {
                lastj = j;
                j = j + diry;
                error = error - dx;
            }
            lasti = i;
        }
        return true;
    } else {
        int de = dx;
        lasti = i1;
        int i = i1;
        for (int j = j1; j != j2; j += diry) {
            if (!map.CellOnGrid(i, j) || map.CellIsObstacle(i, j))
                return false;
            if (lastj != -1) {
                if (!checkSquize(lasti, lastj, i, j, map))
                    return false;
            }
            if (passed != nullptr) {
                passed->push_back(Node(i, j, true));
            }
            error = error + de;
            if (2 * error >= dy) {
                lasti = i;
                i = i + dirx;
                error = error - dy;
            }
            lastj = j;
        }
        return true;
    }
}


Node Theta::resetParent(Node cur, Node *parent, const Map &map, const EnvironmentOptions &options) {
    if (close.find(cur.i * map.getMapHeight() + cur.j) != close.end())
        return cur;
    if (parent->parent != nullptr && lineOfSight(cur.i, cur.j, parent->parent->i, parent->parent->j, map,
                                                 nullptr)) {
        auto tmp = open.find(cur);
        double dx = (cur.i - parent->parent->i);
        double dy = (cur.j - parent->parent->j);
        double len = sqrt(dx * dx + dy * dy);
        if (tmp == open.end()) {
            cur.H = computeHFromCellToCell(cur.i, cur.j, map.getMapGoalI(), map.getMapGoalJ(), options);
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
    } else {
        int dx = abs(cur.i - parent->i);
        int dy = abs(cur.j - parent->j);
        auto tmp = open.find(cur);
        if (tmp == open.end()) {
            cur.H = computeHFromCellToCell(cur.i, cur.j, map.getMapGoalI(), map.getMapGoalJ(), options);
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
    return cur;
}

void Theta::makePathes(Node curNode, const Map &map) {
    Node *cur = &curNode;
    while (cur->parent != nullptr) {
        std::list<Node> passed;
        lineOfSight(cur->i, cur->j, cur->parent->i, cur->parent->j, map, &passed);
        lppath.insert(lppath.end(), passed.begin(), passed.end());
        hppath.push_back(*cur);
        cur = (cur->parent);
    }
    hppath.push_back(*cur);
    hppath.reverse();
    lppath.push_back(*cur);
    lppath.reverse();
    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
}
