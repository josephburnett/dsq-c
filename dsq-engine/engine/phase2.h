#ifndef PHASE2_H_INCLUDED
#define PHASE2_H_INCLUDED

/* Suggests a move given a list of subproblems
 * Returns a move the user is responsible for freeing */
struct move* searchSubproblems(struct subproblem* sub, enum side player, int maxDepth, time_t startTime, double seconds);

/* Searches the moves and nonmove of a subproblem to the terminal depth
 * Returns a move pair that the use is responsible for freeing */
struct movePair* searchToDepth(struct position* pos, struct partition* part, enum side player, int maxDepth, time_t startTime, double seconds);

/* Fills a move array (mv) with all legal moves for a given player
 * Records the number of generated move in *tail
 * Excludes legal but out-of-bounds moves according to part */
void generateMoves(struct position* pos, struct partition* part, enum side player, struct move mv[], int* tail);

/* Order a list of moves from possibly best to possibly worst
     to improve the performance of alpha-beta pruning */
void orderMoves(struct position* pos, struct partition* part, enum side player, struct move uList[], struct move oList[], int count);

/* Makes a move or undoes a move using a static stack
 * Returns the modified position
 * This in an information strength module with operations MAKEMOVE, UNDOMOVE and RESETSTACK */
struct position* stackOperation (struct position* pos, struct move* mv, enum operation op);
/* The following are wrapper functions for stackOperation */
void makeMove(struct position* pos, struct move* mv);
void makeNonMove();
void undoMove(struct position* pos);
void resetStack();

/* Returns the score of a position by an evaluation function
 * Interaction between pieces is not considered, therefore
     individual scores for any partitioning can be summed */
int scorePosition(struct position* p);

/* Returns non-zero if piece u can legally take piece v
 * Assumes that vLoc is adjacent to uLoc by the movement graph of u */
int uTakesV(int u, int uLoc, int v, int vLoc);

/* Returns 1 if move is repeated, 0 otherwise */
int repeatedMove(int from, int to, struct move hist[], int histIndex);

#endif // PHASE2_H_INCLUDED
