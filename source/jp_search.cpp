#include "jp_search.h"

JP_Search::~JP_Search() {
}

void JP_Search::makePathes(Node curNode, const Map &map) {
    Node *cur = &curNode;
    std::vector<Node> tmp;
    while (cur->parent != nullptr) {
        std::list<Node> passed;
        pushForward(cur->i, cur->j, cur->parent->i, cur->parent->j, passed);
        tmp.insert(tmp.end(), passed.begin(), passed.end());
        lppath.insert(lppath.end(), passed.begin(), passed.end());
        cur = (cur->parent);
    }
    lppath.push_back(*cur);
    tmp.push_back(*cur);
    int Dx = 2;
    int Dy = 2;
    Node prev(0, 0, false);
    for (int i = 0; i < lppath.size(); i++) {
        if (i == 0) {
            prev = tmp[i];
            hppath.push_back(tmp[i]);
            continue;
        }
        int ndx = prev.i - tmp[i].i;
        int ndy = prev.j - tmp[i].j;
        if (ndx != Dx || ndy != Dy) {
            hppath.push_back(prev);
        }
        Dx = ndx;
        Dy = ndy;
        prev = tmp[i];
    }
    hppath.push_back(*cur);
    hppath.reverse();
    lppath.reverse();
    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
}

void JP_Search::neighbours(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options) {
    ISearch::neighbours(cur, suc, map, options);
}

void JP_Search::neighbours(Node cur, int dirx, int diry, std::set<Node> &suc, const Map &map,
                           const EnvironmentOptions &options) {
    miniSearch(suc, dirx, diry, cur, map, options);
}

void JP_Search::miniSearch(std::set<Node> &suc, int dirx, int diry, Node cur, const Map &map,
                           const EnvironmentOptions &options) {
    double minimap[3][3];
    double minimap2[3][3];
    int x = cur.i;
    int y = cur.j;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((i == 0 && j == 0)) {
                minimap[i + 1][j + 1] = -1;
                if (dirx && diry)
                    minimap2[i + 1][j + 1] = sqrt(2);
                else
                    minimap2[i + 1][j + 1] = 1;
            } else {
                if (!map.CellOnGrid(x + i, y + j) || map.CellIsObstacle(x + i, y + j)) {
                    minimap[i + 1][j + 1] = -1;
                    minimap2[i + 1][j + 1] = -1;
                } else {
                    minimap[i + 1][j + 1] = 10;
                    minimap2[i + 1][j + 1] = 10;
                }
            }
        }
    }
    minimap[1 - dirx][1 - diry] = 0;
    minimap2[1 - dirx][1 - diry] = -1;
    bool delta = true;
    std::vector<int> dx = {1, -1, 1, -1, -1, 0, 1, 0};
    std::vector<int> dy = {1, -1, -1, 1, 0, 1, 0, -1};
    while (delta) {
        delta = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int d = 0; d < dx.size(); d++) {
                    int nx = i + dx[d];
                    int ny = j + dy[d];
                    if (nx < 3 && nx > -1 && ny < 3 && ny > -1) {
                        if (minimap[nx][ny] >= 0 && minimap[i][j] >= 0 &&
                            checkSquize(x + i - 1, y + j - 1, x + nx - 1, y + ny - 1, map)) {
                            double val = minimap[i][j];
                            if (dx[d] && dy[d])
                                val += sqrt(2);
                            else
                                val += 1;
                            if (val < minimap[nx][ny]) {
                                minimap[nx][ny] = val;
                                delta = true;
                            }
                        }
                        if (minimap2[nx][ny] >= 0 && minimap2[i][j] >= 0 &&
                            checkSquize(x + i - 1, y + j - 1, x + nx - 1, y + ny - 1, map)) {
                            double val = minimap2[i][j];
                            if (dx[d] && dy[d])
                                val += sqrt(2);
                            else
                                val += 1;
                            if (val < minimap2[nx][ny]) {
                                minimap2[nx][ny] = val;
                                delta = true;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (minimap[i][j] < 0 || minimap2[i][j] < 0 || (minimap2[i][j] == 10 && minimap[i][j] == 10)) {
                continue;
            }
            if (dirx && diry) {
                if (minimap[i][j] < minimap2[i][j]) {
                    continue;
                }
            } else {
                if (minimap[i][j] <= minimap2[i][j]) {
                    continue;
                }
            }
            Node n = Node(x + i - 1, y + j - 1, cur.bk);
            suc.insert(n);
        }
    }
}

void JP_Search::findSuccessors(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options) {
    if (cur.parent == nullptr) {
        ISearch::findSuccessors(cur, suc, map, options);
        return;
    }
    std::set<Node> neighb;
    int dx = cur.i - cur.parent->i;
    int dy = cur.j - cur.parent->j;
    int diry = 1;
    int dirx = 1;
    if (dy < 0)
        diry = -1;
    if (dx < 0)
        dirx = -1;
    if (dx == 0)
        dirx = 0;
    if (dy == 0)
        diry = 0;
    neighbours(cur, dirx, diry, neighb, map, options);
    for (auto i: neighb) {
        int dx = i.i - cur.i;
        int dy = i.j - cur.j;
        auto n = jump(cur, dx, dy, map, options);
        if (n.i != -1)
            suc.insert(n);
    }
}

Node JP_Search::resetParent(Node cur, Node *parent, const Map &map, const EnvironmentOptions &options) {
    int dx = abs((*parent).i - cur.i);
    int dy = abs((*parent).j - cur.j);
    double len = 0;
    if (dx > dy) {
        len = sqrt(2) * dy + (dx - dy);
    } else {
        len = sqrt(2) * dx + (dy - dx);
    }
    if (close.find(cur.i * map.getMapHeight() + cur.j) != close.end())
        return cur;
    auto tmp = open.find(cur);
    if (tmp == open.end()) {
        cur.H = computeHFromCellToCell(cur.i, cur.j, map.getMapGoalI(), map.getMapGoalJ(), options);
        cur.g = (*parent).g + len;
        cur.F = cur.g + cur.H;
        cur.parent = parent;
        open.insert(cur);
        return cur;
    } else {
        double val;
        val = (*parent).g + len;
        if (val >= (tmp)->g)
            return cur;
        cur.H = tmp->H;
        cur.parent = parent;
        cur.g = val;
        cur.F = val + cur.H;
        open.erase(tmp);
        open.insert(cur);
        return cur;
    }
}

bool JP_Search::checkForced(Node node, int dx, int dy, std::set<Node> &neighb) {
    if (dx && dy) {
        Node a1 = Node(node.i + dx, node.j + dy, node.bk);
        Node a2 = Node(node.i, node.j + dy, node.bk);
        Node a3 = Node(node.i + dx, node.j, node.bk);
        for (auto i: neighb) {
            if (i != a1 && i != a2 && i != a3)
                return false;
        }
    } else {
        Node a = Node(node.i + dx, node.j + dy, node.bk);
        for (auto i: neighb) {
            if (i != a)
                return false;
        }
    }
    return true;
}

Node JP_Search::jump(Node node, int dx, int dy, const Map &map, const EnvironmentOptions &options) {
    node.i += dx;
    node.j += dy;
    if (!map.CellOnGrid(node.i, node.j) || map.CellIsObstacle(node.i, node.j)) {
        auto nul = Node(-1, -1, false);
        return nul;
    }
    if (!checkSquize(node.i, node.j, node.i - dx, node.j - dy, map)) {
        auto nul = Node(-1, -1, false);
        return nul;
    }
    if (node.i == map.getMapGoalI() && node.j == map.getMapGoalJ()) {
        return node;
    }
    std::set<Node> neighb;
    neighbours(node, dx, dy, neighb, map, options);
    if (!checkForced(node, dx, dy, neighb))
        return node;
    if (dx && dy) {
        Node ret = jump(node, dx, 0, map, options);
        if (ret.i != -1)
            return node;
        ret = jump(node, 0, dy, map, options);
        if (ret.i != -1)
            return node;
    }
    return jump(node, dx, dy, map, options);
}

void JP_Search::pushForward(int i, int j, int i1, int j1, std::list<Node> &pList) {
    int dx = i1 - i;
    int dy = j1 - j;
    int diry = 1;
    int dirx = 1;
    if (dy < 0)
        diry = -1;
    if (dx < 0)
        dirx = -1;
    if (dx == 0)
        dirx = 0;
    if (dy == 0)
        diry = 0;
    while (i != i1 || j != j1) {
        Node n = Node(i, j, false);
        pList.push_back(n);
        i += dirx;
        j += diry;
    }
}
