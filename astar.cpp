#include "astar.h"

Astar::Astar(double HW, bool BT)
{
    hweight = HW;
    breakingties = BT;
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options)
{
    if ((i1 == i2 && j1 == j2) || (map.CellIsObstacle(i1, j1)) || (map.CellIsObstacle(i2, j2)))
        return 0;
    int height = map.getMapHeight();
    int width = map.getMapWidth();
    std::vector<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> u;
    int inf = width * height + 1;
    std::vector<std::vector<int>> set(width, std::vector<int>(height, inf));

    set[i1][j1] = 0;
    q.push_back(std::pair<int, int>(i1, j1));
    std::vector<int> dx = {-1, 0, 1, 0, 1, -1, 1, -1};
    std::vector<int> dy = {-1, 1, 1, -1, 0, 0, -1, 1};
    while (!q.empty()) {
        std::pair<int, int> cur;
        int mini = 0;
        int min = set[q[0].first][q[0].second];
        for (int i = 0; i < q.size(); i++) {
            if (min > set[q[i].first][q[i].second]) {
                min = set[q[i].first][q[i].second];
                mini = i;
            }
        }
        cur = q[mini];
        if (cur.first == i2 && cur.second == j2)
            return set[cur.first][cur.second];
        q.erase(q.begin() + mini);
        u.push_back(std::pair<int,int>(cur.first,cur.second));
        for (int i = 0; i < 8; i++) {
            int ni = cur.first + dx[i];
            int nj = cur.second + dy[i];
            if (ni >= 0 && ni < width && nj >= 0 && nj < height && !map.CellIsObstacle(ni, nj)) {
                int val = set[cur.first][cur.second] + 1;
                std::pair<int, int> v(ni, nj);
                if (find(u.begin(), u.end(), v) != u.end() && val >= set[v.first][v.second]) {
                    continue;
                }
                if (find(u.begin(), u.end(), v) == u.end() && val < set[v.first][v.second]) {
                    set[v.first][v.second] = val;
                    if (find(q.begin(), q.end(), v) == q.end())
                        q.push_back(std::pair<int,int>(v.first,v.second));
                }
            }
        }
    }
    return 0;
}
