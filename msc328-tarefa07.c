   /*********************************************************/
   /*                                                       */
   /*                           Tarefa 07                   */
   /*      Arquivo: Tarefa07.c                              */
   /*      Marcos Kazuya Yamazaki                           */
   /*      MKY                                              */
   /*      Numero USP: 7577622                              */
   /*      Data: 18/04/2013                                 */
   /*                                                       */
   /*********************************************************/
 
#include <stdio.h>
#include <stdlib.h>

#define Vertex int
#define maxV 10000
#define INFINITO -1

static int dist[maxV];

/*   A estrutura digraph representa um digrafo. O campo adj é um ponteiro para a matriz de 
**   adjacência do digrafo. O campo V contém o número de vértices e o campo A contém o número 
**   de arcos do digrafo. 
*/

struct graph {
   int V; 
   int E; 
   int **adj; 
};

/*   Um objeto do tipo Digraph contém o endereço de um digraph. 
*/

typedef struct graph *Graph;

/*   A função abaixo aloca uma matriz com linhas 0..r-1 e colunas 0..c-1. Cada elemento da matriz
**   recebe valor val. 
*/

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

/*   Esta função devolve (o endereço de) um novo digrafo com vértices 0,..,V-1 e nenhum arco. 
*/

Graph GRAPHinit( int V) { 
   Graph G = malloc( sizeof *G);
   G->V = V; 
   G->E = 0;
   G->adj = MATRIXint( V, V, 0);
   return G;
}

/*   Esta função insere um arco v-w no digrafo G. Se v == w ou o digrafo já tem arco v-w, a
**   função não faz nada.  É claro que v e w não podem ser negativos e devem ser menores que G->V. 
*/

void GRAPHinsertE( Graph G, Vertex v, Vertex w) { 
   if (v != w && G->adj[v][w] == 0) {
      G->adj[v][w] = 1;
      G->adj[w][v] = 1; 
      G->E++;
   }
}

/*   Armazena no vetor dist a distância do vértice s a cada um dos demais vértices do grafo G: para
**   cada vértice v, dist[v] é a distância de s a v.  Isso para partindo de cada vertice, quando achado 
**   uma distancia maior que encontrada anteriormente ela é gravada.(Código inspirado no programa 18.9, 
**   p.119, de Sedgewick.) 
*/

int GRAPHdist( Graph G) { 
   Vertex v, w, s, lista[G->V];
   int max = 0, get, put;
   for (s = 0; s < G->V; s++) {
      get = put = 0;
      for (v = 0; v < G->V; v++) dist[v] = INFINITO;
      dist[s] = 0;
      lista[put++] = s;
      while (get < put) {
         v = lista[get++];
         for (w = 0; w < G->V; w++)
            if (G->adj[v][w] == 1 && dist[w] == INFINITO) { 
               dist[w] = dist[v] + 1;
               if (dist[w] > max) max = dist[w];
               lista[put++] = w;
            }
      }
   }
   return max;
}

/*  Imprime o cabeçãlho contento algumas informações do aluno e a data que foi criado o programa */ 

void imprimecabecalho( ) {
   printf( "/*********************************************************/\n");
   printf( "/*                                                       */\n");
   printf( "/*                           TAREFA 7                    */\n");
   printf( "/*      Marcos Kazuya Yamazaki                           */\n");
   printf( "/*      MKY                                              */\n");
   printf( "/*      Numero USP: 7577622                              */\n");
   printf( "/*      Data: 18/04/2013                                 */\n");
   printf( "/*                                                       */\n");
   printf( "/*********************************************************/\n\n");
}

/*   Cria Grafo a partir das entradas padroes do teclado, onde a primeira linha é o numero de
**   vertices que o Grafo possui, a segunda linha é o numero de arestas, e as n's linhas seguintes,
**   são correpondentes a cada arestas, em que vertices são ligagos v-w e o peso do arco. 
*/

Graph leGraph( ) {
   Graph G;
   int V, A, v, w, linha;
   fscanf( stdin, "%d", &V);
   G = GRAPHinit( V);
   fscanf( stdin, "%d", &A);
   for (linha = 0; linha < A; linha++) {
      fscanf( stdin, "%d", &v);
      fscanf( stdin, "%d", &w);
      GRAPHinsertE( G, v, w);
      while (getc(stdin) != '\n') {}
   }
   return G;
}

/*   Programa, que ao digitar um Grafo: o numero de vertices, o numero de arestas, e cada sua aresta,
**   mostra o seu diametro. 
*/

int main( int argc, char *argv[]) {
   int max;
   Graph G;
   G = leGraph();
   imprimecabecalho();
   max = GRAPHdist( G);
   printf("Diametro do Grafo: %i\n", max);
   return 1;
}