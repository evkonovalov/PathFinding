#include "astar.h"
#include <stack>
#include <vector>

Astar::Astar(double HW, bool BT) {
    hweight = HW;
    breakingties = BT;
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options) {

    sresult.time = currTimeInMillSeconds();
    sresult.lppath = NULL;
    sresult.hppath = NULL;
    sresult.nodescreated = 0;
    sresult.numberofsteps = 0;
    sresult.pathfound = false;
    sresult.pathlength = 0;

    if ((i1 == i2 && j1 == j2) || (map.CellIsObstacle(i1, j1)) || (map.CellIsObstacle(i2, j2))) {
        return NULL;
    }
    int height = map.getMapHeight();
    int width = map.getMapWidth();
    std::vector<std::pair<int, int>> open;
    std::vector<std::pair<int, int>> close;
    int inf = width * height + 1;
    std::vector<std::vector<int>> set(width, std::vector<int>(height, inf));
    set[i1][j1] = 0;
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
        int min = set[open[0].first][open[0].second];
        for (int i = 0; i < open.size(); i++) {
            if (min > set[open[i].first][open[i].second]) {
                min = set[open[i].first][open[i].second];
                mini = i;
            }
        }
        cur = open[mini];
        if (cur.first == i2 && cur.second == j2) {

            std::list<Node> *temp = new std::list<Node>();
            while (!(cur.first == i1 && cur.second == j1)) {
                for (int i = 0; i < 8; i++) {
                    int ni = cur.first + dx[i];
                    int nj = cur.second + dy[i];
                    if (map.CellOnGrid(ni,nj) && !map.CellIsObstacle(ni, nj) &&
                        set[ni][nj] == set[cur.first][cur.second] - delta(cur.first,cur.second,ni,nj,options.metrictype)) {
                        Node tmp;
                        tmp.i = cur.first;
                        tmp.j = cur.second;
                        tmp.g = set[cur.first][cur.second];
                        tmp.F = tmp.g;
                        temp->push_back(tmp);
                        cur.first = ni;
                        cur.second = nj;
                    }
                }
            }
            temp->reverse();
            sresult.lppath = temp;
            sresult.hppath = temp;
            sresult.pathlength = set[i2][j2];
            sresult.pathfound = true;
            sresult.nodescreated = open.size() + close.size();
            sresult.time = currTimeInMillSeconds() - sresult.time;
            return set[i2][j2];
        }

        open.erase(open.begin() + mini);
        close.push_back(std::pair<int, int>(cur.first, cur.second));
        for (int i = 0; i < 8; i++) {
            int ni = cur.first + dx[i];
            int nj = cur.second + dy[i];
            if (map.CellOnGrid(ni,nj) && !map.CellIsObstacle(ni, nj) && checkSquize(cur.first,cur.second,ni,nj,options.allowsqueeze,options.cutcorners)) {
                int val = set[cur.first][cur.second] + delta(cur.first,cur.second,ni,nj,options.metrictype);
                std::pair<int, int> v(ni, nj);
                if (find(close.begin(), close.end(), v) != close.end() && val >= set[v.first][v.second]) {
                    continue;
                }
                if (find(close.begin(), close.end(), v) == close.end() && val < set[v.first][v.second]) {
                    set[v.first][v.second] = val;
                    if (find(open.begin(), open.end(), v) == open.end())
                        open.push_back(std::pair<int, int>(v.first, v.second));
                }
            }
        }
    }
    sresult.nodescreated = open.size() + close.size();
    sresult.time = currTimeInMillSeconds() - sresult.time;
    return 0;
}