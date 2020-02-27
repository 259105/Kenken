#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Graph.h"


typedef struct node *link;
struct node { int v; link next; } ;
struct graph { int V; int E;int dim; link *ladj; ST tab; link z; } ;

static link  NEW(int v, link next);
static Edge  EDGEcreate(int v, int w);
static void  insertE(Graph G, Edge e);

static int getv(Graph G,int r,int c){
    return r*G->dim+c;
}
static Edge EDGEcreate(int v, int w) {
  Edge e;
  e.v = v;
  e.w = w;
  return e;
}
static link NEW(int v, link next) {
  link x = malloc(sizeof *x);
  if (x == NULL)
    return NULL;
  x->v = v;
  x->next = next;
  return x;
}
Graph GRAPHinit(int dim) {
  int v;
  Graph G = malloc(sizeof *G);
  if (G == NULL)
    return NULL;
  G->dim=dim;
  int V=dim*dim;
  G->V = V;
  G->E = 0;
  G->z = NEW(-1, NULL);
  if (G->z == NULL)
    return NULL;
  G->ladj = malloc(G->V*sizeof(link));
  if (G->ladj == NULL)
    return NULL;
  for (v = 0; v < G->V; v++)
    G->ladj[v] = G->z;
  G->tab = STinit(V);
  if (G->tab == NULL)
    return NULL;
  return G;
}
void GRAPHfree(Graph G) {
  int v;
  link t, next;
  for (v=0; v < G->V; v++)
    for (t=G->ladj[v]; t != G->z; t = next) {
      next = t->next;
      free(t);
    }
  STfree(G->tab);
  free(G->ladj);
  free(G->z);
  free(G);
}
Graph GRAPHload(FILE *fin) {
  int dim, n,val, r, c,v,w;
  char op;
  Graph G;

  fscanf(fin, "%d", &dim);
  G = GRAPHinit(dim);

  while(fscanf(fin, "%d %d%c",&n,&val,&op) == 3){
      v=w=0;
      for(int i=0;i<n;i++){
          w=v;
          fscanf(fin,"%d %d",&r,&c);
          v=getv(G,r,c);
          STinsert(G->tab,val,op,v);
          if(i)
              GRAPHinsertE(G,v,w);
      }
  }
  return G;
}
void GRAPHinsertE(Graph G, int id1, int id2) {
  insertE(G, EDGEcreate(id1, id2));
}
static void  insertE(Graph G, Edge e) {
  int v = e.v, w = e.w;

  G->ladj[v] = NEW(w, G->ladj[v]);
  G->ladj[w] = NEW(v, G->ladj[w]);
  G->E++;
}

static void checkvalR(Graph G,int **S,int v,int *d,int *visited){
    for(link t=G->ladj[v];t!=G->z;t=t->next){
        if(!visited[t->v]){
            int r=t->v/G->dim,c=t->v%G->dim;
            visited[t->v]++;
            switch (STsearchopByIndex(G->tab,t->v)) {
                case '+':
                    (*d) += S[r][c];
                    break;
                case '-':
                    if (*d > S[r][c])
                        *d -= S[r][c];
                    else *d = S[r][c] - *d;
                    break;
                case '*':
                    (*d) *= S[r][c];
                    break;
                case '/':
                    if (*d > S[r][c]){
                        if(*d%S[r][c]!=0) *d=-1;
                        else *d /= S[r][c];
                    }
                    else{
                        if(S[r][c]%*d!=0) *d=-1;
                        else *d=S[r][c]/(*d);
                    }
                    break;
            }
            checkvalR(G,S,t->v,d,visited);
        }
    }
}
static int full(Graph G,int **S,int v,int *visited){
    int r=v/G->dim,c=v%G->dim;
    if(S[r][c]==0) return 0;
    for(link t=G->ladj[v];t!=G->z;t=t->next)
        if(!visited[t->v]){
            visited[t->v]++;
            if(!full(G,S,t->v,visited))
                return 0;
        }
    return 1;
}
static int primo(int n){
    int f;
    if(n==1) return 0;
    f=2;
    while (f<sqrt(n)){
        if(n%f==0) return 0;
        f++;
    }
    return 1;
}
static int checkval(Graph G,int **S,int v){
    int r=v/G->dim,c=v%G->dim;
    int val=STsearchByIndex(G->tab,v);
    char op=STsearchopByIndex(G->tab,v);
    for(int i=0;i<G->dim;i++){
        if(i!=c && S[r][i]==S[r][c]) return 0;
        if(i!=r && S[i][c]==S[r][c]) return 0;
    }
    if(op=='?'){
        if(S[r][c]==val){
            return 1;
        }else return 0;
    }
    if(op=='*'){
        if(primo(val) && S[r][c]!=1 && S[r][c]!=val) return 0;
        if(val%S[r][c]) return 0;
    }
    if(op=='+'){
        if(S[r][c]>=val) return 0;
    }
    int *visited=calloc(G->V, sizeof(int)),d=S[r][c];
    if(visited==NULL) exit(3);
    visited[v]=1;
    if(full(G,S,v,visited)){
        for(int i=0;i<G->V;i++) visited[i]=0;
        visited[v]=1;
        checkvalR(G,S,v,&d,visited);
        if(d==val){free(visited);
            return 1;}
        free(visited);
        return 0;
    }
    free(visited);
    return 1;
}
static int kenkenR(Graph G,int **S,int pos){
    int r,c;
    if(pos==G->V)
        return 1;
    r=(pos)/G->dim,c=(pos)%G->dim;
    for(int i=1;i<G->dim+1;i++){
        S[r][c]=i;
        if(checkval(G,S,pos))
            if(kenkenR(G,S,pos+1))
                return 1;
        S[r][c]=0;
    }
    return 0;
}
void RESOLVEkenken(Graph G){
    int **sol=malloc(sizeof(int*)*G->dim);
    for(int i=0;i<G->dim;i++)
        sol[i]=calloc(G->dim, sizeof(int));
    for(int i=0;i<G->dim;i++)
        if(sol[i]==NULL) exit(3);

    if(kenkenR(G,sol,0))
        for(int i=0;i<G->dim;i++){
            for(int j=0;j<G->dim;j++)
                printf("%d ",sol[i][j]);
            printf("\n");
        }
    else
        printf("Nessuna soluzione trovata.\n");
}
