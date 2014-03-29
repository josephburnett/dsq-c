#ifndef DATATYPES_H_INCLUDED
#define DATATYPES_H_INCLUDED

#define MAXDEPTH 100    // maximum search depth
#define MAXDURATION 200 // maximum duration of a game

enum side
{
    A,
    B,
    NONE
};

enum limits
{
    ONLIMITS,
    OFFLIMITS
};

enum pieceType
{
    MOUSE,
    CAT,
    WOLF,
    DOG,
    HYENA,
    TIGER,
    LION,
    ELEPHANT,
    EMPTY
};

enum operation
{
    MAKEMOVE,
    MAKENONMOVE,
    UNDOMOVE,
    RESETSTACK
};

enum terrain
{
    NORMAL,
    WATER,
    ATRAP,
    BTRAP,
    ADEN,
    BDEN
};

struct move
{
    int from;
    int to;
    int score;
};

struct position
{
    int loc[16];
    struct move hist[MAXDURATION];
    int histIndex;
};

struct edge
{
    int to;
    int weight;
    struct edge* next;
};

struct vertex
{
    int label;
    struct edge* edges;
    struct vertex* next;
};

struct graph
{
    struct vertex* vertices;
};

struct partition
{
    enum limits lim[63];
};

struct subproblem
{
    struct position* pos;
    struct partition* part;
    struct subproblem* next;
};

struct movePair
{
    struct move mv;
    struct move nm;
};

struct effectcone
{
    int height[63];
};


#endif // DATATYPES_H_INCLUDED












