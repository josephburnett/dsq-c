#include <zmq.h>
#include "common.h"
#include "interface.h"
#include "console.h"
#include "search.pb-c.h"

void console()
{
    enum playerType {HUMAN, COMPUTER};
    enum side player = A;
    struct position* board;
    stackOperation(NULL,NULL,RESETSTACK);

    enum playerType playerA = HUMAN;
    enum playerType playerB = HUMAN;

    int depthA;
    int depthB;
    int tmaxA;
    int tmaxB;
    double timeA;
    double timeB;

    char input[256];

    void *context = zmq_ctx_new ();
    void *requestor = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect (requestor, "tcp://localhost:3773");
    assert (rc == 0);

    struct partition* part = malloc(sizeof(struct partition));
    int i;
    for (i = 0; i < 63; i++)
    {
        part->lim[i] = ONLIMITS;
    }

    board = newGame();

    while(1)
    {
        printf("\n");
        printBoard(board);
        printf("\nscore: %i\n\n",scorePosition(board));

        if (player == A) printf("A> ");
        else printf("B> ");

        if (gameOver(board))
        {
            printf("GAME OVER !!\n\n");
            playerA = HUMAN;
            playerB = HUMAN;
            resetStack();
            free(board);
            board = newGame();
            player = A;
        }

        else if (((player == A) && (playerA == HUMAN)) ||
            ((player == B) && (playerB == HUMAN)))
        {
            scanf("%s",&input);

            // make a move
            if (!strcmp(input,"move"))
            {
                int from;
                int to;

                printf("from: ");
                scanf("%s",&input);
                from = alg2ind(input);
                printf("to:   ");
                scanf("%s",&input);
                to = alg2ind(input);

                struct move moves[64];
                int moveCount;

                generateMoves(board,part,player,moves,&moveCount);

                int validMove = 0;
                int moveIndex = 0;
                for (i = 0; i < moveCount; i++)
                {
                    if ((moves[i].from == from) && (moves[i].to == to))
                    {
                        validMove = 1;
                        moveIndex = i;
                    }
                }

                if (validMove)
                {
                    board = stackOperation(board,&(moves[moveIndex]),MAKEMOVE);
                    if (player == A) player = B;
                    else player = A;
                }
                else
                {
                    printf("\ni don't think so\n");
                }

            }
            else if (!strcmp(input,"quit"))
            {
                return;
            }
            else if (!strcmp(input,"computer"))
            {
                if (player == A) playerA = COMPUTER;
                else playerB = COMPUTER;

                int depth;
                int tmax;
                float time;
                printf("depth: ");
                scanf("%i",&depth);
                printf("tmax:  ");
                scanf("%i",&tmax);
                printf("time:  ");
                scanf("%f",&time);

                if (player == A)
                {
                    depthA = depth;
                    tmaxA = tmax;
                    timeA = (double)time;
                }
                else
                {
                    depthB = depth;
                    tmaxB = tmax;
                    timeB = (double)time;
                }
            }
            else if (!strcmp(input,"new"))
            {
                free(board);
                board = newGame();
                player = A;
                playerA = HUMAN;
                playerB = HUMAN;
                stackOperation(NULL,NULL,RESETSTACK);
            }
            else
            {
                printf("\nhuh??\n");
            }
        }
        // computer player
        else
        {
            printf("searching.");

            struct move* mv;
            char a[3];

            if (player == A)
            {
                mv = search(board,A,depthA,tmaxA,timeA);
                ind2alg(mv->from,a);
                printf("%s",a);
                ind2alg(mv->to,a);
                printf("-%s: %i\n",a,mv->score);
                stackOperation(board,mv,MAKEMOVE);
            }
            else
            {
                mv = search(board,B,depthB,tmaxB,timeB);
                ind2alg(mv->from,a);
                printf("%s",a);
                ind2alg(mv->to,a);
                printf("-%s: %i\n",a,mv->score);
                stackOperation(board,mv,MAKEMOVE);
            }

            if (player == A) player = B;
            else player = A;

            free(mv);

            printf("\n");
        }
    }

    free(part);

}

struct position* newGame()
{
    struct position* p = malloc(sizeof(struct position));

    int i;
    for (i = 0; i < 16; i++)
    {
        p->loc[i] = -1;
    }

    p->loc[0] = 48;
    p->loc[1] = 50;
    p->loc[2] = 44;
    p->loc[3] = 54;
    p->loc[4] = 46;
    p->loc[5] = 56;
    p->loc[6] = 62;
    p->loc[7] = 42;

    p->loc[8] = 14;
    p->loc[9] = 12;
    p->loc[10] = 18;
    p->loc[11] = 8;
    p->loc[12] = 16;
    p->loc[13] = 6;
    p->loc[14] = 0;
    p->loc[15] = 20;

    //p->loc[0] = 41;
    //p->loc[13] = 27;

    p->histIndex = 0;

    return p;
}

struct move* remoteSearch(struct position *p, enum side s) {

    SearchPosition position = SEARCH_POSITION__INIT;
    void *buf;
    unsigned len;

    

    struct move* mv;

    return mv;
}













