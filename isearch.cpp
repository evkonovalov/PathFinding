#include <w32api/ntdef.h>
#include "isearch.h"

ISearch::ISearch() {
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
}

ISearch::~ISearch(void) {}


SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options) {
    (*this).map = map;
    double r = (*this).computeHFromCellToCell(map.getMapStartI(), map.getMapStartJ(), map.getMapGoalI(),
                                              map.getMapGoalJ(), options);
    return sresult;
}

long ISearch::currTimeInMillSeconds() {
    auto time = std::chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    return millis.count();
}

int ISearch::delta(int start_i, int start_j, int fin_i, int fin_j, int metrictype) {
    int dx = abs(start_i - fin_i);
    int dy = abs(start_j - fin_j);
    if (metrictype == CN_SP_MT_DIAG) {
        return 1;
    } else if (metrictype == CN_SP_MT_MANH) {
        return dx + dy;
    } else if (metrictype == CN_SP_MT_EUCL) {
        return 1;
    } else {
        if (dx > dy)
            return dx;
        else
            return dy;
    }
}

bool ISearch::checkSquize(int first, int second, int ni, int nj, bool allowsqueeze, bool cutcorners) {
    int dx = ni - first;
    int dy = nj - second;
    if(dx != 0 && dy != 0){
        if(!cutcorners && (map.CellIsObstacle(first,nj) || map.CellIsObstacle(ni,second)))
            return false;
        else if (!allowsqueeze && (map.CellIsObstacle(first,nj) && map.CellIsObstacle(ni,second)))
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

/*void ISearch::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void ISearch::makeSecondaryPath()
{
    //need to implement
}*/
