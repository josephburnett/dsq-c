#ifndef TESTGAME_H_INCLUDED
#define TESTGAME_H_INCLUDED

#include "phase1.h"
#include "phase2.h"

void testfile(char* filename);

struct move* testSearch(FILE* OUTFILE, struct position* pos, enum side player, int maxDepth, int tmax, double seconds);

struct move* testSearchSubproblems(FILE* OUTFILE, struct subproblem* sub, enum side player, int maxDepth, time_t startTime, double seconds);

void fprintPosition(FILE* OUTFILE, struct position* p);

void fprintBoard(FILE* OUTFILE, struct position* pos);


#endif // TESTGAME_H_INCLUDED
