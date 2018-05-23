#ifndef THETA_H
#define THETA_H
#include "astar.h"

class Theta: public Astar
{
    public:
        Theta(double hweight, bool breakingties):Astar(hweight, breakingties){}
        ~Theta(void);

    protected:
        bool lineOfSight(int i1, int j1, int i2, int j2, const Map &map, std::list<Node>* passed);
        Node resetParent(Node current, Node* parent, const Map &map, const EnvironmentOptions &options) override;
        void makePathes(Node curNode, const Map &map) override;
};


#endif // THETA_H
