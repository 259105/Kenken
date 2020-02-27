#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ST.h"

typedef struct {
    int val;
    char op;
}Item;

struct symboltable {Item *a; int maxN; int N;};


ST STinit(int maxN) {
  ST st;
  st = malloc(sizeof (*st));
  if (st == NULL) {
    printf("Memory allocation error\n");
    return NULL;
  }
  st->a = malloc(maxN*sizeof(Item));
  if (st->a == NULL) {
    printf("Memory allocation error\n");
    free(st);
    return NULL;
  }
  st->maxN = maxN;
  st->N = 0;
  return st;
}

void STfree(ST st) {
  if (st==NULL)
    return;
  free(st->a);
  free(st);
}

void STinsert(ST st, int val, char op, int i) {
  if (i >= st->maxN) {
    st->a = realloc(st->a, (2*st->maxN)*sizeof(char *));
    if (st->a == NULL)
      return;
    st->maxN = 2*st->maxN;
  }
  st->a[i].val = val;
  st->a[i].op = op;
  st->N++;
}

int STsearchByIndex(ST st, int i){
  if (i < 0 || i >= st->N)
    return -1;
  return (st->a[i].val);
}

char STsearchopByIndex(ST st, int i){
    if (i < 0 || i >= st->N)
        return -1;
    return (st->a[i].op);
}

