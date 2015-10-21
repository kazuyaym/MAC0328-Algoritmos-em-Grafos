   /*********************************************************/
   /*                                                       */
   /*                           Tarefa 16                   */
   /*      Arquivo: Tarefa16.c                              */
   /*      Marcos Kazuya Yamazaki                           */
   /*      MKY                                              */
   /*      Numero USP: 7577622                              */
   /*      Data: 17/06/2013                                 */
   /*                                                       */
   /*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define Vertex int
#define maxV 10000
#define maxE 100000

static Vertex pq[maxV+1]; 
static int V;
static int qp[maxV]; 

static Vertex id[maxV];
static int sz[maxV];

typedef struct node *link;
struct node { 
   Vertex w; 
   int cost; 
   link next; 
};

typedef struct graph *Graph;
struct graph {
    int V; 
    int E; 
    link *adj; 
};

typedef struct { 
   Vertex v, w; 
   int cost; 
} Edge;

void imprimecabecalho( ) {
   printf( "/*********************************************************/\n");
   printf( "/*                                                       */\n");
   printf( "/*                          TAREFA 16                    */\n");
   printf( "/*      Marcos Kazuya Yamazaki                           */\n");
   printf( "/*      MKY                                              */\n");
   printf( "/*      Numero USP: 7577622                              */\n");
   printf( "/*      Data: 17/06/2013                                 */\n");
   printf( "/*                                                       */\n");
   printf( "/*********************************************************/\n\n");
}

link NEW( Vertex w, int cost, link next) { 
   link x = malloc( sizeof *x);
   x->w = w; 
   x->cost = cost;     
   x->next = next;     
   return x;                         
}

Graph GRAPHinit( int V) { 
   Vertex v;
   Graph G = malloc( sizeof *G);
   G->V = V; 
   G->E = 0;
   G->adj = malloc( V * sizeof (link));
   for (v = 0; v < V; v++) G->adj[v] = NULL;
   return G;
}

void GRAPHinsertE( Graph G, Vertex v, Vertex w, int cost) { 
   link p;
   if (v == w) return;
   for (p = G->adj[v]; p != NULL; p = p->next) 
      if (p->w == w) return;
   G->adj[v] = NEW( w, cost, G->adj[v]);
   G->adj[w] = NEW( v, cost, G->adj[w]);
   G->E++;
}

Vertex randV( int tamM) { 
   double r;
   r = rand( ) / (RAND_MAX + 1.0);
   return r * tamM;
}

int randCost( ) {
   double k;
   k = rand( ) / (RAND_MAX + 1.0);
   return k * 101;
}

void troca( Vertex *n, Vertex indN, int nn, Vertex *s, int ss) {
   s[ss] = n[indN];
   n[indN] = n[nn];
}

/*      Gera uma grafo conexo, primeiramente ele coloca uma aresta em dois   */
/*   vertices totalmente aleatorios, apos isso, escolhe um vertice dentro    */
/*   da arvore e uma fora, ate que a arvore contenha todos os verticess      */
/*   spos isso, basta inserir aleatoriamente                                 */ 
/*****************************************************************************/

Graph GRAPHrand_modif( int V, int E) { 
   Graph G = GRAPHinit( V);
   Vertex s[maxV], n[maxV], v, w, indS, indN;
   int ss = 0, nn = V, i;
   for (i = 0; i < V; i++) n[i] = i;
   do {
      v = randV( V);
      w = randV( V);
   } while (v == w);
   GRAPHinsertE( G, v, w, randCost( ));
   troca( n, v, --nn, s, ss++);
   troca( n, w, --nn, s, ss++);
   while (G->E < V-1) {
      indS = randV( ss);
      indN = randV( nn);
      v = s[indS];
      w = n[indN];
      troca( n, indN, --nn, s, ss++);
      GRAPHinsertE( G, v, w, randCost( ));
   }
   while (G->E < E) {
      v = randV( V);
      w = randV( V);
      GRAPHinsertE( G, v, w, randCost( ));
   }
   return G;
}

/* A função EDGE devolve uma aresta com pontas v e w e com custo cost. */
Edge EDGE( Vertex v, Vertex w, int cost) {
   Edge e;
   e.v = v; e.w = w;
   e.cost = cost;
   return e;
}

/*      A função UFfind devolve 1 se os vértices v e w estiverem na mesma    */
/*   árvore da floresta geradora mst[0..k-1] e devolve 0 em caso contrário.  */
/*   A função UFunion faz a união das árvores que contêm os vértices v e w.  */
/*   (O código é uma versão adaptada do programa 4.8, p.152,                 */ 
/*   do volume 1 do livro de Sedgewick).                                     */
/*****************************************************************************/

void UFinit( int N) { 
   Vertex i;
   for (i = 0; i < N; i++) { 
      id[i] = i; 
      sz[i] = 1; 
   }
}

static Vertex find( Vertex x) { 
   Vertex i = x; 
   while (i != id[i]) 
      i = id[i]; 
   return i; 
}

int UFfind( Vertex v, Vertex w) { 
   return (find( v) == find( w)); 
}

void UFunion( Vertex v, Vertex w) { 
   Vertex i = find( v), j = find( w);
   if (i == j) return;
   if (sz[i] < sz[j]) { 
      id[i] = j; 
      sz[j] += sz[i]; 
   }
   else { 
      id[j] = i; 
      sz[i] += sz[j]; 
   }
}

/* A função GRAPHmstK recebe um grafo G com custos arbitrários nas arestas
 e calcula uma MST em cada componente de G.  O grafo é dado por suas listas
 de adjacência. A função armazena as arestas das MSTs no vetor mst[0..k-1]
 e devolve k.  Essa função é uma implementação do algoritmo de Kruskal.*/

/* A função supõe que o grafo tem no máximo maxE arestas.  Supõe também que
 INFINITO é estritamente maior que o custo de qualquer aresta.  O código
 foi copiado, com ligeiras modificações, do programa 20.5, p.249, de Sedgewick. */

void merge(Edge vec[], int vecSize) {
   int mid, i, j, k;
   Edge *tmp;
   tmp = (Edge*) malloc(vecSize * sizeof(Edge));
   j = mid = vecSize / 2;
   i = k = 0;
   while (i < mid && j < vecSize) {
      if (vec[i].cost < vec[j].cost) tmp[k] = vec[i++];
      else tmp[k] = vec[j++];
      ++k;
   }
   if (i == mid) while (j < vecSize) tmp[k++] = vec[j++];
   else while (i < mid) tmp[k++] = vec[i++]; 
   for (i = 0; i < vecSize; ++i) vec[i] = tmp[i];
   free(tmp);
}
 
void mergeSort(Edge vec[], int vecSize) {
   int mid;
   if (vecSize < 2) return;
   mid = vecSize / 2;
   mergeSort(vec, mid);
   mergeSort(vec + mid, vecSize - mid);
   merge(vec, vecSize);
}
 
int GRAPHmstK( Graph G, Edge mst[]) { 
   Vertex v;
   link p;
   int i, k, E = G->E;
   Edge a[maxE]; 
   for (i = 0, v = 0; v < G->V; ++v) 
      for (p = G->adj[v]; p != NULL; p = p->next) 
         if (v < p->w) 
            a[i++] = EDGE( v, p->w, p->cost);
   mergeSort( a, E-1);
   UFinit( G->V);
   for (i = E - 1,k = 0; i >= 0 && k < G->V-1; i--)
      if (!UFfind( a[i].v, a[i].w)) {
         UFunion( a[i].v, a[i].w);
         mst[k++] = a[i];
      }
   return k;
}

/******************************************************************************/

static void exch( int i, int j, int *price) {
   Vertex t;
   t = pq[i]; pq[i] = pq[j]; pq[j] = t;
   qp[pq[i]] = i;
   qp[pq[j]] = j;
}

static void fixUp( int k, int *price) {
   while (k > 1 && price[pq[k/2]] > price[pq[k]]) {
      exch( k/2, k, price);
      k = k/2;
   }
}

static void fixDown( int k, int *price) { 
   int j;
   while (2*k <= V) { 
      j = 2*k;
      if (j < V && price[pq[j]] > price[pq[j+1]]) j++;
      if (price[pq[k]] <= price[pq[j]]) break;
      exch( k, j, price); 
      k = j;
   }
}

void PQinsert( Vertex v, int *price) { 
   qp[v] = ++V; 
   pq[V] = v; 
   fixUp( V, price); 
}

Vertex PQdelmin( int *price) { 
   exch( 1, V, price); 
   --V; 
   fixDown( 1, price); 
   return pq[V+1]; 
}

void PQdec( Vertex w, int *price) { 
   fixUp( qp[w], price); 
}

/*  Recebe grafo G com custos arbitrários nas arestas e calcula uma MST da    */
/* componente de G que contém o vértice 0.  A função armazena a MST no vetor  */
/* parent, tratando-a como uma arborescência com raiz 0.                      */
/*                                                                            */
/*  O grafo G e os custos são representados por listas de adjacência.         */
/* A implementação supõe que o grafo tem no máximo maxV vértices.             */
/* (O código abaixo foi copiado do Programa 20.4, p.242, de Sedgewick.)       */
/******************************************************************************/

void GRAPHmstP2( Graph G, Vertex parent[]) { 
   Vertex v0, w, frj[maxV]; link p; 
   int price[maxV], c;
   for (w = 0; w < G->V; w++) 
      parent[w] = frj[w] = -1; 
   parent[0] = 0;
   V = 0; /*PQinit( ); */
   for (p = G->adj[0]; p != NULL; p = p->next) {
      w = p->w; c = p->cost;
      price[w] = c; 
      PQinsert( w, price); 
      frj[w] = 0; 
   } 
   while (V != 0 /*!PQempty( )*/) {
      v0 = PQdelmin( price); 
      parent[v0] = frj[v0]; 
      for (p = G->adj[v0]; p != NULL; p = p->next) {
         w = p->w; c = p->cost;
         if (parent[w] != -1) continue;
         if (frj[w] == -1) { 
            price[w] = c; 
            PQinsert( w, price); 
            frj[w] = v0; 
         } 
         else if (price[w] > c) {
            price[w] = c; 
            PQdec( w, price); 
            frj[w] = v0; 
         }
      }
   }
}

/*                                                                            */
/*     Verificacao de corte, para o algoritmo Prim,  recebe como parametro    */
/*     a arvore geradora pelo vetor dos pais, para cada aresta da arvore,     */
/*     ela eh 'retirada' e assim, para cada aresta que atravessa essas duas'  */
/*     'novas' componemte compara com o valor do preco da aresta tirada       */ 
/*     Caso ela seja menor ou igual a qualquer outra, a arvore é uma MST      */
/******************************************************************************/

void rec( Vertex *parent, int *comp, Vertex k) {
   if (comp[parent[k]] == -1) rec(parent, comp, parent[k]);
   comp[k] = comp[parent[k]];
}

int verificacaoCorte( Graph G, Vertex parent[]) {
   int comp[maxV], i, t, custo = 0;
   link p;
   comp[0] = 0;
   for (t = 1; t < G->V; t++){
      for (i = 1; i < G->V; i++) comp[i] = -1;
      comp[t] = 1;
      for (i = 1; i < G->V; i++) {
         if (comp[i] != -1) continue;
         if (comp[parent[i]] == -1) rec(parent, comp, parent[i]);
         comp[i] = comp[parent[i]];
      }
      for (p = G->adj[t]; p != NULL; p = p->next) if (p->w == parent[t]) {
         custo = p->cost; break;
      }
      for (i = 0; i < G->V; ++i) {
         for (p = G->adj[i]; p != NULL; p = p->next) {
            if (comp[i] != comp[p->w]) {
               if (custo > p->cost) {
                  printf("A arvore geradora nao eh uma MST\n");
                  printf("A aresta %i-%i nao eh de custo minimo\n", t, parent[t]);
                  return 0;
      }}}}
   }
   printf("Verificacao 1: correta, a arvore gerada pelo algoritmo Prim eh uma MST\n");
   return 1;
}

/*                                                                            */
/*     Main do programa, recebe 3 parametros, o numero de vertice, arestas e  */ 
/*     o numero de vezes que os algoritmos Primm e Kruskal serao executadas   */
/*     Gera um grafo aleatorio com apenas uma componente                      */
/*                                                                            */
/******************************************************************************/

int main( int argc, char* argv[])
{
   float timeP1, timeP2, timeK1, timeK2;
   int n, m, r, i;
   Graph G;
   Edge mst[maxV];
   Vertex parent[maxV];
   n = atoi( argv[1]); /* n vertices */
   m = atoi( argv[2]); /* m arestas */
   r = atoi( argv[3]); /* r interacao */
   if (r < 1) r = 1;
   if (m > 100000) return 0;
   if (m > n*(n-1)/2) return 0;
   imprimecabecalho( );
   G = GRAPHrand_modif (n, m);
   timeP1 = (float) clock()/(float)(CLOCKS_PER_SEC);	
   for (i = 0; i < r; i++) GRAPHmstP2( G, parent);
   timeP2 = (float) clock()/(float)(CLOCKS_PER_SEC);
   printf("Algoritmo Prim: %.3f segundos\n", timeP2-timeP1);
   timeK1 = (float) clock()/(float)(CLOCKS_PER_SEC);	
   for (i = 0; i < r; i++) GRAPHmstK( G, mst);
   timeK2 = (float) clock()/(float)(CLOCKS_PER_SEC);
   printf( "Algoritmo Kruskal: %.3f segundos\n", timeK2-timeK1);
   
   verificacaoCorte(G, parent);
   return 0;
}