//
// Created by DNS on 26.08.2018.
//

#include "anya.h"

void Anya::image(const Map &map) {
    int w = map.getMapWidth() * 3 + 1;
    int h = map.getMapHeight() * 3 + 1;
    int **red = (int **) malloc(sizeof(int *) * w);
    int **green = (int **) malloc(sizeof(int *) * w);
    int **blue = (int **) malloc(sizeof(int *) * w);
    for (int i = 0; i < w; i++) {
        red[i] = (int *) malloc(sizeof(int) * h);
        green[i] = (int *) malloc(sizeof(int) * h);
        blue[i] = (int *) malloc(sizeof(int) * h);
    }
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (i % 3 == 0 || j % 3 == 0) {
                red[i][j] = 0;
                green[i][j] = 0;
                blue[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < map.getMapWidth(); i++) {
        for (int j = 0; j < map.getMapHeight(); j++) {
            Node c(i, j, breakingties);
            if (!map.CellIsObstacle(c.i, c.j)) {
                red[1 + i*3][1 + j*3] = 150;
                green[1 + i*3][1 + j*3] = 150;
                blue[1 + i*3][1 + j*3] = 150;
                red[2 + i*3][1 + j*3] = 150;
                green[2 + i*3][1 + j*3] = 150;
                blue[2 + i*3][1 + j*3] = 150;
                red[2 + i*3][2 + j*3] = 150;
                green[2 + i*3][2 + j*3] = 150;
                blue[2 + i*3][2 + j*3]= 150;
                red[1 + i*3][2 + j*3] = 150;
                green[1 + i*3][2 + j*3] = 150;
                blue[1 + i*3][2 + j*3] = 150;
            } else {
                red[1 + i*3][1 + j*3] = 150;
                green[1 + i*3][1 + j*3] = 100;
                blue[1 + i*3][1 + j*3] = 50;
                red[2 + i*3][1 + j*3] = 150;
                green[2 + i*3][1 + j*3] = 100;
                blue[2 + i*3][1 + j*3] = 50;
                red[2 + i*3][2 + j*3] = 150;
                green[2 + i*3][2 + j*3] = 100;
                blue[2 + i*3][2 + j*3]= 50;
                red[1 + i*3][2 + j*3] = 150;
                green[1 + i*3][2 + j*3] = 100;
                blue[1 + i*3][2 + j*3] = 50;
            }
        }
    }
    for (auto i: lppath) {
        int dx = (i.i - round(i.i)) / 0.25;
        int dy = (i.j - round(i.j)) / 0.25;
        int x = round(i.i);
        int y = round(i.j);
        red[x*3 + dx][y*3 + dy] = 100;
        green[x*3 + dx][y*3 + dy] = 50;
        blue[x*3 + dx][y*3 + dy] = 10;
    }
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3 * w * h;
    int x, y, r, g, b;

    img = (unsigned char *) malloc(3 * w * h);
    memset(img, 0, 3 * w * h);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            x = i;
            y = (h - 1) - j;
            r = red[i][j];
            g = green[i][j];
            b = blue[i][j];
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            img[(x + y * w) * 3 + 2] = (unsigned char) (r);
            img[(x + y * w) * 3 + 1] = (unsigned char) (g);
            img[(x + y * w) * 3 + 0] = (unsigned char) (b);
        }
    }

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char) (filesize);
    bmpfileheader[3] = (unsigned char) (filesize >> 8);
    bmpfileheader[4] = (unsigned char) (filesize >> 16);
    bmpfileheader[5] = (unsigned char) (filesize >> 24);

    bmpinfoheader[4] = (unsigned char) (w);
    bmpinfoheader[5] = (unsigned char) (w >> 8);
    bmpinfoheader[6] = (unsigned char) (w >> 16);
    bmpinfoheader[7] = (unsigned char) (w >> 24);
    bmpinfoheader[8] = (unsigned char) (h);
    bmpinfoheader[9] = (unsigned char) (h >> 8);
    bmpinfoheader[10] = (unsigned char) (h >> 16);
    bmpinfoheader[11] = (unsigned char) (h >> 24);

    f = fopen("img.bmp", "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for (int i = 0; i < h; i++) {
        fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
        fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
    }
    for (int i = 0; i < w; i++) {
        free(red[i]);
        free(green[i]);
        free(blue[i]);
    }
    free(red);
    free(green);
    free(blue);
    free(img);
    fclose(f);
}

SearchResult Anya::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, bool visual) {
    allowsqueeze = options.allowsqueeze;
    cutcorners = options.cutcorners;
    sresult.time = currTimeInMillSeconds();
    sresult.lppath = nullptr;
    sresult.hppath = nullptr;
    sresult.nodescreated = 0;
    sresult.numberofsteps = 0;
    sresult.pathfound = false;
    sresult.pathlength = 0;
    auto start = AnyaPoint(map.getMapStartI(), map.getMapStartJ());
    std::set<AnyaNode> successors;
    generateStartSuccessors(start, successors, map);
    for (auto i: successors) {
        std::cout << i.r.x << " " << i.r.y << "\n" << i.i.left.x << " " << i.i.left.y << "\n" << i.i.right.x << " "
                  << i.i.right.y << "\n";
    }
    open.insert(successors.begin(), successors.end());
    AnyaPoint goal(map.getMapGoalI(), map.getMapGoalJ());
    while (!open.empty()) {
        sresult.numberofsteps++;
        if(sresult.numberofsteps % 1000 == 0)
            std::cout << sresult.numberofsteps << "\n";
        AnyaNode currentNode = *open.begin();
        AnyaPoint curInt = getIntPoint(currentNode, map);
        if (checkLine(curInt, goal, map)) {
            lppath.push_back(Node(goal.x, goal.y, true));
            getIntPoint(currentNode, map);
            checkLine(curInt, goal, map);
            makePathes(currentNode, map);
            sresult.pathlength = static_cast<float>(currentNode.G);
            sresult.pathfound = true;
            sresult.nodescreated = static_cast<unsigned int>(open.size() + close.size());
            sresult.time = (currTimeInMillSeconds() - sresult.time) / 1000;
            if (visual)
                image(map);
            open.clear();
            close.clear();
            return sresult;
        }
        std::pair<double, AnyaNode> closeNode(curInt.x * map.getMapHeight() + curInt.y, currentNode);
        close.push_back(currentNode);
        const AnyaNode *parent = &((close.back()));
        /*std::cout << "Parent: " << "R:(" << (*parent).r.x << "," << (*parent).r.y << ") I:(" << (*parent).i.right.x
                  << "," << (*parent).i.right.y << ") (" << (*parent).i.left.x << "," << (*parent).i.left.y << ")"
                  << "\n";*/
        open.erase(open.begin());
        std::set<AnyaNode> successors;
        findSuccessors(*parent, successors, map, options);

        //std::cout << "Succsesors created: " << successors.size() << "\n";
        for (auto i: successors) {
            //std::cout << "R:(" << i.r.x << "," << i.r.y << ") I:(" << i.i.right.x << "," << i.i.right.y << ") ("
              //        << i.i.left.x << "," << i.i.left.y << ")" << "\n";
            resetParent(i, parent, map, options);

        }
        std::cout << "Open Size: " << open.size() << "\n";
        for (auto i: open) {
            std::cout << "R:(" << i.r.x << "," << i.r.y << ") I:(" << i.i.right.x << "," << i.i.right.y << ") ("
                      << i.i.left.x << "," << i.i.left.y << ") " << i.F << " " << i.G << "\n";
        }
        std::cout << "_________________________" << "\n";
    }
    sresult.nodescreated = static_cast<unsigned int>(open.size() + close.size());
    sresult.time = (currTimeInMillSeconds() - sresult.time) / 1000;
    return sresult;
}

void Anya::makePathes(AnyaNode curNode, const Map &map) {
    const AnyaNode *cur = &curNode;
    while (cur->parent != nullptr) {
        AnyaPoint ip = getIntPoint(*cur, map);
        lppath.emplace_back(ip.x, ip.y, true);
        lppath.emplace_back(cur->r.x, cur->r.y, true);
        cur = (cur->parent);
    }
    AnyaPoint ip = getIntPoint(curNode, map);
    lppath.emplace_back(ip.x, ip.y, true);
    lppath.emplace_back(cur->r.x, cur->r.y, true);
    lppath.reverse();
    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
}

AnyaPoint Anya::getIntPoint(AnyaNode node, const Map &map) {
    AnyaPoint end(map.getMapGoalI(), map.getMapGoalJ());
    if ((end.y < node.i.right.y && end.y > node.r.y) || (end.y > node.i.right.y && end.y < node.r.y))
        end.y = node.i.right.y + (node.i.right.y - end.y);
    if (end.y == node.i.right.y || (end.x <= node.i.right.x && end.x <= node.i.left.x)
        || (end.x >= node.i.right.x && end.x >= node.i.left.x)) {
        double dx = end.x - node.i.right.x;
        double dy = end.y - node.i.right.y;
        double d1 = sqrt(dx * dx + dy * dy);
        dx = end.x - node.i.left.x;
        dy = end.y - node.i.left.y;
        double d2 = sqrt(dx * dx + dy * dy);
        return d1 < d2 ? node.i.right : node.i.left;
    }
    return findEndPoint(node.r, end, node.i.right.y);
}

AnyaNode Anya::resetParent(AnyaNode cur, const AnyaNode *parent, const Map &map, const EnvironmentOptions &options) {
    AnyaPoint intPoint = getIntPoint(cur, map);
    AnyaPoint pIntPoint = getIntPoint(*parent, map);
    if (std::find(close.begin(), close.end(), cur) != close.end())
        return cur;
    auto tmp = std::find(open.begin(), open.end(), cur);
    if (tmp == open.end()) {
        cur.H = computeHFromCellToCell(intPoint.x, intPoint.y, map.getMapGoalI(), map.getMapGoalJ(), options);
        cur.G = (*parent).G + computeHFromCellToCell(intPoint.x, intPoint.y, pIntPoint.x, pIntPoint.y, options);
        cur.F = cur.G + cur.H;
        cur.parent = parent;
        open.insert(cur);
        return cur;
    } else {
        double val = (*parent).G + computeHFromCellToCell(intPoint.x, intPoint.y, pIntPoint.x, pIntPoint.y, options);
        if (val >= (tmp)->G)
            return cur;
        cur.H = tmp->H;
        cur.parent = parent;
        cur.G = val;
        cur.F = val + cur.H;
        open.erase(tmp);
        open.insert(cur);
        return cur;
    }
}

void Anya::findSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map, const EnvironmentOptions &options) {
    if (cur.r.y == cur.i.right.y) {
        generateFlatSuccessors(cur, suc, map, options);
        double dx = cur.r.x - cur.i.right.x;
        double dy = cur.r.y - cur.i.right.y;
        double d1 = sqrt(dx * dx + dy * dy);
        dx = cur.r.x - cur.i.left.x;
        dy = cur.r.y - cur.i.left.y;
        double d2 = sqrt(dx * dx + dy * dy);
        AnyaPoint p = d1 > d2 ? cur.i.right : cur.i.left;
        if (isCornerPoint(p.x, p.y, map)) {
            AnyaNode nc;
            nc.r = cur.r;
            nc.i.right = p;
            nc.i.left = p;
            generateConeSuccessors(nc, suc, map, options);
        }
    } else {
        generateConeSuccessors(cur, suc, map, options);
        if (isCornerPoint(cur.i.right.x, cur.i.right.y, map)) {
            AnyaNode nc;
            nc.r = cur.r;
            nc.i.right = cur.i.right;
            nc.i.left = cur.i.right;
            generateConeSuccessors(nc, suc, map, options);
            nc.i.left = cur.r;
            generateFlatSuccessors(nc, suc, map, options);
        }
        if (isCornerPoint(cur.i.left.x, cur.i.left.y, map)) {
            AnyaNode nc;
            nc.r = cur.r;
            nc.i.right = cur.i.left;
            nc.i.left = cur.i.left;
            generateConeSuccessors(nc, suc, map, options);
            generateFlatSuccessors(nc, suc, map, options);
        }
    }
}

bool Anya::checkSquare(int x, int y, const Map &map) {
    return map.CellOnGrid(x, y) && !map.CellIsObstacle(x, y);
}

void Anya::move(double *cor, int delta) {
    if (delta > 0) {
        if (*cor != trunc(*cor)) {
            *cor = trunc(*cor) + 1;
        } else {
            *cor += 1;
        }
    } else {
        if (*cor != trunc(*cor)) {
            *cor = trunc(*cor);
        } else {
            *cor -= 1;
        }
    }
}

void
Anya::generateConeSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map, const EnvironmentOptions &options) {
    AnyaNode newNode;
    double dx = cur.r.x - cur.i.right.x;
    double dy = cur.r.y - cur.i.right.y;
    double d1 = sqrt(dx * dx + dy * dy);
    dx = cur.r.x - cur.i.left.x;
    dy = cur.r.y - cur.i.left.y;
    double d2 = sqrt(dx * dx + dy * dy);
    if (cur.r.y == cur.i.right.y && cur.i.right.y == cur.i.left.y) {
        newNode.r = d1 > d2 ? cur.i.right : cur.i.left;
        AnyaPoint tmp = cur.i.left;
        AnyaPoint p = tmp;
        if (checkSquare(tmp.x, tmp.y, map) || checkSquare(tmp.x - 1, tmp.y, map)) {
            move(&p.y, 1);
            int delta = 0;
            if (checkSquare(tmp.x, tmp.y, map))
                delta = -1;
            else
                delta = 1;
            tmp = p;
            while (checkLine(newNode.r, tmp, map))
                move(&tmp.x, delta);
            if (tmp.x == p.x && tmp.y == p.y)
                return;
            move(&tmp.x, -delta);
            if (delta == -1) {
                newNode.i.right = p;
                newNode.i.left = tmp;
            } else {
                newNode.i.right = tmp;
                newNode.i.left = p;
            }
        } else if (checkSquare(tmp.x, tmp.y - 1, map) || checkSquare(tmp.x - 1, tmp.y - 1, map)) {
            move(&p.y, -1);
            int delta = 0;
            if (checkSquare(tmp.x, tmp.y - 1, map))
                delta = -1;
            else
                delta = 1;
            tmp = p;
            while (checkLine(newNode.r, tmp, map))
                move(&tmp.x, delta);
            if (tmp.x == p.x && tmp.y == p.y)
                return;
            move(&tmp.x, -delta);
            if (delta == -1) {
                newNode.i.right = p;
                newNode.i.left = tmp;
            } else {
                newNode.i.right = tmp;
                newNode.i.left = p;
            }
        }
    } else if (cur.i.right.x == cur.i.left.x) {
        newNode.r = cur.i.right;
        AnyaPoint p = findEndPoint(cur.r, cur.i.right);
        AnyaPoint tmp = p;
        int l1 = 0;
        for (l1 = 0; checkLine(newNode.r, tmp, map); l1++)
            move(&tmp.x, 1);
        move(&tmp.x, -1);
        AnyaPoint tmp2 = p;
        int l2 = 0;
        for (l2 = 0; checkLine(newNode.r, tmp2, map); l2++)
            move(&tmp2.x, -1);
        move(&tmp2.x, 1);
        if (l1 == 0 && l2 == 0)
            return;
        if (l1 > l2) {
            newNode.i.left = tmp2;
            newNode.i.right = tmp;
        } else {
            newNode.i.left = tmp2;
            newNode.i.right = tmp;
        }
    } else {
        newNode.r = cur.r;
        newNode.i.right = findEndPoint(cur.r, cur.i.right);
        newNode.i.left = findEndPoint(cur.r, cur.i.left);
        while (!checkLine(cur.r, newNode.i.right, map)) {
            move(&newNode.i.right.x, -1);
            if (newNode.i.right.x < newNode.i.left.x)
                return;
        }
        while (!checkLine(cur.r, newNode.i.left, map)) {
            move(&newNode.i.left.x, 1);
            if (newNode.i.right.x < newNode.i.left.x)
                return;
        }

    }
    AnyaPoint run = newNode.i.left;
    AnyaPoint lastRun = newNode.i.left;
    AnyaPoint end = newNode.i.right;
    while (true) {
        if (run.x >= end.x) {
            if (run.x > end.x)
                run.x = end.x;
            AnyaNode nN(newNode.r, lastRun, run);
            initGHF(&nN, map);
            suc.insert(nN);
            break;
        }
        run.x++;
        if (isCornerPoint(run.x, run.y, map)) {
            AnyaNode nN(newNode.r, lastRun, run);
            initGHF(&nN, map);
            suc.insert(nN);
            lastRun = run;
        }
    }
}

void
Anya::generateFlatSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map, const EnvironmentOptions &options) {
    double dx = cur.r.x - cur.i.right.x;
    double dy = cur.r.y - cur.i.right.y;
    double d1 = sqrt(dx * dx + dy * dy);
    dx = cur.r.x - cur.i.left.x;
    dy = cur.r.y - cur.i.left.y;
    double d2 = sqrt(dx * dx + dy * dy);
    AnyaPoint p = d1 > d2 ? cur.i.right : cur.i.left;
    dx = p.x - cur.r.x;
    if (dx < 0)
        dx = -1;
    else
        dx = 1;
    AnyaPoint p2 = p;
    move(&p2.x, dx);
    AnyaPoint lastBackUp = p;
    while (true) {
        if (isCornerPoint(p2.x, p2.y, map))
            break;
        if (!checkLine(cur.r, p2, map) || !map.CellOnGrid(p2.x, p2.y)) {
            p2 = lastBackUp;
            break;
        }
        move(&p2.x, dx);
        if (isTouchableCross(p2.x, p2.y, map))
            lastBackUp = p2;
    }
    AnyaNode newNode;
    if (p2.x > p.x) {
        newNode.i.right = p2;
        newNode.i.left = p;
    } else {
        newNode.i.left = p2;
        newNode.i.right = p;
    }
    if (cur.r.y == p.y) {
        newNode.r = cur.r;
    } else {
        newNode.r = p;
    }
    suc.insert(newNode);

    p = d1 < d2 ? cur.i.right : cur.i.left;
    dx = -dx;
    p2 = p;
    move(&p2.x, dx);
    lastBackUp = p;
    while (true) {
        if (isCornerPoint(p2.x, p2.y, map))
            break;
        if (!checkLine(cur.r, p2, map) || !map.CellOnGrid(p2.x, p2.y)) {
            p2 = lastBackUp;
            break;
        }
        move(&p2.x, dx);
        if (isTouchableCross(p2.x, p2.y, map))
            lastBackUp = p2;
    }
    newNode;
    if (p2.x > p.x) {
        newNode.i.right = p2;
        newNode.i.left = p;
    } else {
        newNode.i.left = p2;
        newNode.i.right = p;
    }
    if (cur.r.y == p.y) {
        newNode.r = cur.r;
    } else {
        newNode.r = p;
    }
    suc.insert(newNode);
}

double Anya::dist(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

void Anya::initGHF(AnyaNode *node, const Map &map) {
    AnyaPoint intPoint = getIntPoint(*node, map);
    node->H = dist(intPoint.x, intPoint.y, map.getMapGoalI(), map.getMapGoalJ());
    node->G = dist(intPoint.x, intPoint.y, node->r.x, node->r.y);
    node->F = node->G + node->H;
}

void Anya::generateStartSuccessors(AnyaPoint begin, std::set<AnyaNode> &suc, const Map &map) {
    AnyaPoint cur(begin.x, begin.y);
    while (true) {
        cur.x++;
        if (!checkLine(begin, cur, map)) {
            cur.x--;
            AnyaNode newNode(begin, begin, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
            break;
        } else if (isCornerPoint(cur.x, cur.y, map)) {
            AnyaNode newNode(begin, begin, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
        }
    }
    cur = begin;
    while (true) {
        cur.x--;
        if (!checkLine(begin, cur, map)) {
            cur.x++;
            AnyaNode newNode(begin, cur, begin);
            initGHF(&newNode, map);
            suc.insert(newNode);
            break;
        } else if (isCornerPoint(cur.x, cur.y, map)) {
            AnyaNode newNode(begin, cur, begin);
            initGHF(&newNode, map);
            suc.insert(newNode);
        }
    }
    AnyaPoint curLeft = begin;
    AnyaPoint curRight = begin;
    curLeft.y--;
    curRight.y--;
    while (true) {
        curLeft.x--;
        if (!checkLine(begin, curLeft, map)) {
            curLeft.x++;
            break;
        }
    }
    while (true) {
        curRight.x++;
        if (!checkLine(begin, curRight, map)) {
            curRight.x--;
            break;
        }
    }
    cur = curLeft;
    AnyaPoint lastCur = curLeft;
    while (true) {
        if (cur.x == curRight.x) {
            AnyaNode newNode(begin, lastCur, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
            break;
        }
        cur.x++;
        if (isCornerPoint(cur.x, cur.y, map)) {
            AnyaNode newNode(begin, lastCur, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
            lastCur = cur;
        }

    }

    curLeft = begin;
    curRight = begin;
    curLeft.y++;
    curRight.y++;
    while (true) {
        curLeft.x--;
        if (!checkLine(begin, curLeft, map)) {
            curLeft.x++;
            break;
        }
    }
    while (true) {
        curRight.x++;
        if (!checkLine(begin, curRight, map)) {
            curRight.x--;
            break;
        }
    }
    cur = curLeft;
    lastCur = curLeft;
    while (true) {
        if (cur.x == curRight.x) {
            AnyaNode newNode(begin, lastCur, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
            break;
        }
        cur.x++;
        if (isCornerPoint(cur.x, cur.y, map)) {
            AnyaNode newNode(begin, lastCur, cur);
            initGHF(&newNode, map);
            suc.insert(newNode);
            lastCur = cur;
        }
    }
}

bool Anya::isCornerPoint(int x, int y, const Map &map) {
    int dx[4] = {-1, -1, 0, 0};
    int dy[4] = {0, -1, 0, -1};
    bool findOne = false;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!checkSquare(nx, ny, map)) {
            if (!findOne)
                findOne = true;
            else
                return false;
        }
    }
    return findOne;
}

AnyaPoint Anya::findEndPoint(AnyaPoint x1, AnyaPoint x2) {
    if (x1.x == x2.x) {
        if(x1.y < x2.y)
            return {x1.x, x2.y + 1};
        else
            return {x1.x, x1.y - 1};
    }
    int cor = 0;
    double m = (x1.y - x2.y) / (x1.x - x2.x);
    double b = x1.y - m * x1.x;
    if (x1.y - x2.y > 0)
        cor = x2.y - 1;
    else
        cor = x2.y + 1;
    return {(cor - b) / m, cor};
}

AnyaPoint Anya::findEndPoint(AnyaPoint x1, AnyaPoint x2, double cor) {
    if (x1.x == x2.x) {
        return {x1.x, x1.y};
    }
    double m = (x1.y - x2.y) / (x1.x - x2.x);
    double b = x1.y - m * x1.x;
    return {(cor - b) / m, cor};
}

bool Anya::isTouchableCross(int x, int y, const Map &map) {
    return checkSquare(x, y, map) || checkSquare(x - 1, y - 1, map) ||
           checkSquare(x - 1, y, map) || checkSquare(x, y - 1, map);
}

//may be a problem
bool Anya::isPassableCross(double x, double y, const Map &map) {
    if (round(x) == x && round(y) == y) {
        return map.CellOnGrid(x, y) && !((!checkSquare(x, y, map) && !checkSquare(x - 1, y - 1, map)) ||
                                         (!checkSquare(x - 1, y, map) && !checkSquare(x, y - 1, map)));
    } else
        return true;
}

bool Anya::isPassableShift(int x, int y, int dx, int dy, const Map &map) {
    if (dx == 0 && dy == 0)
        return isPassableCross(x, y, map);
    if (!isPassableCross(x, y, map) || !isPassableCross(x + dx, y + dy, map))
        return false;
    if (dx == 0) {
        if (dy > 0) {
            return checkSquare(x, y, map) || checkSquare(x - 1, y, map);
        } else {
            return checkSquare(x, y - 1, map) || checkSquare(x - 1, y - 1, map);
        }
    } else if (dy == 0) {
        if (dx > 0) {
            return checkSquare(x, y, map) || checkSquare(x, y - 1, map);
        } else {
            return checkSquare(x - 1, y, map) || checkSquare(x - 1, y - 1, map);
        }
    } else {
        if (dx > 0 && dy > 0)
            return checkSquare(x, y, map);
        if (dx < 0 && dy > 0)
            return checkSquare(x - 1, y, map);
        if (dx < 0 && dy < 0)
            return checkSquare(x - 1, y - 1, map);
        if (dx > 0 && dy < 0)
            return checkSquare(x, y - 1, map);
    }
    return false;
}

bool Anya::checkLine(AnyaPoint a, AnyaPoint b, const Map &map) {
    if (!isPassableCross(a.x, a.y, map) || !isPassableCross(b.x, b.y, map))
        return false;
    if (a.x == b.x) {
        int dy = a.y - b.y;
        if (dy == 0)
            return isPassableCross(a.x, a.y, map);
        else {
            dy = dy / abs(dy);
            for (int i = a.y; checkForEnd(i, b.y, dy); i -= dy) {
                if (!isPassableShift(a.x, i, 0, -dy, map))
                    return false;
            }
            return true;
        }
    }
    if (a.y == b.y) {
        int dx = a.x - b.x;
        if (dx == 0)
            return isPassableCross(a.x, a.y, map);
        else {
            dx = dx / abs(dx);
            for (int i = a.x; i != b.x; i -= dx) {
                if (!isPassableShift(i, a.y, -dx, 0, map))
                    return false;
            }
            return true;
        }
    }
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    if (dx > dy) {
        if (a.x < b.x) {
            AnyaPoint temp = a;
            a = b;
            b = temp;
        }
        double m = (a.y - b.y) / (a.x - b.x);
        double c = a.y - m * a.x;
        dx = (a.x - b.x) / dx;
        if (a.x != trunc(a.x))
            a.x = trunc(a.x);
        if (b.x != trunc(b.x)) {
            if (a.y > b.y) {
                if (!checkSquare(trunc(b.x), b.y, map))
                    return false;

            } else {
                if (!checkSquare(trunc(b.x), b.y - 1, map))
                    return false;
            }
            b.x = trunc(b.x) + 1;
        }

        double oldy = a.y;
        for (int i = a.x - dx; checkForEnd(i, b.x, dx); i -= dx) {
            double y = i * m + c;
            if (y == trunc(y)) {
                if (!isPassableCross(i, y, map))
                    return false;
                if (!checkSquare(i, y, map))
                    return false;
                oldy = -1;
                continue;
            } else {
                y = trunc(y);
                if (!checkSquare(i, y, map))
                    return false;
            }
            if(oldy != y && oldy != -1){
                if (!checkSquare(i, oldy, map))
                    return false;
            }
            oldy = y;
        }
        return true;
    } else if (dx < dy) {
        if (a.y < b.y) {
            AnyaPoint temp = a;
            a = b;
            b = temp;
        }
        double m = (a.y - b.y) / (a.x - b.x);
        double c = a.y - m * a.x;
        dy = (a.y - b.y) / dy;
        if (a.y != trunc(a.y))
            a.y = trunc(a.y);
        if (b.y != trunc(b.y)) {
            if (a.x > b.x) {
                if (!checkSquare(b.x, trunc(b.y), map))
                    return false;

            } else {
                if (!checkSquare(b.x - 1, trunc(b.y), map))
                    return false;
            }
            b.y = trunc(b.y);
        }
        double oldx = a.x;
        for (int i = a.y - dy; checkForEnd(i, b.y, dy); i -= dy) {
            double x = (i - c) / m;
            if (x == trunc(x)) {
                if (!isPassableCross(x, i, map))
                    return false;
                if (!checkSquare(x, i, map))
                    return false;
                oldx = -1;
                continue;
            } else {
                x = trunc(x);
                if (!checkSquare(x, i, map))
                    return false;
            }
            if(x != oldx && oldx != -1){
                if (!checkSquare(oldx, i, map))
                    return false;
            }
            oldx = x;
        }
        return true;
    } else {
        dx = (a.x - b.x) / dx;
        dy = (a.y - b.y) / dy;
        int t = 1;
        for (int i = a.y - dy; i != b.y; i -= dy) {
            if (!isPassableShift(a.x - t * dx, i, -dx, -dy, map))
                return false;
            t++;
        }
        return true;
    }
}

bool Anya::checkForEnd(int i, double x, int dx) {
    if (dx > 0)
        return i >= x;
    else
        return i <= x;
}
