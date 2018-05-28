#ifndef JP_SEARCH_H
#define JP_SEARCH_H

#include "astar.h"

class JP_Search : public Astar {
    public:
        JP_Search(float hweight, bool breakingties) : Astar(hweight, breakingties) {}

        ~JP_Search();

    private:
        void findSuccessors(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options) override;

        void neighbours(Node cur, std::set<Node> &suc, const Map &map, const EnvironmentOptions &options) override;;

        Node jump(Node node, int dx, int dy, const Map &map, const EnvironmentOptions &options);

        void neighbours(Node cur, int dirx, int diry, std::set<Node> &suc, const Map &map,
                        const EnvironmentOptions &options);

        void miniSearch(std::set<Node> &suc, int dirx, int diry, Node cur, const Map &map,
                        const EnvironmentOptions &options);

        Node resetParent(Node cur, Node *parent, const Map &map, const EnvironmentOptions &options) override;

        bool checkForced(Node node, int dx, int dy, std::set<Node> &neighb);

        void makePathes(Node curNode, const Map &map) override;

        void pushForward(int i, int j, int i1, int j1, std::list<Node> &pList);
};

#endif // JP_SEARCH_H
