#include "isearch.h"

ISearch::ISearch() {
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
}

void ISearch::image(const Map& map){
    int w = map.getMapWidth();
    int h = map.getMapHeight();
    int** red = (int**)malloc(sizeof(int*)*w);
    int** green = (int**)malloc(sizeof(int*)*w);
    int** blue = (int**)malloc(sizeof(int*)*w);
    for(int i = 0; i < w; i++){
        red[i] = (int*)malloc(sizeof(int)*h);
        green[i] = (int*)malloc(sizeof(int)*h);
        blue[i] = (int*)malloc(sizeof(int)*h);
    }
    for (int i = 0; i < map.getMapWidth(); i++) {
        for (int j = 0; j < map.getMapHeight(); j++) {
            Node c(i, j,breakingties);
            auto t1 = find(lppath.begin(), lppath.end(), c);
            if (t1 == lppath.end()) {
                if (!map.CellIsObstacle(c.i, c.j)) {
                    red[i][j] = 150;
                    green[i][j] = 150;
                    blue[i][j] = 150;
                } else {
                    red[i][j] = 100;
                    green[i][j] = 50;
                    blue[i][j] = 50;
                }
            } else {
                red[i][j] = 10;
                green[i][j] = 200;
                blue[i][j] = 10;
            }
        }
    }
    FILE *f;
    unsigned char *img = NULL;
    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
    int x,y,r,g,b;

    img = (unsigned char *)malloc(3*w*h);
    memset(img,0,3*w*h);

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            x=i; y=(h-1)-j;
            r = red[i][j];
            g = green[i][j];
            b = blue[i][j];
            if (r > 255) r=255;
            if (g > 255) g=255;
            if (b > 255) b=255;
            img[(x+y*w)*3+2] = (unsigned char)(r);
            img[(x+y*w)*3+1] = (unsigned char)(g);
            img[(x+y*w)*3+0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen("img.bmp","wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }
    for(int i = 0; i < w; i++){
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

SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options) {
    sresult.time = currTimeInMillSeconds();
    sresult.lppath = nullptr;
    sresult.hppath = nullptr;
    sresult.nodescreated = 0;
    sresult.numberofsteps = 0;
    sresult.pathfound = false;
    sresult.pathlength = 0;
    int startI = map.getMapStartI();
    int goalI = map.getMapGoalI();
    int startJ = map.getMapStartJ();
    int goalJ = map.getMapGoalJ();
    auto start = Node(startI, startJ, breakingties);
    double sqrt2 = sqrt(2);
    open.insert(start);
    std::vector<int> dx = {-1, 0, 1, 0, 1, -1, 1, -1};
    std::vector<int> dy = {0, 1, 0, -1, 1, -1, -1, 1};
    if (!options.allowdiagonal) {
        dx = {1, -1, 0, 0};
        dy = {0, 0, -1, 1};
    }

    while (!open.empty()) {
        sresult.numberofsteps++;
        Node cur = *open.begin();
        if (cur.i == goalI && cur.j == goalJ) {
            makePathes(cur);
            sresult.pathlength = static_cast<float>(sresult.lppath->back().g);
            sresult.pathfound = true;
            sresult.nodescreated = static_cast<unsigned int>(open.size() + close.size());
            sresult.time = (currTimeInMillSeconds() - sresult.time) / 1000;
            //image(map);
            open.clear();
            close.clear();
            return sresult;
        }
        std::pair<int, Node> closeNode(cur.i * map.getMapHeight() + cur.j,cur);
        auto ret = close.insert(closeNode);
        auto iter = ret.first;
        Node* parent = &((*iter).second);
        open.erase(cur);
        for (int i = 0; i < dx.size(); i++) {
            int ni = cur.i + dx[i];
            int nj = cur.j + dy[i];
            auto ncur = Node(ni, nj, breakingties);
            if (map.CellOnGrid(ni, nj) && !map.CellIsObstacle(ni, nj) &&
                checkSquize(cur.i, cur.j, ni, nj, options.allowsqueeze, options.cutcorners, map)) {
                if (close.find(ni * map.getMapHeight() + nj) != close.end())
                    continue;
                auto tmp = open.find(ncur);
                if (tmp == open.end()) {
                    ncur.H = computeHFromCellToCell(ni, nj, goalI, goalJ, options);
                    if (dx[i] && dy[i])
                        ncur.g = cur.g + sqrt2;
                    else
                        ncur.g = cur.g + 1;
                    ncur.F = ncur.g + ncur.H;
                    ncur.parent = parent;
                    open.insert(ncur);
                } else {
                    double val;
                    if (dx[i] && dy[i])
                        val = cur.g + sqrt2;
                    else
                        val = cur.g + 1;
                    if (val >= (tmp)->g)
                        continue;
                    ncur.H = tmp->H;
                    ncur.parent = parent;
                    ncur.g = val;
                    ncur.F = val + ncur.H;
                    open.erase(tmp);
                    open.insert(ncur);
                }
            }
        }
    }
    sresult.nodescreated = static_cast<unsigned int>(open.size() + close.size());
    sresult.time = (currTimeInMillSeconds() - sresult.time) / 1000;
    return sresult;
}

long ISearch::currTimeInMillSeconds() {
    auto time = std::chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    return millis.count();
}


bool ISearch::checkSquize(int first, int second, int ni, int nj, bool allowsqueeze, bool cutcorners, const Map &map) {
    int dx = ni - first;
    int dy = nj - second;
    if (dx != 0 && dy != 0) {
        if (!cutcorners && (map.CellIsObstacle(first, nj) || map.CellIsObstacle(ni, second)))
            return false;
        else if (!allowsqueeze && (map.CellIsObstacle(first, nj) && map.CellIsObstacle(ni, second)))
            return false;
    }
    return true;
}


void ISearch::makePathes(Node curNode) {
    Node *cur = &curNode;
    int Dx = 2;
    int Dy = 2;
    while (cur->parent != nullptr) {
        int ndx = cur->parent->i - cur->i;
        int ndy = cur->parent->j - cur->j;
        if (ndx != Dx || ndy != Dy) {
            hppath.push_back(*cur);
        }
        Dx = ndx;
        Dy = ndy;
        lppath.push_back(*cur);
        cur = (cur->parent);
    }
    hppath.push_back(*cur);
    hppath.reverse();
    lppath.push_back(*cur);
    lppath.reverse();
    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
}

ISearch::~ISearch() {
}

