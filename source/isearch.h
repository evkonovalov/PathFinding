#ifndef ISEARCH_H
#define ISEARCH_H

#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <limits>
#include <chrono>
#include <cmath>
#include <set>

class ISearch {
    public:
        ISearch();

        ~ISearch(void);

        SearchResult startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, bool visual);

    protected:
        bool checkSquize(int i1, int j1, int i2, int j2, const Map &map);

        long currTimeInMillSeconds();

        virtual double computeHFromCellToCell(int start_i, int start_j, int fin_i, int fin_j,
                                              const EnvironmentOptions &options) { return 0; }

        virtual void makePathes(Node curNode, const Map &map);

        virtual Node resetParent(Node current, Node *parent, const Map &map, const EnvironmentOptions &options);

        void image(const Map &map);

        virtual void findSuccessors(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options);

        virtual void neighbours(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options);

        SearchResult sresult;
        std::list<Node> lppath, hppath;
        double hweight;
        bool breakingties;
        std::set<Node> open;
        std::unordered_map<int, Node> close;
        bool cutcorners;
        bool allowsqueeze;
};

#endif
