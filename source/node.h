#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <cmath>

struct Node {
    int i, j;
    double F, g, H;
    Node *parent;
    bool bk;

    bool operator==(const Node &other) const {
        return i == other.i && j == other.j;
    }

    bool operator!=(const Node &other) const {
        return i != other.i || j != other.j;
    }

    bool operator<(const Node &other) const {
        if (fabs(F - other.F) < 0.1) {
            if (g != other.g) {
                if (!bk)
                    return g < other.g;
                else
                    return g > other.g;
            } else {
                if(other.i != i)
                    return i < other.i;
                else if(other.j != j)
                    return j < other.j;
            }
        }
        return F < other.F;
    }

    Node(int ni, int nj, bool breakingties) {
        bk = breakingties;
        i = ni;
        j = nj;
        F = 0;
        g = 0;
        H = 0;
        parent = nullptr;
    }
};

#endif
