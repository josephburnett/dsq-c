#include "common.h"
#include "testgame.h"

void testfile(char* filename)
{
    FILE* INFILE;
    FILE* OUTFILE;
    struct position* game;
    int AThresh, BThresh, AMO, BMO, seconds, i;
    enum side currentPlayer;
    int currentMove;

    game = malloc(sizeof(struct position));
    if (!game) return;

    game->histIndex = 0;

    // read the problem file
    INFILE = fopen(filename,"r");
    if (!INFILE)
    {
        printf("Error opening file\n");
        return;
    }
    else
    {
        // read the piece locations
        for (i = 0; i < 16; i ++)
        {
            fscanf(INFILE,"%d",&(game->loc[i]));
        }

        // read the theshold values
        fscanf(INFILE,"%d",&AThresh);
        fscanf(INFILE,"%d",&BThresh);
        fscanf(INFILE,"%d",&AMO);
        fscanf(INFILE,"%d",&BMO);
        fscanf(INFILE,"%d",&seconds);
    }
    fclose(INFILE);

    OUTFILE = fopen(strcat(filename,".game"),"w");
    if (!OUTFILE) 
    {
        printf("Error creating file\n");
        return;
    }
    else
    {
        // record the problem conditions
        fprintf(OUTFILE,"INPUT FILE:\n");
        for (i = 0; i < 16; i++)
        {
            fprintf(OUTFILE,"%d\n",game->loc[i]);
        }
        fprintf(OUTFILE,"%d\n%d\n%d\n%d\n%d\n",AThresh,BThresh,AMO,BMO,seconds);
    }

    // play the game
    currentMove = 0;
    currentPlayer = A;
    resetStack();

    while(!gameOver(game))
    {
        struct move* mv;
        char a[3];
       
        currentMove++;
        
        fprintf(OUTFILE,"MOVE: %d\n",currentMove);

        if (currentPlayer == A)
        {
            fprintf(OUTFILE,"PLAYER: A\n");
        }
        else
        {
            fprintf(OUTFILE,"PLAYER: B\n");
        }
        
        fprintf(OUTFILE,"POSITION: ");
        fprintPosition(OUTFILE,game);
        fprintBoard(OUTFILE,game);

        printBoard(game);

        // search the position
        if (currentPlayer == A)
        {
            MOVEORDERING = AMO;
            mv = testSearch(OUTFILE,game,A,999,AThresh,seconds);
        }
        else
        {
            MOVEORDERING = BMO;
            mv = testSearch(OUTFILE,game,B,999,BThresh,seconds);
        }
        makeMove(game,mv);
           
        fprintf(OUTFILE,"BEST MOVE: ");
        ind2alg(mv->from,a);
        fprintf(OUTFILE,"%s",a);
        ind2alg(mv->to,a);
        fprintf(OUTFILE,"-%s (%d)\n",a,mv->score);

        fprintf(OUTFILE,"SCORE: %d\n",scorePosition(game));

        // switch players
        if (currentPlayer == A) currentPlayer = B;
        else currentPlayer = A;

        free(mv);
    }

    // game is a draw
    if (game->histIndex == MAXDURATION)
    {
        fprintf(OUTFILE,"WINNING PLAYER: D\n");
    }
    // player A caused an end of game
    else if (currentPlayer == B)
    {
        fprintf(OUTFILE,"WINNING PLAYER: A\n");
    }
    // player B caused an end of game
    else
    {
        fprintf(OUTFILE,"WINNING PLAYER: B\n");
    }

    fprintf(OUTFILE,"POSITION: ");
    fprintPosition(OUTFILE,game);
    fprintBoard(OUTFILE,game);
    printBoard(game);

    fclose(OUTFILE);
}

struct move* testSearch(FILE* OUTFILE, struct position* pos, enum side player, int maxDepth, int tmax, double seconds)
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

    // TEST
    int count = 0;
    struct subproblem* s = subproblems;
    while (s) 
    {
        count++;
        fprintf(OUTFILE,"SUBPROBLEM %d: ",count);
        fprintPosition(OUTFILE,s->pos);
        fprintBoard(OUTFILE,s->pos);
        s = s->next;
    }
    fprintf(OUTFILE,"SUBPROBLEM COUNT: %d\n",count);

    // phase 2
    // search subproblems
    suggestedMove = testSearchSubproblems(OUTFILE,subproblems,player,maxDepth,startTime,seconds); // TEST
    freeSubproblems(subproblems);
    if (!suggestedMove) return NULL;

    return suggestedMove;
}

struct move* testSearchSubproblems(FILE* OUTFILE, struct subproblem* sub, enum side player, int maxDepth, time_t startTime, double seconds)
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

                // TEST
                fprintf(OUTFILE,"SEARCH DEPTH: %d\n",d);
                fprintf(OUTFILE,"MOVE ORDERING: %d\n",MOVEORDERING);

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

            // TEST
            fprintf(OUTFILE,"SEARCH DEPTH: %d (cutoff)\n",d);
            fprintf(OUTFILE,"MOVE ORDERING: %d\n",MOVEORDERING);

            d = maxDepth;
        }

    } // end iterative deepening

    return bestMove;
}

void fprintPosition(FILE* OUTFILE, struct position* p)
{
    int i;
    
    for (i = 0; i < 16; i++)
    {
        fprintf(OUTFILE,"%i ",p->loc[i]);
    }
    fprintf(OUTFILE,"\n");
}

void fprintBoard(FILE* OUTFILE, struct position* pos)
{
    int i,j,k;

    fprintf(OUTFILE,"   +--+--+--+--+--+--+--+\n");
    for (i = 0; i < 63; i = i + 7)
    {
        fprintf(OUTFILE," %i ",9-i/7);

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
                    fprintf(OUTFILE,"|a%i",p+1);
                    break;
                case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
                    fprintf(OUTFILE,"|b%i",p-7);
                    break;
                default:
                    switch (i+j)
                    {
                        case 3: case 59:
                            fprintf(OUTFILE,"| d");
                            break;
                        case 2: case 4: case 10: case 52: case 58: case 60:
                            fprintf(OUTFILE,"| t");
                            break;
                        case 22: case 23: case 25: case 26:
                        case 29: case 30: case 32: case 33:
                        case 36: case 37: case 39: case 40:
                            fprintf(OUTFILE,"| w");
                            break;
                        default:
                            fprintf(OUTFILE,"|  ");
                    }
            }
        }
        fprintf(OUTFILE,"|\n   +--+--+--+--+--+--+--+\n");
    }
    fprintf(OUTFILE,"     A  B  C  D  E  F  G\n");
}

            
