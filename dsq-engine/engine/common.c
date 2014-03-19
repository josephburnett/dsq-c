#include "common.h"

void printPosition(struct position* p)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if (p->loc[i] >= 0) // in play
        {
            printf("%i at %i\n",i,p->loc[i]);
        }
    }
}

void printBoard(struct position* pos)
{
    int i,j,k;

    printf("   +--+--+--+--+--+--+--+\n");
    for (i = 0; i < 63; i = i + 7)
    {
        printf(" %i ",9-i/7);

        for (j = 0; j < 7; j++)
        {
            int p = -1;
            for (k = 0; k < 16; k++)
            {
                if (pos->loc[k] == i+j)
                {
                    p = k;
                }
            }
            switch (p)
            {
                case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                    printf("|a%i",p+1);
                    break;
                case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
                    printf("|b%i",p-7);
                    break;
                default:
                    switch (i+j)
                    {
                        case 3: case 59:
                            printf("| d");
                            break;
                        case 2: case 4: case 10: case 52: case 58: case 60:
                            printf("| t");
                            break;
                        case 22: case 23: case 25: case 26:
                        case 29: case 30: case 32: case 33:
                        case 36: case 37: case 39: case 40:
                            printf("| w");
                            break;
                        default:
                            printf("|  ");
                    }
            }
        }
        printf("|\n   +--+--+--+--+--+--+--+\n");
    }
    printf("     A  B  C  D  E  F  G\n");
}

void printGraph(struct graph* g)
{
    struct vertex* v;
    struct edge* e;

    v = g->vertices;

    while (v)
    {
        printf("%i: ",v->label);
        e = v->edges;

        while (e)
        {
            printf("-> %i(%i) ",e->to,e->weight);
            e = e->next;
        }

        printf("\n");
        v = v->next;
    }
}

void printEffectcone(struct effectcone* cone)
{
    int i,j;

    for (i = 0; i < 63; i = i + 7)
    {
        for (j = 0; j < 7; j++)
        {
            printf("%i ",cone->height[i+j]);
        }
        printf("\n");
    }
}

void printPartition(struct partition* part)
{
    int i,j;

    for (i = 0; i < 63; i = i + 7)
    {
        for (j = 0; j < 7; j++)
        {
            if (part->lim[i+j] == ONLIMITS)
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
        printf("\n");
    }
}

void printSubproblems(struct subproblem* sub)
{
    int count = 0;
    struct subproblem* s;

    s = sub;
    while (s)
    {
        count++;
        printf("Subproblem %i:\n",count);

        printPosition(s->pos);
        printPartition(s->part);
        printf("\n");
        s = s->next;
    }
}

void freeGraph(struct graph* g)
{
    struct edge* e;
    struct vertex* v;

    while (g->vertices)
    {
        while (g->vertices->edges)
        {
            e = g->vertices->edges;
            g->vertices->edges = e->next;
            free(e);
        }
        v = g->vertices;
        g->vertices = v->next;
        free(v);
    }

    free(g);
}

void freeSubproblems(struct subproblem* sub)
{
    struct subproblem* s;
    struct subproblem* lastS;

    s = sub;
    while (s)
    {
        free(s->pos);
        free(s->part);

        lastS = s;
        s = s->next;

        free(lastS);
    }
}

void adjacentSquares(int piece, int from, int to[], int hash[])
{
    int i = 0;

    enum pieceType type = indexToType(piece);

    switch (from) {
    case 1: case 2: case 3: case 4: case 5: // top row
        to[i++] = from + 7; // down
        to[i++] = from - 1; // left
        to[i++] = from + 1; // right
        break;
    case 57: case 58: case 59: case 60: case 61: // bottom row
        to[i++] = from - 7; // up
        to[i++] = from - 1; // left
        to[i++] = from + 1; // right
        break;
    case 7: case 14: case 42: case 49: // left column (excluding aisle)
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        to[i++] = from + 1; // right
        break;
    case 13: case 20: case 48: case 55: // right column (excluding aisle)
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        to[i++] = from - 1; // left
        break;
    case 0: // upper-left corner
        to[i++] = from + 7; // down
        to[i++] = from + 1; // right
        break;
    case 6: // upper-right corner
        to[i++] = from + 7; // down
        to[i++] = from - 1; // left
        break;
    case 56: // lower-left corner
        to[i++] = from - 7; // up
        to[i++] = from + 1; // right
        break;
    case 62: // lower-right corner
        to[i++] = from - 7; // up
        to[i++] = from - 1; // left
        break;
    case 15: case 16: case 18: case 19: // top of water
        to[i++] = from - 7; // up
        if (type == MOUSE) to[i++] = from + 7; // down
        if (((type == LION) || (type == TIGER)) &&
            (hash[from+7] == -1 && hash[from+14] == -1 && hash[from+21] == -1)) // clear water
            to[i++] = from + 28; // down
        to[i++] = from - 1; // left
        to[i++] = from + 1; // right
        break;
    case 43: case 44: case 46: case 47: // base of water
        if (type == MOUSE) to[i++] = from - 7; // up
        if (((type == LION) || (type == TIGER)) &&
            (hash[from-7] == -1 && hash[from-14] == -1 && hash[from-21] == -1)) // clear water
            to[i++] = from - 28; // up
        to[i++] = from + 7; // down
        to[i++] = from - 1; // left
        to[i++] = from + 1; // right
        break;
    case 21: case 28: case 35: // left aisle
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        if (type == MOUSE) to[i++] = from + 1; // right
        if (((type == LION) || (type == TIGER)) &&
            (hash[from+1] == -1 && hash[from+2] == -1)) // clear water
            to[i++] = from + 3; // right
        break;
    case 27: case 34: case 41: // right aisle
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        if (type == MOUSE) to[i++] = from - 1; // left
        if (((type == TIGER) || (type == LION)) &&
            (hash[from-1] == -1 && hash[from-2] == -2)) // clear water
            to[i++] = from - 3; // left
        break;
    case 24: case 31: case 38: // center aisle
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        if (type == MOUSE) {
            to[i++] = from - 1; // left
            to[i++] = from + 1; // right
        }
        if ((type == TIGER) || (type == LION)) {
            if (hash[from-1] == -1 && hash[from-2] == -1) // clear water
                to[i++] = from - 3; // left
            if (hash[from+1] == -1 && hash[from+2] == -1) // clear water
                to[i++] = from + 3; // right
        }
        break;
    default:
        to[i++] = from - 7; // up
        to[i++] = from + 7; // down
        to[i++] = from - 1; // left
        to[i++] = from + 1; // right
        break;
    }

    to[i] = -1; // no more moves
}

int typeToIndex(enum pieceType type, char side)
{
    if ((side == 'a') || (side == 'A'))
    {
        switch (type)
        {
            case MOUSE: return 0;
            case CAT: return 1;
            case WOLF: return 2;
            case DOG: return 3;
            case HYENA: return 4;
            case TIGER: return 5;
            case LION: return 6;
            case ELEPHANT: return 7;
            default: return -1;
        }
    }
    else if ((side == 'b') || (side == 'B'))
    {
        switch (type)
        {
            case MOUSE: return 8;
            case CAT: return 9;
            case WOLF: return 10;
            case DOG: return 11;
            case HYENA: return 12;
            case TIGER: return 13;
            case LION: return 14;
            case ELEPHANT: return 15;
            default: return -1;
        }
    }
    else
    {
        return -1;
    }
}

enum pieceType indexToType(int p)
{
    switch (p)
    {
        case 0: case 8: return MOUSE;
        case 1: case 9: return CAT;
        case 2: case 10: return WOLF;
        case 3: case 11: return DOG;
        case 4: case 12: return HYENA;
        case 5: case 13: return TIGER;
        case 6: case 14: return LION;
        case 7: case 15: return ELEPHANT;
        default: return EMPTY;
    }
}

enum side indexToSide(int p)
{
    if ((p >= 0) && (p <= 7)) return A;
    if ((p >= 8) && (p <= 15)) return B;
    return NONE;
}

enum terrain indexToTerrain(int square)
{
    switch (square) {

    case 3:
        return BDEN;

    case 59:
        return ADEN;

    case 2: case 4: case 10:
        return BTRAP;

    case 52: case 58: case 60:
        return ATRAP;

    case 22: case 23: case 25: case 26:
    case 29: case 30: case 32: case 33:
    case 36: case 37: case 39: case 40:
        return WATER;

    default:
        return NORMAL;
    }
}

int alg2ind(char* cstring)
{
	if (strlen(cstring) != 2)
	{
		return -1;
	}

	int column, row, index;

	switch (cstring[0]) {
		case 'a': case 'A':
			column = 0;
			break;
		case 'b': case 'B':
			column = 1;
			break;
		case 'c': case 'C':
			column = 2;
			break;
		case 'd': case 'D':
			column = 3;
			break;
		case 'e': case 'E':
			column = 4;
			break;
		case 'f': case 'F':
			column = 5;
			break;
		case 'g': case 'G':
			column = 6;
			break;
		default:
			return -1;
			break;
	}

	row = 9 - atoi(&(cstring[1]));

	index = row * 7 + column;

	return index;
}

void ind2alg(int index, char* alg)
{
	if (index > 62 || index < 0)
	{
		return;
	}

	int row = 8 - index/7;
	int column = (index - (index/7)*7);

	switch (column)
	{
		case 0:
			alg[0] = 'a';
			break;
		case 1:
			alg[0] = 'b';
			break;
		case 2:
			alg[0] = 'c';
			break;
		case 3:
			alg[0] = 'd';
			break;
		case 4:
			alg[0] = 'e';
			break;
		case 5:
			alg[0] = 'f';
			break;
		case 6:
			alg[0] = 'g';
			break;
		default:
			alg[0] = '?';
			break;
	}

	alg[1] = '1' + row;		// offset char from '1'
	alg[2] = 0;				// null terminate string

}

int computeM(int uLoc, int vLoc)
{
    int uRow = uLoc / 7;
    int uCol = uLoc - (uRow * 7);
    int vRow = vLoc / 7;
    int vCol = vLoc - (vRow * 7);

    int deltaX = (uRow - vRow);
    int deltaY = (uCol - vCol);

    return abs(deltaX) + abs(deltaY);
}

struct position* rotatePosition(struct position* pos)
{
    struct position* newPos;
    int i;

    newPos = malloc(sizeof(struct position));
    if (!newPos) return NULL;

    for (i = 0; i < 16; i++)
    {
        int loc;

        loc = pos->loc[i];
        loc = oppositeSquare(loc);

        if (i < 8)
        {
            newPos->loc[i+8] = loc;
        }
        else
        {
            newPos->loc[i-8] = loc;
        }
    }

    return newPos;
}

int oppositeSquare(int square)
{
    if (square == -1) return -1;
    return 62-square;
}

int gameOver(struct position* pos)
{
    int i;
    int over;

    // player A has no pieces
    over = 1;
    for (i = 0; i < 8; i++)
    {
        if (pos->loc[i] != -1) over = 0;
    }
    if (over) return 1;

    // player B has no pieces
    over = 1;
    for (i = 8; i < 16; i++)
    {
        if (pos->loc[i] != -1) over = 0;
    }
    if (over) return 1;

    // a den is occupied
    for (i = 0; i < 16; i++)
    {
        if (pos->loc[i] == 3) return 1;
        if (pos->loc[i] == 59) return 1;
    }

    // game has exceeded max length
    if (pos->histIndex == MAXDURATION) return 1;

    return 0;
}


