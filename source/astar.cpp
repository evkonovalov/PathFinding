#include "astar.h"

Astar::Astar(double HW, bool BT) {
    hweight = HW;
    breakingties = BT;
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options) {
    int metrictype = options.metrictype;
    int dx = abs(i1 - i2);
    int dy = abs(j1 - j2);
    if (metrictype == CN_SP_MT_DIAG) {
        if (dx > dy) {
            return (sqrt(2) * dy + (dx - dy)) * hweight;
        } else {
            return (sqrt(2) * dx + (dy - dx)) * hweight;
        }
    } else if (metrictype == CN_SP_MT_MANH) {
        return (dx + dy) * hweight;
    } else if (metrictype == CN_SP_MT_EUCL) {
        return sqrt(dx * dx + dy * dy) * hweight;
    } else if (metrictype == CN_SP_MT_CHEB) {
        if (dx > dy)
            return dx * hweight;
        else
            return dy * hweight;
    }
}
