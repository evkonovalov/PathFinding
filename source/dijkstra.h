#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "astar.h"

class Dijkstra : public Astar
{
    public:
        Dijkstra();
    protected:
        double computeHFromCellToCell(double i1, double j1, double i2, double j2, const EnvironmentOptions &options) override;
};
#endif
