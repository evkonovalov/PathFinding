#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H

#include "gl_const.h"

class EnvironmentOptions {
    public:
        EnvironmentOptions(bool AS, bool AD, bool CC, int MT = CN_SP_MT_EUCL);

        EnvironmentOptions();

        int metrictype;
        bool allowsqueeze;
        bool allowdiagonal;
        bool cutcorners;

};

#endif
