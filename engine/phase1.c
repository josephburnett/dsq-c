#include "common.h"
#include "phase1.h"

struct graph* mstOfPosition(struct position* pos)
{
    struct graph* cmpl = NULL;
    struct graph* mst = NULL;

    cmpl = graphOfPosition(pos);
    if (!cmpl) return NULL;

    mst = mstOfGraph(cmpl);
    free(cmpl);

    return mst;
}

struct subproblem* buildSubproblems(struct position* pos, struct graph* mst, int tmax)
{
    int i,j;
    int mark[16];
    struct effectcone* claim[16];
    struct subproblem* sub = NULL;
    struct subproblem* s;

    // mark the pieces that aren't in the position
    for (i = 0; i < 16; i++)
    {
        if (pos->loc[i] == -1)
        {
            mark[i] = 1;
        }
        else
        {
            mark[i] = 0;
        }
    }

    // remove edges from mst to create disconnected components
    removeGreaterEdges(mst,tmax-1);

    // make all the new positions
    int done = 0;
    while (!done)
    {
        // find next component
        i = 0;
        while (mark[i]) i++;

        // new subproblem for component
        s = malloc(sizeof(struct subproblem));
        if (!s)
        {
            freeSubproblems(sub);
            return NULL;
        }
        s->part = NULL;
        s->next = sub;
        sub = s;

        // the component represents a new position
        sub->pos = positionFromComponent(mst,pos,i);

        // mark the vertices included in component
        for (i = 0; i < 16; i++)
        {
            if (sub->pos->loc[i] != -1)
            {
                mark[i] = 1;
            }
        }

        // check for remaining components
        done = 1;
        for (i = 0; i < 16; i++)
        {
            if (mark[i] == 0)
            {
                done = 0;
            }
        }
    }

    // make partitions for the subproblems
    s = sub;
    while (s)
    {
        s->part = malloc(sizeof(struct partition));
        if (!(s->part))
        {
            freeSubproblems(sub);
            return NULL;
        }
        s = s->next;
    }

    // each subproblems claims squares with its effect cone
    for (i = 0, s = sub; s != NULL; i++, s = s->next)
    {
        claim[i] = projectCumulativeEffectcone(s->pos);
    }

    // hand out the squares
    struct subproblem* minSub;
    int minHeight;
    for (j = 0; j < 63; j++)
    {
        minSub = sub;
        minHeight = INFTY;

        for (i = 0, s = sub; s != NULL; i++, s = s->next)
        {
            int h = claim[i]->height[j];
            if (h < minHeight)
            {
                minHeight = h;
                minSub = s;
            }
        }

        // give the square to the subproblem
        s = sub;
        while (s)
        {
            if (s == minSub)
            {
                s->part->lim[j] = ONLIMITS;
            }
            else
            {
                s->part->lim[j] = OFFLIMITS;
            }

            s = s->next;
        }
    }

    // give every subproblem access to the dens
    s = sub;
    while(s)
    {
        s->part->lim[3] = ONLIMITS;
        s->part->lim[59] = ONLIMITS;

        s = s->next;
    }

    return sub;
}

struct graph* graphOfPosition(struct position* pos)
{
    int i;
    struct graph* g;
    struct vertex* v;
    struct edge* e;

    g = malloc(sizeof(struct graph));
    if (!g) return NULL;

    // empty graph
    g->vertices = NULL;

    // make the vertices
    for (i = 0; i < 16; i++)
    {
        if (pos->loc[i] >= 0) // piece in play
        {
            struct vertex* v;
            v = malloc(sizeof(struct vertex));

            if (!v)
            {
                freeGraph(g);
                return NULL;
            }

            v->label = i;
            v->edges = NULL;
            v->next = g->vertices;
            g->vertices = v;
        }
    }

    // make the edges
    v = g->vertices;
    while (v)
    {
        int a = v->label;
        int aLoc = pos->loc[a];
        int b;
        int bLoc;

        for (b = 0; b < 16; b++)
        {
            bLoc = pos->loc[b];
            if ((a != b) && (bLoc >= 0)) // different pieces, b in play
            {
                e = malloc(sizeof(struct edge));

                if (!e)
                {
                    freeGraph(g);
                    return NULL;
                }

                e->to = b;
                e->weight = computeT(a,aLoc,b,bLoc);
                e->next = v->edges;
                v->edges = e;

                if (e->weight == -1)
                {
                    freeGraph(g);
                    return NULL;
                }
            }
        }

        v = v->next;
    }

    return g;
}

struct graph* mstOfGraph(struct graph* g)
{
    struct graph* mst;
    struct edge* e;
    struct vertex* v;
    struct vertex* gvertices[16];
    struct vertex* mstvertices[16];
    int label;
    int set[16];
    int from[16];
    int i;

    for (i = 0; i < 16; i++)
    {
        gvertices[i] = NULL;
        mstvertices[i] = NULL;
        set[i] = INFTY;
        from[i] = INFTY;
    }

    mst = malloc(sizeof(struct graph));
    if (!mst) return NULL;

    // empty graph
    mst->vertices = NULL;

    // mst will have all the vertices of g
    v = g->vertices;
    while (v)
    {
        struct vertex* newVert;
        label = v->label;

        newVert = malloc(sizeof(struct vertex));
        if (!newVert)
        {
            freeGraph(mst);
            return NULL;
        }
        newVert->label = label;
        newVert->next = mst->vertices;
        newVert->edges = NULL;
        mst->vertices = newVert;

        gvertices[label] = v;
        mstvertices[label] = newVert;

        v = v->next;
    }

    v = g->vertices;
    label = v->label;
    e = v->edges;
    set[label] = 0; // in the mst

    // add edges adjacent to e to the periphery
    while(e)
    {
        set[e->to] = e->weight;
        from[e->to] = label;
        e = e->next;
    }

    int done = 0;
    while(!done)
    {
        // find the minimum vertex on the periphery
        int min = INFTY;
        int labelTo = 0;
        int labelFrom = 0;

        for (i = 0; i < 16; i++)
        {
            if ((set[i] < min) && (set[i] != 0))
            {
                min = set[i];
                labelTo = i;
                labelFrom = from[i];
            }
        }

        // adjacency to new vertex
        e = malloc(sizeof(struct edge));
        if (!e)
        {
            freeGraph(mst);
            return NULL;
        }
        e->to = labelTo;
        e->weight = set[labelTo];
        e->next = mstvertices[labelFrom]->edges;
        mstvertices[labelFrom]->edges = e;

        // adjacency from new vertex
        e = malloc(sizeof(struct edge));
        if (!e)
        {
            freeGraph(mst);
            return NULL;
        }
        e->to = labelFrom;
        e->weight = set[labelTo];
        e->next = mstvertices[labelTo]->edges;
        mstvertices[labelTo]->edges = e;

        // add new vertices to periphery
        e = gvertices[labelTo]->edges;
        while (e)
        {
            if (e->weight < set[e->to])
            {
                set[e->to] = e->weight;
                from[e->to] = labelTo;
            }

            e = e->next;
        }

        // vertex is now part of the mst
        set[labelTo] = 0;

        // done if no more unreached vertices
        done = 1;
        for (i = 0; i < 16; i++)
        {
            if ((set[i] != 0) && (set[i] != INFTY))
            {
                done = 0;
            }
        }
    }

    return mst;
}

void removeGreaterEdges(struct graph* g, int threshold)
{
    struct vertex* v;
    struct edge* e;
    struct edge** last;

    v = g->vertices;
    while (v)
    {
        e = v->edges;
        last = &(v->edges);

        while (e)
        {
            if (e->weight > threshold)
            {
                *last = e->next;
                free(e);
                e = *last;
            }
            else
            {
                last = &(e->next);
                e = e->next;
            }
        }

        v = v->next;
    }
}

struct position* positionFromComponent(struct graph* g, struct position* p, int component)
{
    int i;
    struct position* pos;
    struct edge* e;
    struct vertex* v;
    struct vertex* hash[16];
    int queue[16];
    int head,tail;
    int mark[16];

    pos = malloc(sizeof(struct position));
    if (!pos) return NULL;

    // empty board
    for (i = 0; i < 16; i++)
    {
        mark[i] = 0;
        pos->loc[i] = -1;
    }

    // duplicate position history
    for (i = 0; i < p->histIndex; i++)
    {
        pos->hist[i].from = p->hist[i].from;
        pos->hist[i].to = p->hist[i].to;
    }
    pos->histIndex = p->histIndex;

    // hash vertices by label
    v = g->vertices;
    while (v)
    {
        hash[v->label] = v;
        v = v->next;
    }

    head = 0;
    tail = 0;

    // enqueue first vertex
    queue[tail++] = component;
    while (head != tail)
    {
        int label = queue[head++];

        // add piece to position
        pos->loc[label] = p->loc[label];
        mark[label] = 1;

        // add adjacent vertices to queue
        e = hash[label]->edges;
        while (e)
        {
            if (mark[e->to] == 0)
            {
                queue[tail++] = e->to;
            }

            e = e->next;
        }
    }

    return pos;
}

int computeT(int u, int uLoc, int v, int vLoc)
{
    char uSide = indexToSide(u);
    char vSide = indexToSide(v);

    int D = computeD(u,uLoc,v,vLoc);

    // error condition
    if (D == -1) return -1;

    // case 1
    if (uSide == vSide)
    {
        return (2 * D - 1);
    }

    // case 2
    else
    {
        return D;
    }

    // case 3 (uType == vType)
    //return (2 * (D / 2) + 1); // rounded up to nearest odd
}

int computeD(int u, int uLoc, int v, int vLoc)
{
    struct effectcone* uCone;
    struct effectcone* vCone;
    struct effectcone* sumUV;
    int min;

    uCone = projectEffectCone(u,uLoc);
    if (!uCone) return -1;

    vCone = projectEffectCone(v,vLoc);
    if (!vCone)
    {
        free(uCone);
        return -1;
    }

    sumUV = sumOfEffectcones(uCone,vCone);
    free(uCone);
    free(vCone);
    if (!sumUV) return -1;

    min = minimumHeightOfEffectcone(sumUV);
    free(sumUV);

    return min;
}

struct effectcone* projectEffectCone(int piece, int origin)
{
    int i;

    int mark[63];
    int Q[256];
    int H[256];
    int head,tail;

    struct effectcone* cone;

    cone = malloc(sizeof(struct effectcone));
    if (!cone) return NULL;

    // initialization
    for (i = 0; i < 63; i++)
    {
        mark[i] = 0;
        cone->height[i] = INFTY;
    }
    head = 0;
    tail = 0;

    // enqueue first set of squares
    mark[origin] = 1;
    cone->height[origin] = 0;
    enqueueAdjacentSquares(piece,origin,0,Q,H,&tail);

    // breadth-first search
    while (head != tail)
    {
        int square = Q[head];
        int height = H[head];

        if (mark[square] == 0)
        {
            cone->height[square] = height;
            enqueueAdjacentSquares(piece,square,height,Q,H,&tail);
            mark[square] = 1;
        }

        head++;
    }

    return cone;
}

void enqueueAdjacentSquares(int piece, int square, int height, int Q[], int H[], int* tail)
{
    int i;
    int hash[63];

    // empty hash to allow free piece movement
    // allows lions and tigers to jump over mice
    for (i = 0; i < 63; i++)
    {
        hash[i] = -1;
    }

    int adj[5];
    adjacentSquares(piece,square,adj,hash);

    i = 0;
    while (adj[i] != -1)
    {
        Q[*tail] = adj[i];
        H[*tail] = height + 1;
        (*tail)++;
        i++;
    }
}

struct effectcone* sumOfEffectcones(struct effectcone* a, struct effectcone* b)
{
    int i;
    struct effectcone* sum;

    sum = malloc(sizeof(struct effectcone));
    if (!sum) return NULL;

    for (i = 0; i < 63; i++)
    {
        int aHeight = a->height[i];
        int bHeight = b->height[i];

        // prevent overflow
        if ((aHeight == INFTY) || (bHeight == INFTY))
        {
            sum->height[i] = INFTY;
        }

        else
        {
            sum->height[i] = a->height[i] + b->height[i];
        }
    }

    return sum;
}

int minimumHeightOfEffectcone(struct effectcone* a)
{
    int i;
    int min = INFTY;

    for (i = 0; i < 63; i++)
    {
        int height = a->height[i];

        if (height < min)
        {
            min = height;
        }
    }

    return min;
}

struct effectcone* projectCumulativeEffectcone(struct position* pos)
{
    int i;
    struct effectcone* cumulative;

    cumulative = malloc(sizeof(struct effectcone));
    if (!cumulative) return NULL;

    // starting point
    for (i = 0; i < 63; i++)
    {
        cumulative->height[i] = INFTY;
    }

    // cumulative is the minima of all effectcones
    for (i = 0; i < 16; i++)
    {
        // for pieces in play
        if (pos->loc[i] != -1)
        {
            struct effectcone* next;
            struct effectcone* temp;

            next = projectEffectCone(i,pos->loc[i]);
            if (!next)
            {
                free(cumulative);
                return NULL;
            }

            temp = minimaOfEffectcones(cumulative,next);
            free(next);
            free(cumulative);
            if (!temp) return NULL;

            cumulative = temp;
        }
    }

    return cumulative;
}

struct effectcone* minimaOfEffectcones(struct effectcone* a, struct effectcone* b)
{
    int i;
    struct effectcone* minima;

    minima = malloc(sizeof(struct effectcone));
    if (!minima) return NULL;

    for (i = 0; i < 63; i++)
    {
        int aHeight = a->height[i];
        int bHeight = b->height[i];

        if (aHeight < bHeight)
        {
            minima->height[i] = aHeight;
        }
        else
        {
            minima->height[i] = bHeight;
        }
    }

    return minima;
}

