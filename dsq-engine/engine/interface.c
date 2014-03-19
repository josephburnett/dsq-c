#include "common.h"
#include "interface.h"


struct move* search(struct position* pos, enum side player, int maxDepth, int tmax, double seconds)
{
    time_t startTime;

    struct move* suggestedMove = NULL;
    struct graph* mst = NULL;
    struct subproblem* subproblems = NULL;

    // start the clock
    time(&startTime);

    // phase 1
    // decompose the position into subproblems
    mst = mstOfPosition(pos);
    if (!mst) return NULL;
    subproblems = buildSubproblems(pos,mst,tmax);
    freeGraph(mst);
    if (!subproblems) return NULL;

    // phase 2
    // search subproblems
    suggestedMove = searchSubproblems(subproblems,player,maxDepth,startTime,seconds);
    freeSubproblems(subproblems);
    if (!suggestedMove) return NULL;

    return suggestedMove;
}


