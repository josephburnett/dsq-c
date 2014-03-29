#ifndef PHASE1_H_INCLUDED
#define PHASE1_H_INCLUDED

/* Returns an MST representing time-to-effect based clustering structure
 * Returns a graph the user is responsible for freeing */
struct graph* mstOfPosition(struct position* pos);

/* Breaks a position into subproblems according an MST and Tmax threshold */
/* Returns a list of subproblems the user is responsible for freeing */
struct subproblem* buildSubproblems(struct position* pos, struct graph* mst, int tmax);

/* Creates the complete graph of a position according to the metric T[u,v]
 * Returns a graph the user is responsible for freeing */
struct graph* graphOfPosition(struct position* pos);

/* Finds an MST of a graph
 * Implimentation of Prim's MST algorithm
 * Returns a graph the user is responsible for freeing */
struct graph* mstOfGraph(struct graph* g);

/* Removes all edges from a graph greater than threshold
 * Returns the original modified graph */
void removeGreaterEdges(struct graph* g, int threshold);

/* Constructs a new position based on p with pieces (vertices) in component
 * Any vertex in a component can be used to identify the component
 * Returns a position the user is responsible for freeing */
struct position* positionFromComponent(struct graph* g, struct position* p, int component);

/* Determines time-to-effect for pieces u,v -- T[u,v]
 * T[u,v]'s value depends on the relative piece strength and sides */
int computeT(int u, int uLoc, int v, int vLoc);

/* Determines height of intersection for u,v effect cones -- D[u,v]
 * D[u,v] is the minimum height of the sum of effectcones u,v */
int computeD(int u, int uLoc, int v, int vLoc);

/* Projects the effect cone for piece whose height is distance from origin
 * Returns an effectcone the user is responsible for freeing */
struct effectcone* projectEffectCone(int piece, int origin);
/* Helper function for projectEffectCone */
void enqueueAdjacentSquares(int piece, int square, int height, int Q[], int H[], int* tail);

/* Sums two effect cones by summing the height at each square
 * Returns an effectcone the user is responsible for freeing */
struct effectcone* sumOfEffectcones(struct effectcone* a, struct effectcone* b);

/* Finds the minimum height of an effect cone across all squares */
int minimumHeightOfEffectcone(struct effectcone* a);

/* Project an effectcone from position which represents distance from any piece
 * A cumulative effectcone is the minima of effectcones for all pieces
 * Returns an effectcone the user is responsible for freeing */
struct effectcone* projectCumulativeEffectcone(struct position* pos);

/* Take the minima of two effectcones
 * The minima of effectcones is the minimum height for each square
 * Returns an effectcone the user is responsible for freeing */
struct effectcone* minimaOfEffectcones(struct effectcone* a, struct effectcone* b);

#endif // PHASE1_H_INCLUDED
