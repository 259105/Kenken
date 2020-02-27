#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main() {
    FILE *fin;
    fin=fopen("Kenken5.txt","r");
    if(fin==NULL) exit(1);
    Graph G=GRAPHload(fin);

    RESOLVEkenken(G);

    GRAPHfree(G);
    return 0;
}
