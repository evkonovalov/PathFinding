#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <list>
#include "node.h"

struct SearchResult {
    bool pathfound;
    float pathlength; //if path not found, then pathlength=0
    const std::list<Node> *lppath; //path as the sequence of adjacent nodes (see above)
    //This is a pointer to the list of nodes that is actually created and hadled by ISearch class,
    //so no need to re-create them, delete them etc. It's just a trick to save some memory
    const std::list<Node> *hppath; //path as the sequence of non-adjacent nodes: "sections" (see above)
    //This is a pointer to the list of nodes that is actually created and hadled by ISearch class,
    //so no need to re-create them, delete them etc. It's just a trick to save some memory
    unsigned int nodescreated; //|OPEN| + |CLOSE| = total number of nodes saved in memory during search process.
    unsigned int numberofsteps; //number of iterations (expansions) made by algorithm to find a solution
    double time; //runtime of the search algorithm (expanding nodes + reconstructing the path)
    SearchResult() {
        pathfound = false;
        pathlength = 0;
        lppath = nullptr;
        hppath = nullptr;
        nodescreated = 0;
        numberofsteps = 0;
        time = 0;
    }

};

#endif
