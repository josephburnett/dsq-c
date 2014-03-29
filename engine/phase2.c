#include "common.h"
#include "phase2.h"

struct move* searchSubproblems(struct subproblem* sub, enum side player, int maxDepth, time_t startTime, double seconds)
{
    time_t now;

    int i,j,d;
    int maxImpliedScore;
    int count;
    int bestMoveIndex;

    struct move* bestMove;
    struct movePair* mvPairs[16];
    struct subproblem* s;

    // memory allocation
    bestMove = malloc(sizeof(struct move));
    if (!bestMove) return NULL;

    // make sure we can't undo past this point
    resetStack();

    // iterative deepening
    for (d = 1; d <= maxDepth; d++)
    {
        // a little progress indicator for the console
        //printf(".");

        // search the subproblems
        // count the subproblems
        count = 0;
        s = sub;
        while (s)
        {
            // search the next subproblem
            mvPairs[count] = searchToDepth(s->pos,s->part,player,d,startTime,seconds);

            // check the time
            time(&now);

            // time has elapsed
            if (difftime(now,startTime) > seconds)
            {
                // free the search results
                for (i = 0; i < count; i++)
                {
                    free(mvPairs[i]);
                }

                // return what we have so far
                return bestMove;
            }

            // error condition
            if (!(mvPairs[count]))
            {
                for (i = 0; i < count-1; i++) free(mvPairs[i]);
                free(bestMove);
                return NULL;
            }

            s = s->next;
            count++;
        }

        // initially no best move
        bestMoveIndex = -1;

        // set initial implied score
        if (player == A) maxImpliedScore = NEGINFTY;
        else maxImpliedScore = INFTY;

        // find the best subproblem to move in
        for (i = 0; i < count; i++)
        {
            // only consider subproblems with a move
            if (mvPairs[i]->mv.from != -1 && mvPairs[i]->mv.to != -1)
            {
                int impliedScore = 0;

                for (j = 0; j < count; j++)
                {
                    // sum all non-moves except the ith
                    if (j != i)
                    {
                        // prevent overflow of implied score
                        if (impliedScore == INFTY || mvPairs[j]->nm.score == INFTY)
                        {
                            impliedScore = INFTY;
                        }
                        // prevent underflow of implied score
                        else if (impliedScore == NEGINFTY || mvPairs[j]->nm.score == NEGINFTY)
                        {
                            impliedScore = NEGINFTY;
                        }
                        // add non-move score
                        else
                        {
                            impliedScore += mvPairs[j]->nm.score;
                        }
                    }
                }

                // add move score
                // prevent overflow of implied score
                if (impliedScore == INFTY || mvPairs[i]->mv.score == INFTY)
                {
                    impliedScore = INFTY;
                }
                // prevent underflow of implied score
                else if (impliedScore == NEGINFTY || mvPairs[i]->mv.score == NEGINFTY)
                {
                    impliedScore = NEGINFTY;
                }
                // plus the ith move
                else
                {
                    impliedScore += mvPairs[i]->mv.score;
                }

                // remember best move index
                if ((player == A && impliedScore > maxImpliedScore) ||
                    (player == B && impliedScore < maxImpliedScore) ||
                    (bestMoveIndex == -1))
                {
                    maxImpliedScore = impliedScore;
                    bestMoveIndex = i;
                }

            } // end if subproblem has a move

        } // end for each subproblem

        // if there are no moves available return null
        if (bestMoveIndex ==  -1)
        {
            free(bestMove);
            for (i = 0; i < count; i++)
            {
                free(mvPairs[i]);
            }

            return NULL;
        }

        // update the best move
        bestMove->from = mvPairs[bestMoveIndex]->mv.from;
        bestMove->to = mvPairs[bestMoveIndex]->mv.to;
        bestMove->score = maxImpliedScore;

        // free the search results
        for (i = 0; i < count; i++)
        {
            free(mvPairs[i]);
        }

        // prioritize shallow wins by short circuiting iterative deepening
        if ((player == A && maxImpliedScore == INFTY) ||
            (player == B && maxImpliedScore == NEGINFTY))
        {
            // DEBUG
            // Why am I not seeing this show up?
            // printf("I just prioritized a shallow win.  So there.\n");

            d = maxDepth;
        }

    } // end iterative deepening

    return bestMove;
}

struct movePair* searchToDepth(struct position* pos, struct partition* part, enum side player, int maxDepth, time_t startTime, double seconds)
{
    int i;

    time_t now;

    int cl;                     // current level
    int cm[MAXDEPTH];           // current move
    int mc[MAXDEPTH];           // move count
    int bestScore[MAXDEPTH+1];  // best score
    int* bs = bestScore + 1;    // new index 0 of bestscore to allow bs[-1]

    struct move* ml[MAXDEPTH];  // move list
    struct movePair* bm;        // best move
    struct move temp[64];       // temporary list for move ordering

    enum side cp;               // current player

    // memory allocation
    bm = malloc(sizeof(struct movePair));
    if (!bm) return NULL;

    bm->nm.from = -1;
    bm->nm.to = -1;

    // two dimensional array of moves
    for (i = 0; i < MAXDEPTH; i++)
    {
        ml[i] = malloc(sizeof(struct move)*64);
        if (!(ml[i]))
        {
            for (i--; i >= 0; i--) free(ml[i]);
            free(bm);
            return NULL;
        }
    }

    // start with the correct player
    cp = player;

    // set initial best scores
    if (cp == A)
    {
        bs[-1] = INFTY;
        bs[0] = NEGINFTY;
        bm->mv.score = NEGINFTY;
        bm->nm.score = NEGINFTY;
    }
    else
    {
        bs[-1] = NEGINFTY;
        bs[0] = INFTY;
        bm->mv.score = INFTY;
        bm->nm.score = INFTY;
    }

    // generate level 0 moves
    if (MOVEORDERING)
    {
        generateMoves(pos,part,cp,temp,&(mc[0]));
        orderMoves(pos,part,cp,temp,ml[0],mc[0]);
    }
    else
    {
        generateMoves(pos,part,cp,ml[0],&(mc[0]));
    }
    cm[0] = -1; // implied non-move

    // default to first generated move
    if (mc[0] > 0)
    {
        bm->mv.from = ml[0][0].from;
        bm->mv.to = ml[0][0].to;
    }
    // no moves available
    else
    {
        bm->mv.from = -1;
        bm->mv.to = -1;
    }

    // start at the top
    cl = 0;

    // while there are root moves to explore
    while (cl >= 0)
    {
        // check the time
        time(&now);

        // time has elapsed
        if (difftime(now,startTime) > seconds)
        {
            // free the allocated memory
            for (i = 0; i < MAXDEPTH; i++)
            {
                free(ml[i]);
            }
            free(bm);

            // search did not complete
            return NULL;
        }

        // go up
        // terminal depth or no more moves at current level)
        if (cl == maxDepth || mc[cl] == cm[cl])
        {
            // score a terminal position
            if (cl == maxDepth)
            {
                bs[cl] = scorePosition(pos);
            }

            // undo last move or non-move
            undoMove(pos);

            // go up a level
            cl--;

            // switch players
            if (cp == A) cp = B;
            else cp = A;

            // maximize or minimize
            if ((cp == A && bs[cl] < bs[cl+1]) ||   // maximize
                (cp == B && bs[cl] > bs[cl+1]))     // minimize
            {
                // bring up the score
                bs[cl] = bs[cl+1];

                // remember best move and score
                if (cl == 0 && cm[cl] > 0)
                {
                    bm->mv.score = bs[cl];
                    bm->mv.from = ml[0][cm[0]-1].from;
                    bm->mv.to = ml[0][cm[0]-1].to;
                }
            }

            // completed level 0 non-move
            if (cl == 0 && cm[cl] == 0)
            {
                // remember non-move score
                bm->nm.score = bs[0];

                // set initial best score again
                if (player == A) bs[0] = NEGINFTY;
                else bs[0] = INFTY;
            }

            // alpha-beta pruning
            else if ((cp == A && bs[cl] >= bs[cl-1]) ||
                    (cp == B && bs[cl] <= bs[cl-1]))
            {
                // no more moves need to be explored
                cm[cl] = mc[cl];
            }
        }

        // go down
        else
        {
            // make a move or non-move
            if (cm[cl] == -1)
            {
                makeNonMove();
            }
            else
            {
                makeMove(pos,&(ml[cl][cm[cl]]));
            }
            cm[cl]++;

            // go down a level
            cl++;

            // switch players
            if (cp == A) cp = B;
            else cp = A;

            // pull down initial score from two levels above
            bs[cl] = bs[cl-2];

            // generate current level moves
            if (MOVEORDERING)
            {
                generateMoves(pos,part,cp,temp,&(mc[cl]));
                orderMoves(pos,part,cp,temp,ml[cl],mc[cl]);
            }
            else
            {
                generateMoves(pos,part,cp,ml[cl],&(mc[cl]));
            }
            cm[cl] = -1; // implied non-move
        }
    }

    for (i = 0; i < MAXDEPTH; i++) free(ml[i]);

    return bm;
}

void generateMoves(struct position* pos, struct partition* part, enum side player, struct move mv[], int* tail)
{
    int i,j;
    int to,from;
    int adj[5];
    int hash[63];

    // hash the pieces by location
    for (i = 0; i < 63; i++)
    {
        hash[i] = -1;
    }
    for (i = 0; i < 16; i++)
    {
        int loc = pos->loc[i];
        if (loc != -1) hash[loc] = i;
    }

    *tail = 0;

    // if the game is over, there are no moves
    if (hash[3] != -1 || hash[59] != -1) return;

    int start, finish;
    if (player == A)
    {
        start = 0;
        finish = 7;
    }
    else
    {
        start = 8;
        finish = 15;
    }

    // consider each piece
    for (i = start; i <= finish; i++)
    {
        from = pos->loc[i];

        // piece is in play
        if (from != -1)
        {
            // consider adjacent squares
            adjacentSquares(i,from,adj,hash);

            j = 0;
            while (adj[j] != -1)
            {
                to = adj[j];

                // square is onlimits
                // u can take v
                // is not a repeated move
                if ((part->lim[to] == ONLIMITS) &&
                    (uTakesV(i,from,hash[to],to)) &&
                    (!repeatedMove(from,to,pos->hist,pos->histIndex)))
                {
                    mv[*tail].from = from;
                    mv[*tail].to = to;
                    (*tail)++;
                }

                // DEBUG
                /*if ((part->lim[to] == ONLIMITS) &&
                    (uTakesV(i,from,hash[to],to)) &&
                    (repeatedMove(from,to,hist)))
                {
                    printf("Repeated move!\n");
                }*/

                j++;
            }
        }
    }
}

void orderMoves(struct position* pos, struct partition* part, enum side player, struct move uList[], struct move oList[], int count)
{
    int i, tail;
    int hash[63];

    // hash the pieces by location
    for (i = 0; i < 63; i++)
    {
        hash[i] = -1;
    }
    for (i = 0; i < 16; i++)
    {
        int loc = pos->loc[i];
        if (loc != -1) hash[loc] = i;
    }

    tail = 0;
    
    // capture moves
    for (i = 0; i < count; i++)
    {
        int to = uList[i].to;
        if (hash[to] != -1)
        {
            // copy the move
            oList[tail].from = uList[i].from;
            oList[tail].to = uList[i].to;
            
            // mark the move
            uList[i].from = -1;
            uList[i].to = -1;

            tail++;
        }
    }

    // lion and tiger moves
    int lion,tiger;
    if (player == A) lion = pos->loc[6];
    else lion = pos->loc[14];
    if (player == A) tiger = pos->loc[5];
    else tiger = pos->loc[13];

    for (i = 0; i < count; i++)
    {
        int from = uList[i].from;
        if ((from == tiger || from == lion) &&
            (from != -1))
        {
            // copy the move
            oList[tail].from = uList[i].from;
            oList[tail].to = uList[i].to;

            // mark the move
            uList[i].from = -1;
            uList[i].to = -1;

            tail++;
        }
    }

    // remaining moves
    for (i = 0; i < count; i++)
    {
        if (uList[i].from != -1 && uList[i].to != -1)
        {
            // copy the move
            oList[tail].from = uList[i].from;
            oList[tail].to = uList[i].to;

            // mark the move
            uList[i].from = -1;
            uList[i].to = -1;

            tail++;
        }
    }
}

struct position* stackOperation (struct position* pos, struct move* mv, enum operation op)
{
    // static stack
    static struct move moveStack[MAXDEPTH];
    static int displacedPiece[MAXDEPTH];
    static int top = 0;

    int i;

    switch (op) {

    case RESETSTACK:
        top = 0;
        break;

    case MAKEMOVE:

        moveStack[top].from = mv->from;
        moveStack[top].to = mv->to;
        displacedPiece[top] = -1;

        for (i = 0; i < 16; i++)
        {
            // save a displaced piece
            if (pos->loc[i] == mv->to)
            {
                displacedPiece[top] = i;
                pos->loc[i] = -1;
            }

            // make the move
            if (pos->loc[i] == mv->from)
            {
                pos->loc[i] = mv->to;
            }
        }

        // push move onto position history
        pos->hist[pos->histIndex].from = mv->from;
        pos->hist[pos->histIndex].to = mv->to;
        pos->histIndex++;

        top++;
        break;

    case MAKENONMOVE:

        // push a non-move
        moveStack[top].from = -1;
        moveStack[top].to = -1;
        displacedPiece[top] = -1;

        top++;
        break;

    case UNDOMOVE:

        if (!top) break;    // empty stack

        // pop the stack
        top--;

        // undo a non-move and break
        if (moveStack[top].to == -1 && moveStack[top].from == -1) break;

        // pop the position history
        pos->histIndex--;

        // undo a move
        for (i = 0; i < 16; i++)
        {
            if (pos->loc[i] == moveStack[top].to)
            {
                pos->loc[i] = moveStack[top].from;
            }
        }

        // replace a displaced piece
        if (displacedPiece[top] != -1)
        {
            pos->loc[displacedPiece[top]] = moveStack[top].to;
        }

        break;

    default:
        return pos;
    }

    return pos;
}

void makeMove(struct position* pos, struct move* mv)
{
    stackOperation(pos,mv,MAKEMOVE);
}

void makeNonMove()
{
    stackOperation(NULL,NULL,MAKENONMOVE);
}

void undoMove(struct position* pos)
{
    stackOperation(pos,NULL,UNDOMOVE);
}

void resetStack()
{
    stackOperation(NULL,NULL,RESETSTACK);
}

int scorePosition(struct position* p)
{
    const int MX = INFTY;

    const int mouse[] = {   11, 13, 50, MX, 50, 13, 13,
                            11, 12, 13, 50, 13, 13, 13,
                            10, 11, 11, 13, 13, 13, 13,
                            8,  9,  9,  11, 12, 12, 13,
                            8,  9,  9,  11, 12, 12, 13,
                            8,  8,  9,  10, 12, 12, 11,
                            8,  8,  8,  9,  10, 11, 10,
                            8,  8,  8,  9,  9,  9,  9,
                            8,  8,  8,  0,  8,  8,  8   };

    const int cat[] = {     10, 15, 50, MX, 50, 15, 11,
                            11, 11, 15, 50, 15, 11, 11,
                            10, 11, 11, 15, 11, 11, 10,
                            10, 0,  0,  10, 0,  0,  8,
                            10, 0,  0,  8,  0,  0,  8,
                            12, 0,  0,  8,  0,  0,  8,
                            12, 12, 10, 8,  8,  8,  8,
                            13, 10, 8,  8,  8,  8,  8,
                            8,  8,  8,  0,  8,  8,  8   };

    const int wolf[] = {    11, 15, 50, MX, 50, 15, 11,
                            10, 11, 15, 50, 15, 11, 10,
                            9,  10, 11, 15, 11, 10, 9,
                            9,  0,  0,  10, 0,  0,  9,
                            8,  0,  0,  8,  0,  0,  8,
                            8,  0,  0,  8,  0,  0,  8,
                            8,  8,  10, 8,  8,  8,  8,
                            8,  12, 13, 8,  8,  8,  8,
                            8,  12, 12, 0,  8,  8,  8   };

    const int dog[] = {     11, 15, 50, MX, 50, 15, 11,
                            10, 11, 15, 50, 15, 11, 10,
                            9,  10, 11, 15, 11, 10, 9,
                            9,  0,  0,  10, 0,  0,  9,
                            9,  0,  0,  9,  0,  0,  9,
                            9,  0,  0,  9,  0,  0,  9,
                            9,  9,  9,  9,  9,  9,  9,
                            9,  9,  9,  9,  13, 12, 8,
                            9,  9,  9,  0,  12, 12, 8   };

    const int hyena[] = {   14, 15, 50, MX, 50, 15, 14,
                            13, 14, 15, 50, 15, 14, 13,
                            13, 13, 14, 15, 14, 13, 13,
                            12, 0,  0,  15, 0,  0,  12,
                            11, 0,  0,  14, 0,  0,  11,
                            10, 0,  0,  13, 0,  0,  10,
                            9,  9,  9,  10, 10, 9,  9,
                            9,  9,  9,  9,  9,  9,  9,
                            9,  9,  9,  0,  9,  9,  9   };

    const int tiger[] = {   25, 30, 50, MX, 50, 30, 25,
                            25, 25, 30, 50, 30, 25, 25,
                            18, 20, 20, 30, 20, 20, 18,
                            15, 0,  0,  15, 0,  0,  15,
                            15, 0,  0,  15, 0,  0,  15,
                            15, 0,  0,  15, 0,  0,  15,
                            14, 16, 16, 14, 16, 16, 14,
                            12, 14, 12, 12, 12, 12, 12,
                            10, 12, 12, 0,  12, 12, 10  };

    const int lion[] = {    25, 30, 50, MX, 50, 30, 25,
                            25, 25, 30, 50, 30, 25, 25,
                            18, 20, 20, 30, 20, 20, 18,
                            15, 0,  0,  15, 0,  0,  15,
                            15, 0,  0,  15, 0,  0,  15,
                            15, 0,  0,  15, 0,  0,  15,
                            14, 16, 16, 14, 16, 16, 14,
                            12, 12, 12, 12, 12, 14, 12,
                            10, 12, 12, 0,  12, 12, 10  };

    const int elephant[] = {25, 30, 50, MX, 50, 30, 25,
                            25, 25, 30, 50, 30, 25, 25,
                            18, 20, 20, 30, 20, 20, 18,
                            16, 0,  0,  16, 0,  0,  16,
                            14, 0,  0,  14, 0,  0,  14,
                            12, 0,  0,  12, 0,  0,  12,
                            10, 15, 14, 14, 14, 14, 12,
                            14, 11, 11, 11, 11, 11, 11,
                            11, 11, 11, 0,  11, 11, 11  };

    const int materialValue[] = { 500, 200, 300, 400, 500, 800, 900, 990 };

    const int* positionalValue[] = {  &mouse,
                                      &cat,
                                      &wolf,
                                      &dog,
                                      &hyena,
                                      &tiger,
                                      &lion,
                                      &elephant   };

    int score = 0;

    int i;
    for (i = 0; i < 8; i++)
    {
        int locA = p->loc[i];
        int locB = p->loc[i+8];

        // add player A pieces
        if (locA != -1)
        {
            // prevent overflow of score
            if (score == INFTY || positionalValue[i][locA] == INFTY)
            {
                score = INFTY;
            }
            // prevent underflow of score
            else if (score == NEGINFTY || positionalValue[i][locA] == NEGINFTY)
            {
                score = NEGINFTY;
            }
            else
            {
                score += materialValue[i];
                score += positionalValue[i][locA];
            }
        }

        // subtract player B pieces
        if (locB != -1)
        {
            // prevent underflow of score
            if (score == NEGINFTY || positionalValue[i][62-locB] == INFTY)
            {
                score = NEGINFTY;
            }
            // prevent overflow of score
            else if (score == INFTY || positionalValue[i][62-locB] == NEGINFTY)
            {
                score = INFTY;
            }
            else
            {
                score -= materialValue[i];
                score -= positionalValue[i][62-locB];
            }
        }
    }

    return score;
}

int uTakesV(int u, int uLoc, int v, int vLoc)
{
    enum terrain uTerrain = indexToTerrain(uLoc);
    enum terrain vTerrain = indexToTerrain(vLoc);
    enum side uSide = indexToSide(u);
    enum side vSide = indexToSide(v);
    enum pieceType uType = indexToType(u);
    enum pieceType vType = indexToType(v);

    // #0  own den
    if ((uSide == A && vTerrain == ADEN) ||
        (uSide == B && vTerrain == BDEN)) return 0;

    // #1  non-water, empty terrain
    if (vTerrain != WATER && vType == EMPTY) return 1;

    // #1.5 opposing den
    if ((uSide == A && vTerrain == BDEN) ||
        (uSide == B && vTerrain == ADEN)) return 1;

    // #2  piece of same side
    if (uSide == vSide) return 0;

    // #3  mouse into empty water
    if (uType == MOUSE && vTerrain == WATER && vType == EMPTY) return 1;

    // #4  v on u trap
    if ((uSide == A && vSide == B && vTerrain == ATRAP) ||
        (uSide == B && vSide == A && vTerrain == BTRAP)) return 1;

    // #5  mouse on elephant, mouse out of water
    if ((uType == MOUSE && vType == ELEPHANT) &&
        (uTerrain != WATER)) return 1;

    // #6  elephant on mouse (depends on #4 and #5)
    if (uType == ELEPHANT && vType == MOUSE) return 0;

    // #6.5 across water/land boundary
    if ((uTerrain == WATER && vTerrain == NORMAL) ||
        (uTerrain == NORMAL && vTerrain == WATER)) return 0;

    // #7  stronger or equal (depends on #2, #4 and #5)
    if (uType >= vType) return 1;

    return 0;
}

int repeatedMove(int from, int to, struct move hist[], int histIndex)
{
    // non-moves are exempt from move repetition rules
    if (from == -1 && to == -1) return 0;

    // no triple-repeated moves before move ten
    if (histIndex < 8) return 0;

    // triple-repeated move
    if (hist[histIndex-4].from == from && hist[histIndex-4].to == to &&
        hist[histIndex-8].from == from && hist[histIndex-8].to == to) return 1;

    return 0;
}


















