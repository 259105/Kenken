#ifndef GRAPH_H
#define GRAPH_H

#include "ST.h"

typedef struct edge { int v; int w; } Edge;

typedef struct graph *Graph;

Graph GRAPHinit(int V);
void  GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void  GRAPHinsertE(Graph G, int id1, int id2);
void  RESOLVEkenken(Graph G);

#endif
