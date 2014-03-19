#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "datatypes.h"
#include "phase1.h"
#include "phase2.h"
#include "interface.h"

// CONSTANTS
static int NEGINFTY = INT_MIN;
static int INFTY = INT_MAX;
int MOVEORDERING;

// OUTPUT

/* Prints a list of pieces and their locations */
void printPosition(struct position* p);
void fprintfPosition(FILE* OUTFILE, struct position* p);

/* Prints a representation of a position as a DSQ board */
void printBoard(struct position* pos);
void fprintBoard(FILE* OUTFILE, struct position* pos);

/* Prints a graph's adjacency list */
void printGraph(struct graph* g);

/* Prints the height of an effect cone for each square */
void printEffectcone(struct effectcone* cone);

/* Prints the partitioning of a board
 * ONLIMITS is printed as 1, OFFLIMITS is is printed as 0 */
void printPartition(struct partition* part);

/* Prints the position and partition of all subproblems in a list */
void printSubproblems(struct subproblem* sub);


// MEMORY MANAGEMENT

/* Properly release the memory of a graph */
void freeGraph(struct graph* g);

/* Properly release the memory of a list of subproblems */
void freeSubproblems(struct subproblem* sub);


// MOVEMENT GRAPHS

/* List the adjacent squares for a given position and piece type
 * Indexes of adjacent squares are returned in the to[] array
 * to[] must have at least 5 elements and is terminated with -1 */
void adjacentSquares(int piece, int from, int to[], int hash[]);


// UNIT CONVERSION

/* Converts a piece type and side to a piece index (label) */
int typeToIndex(enum pieceType type, char side);

/* Extracts a type from a piece index */
enum pieceType indexToType(int p);

/* Extracts a side from a piece index */
enum side indexToSide(int p);

/* Extracts a terrain type from a square index */
enum terrain indexToTerrain(int square);

/* Converts algebraic notation to board index */
int alg2ind(char* cstring);

/* Converts board index to algebraic notation */
void ind2alg(int index, char* alg);


// UTILITIES

/* Returns the Manhattan distance between two squares */
int computeM(int uLoc, int vLoc);

/* Rotates a position (like turning the board 180 degrees)
 * Returns a position the user is responsible for freeing */
struct position* rotatePosition(struct position* pos);

/* Returns the index opposite of square */
int oppositeSquare(int square);

/* Checks for game over condition */
int gameOver(struct position* pos);


// TESTING

/* Decomposes a position into subproblems according to tmax
 * Searches the subproblems for a move for player
 * Prints all subproblems and the suggested move */
void test1(struct position* p, int tmax, enum side player);

#endif // COMMON_H_INCLUDED
