#ifndef ASTAR_H
#define ASTAR_H

#include "isearch.h"

class Astar : public ISearch {
    public:
        Astar(double HW, bool BT);

    protected:
        double computeHFromCellToCell(double i1, double j1, double i2, double j2, const EnvironmentOptions &options) override;
};

#endif
