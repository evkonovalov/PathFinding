//
// Created by DNS on 26.08.2018.
//

#ifndef ASEARCH_ANYA_H
#define ASEARCH_ANYA_H


#include <unordered_set>
#include "theta.h"


struct AnyaPoint {
    double x, y;

    AnyaPoint(double nx, double ny) {
        x = nx;
        y = ny;
    }
};

struct AnyaInterval {
    AnyaPoint right = AnyaPoint(0, 0);
    AnyaPoint left = AnyaPoint(0, 0);
    bool rightOpen;
    bool leftOpen;
};

struct AnyaNode {
    AnyaPoint r = AnyaPoint(0, 0);
    AnyaInterval i;
    const AnyaNode *parent = nullptr;
    double H, F, G;

    AnyaNode(AnyaPoint p, AnyaPoint l, AnyaPoint r) {
        this->r = p;
        this->i.left = l;
        this->i.right = r;
    }

    AnyaNode() {
    }

    AnyaNode(AnyaPoint p, AnyaInterval in) {
        this->r = p;
        this->i = in;
    }

    bool operator== (const AnyaNode &other) const{
        return (fabs(other.r.x - r.x) < 0.1) && (fabs(other.r.y - r.y) < 0.1) &&
                (fabs(other.i.right.x - i.right.x) < 0.1) && (fabs(other.i.right.y - i.right.y) < 0.1 ) &&
                (fabs(other.i.left.x - i.left.x) < 0.1 ) && (fabs(other.i.left.y - i.left.y) < 0.1);
    }

    bool operator!= (const AnyaNode &other) const{
        return !(*this == other);
    }

    bool operator<(const AnyaNode &other) const {
        if (fabs(F - other.F) < 0.1) {
            if (G != other.G) {
                return G < other.G;
            } else {
                if (fabs(other.r.x - r.x) > 0.1)
                    return r.x < other.r.x;
                else if (fabs(other.r.y - r.y) > 0.1)
                    return r.y < other.r.y;
                else if(fabs(other.i.right.x - i.right.x) > 0.1)
                    return i.right.x < other.i.right.x;
                else if(fabs(other.i.right.y - i.right.y) > 0.1 )
                    return i.right.y < other.i.right.y;
                else if(fabs(other.i.left.x - i.left.x) > 0.1)
                    return i.left.x < other.i.left.x;
                else if (fabs(other.i.left.y - i.left.y) > 0.1)
                    return i.left.y < other.i.left.y;
                else
                    return false;
            }
        }
        return F < other.F;
    }
};


class Anya : public Theta {
    public:
        Anya(double hweight, bool breakingties) : Theta(hweight, breakingties) {}

        ~Anya(void);

    private:
        SearchResult
        startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options, bool visual) override;

        void makePathes(AnyaNode curNode, const Map &map);

        AnyaNode resetParent(AnyaNode current, const AnyaNode *parent, const Map &map, const EnvironmentOptions &options);

        void image(const Map &map) override;

        void findSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map, const EnvironmentOptions &options);

        std::set<AnyaNode> open;
        std::list<AnyaNode> close;

        bool isCornerPoint(int i, int j, const Map &map);

        void generateStartSuccessors(AnyaPoint begin, std::set<AnyaNode> &suc, const Map &map);

        void generateFlatSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map,
                                    const EnvironmentOptions &options);

        bool checkLine(AnyaPoint a, AnyaPoint b, const Map &map);


        bool isPassableShift(int x, int y, int dx, int dy, const Map &map);

        bool isTouchableCross(int x, int y, const Map &map);

        void generateConeSuccessors(AnyaNode cur, std::set<AnyaNode> &suc, const Map &map,
                                    const EnvironmentOptions &options);

        bool checkSquare(int x, int y, const Map &map);

        AnyaPoint getIntPoint(AnyaNode node, const Map &map);

        AnyaPoint findEndPoint(AnyaPoint x1, AnyaPoint x2, double cor);

        void initGHF(AnyaNode *node, const Map &map);

        double dist(double x1, double y1, double x2, double y2);

        AnyaPoint findEndPoint(AnyaPoint x1, AnyaPoint x2);

        bool checkForEnd(int i, double x, int dx);

        bool isPassableCross(double x, double y, const Map &map);

        void move(double *cor, int delta);
};

#endif //ASEARCH_ANYA_H
