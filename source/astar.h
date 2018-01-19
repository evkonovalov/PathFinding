#ifndef ASTAR_H
#define ASTAR_H
#include "isearch.h"

//A* search.
class Astar : public ISearch
{
    public:
        Astar(double HW, bool BT);
    protected:
        virtual double computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options);
        virtual void search(const EnvironmentOptions &options);

};

#endif