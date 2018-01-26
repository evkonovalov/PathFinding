#include "astar.h"
#include <stack>
#include <vector>

Astar::Astar(double HW, bool BT) {
    hweight = HW;
    breakingties = BT;
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options) {
    int metrictype = options.metrictype;
    int dx = abs(i1 - i2);
    int dy = abs(j1 - j2);
    if (metrictype == CN_SP_MT_DIAG) {
        if (dx > dy) {
            return dy + sqrt(2)*(dx - dy);
        } else {
            return dx + sqrt(2)*(dy - dx);
        }
    } else if (metrictype == CN_SP_MT_MANH) {
        return dx + dy;
    } else if (metrictype == CN_SP_MT_EUCL) {
        return sqrt(dx*dx + dy*dy);
    } else {
        if (dx > dy)
            return dx;
        else
            return dy;
    }
}

void Astar::search(const EnvironmentOptions &options) {
    sresult.time = currTimeInMillSeconds();
    sresult.lppath = NULL;
    sresult.hppath = NULL;
    sresult.nodescreated = 0;
    sresult.numberofsteps = 0;
    sresult.pathfound = false;
    sresult.pathlength = 0;
    int i1 = map.getMapStartI();
    int i2 = map.getMapGoalI();
    int j1 = map.getMapStartJ();
    int j2 = map.getMapGoalJ();
    int height = map.getMapHeight();
    int width = map.getMapWidth();
    std::vector<std::pair<int, int>> open;
    std::vector<std::pair<int, int>> close;
    double inf = width * height + 1;
    std::vector<std::vector<Node>> set(width, std::vector<Node>(height));
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            set[i][j].i = i;
            set[i][j].j = j;
            set[i][j].g = inf;
            set[i][j].H = inf;
            set[i][j].F = inf;
        }
    }
    set[i1][j1].g = 0;
    set[i1][j1].H = computeHFromCellToCell(i1,j1,i2,j2,options);
    set[i1][j1].F = set[i1][j1].H;
    open.push_back(std::pair<int, int>(i1, j1));
    std::vector<int> dx = {-1, 0, 1, 0, 1, -1, 1, -1};
    std::vector<int> dy = {-1, 1, 1, -1, 0, 0, -1, 1};
    if (!options.allowdiagonal) {
        dx = {1, -1, 0, 0};
        dy = {0, 0, -1, 1};
    }

    while (!open.empty()) {
        sresult.numberofsteps++;
        std::pair<int, int> cur;
        int mini = 0;
        double min = set[open[0].first][open[0].second].F;
        for (int i = 0; i < open.size(); i++) {
            if (min > set[open[i].first][open[i].second].F) {
                min = set[open[i].first][open[i].second].F;
                mini = i;
            }
        }
        cur = open[mini];
        if (cur.first == i2 && cur.second == j2) {
            std::list<Node> *temp = new std::list<Node>();
            while (!(cur.first == i1 && cur.second == j1)) {
                Node minNode = set[cur.first][cur.second];
                for (int i = 0; i < dx.size(); i++) {
                    int ni = cur.first + dx[i];
                    int nj = cur.second + dy[i];
                    if (map.CellOnGrid(ni, nj) && !map.CellIsObstacle(ni, nj) &&
                        set[ni][nj].F < minNode.F) {
                        minNode = set[ni][nj];
                    }
                }
                Node tmp;
                tmp.i = minNode.i;
                tmp.j = minNode.j;
                tmp.g = minNode.g;
                tmp.F = minNode.F;
                tmp.H = minNode.H;
                temp->push_back(tmp);
                cur.first = minNode.i;
                cur.second = minNode.j;
            }
            temp->push_back(set[i1][j1]);
            temp->reverse();
            temp->push_back(set[i2][j2]);
            sresult.lppath = temp;
            sresult.hppath = temp;
            sresult.pathlength = set[i2][j2].g;
            sresult.pathfound = true;
            sresult.nodescreated = open.size() + close.size();
            sresult.time = currTimeInMillSeconds() - sresult.time;
            return;
        }

        open.erase(open.begin() + mini);
        close.push_back(std::pair<int, int>(cur.first, cur.second));
        for (int i = 0; i < dx.size(); i++) {
            int ni = cur.first + dx[i];
            int nj = cur.second + dy[i];
            if (map.CellOnGrid(ni, nj) && !map.CellIsObstacle(ni, nj) &&
                checkSquize(cur.first, cur.second, ni, nj, options.allowsqueeze, options.cutcorners)) {
                if (set[ni][nj].H == inf)
                    set[ni][nj].H = computeHFromCellToCell(cur.first, cur.second, ni, nj, options);
                double val = set[cur.first][cur.second].F + set[ni][nj].H + computeHFromCellToCell(cur.first,cur.second,ni,nj,options);
                std::pair<int, int> v(ni, nj);
                if (find(close.begin(), close.end(), v) != close.end() && val >= set[v.first][v.second].F) {
                    continue;
                }
                if (find(close.begin(), close.end(), v) == close.end() && val < set[v.first][v.second].F) {
                    set[v.first][v.second].F = val;
                    set[v.first][v.second].g = set[cur.first][cur.second].g + 1;
                    if (find(open.begin(), open.end(), v) == open.end())
                        open.push_back(std::pair<int, int>(v.first, v.second));
                }
            }
        }
    }
    sresult.nodescreated = open.size() + close.size();
    sresult.time = currTimeInMillSeconds() - sresult.time;
    return;
}
