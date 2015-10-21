   /*********************************************************/
   /*                                                       */
   /*                           Tarefa 06                   */
   /*      Arquivo: Tarefa06.c                              */
   /*      Marcos Kazuya Yamazaki                           */
   /*      MKY                                              */
   /*      Numero USP: 7577622                              */
   /*      Data: 11/04/2013                                 */
   /*                                                       */
   /*********************************************************/
 
#include <stdio.h>
#include <stdlib.h>

#define Vertex int
#define maxV 10000

/* Vamos supor que nossos digrafos têm no máximo maxV vértices. */
static int conta, contats, ciclo,  pre[maxV], pos[maxV], ts[maxV];

/* A lista de adjacência de um vértice v é composta por nós do tipo node. Um link é um ponteiro para um node. Cada nó da lista contém um vizinho w de v e o endereço do nó seguinte da lista. */

typedef struct node *link;
struct node { 
   Vertex w; 
   link next; 
};

/* A estrutura digraph representa um digrafo. O campo adj é um ponteiro para o vetor de listas de adjacência do digrafo, o campo V contém o número de vértices e o campo A contém o número de arcos do digrafo. */

struct digraph {
   int V; 
   int A; 
   link *adj; 
};

/* Um objeto do tipo Digraph contém o endereço de um digraph. */
typedef struct digraph *Digraph;

/* A função NEW recebe um vértice w e o endereço next de um nó e devolve um novo nó x com x.w = w e x.next = next. */

link NEW( Vertex w, link next) { 
   link x = malloc( sizeof *x);
   x->w = w; 
   x->next = next;     
   return x;                         
}

/* Esta função devolve (o endereço de) um novo digrafo com vértices 0,..,V-1 e nenhum arco. */

Digraph DIGRAPHinit( int V) { 
   Vertex v;
   Digraph G = malloc( sizeof *G);
   G->V = V; 
   G->A = 0;
   G->adj = malloc( V * sizeof (link));
   for (v = 0; v < V; v++) 
      G->adj[v] = NULL;
   return G;
}

/* A função abaixo insere um arco v-w no digrafo G. Se v == w ou o digrafo já tem arco v-w, a função não faz nada. */

void DIGRAPHinsertA( Digraph G, Vertex v, Vertex w) { 
   link p;
   if (v == w) return;
   for (p = G->adj[v]; p != NULL; p = p->next) 
      if (p->w == w) return;
   G->adj[v] = NEW( w, G->adj[v]);
   G->A++;
}

/* A função cycleR devolve 1 quando encontrar um ciclo e os vertices do ciclo encontrado serão gravados no vetor ts[0..n]. E devolve 0 em caso contrário, a ordem da topologia também será guardada no vetor ts[0..n] */
   
int cycleR( Digraph G, Vertex v) { 
   link p;
   pre[v] = conta++;
   for (p = G->adj[v]; p != NULL; p = p->next) {
      Vertex w = p->w;
      if (pre[w] == -1) {
         if (cycleR( G, w) == 1) {
            if (ts[0] == v) ciclo = 1;
            if (!ciclo) ts[++contats] = v;
            return 1;
         }
      } else if (pos[w] == -1) {
           contats = 1;
           ts[0] = w; ts[1] = v;
           return 1; /* arco de retorno */
      }
   }
   pos[v] = conta++;
   ts[contats--] = v;
   return 0;
}

/* Recebe um Digrafo G e armazena em ts[0..V-1] uma ordenação topológica ou os vertices de um ciclo de G. */

int DIGRAPHcycle( Digraph G) { 
   int i, j, k;
   Vertex v;
   conta = ciclo = 0;
   contats = G->V - 1;
   for (v = 0; v < G->V; v++)
      pre[v] = pos[v] = ts[v] = -1;
   for (v = 0; v < G->V; v++) {
      if (pre[v] == -1) {
         if (cycleR( G, v) == 1) {
            i = 1; j = contats;
            while (i < j){
               k = ts[i]; ts[i] = ts[j]; ts[j] = k;
               i++; j--;
            }
            return 1;
         }
      }
   }
   return 0;
}

/*  Imprime o cabeçãlho contento algumas informações do aluno e a data que foi criado o programa */ 

void imprimecabecalho( ) {
    printf( "/*********************************************************/\n");
    printf( "/*                                                       */\n");
    printf( "/*                           TAREFA 6                    */\n");
    printf( "/*      Marcos Kazuya Yamazaki                           */\n");
    printf( "/*      MKY                                              */\n");
    printf( "/*      Numero USP: 7577622                              */\n");
    printf( "/*      Data: 11/04/2013                                 */\n");
    printf( "/*                                                       */\n");
    printf( "/*********************************************************/\n\n");
}

/* Cria Digrafo a partir das entradas padroes do teclado, onde a primeira linha é o numero de vertices que o Digrafo possui, a segunda linha é o numero de arcos, e as n's linhas seguintes, são correpondentes a cada arco, em que vertices são ligagos v-w e o peso do arco. */

Digraph leDigraph( ) {
   Digraph G;
   int V, A, v, w, linha;
   fscanf( stdin, "%d", &V);
   G = DIGRAPHinit( V);
   fscanf( stdin, "%d", &A);
   for (linha = 0; linha < A; linha++) {
      fscanf( stdin, "%d", &v);
      fscanf( stdin, "%d", &w);
      DIGRAPHinsertA( G, v, w);
      while (getc(stdin) != '\n') {}
   }
   return G;
}

/* Se a função DIGRAPHcycle resultou que o Digrafo possui ciclos, foi guardada num vetor uma de seus possiveis ciclos, essa função auxiliar, dado um Digrafo G e uma sequencia de vértices e o número de vértices que este ciclo possui, verifica se ele é realmente um ciclo. Devolvendo 1 caso verdadeiro. */ 

int verificaCiclo( Digraph G, int *t, int n) {
   int v, c = 0;
   link p;
   for (p = G->adj[t[n]]; p != NULL; p = p->next) {
      if (p->w == t[0]) c = 1;
   }
   if (c == 0) return 0;
   for (v = 0, c = 0; v < n; v++, c = 0) {
      for (p = G->adj[t[v]]; p != NULL; p = p->next)
         if (p->w == t[v+1]) c = 1;
      if (c == 0) return 0;
   }
   return 1;
}

/* Se a função DIGRAPHcycle resultou que o Digrafo é uma DAG, foi guardada num vetor uma das suas possiveis ordens topologicas, essa função auxiliar, dado um Digrafo G e uma sequencia de vértices, verifica se ela é uma ordem topologica. Devolvendo 1 caso verdadeiro. */

int verificaTopologia( Digraph G, int *t) {
   int visitado[maxV], v;
   link p;
   for (v = 0; v < G->V; v++)
      visitado[v] = 1;
   for (v = 0; v < G->V; v++) {
      for (p = G->adj[t[v]]; p != NULL; p = p->next) {
         if (visitado[p->w] == 0) return 0;
      }
	  visitado[t[v]] = 0;
   }
   return 1;
}

/* As duas proximas funções imprimem o resultado após duas verificações, se é um ciclo, ou uma ordem topologica */

void imprimeCiclo() {
   int v;
   printf( "O Digrafo possui um ciclo:\n");
   for (v = 0; v <= contats; v++)
      printf( "%i-", ts[v]);
   printf( "%i\n", ts[0]);
}

void imprimeTopologia( Digraph G) {
   int v;
   printf( "O Digrafo possui uma Ordem Topologica:\n");
   for (v = 0; v < G->V; v++)
      printf( "%i ", ts[v]);
   printf( "\n");
}

/* Programa, que ao digitar um Digrafo, o numero de arestas, e cada sua aresta, mostra se o digrafo possui um ciclo ou nao. */

int main( int argc, char *argv[]) {
   Digraph G;
   G = leDigraph();
   imprimecabecalho();
   if (DIGRAPHcycle( G) == 1) {
      if(verificaCiclo( G, ts, contats) == 1)
         imprimeCiclo();
   } else if(verificaTopologia( G, ts) == 1)
      imprimeTopologia( G);
   return 1;
}