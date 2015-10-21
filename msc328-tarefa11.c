   /*********************************************************/
   /*                                                       */
   /*                           Tarefa 11                   */
   /*      Arquivo: Tarefa11.c                              */
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

typedef struct node *link;
struct node { 
   Vertex w; 
   int cost; 
   link next; 
};

typedef struct digraph *Digraph;
struct digraph {
   int V; 
   int A; 
   link *adj; 
};

link NEW( Vertex w, int cost, link next) { 
   link x = malloc( sizeof *x);
   x->w = w; 
   x->cost = cost;     
   x->next = next;     
   return x;                         
}

Digraph DIGRAPHinit( int V) { 
   Vertex v;
   Digraph G = malloc( sizeof *G);
   G->V = V; 
   G->A = 0;
   G->adj = malloc( V * sizeof (link));
   for (v = 0; v < V; v++) G->adj[v] = NULL;
   return G;
}

void DIGRAPHinsertA( Digraph G, Vertex v, Vertex w, int cost) { 
   link p;
   if (v == w) return;
   for (p = G->adj[v]; p != NULL; p = p->next) 
      if (p->w == w) return;
   G->adj[v] = NEW( w, cost, G->adj[v]);
   G->A++;
}

/* Algoritmo de Dijsktra, modificado, com duas váriaveis a mais             */
/*    t1,t2 são os vestices cujo arco t1-t2 será removido, assim            */
/* calcula a nova priceância ignorando esse arco quando encontrado.          */
/****************************************************************************/
void DIGRAPHsptD2( Digraph G, Vertex s, 
Vertex parent[], int price[],Vertex t1, Vertex t2) { 
   link p; Vertex w, w0, frj[maxV];
   int qp = 1, i, j;
   Vertex *pq = malloc( G->V * sizeof( Vertex)); /* PQinit( );*/ 
   for (w = 0; w < G->V; w++) price[w] = (parent[w] = frj[w] = -1) + 1; 
   parent[s] = s;
   for (p = G->adj[s]; p != NULL; p = p->next) { 
      if (!(t1 == s && t2 == p->w)){ /* verifica arco removido */
         price[p->w] = p->cost;
         pq[qp] = p->w; 
         i = qp++; j = i/2; 
         while (j > 0 && price[pq[i]] < price[pq[j]]) {
            pq[0] = pq[j]; pq[j] = pq[i]; pq[i] = pq[0];
            i = j; j = j/2;
         } /* PQinsert( p->w);*/
         frj[p->w] = s; 
      }
   } 
   while (qp != 1/* !PQempty( )*/) {
      w0 = pq[1]; 
      qp--;
      if (qp > 1) {
         i = 1; j = 2;
         while (j <= qp) {
            pq[i] = pq[j];
            if (j + 1 <= qp && price[pq[j+1]] < price[pq[i]]) {
               pq[i] = pq[j+1]; i = j + 1;
            } else i = j;
            j = i * 2;
         }
      } /* PQdelmin( ); */ 
      parent[w0] = frj[w0];
      for (p = G->adj[w0]; p != NULL; p = p->next) {
         if (!(t1 == w0 && t2 == p->w)){ /* verifica arco removido */
            if (frj[p->w] == -1) { 
               price[p->w] = price[w0] + p->cost; 
               pq[qp] = p->w; 
               i = qp++; j = i/2; 
               while (j > 0 && price[pq[i]] < price[pq[j]]) {
                  pq[0] = pq[i]; pq[i] = pq[j]; pq[j] = pq[0];
                  i = j; j = j/2;
               } /*PQinsert( p->w);*/
               frj[p->w] = w0; 
            } 
            else if (price[p->w] > price[w0] + p->cost) {
               price[p->w] = price[w0] + p->cost;
               for (i = 1, j = i/2; pq[i] != p->w; i++, j = i/2);
               while (j > 0 && price[pq[i]] < price[pq[j]]) {
                  pq[0] = pq[j]; pq[j] = pq[i]; pq[i] = pq[0];
                  i = j; j = j/2;
               } /* PQdec( p->w); */
               frj[p->w] = w0; 
            }
         }
      }
   }
   free( pq);
}

/* Verifica para cada caminho de s-t, se todos os arcos do caminho estão    */
/*    relaxados, caso encontre algum arco que não esteja relaxado o prog    */
/*    devolve 0, e 1 caso contrario.                                        */
/****************************************************************************/
int verificaRelaxamento( Digraph G, Vertex parent[], int price[], Vertex s) {
   Vertex i, j, k;
   link p;
   for (k = G->V - 1; k >= 0; k--) {
      j = parent[k];
      i = k;
      if (j != -1) {
         while (j != s && i != s) {
            for (p = G->adj[j]; p->w != i; p = p->next);
            if (!(price[i] <= price[j] + p->cost)) return 0;
            i = j; j = parent[i];
         }
      }
   }
   return 1;
}

/*  Imprime cabeçalho                                                       */
/****************************************************************************/
void imprimecabecalho( ) {
   printf( "/*********************************************************/\n");
   printf( "/*                                                       */\n");
   printf( "/*                          TAREFA 11                    */\n");
   printf( "/*      Marcos Kazuya Yamazaki                           */\n");
   printf( "/*      MKY                                              */\n");
   printf( "/*      Numero USP: 7577622                              */\n");
   printf( "/*      Data: 19/05/2013                                 */\n");
   printf( "/*                                                       */\n");
   printf( "/*********************************************************/\n\n");
}

/* Lê os parametros para o Digrafo com custo positivo nos arcos             */
/* Cria o digrafo, e executa o algoritmo Dijsktra para calcular as priceanci */
/* Para cada arco do caminho de s a t,                                      */
/* calcula a priceancia do caminho sem este arco                             */
/* PARAMETROS: arc: Numero de arcos do Digrafo                              */
/*             t1,t2,c: Custo do arco de entrada t1-t2                      */
/*             maxC1, maxC2: priceancia do caminho minimo, com e sem um arco */
/*             x,y: arco x-y que será removido para novo calculo            */
/*             s,t: caminho de s a t que sera calculado                     */
/****************************************************************************/
int main( int argc, char *argv[]) {
   Digraph G;
   int V, arc, i, c, maxC1 = 1, maxC2 = 0, *price;
   Vertex *parent, x, y, s, t, t1, t2;
   scanf( "%i", &V); G = DIGRAPHinit( V);
   parent = malloc( G->V * sizeof( Vertex));
   price = malloc( G->V * sizeof( int)); 
   scanf( "%i", &arc); scanf( "%i %i", &s, &t);
   for (i = 0; i < arc; i++) {
      scanf( "%i %i %i", &t1, &t2, &c);
      DIGRAPHinsertA( G, t1, t2, c);
      maxC1 += c;
   }
   t1 = t2 = -1;
   imprimecabecalho();
   DIGRAPHsptD2( G, s, parent, price, -1, -1);
   if (!verificaRelaxamento( G, parent, price, s)) return 0;
   if(parent[t] != -1) {
      maxC1 = price[t];
      x = parent[t]; y = t;
      while (x != y) {
         DIGRAPHsptD2( G, s, parent, price, x, y);
         if (!verificaRelaxamento( G, parent, price, s)) return 0;
         if (price[t] > maxC2) {
            maxC2 = price[t];
            t1 = x; t2 = y;
         }
         y = x; x = parent[x]; 
      }
      printf( "priceancia de %i a %i sem remocao de arcos: %i\n", s, t, maxC1);
      if (maxC2 != 0) {
         printf( "Arco removido, cuja nova priceancia ");
         printf( "%i a %i e' maxima: %i-%i\n", s, t, t1, t2);
         printf( "Nova priceancia: %i\n", maxC2);
      }
      else printf("Nao se pode remover nenhum arco.\n");
   }
   else printf("Nao existe caminho de %i a %i.\n", s, t);
   free( parent);
   free( price);  
   return 0;
}