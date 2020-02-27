#ifndef ST_H
#define ST_H

typedef struct symboltable *ST;

ST    STinit(int maxN);
void  STfree(ST st);
void  STinsert(ST st, int val, char op, int i);
int   STsearchByIndex(ST st, int i);
char  STsearchopByIndex(ST st, int i);

#endif

