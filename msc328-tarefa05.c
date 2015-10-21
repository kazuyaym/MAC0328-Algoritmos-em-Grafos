    /*********************************************************/
    /*                                                       */
    /*                           Tarefa 05                   */
    /*      Arquivo: Tarefa05.c                              */
    /*      Marcos Kazuya Yamazaki                           */
    /*      MKY                                              */
    /*      Numero USP: 7577622                              */
    /*      Data: 01/04/2013                                 */
    /*                                                       */
    /*********************************************************/

#include <stdio.h>
#include <stdlib.h>

#define Vertex int
#define maxV 10000

/* Vamos supor que nossos digrafos têm no máximo maxV vértices. */

static int tabela[7][2][3];
static int ncomp, maxcomp;
float k[7];

/* Vamos supor que nossos digrafos têm no máximo maxV vértices. */

static int conta, pre[maxV];

/* A estrutura digraph representa um digrafo. O campo adj é um ponteiro para a matriz de adjacência do digrafo. 
O campo V contém o número de vértices e o campo A contém o número de arcos do digrafo. */

struct graph {
    int V; 
    int E; 
    int **adj; 
};

typedef struct graph *Graph;

/* A função dfsR supõe que o digrafo G é representado por uma matriz de adjacência.  (Inspirado no programas 18.1, p.82, de Sedgewick.) */

void dfsR( Graph G, Vertex v) { 
   Vertex w;
   pre[v] = conta++; 
   for (w = 0; w < G->V; w++)
      if (G->adj[v][w] != 0 && pre[w] == -1)
         dfsR( G, w); 
}

/* A função DIGRAPHdfs visita todos os vértices e todos os arcos do digrafo G. A função atribui um número de ordem pre[x] a cada vértice x:  o k-ésimo vértice visitado recebe número de ordem k-1.  (Código inspirado no programa 18.3, p.87, de Sedgewick.) */

void GRAPHdfs( Graph G, int x, int z) { 
   Vertex v;
   conta = 0;
   ncomp = 0;
   maxcomp = 0;
   for (v = 0; v < G->V; v++) pre[v] = -1;
   for (v = 0; v < G->V; v++)
      if (pre[v] == -1){ 
          conta = 0;
          ncomp++;
     	  dfsR( G, v);
          if(conta > maxcomp) maxcomp = conta;
      }
   tabela[x][0][z] += ncomp;
   tabela[x][1][z] += maxcomp;
}


void freeM( Graph G) {
   Vertex i;
   for (i = G->V - 1; i >= 0; i--) free( G->adj[i]);
   free( G->adj);
   free( G);
}

/* A função abaixo aloca uma matriz com linhas 0..r-1 e colunas 0..c-1. Cada elemento da matriz recebe valor val. */

int **MATRIXint( int r, int c, int val) { 
   Vertex i, j;
   int **m = malloc( r * sizeof (int *));
   for (i = 0; i < r; i++)
       m[i] = malloc( c * sizeof (int));
   for (i = 0; i < r; i++)
       for (j = 0; j < c; j++)
           m[i][j] = val;
   return m;
}

/* Esta função devolve (o endereço de) um novo digrafo com vértices 0,..,V-1 e nenhum arco. */
Graph GRAPHinit( int V) { 
    Graph G = malloc( sizeof *G);
    G->V = V; 
    G->E = 0;
    G->adj = MATRIXint( V, V, 0);
    return G;
}

/* Esta função insere um arco v-w no digrafo G. Se v == w ou o digrafo já tem arco v-w, a função não faz nada.  É claro que v e w não podem ser negativos e devem ser menores que G->V. */

void GRAPHinsertE( Graph G, Vertex v, Vertex w) { 
   if (v != w && G->adj[v][w] == 0) {
      G->adj[v][w] = 1; 
	  G->adj[w][v] = 1; 
      G->E++;
   }
}

Vertex randV( Graph G) { 
   double r;
   r = rand( ) / (RAND_MAX + 1.0);
   return r * G->V;
}

Graph GRAPHrand1( int V, int E) { 
   Graph G = GRAPHinit( V);
   while (G->E < E) {
      Vertex v = randV( G);
      Vertex w = randV( G);
      GRAPHinsertE( G, v, w);
   }
   return G;
}

/*  Imprime o cabeçãlho contento algumas informações do aluno e a data que foi criado o programa */ 

void imprimecabecalho( )
{
    printf( "/*********************************************************/\n");
    printf( "/*                                                       */\n");
    printf( "/*                           TAREFA 5                    */\n");
    printf( "/*      Marcos Kazuya Yamazaki                           */\n");
    printf( "/*      MKY                                              */\n");
    printf( "/*      Numero USP: 7577622                              */\n");
    printf( "/*      Data: 01/04/2013                                 */\n");
    printf( "/*                                                       */\n");
    printf( "/*********************************************************/\n\n");
}

void imprime( ) {
   int x, v, z = 0;
   printf("Foram criado 10 grafos para cada um dos valores abaixo\n\n");
   for (v = 100; v <= 10000; v *= 10, z++){
      printf("Numero de vertices: %i\n", v);
      printf("nAresta | ncomp | maxcomp\n");
      for (x = 0; x < 7; x++){
         printf("  %i   |   %.1f   |   %.1f\n", ( int) (k[x]*v), tabela[x][0][z]/10.0, tabela[x][1][z]/10.0);
      }		
      printf("\n\n");
   }
}

/*  Função main, cria os grafos aleatorios */

int main( int argc, char *argv[])
{
   int v, i, j, E, teste = 0;
   Graph G;
   k[0] = 0.1; k[1] = 0.2; 
   k[2] = 0.5; k[3] = 1; 
   k[4] = 2;   k[5] = 5; 
   k[6] = 10;
   for (v = 100; v <= 10000; v *= 10, teste++) {
      for (i = 0; i < 7; i++){
         for (j = 0; j < 10; j++) {
            E = ( int) (k[i]*v);
            G = GRAPHrand1( v, E);
            GRAPHdfs( G, i, teste);
            freeM( G);
         }
      }
   }
   imprimecabecalho();
   imprime();
   return 0;
}
