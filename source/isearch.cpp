#include "isearch.h"

ISearch::ISearch() {
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
}

bool compGmax(const Node* a, const Node* b) {
    if(a->F == b->F){
        return a->g < b->g;
    } else
        return a->F < b->F;
}

bool compGmin(const Node* a, const Node* b) {
    if(a->F == b->F){
        return a->g > b->g;
    } else
        return a->F < b->F;
}

Node* findPtr(std::vector<Node*>& vec, Node* c){
    for(auto i: vec){
        if(*i == *c)
            return i;
    }
    return nullptr;
}

ISearch::~ISearch(void) {}

SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options) {
    auto comp = &compGmax;
    if(!breakingties)
        comp = &compGmin;
    sresult.time = currTimeInMillSeconds();
    sresult.lppath = nullptr;
    sresult.hppath = nullptr;
    sresult.nodescreated = 0;
    sresult.numberofsteps = 0;
    sresult.pathfound = false;
    sresult.pathlength = 0;
    int i1 = map.getMapStartI();
    int i2 = map.getMapGoalI();
    int j1 = map.getMapStartJ();
    int j2 = map.getMapGoalJ();
    Node* cur = new Node(i1,j1);
    open.push_back(cur);
    std::vector<int> dx = {-1, 0, 1, 0, 1, -1, 1, -1};
    std::vector<int> dy = {0, 1, 0, -1, 1, -1, -1, 1};
    if (!options.allowdiagonal) {
        dx = {1, -1, 0, 0};
        dy = {0, 0, -1, 1};
    }

    while (!open.empty()) {
        sresult.numberofsteps++;
        auto t = open.begin();
        cur = *t;
        if (cur->i == i2 && cur->j == j2) {
            makePrimaryPath(*cur);
            sresult.pathlength = static_cast<float>(sresult.lppath->back().g);
            sresult.pathfound = true;
            sresult.nodescreated = static_cast<unsigned int>(open.size() + close.size());
            sresult.time = (currTimeInMillSeconds() - sresult.time) / 1000;
            for(auto i: open)
                delete i;
            for(auto i: close)
                delete i.second;
            return sresult;
        }
        std::pair<std::pair<int,int>, Node*> n;
        n.first.first = cur->i;
        n.first.second = cur->j;
        n.second = cur;
        close.insert(n);
        open.erase(t);
        Node* s = cur;
        for (int i = 0; i < dx.size(); i++) {
            int ni = cur->i + dx[i];
            int nj = cur->j + dy[i];
            Node* ncur = new Node(ni,nj);
            if (map.CellOnGrid(ni, nj) && !map.CellIsObstacle(ni, nj) &&
                checkSquize(cur->i, cur->j, ni, nj, options.allowsqueeze, options.cutcorners, map)) {
                if (close.find(std::pair<int,int>(ni,nj)) != close.end())
                    continue;
                auto tmp = findPtr(open, ncur);
                if (tmp == nullptr) {
                    ncur->H = computeHFromCellToCell(ni, nj, i2, j2, options);
                    if (dx[i] && dy[i])
                        ncur->g = cur->g + sqrt(2);
                    else
                        ncur->g = cur->g + 1;
                    ncur->F = ncur->g + ncur->H;
                    ncur->parent = s;
                    auto plc = lower_bound(open.begin(),open.end(),ncur,comp);
                    open.insert(plc,ncur);
                } else {
                    double val;
                    if (dx[i] && dy[i])
                        val = cur->g + (tmp)->H + sqrt(2);
                    else
                        val = cur->g + (tmp)->H + 1;
                    if (val >= (tmp)->F)
                        continue;
                    (tmp)->parent = s;
                    if (dx[i] && dy[i])
                        (tmp)->g = cur->g + sqrt(2);
                    else
                        (tmp)->g = cur->g + 1;
                    (tmp)->F = val;
                    delete ncur;
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


bool ISearch::checkSquize(int first, int second, int ni, int nj, bool allowsqueeze, bool cutcorners, const Map& map) {
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


/*std::list<Node> ISearch::findSuccessors(Node curNode, const Map &map, const EnvironmentOptions &options)
{
    std::list<Node> successors;
    //need to implement
    return successors;
}*/

void ISearch::makePrimaryPath(Node curNode)
{
    Node* cur = &curNode;
    auto *lp = new std::list<Node>();
            auto *hp = new std::list<Node>();
            int Dx = 2;
            int Dy = 2;
            while (!(cur->parent == nullptr)) {
                int ndx = cur->parent->i - cur->i;
                int ndy = cur->parent->j - cur->j;
                if(ndx != Dx || ndy != Dy){
                    hp->push_back(*cur);
                }
                Dx = ndx;
                Dy = ndy;
                lppath.push_back(*cur);
                lp->push_back(*cur);
                cur = (cur->parent);
            }
            hp->push_back(*cur);
            hp->reverse();
            lppath.push_back(*cur);
            lppath.reverse();
            lp->push_back(*cur);
            lp->reverse();
            sresult.lppath = lp;
            sresult.hppath = hp;
}

/*void ISearch::makeSecondaryPath()
{
    //need to implement
}*/
