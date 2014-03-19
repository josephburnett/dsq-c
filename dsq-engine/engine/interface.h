#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "phase1.h"
#include "phase2.h"

struct move* search(struct position* pos, enum side player, int maxDepth, int tmax, double seconds);

#endif // INTERFACE_H_INCLUDED
