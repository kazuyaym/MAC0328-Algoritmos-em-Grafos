/*************************************************************/
/**           Marcos Kazuya Yamazaki 7577622                **/
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define Vertex int
#define maxV 10000

static int conta, lbl[maxV];
int profundidade;

struct graph {
   int V; 
   int E; 
   int A;
   int **adj; 
};
typedef struct graph *Graph;

void DIGRAPHinsertA( Graph G, Vertex v, Vertex w) { 
   if (v != w && G->adj[v][w] == 0) {
      G->adj[v][w] = 1; 
      G->A++;
   }
}

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

Graph GRAPHinit( int V) { 
   Graph G = malloc( sizeof *G);
   G->V = V; 
   G->E = 0;
   G->A = 0;
   G->adj = MATRIXint( V, V, 0);
   return G;
}

void GRAPHinsertE( Graph G, Vertex v, Vertex w) { 
   DIGRAPHinsertA( G, v, w);
   DIGRAPHinsertA( G, w, v);
   G->E = G->A/2;
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

void dfsR ( Graph G, Vertex v) { 
   int p, i;
   profundidade++;
   Vertex w;
   lbl[v] = conta++; 
   for (w = 0; w < G->V; w++)
      if (G->adj[v][w] != 0) 
         if (lbl[w] == -1){
			for (p = 0; p < profundidade; p++) printf("   ");
			printf("%i-%i dfsR(G,%i)", v, w, w);
			for (i = 0; i < 23-(profundidade*3); i++) printf(" ");
			for (i = 0; i < G->V; i++){
				if( i == w) printf("%i ", conta);
				else{
					if (lbl[i] != -1) printf("%i ", lbl[i]);
					else printf ("- ");
				}
			}
		    printf("\n");
            dfsR( G, w); 
			profundidade--;
		 }
		 else{
			for(p = 0; p < profundidade-1; p++) printf("   ");
			printf("%i-%i\n", v, w);
		 }
}

void DIGRAPHdfs( Graph G) { 
   int i;
   Vertex v;
   conta = 0;
   profundidade = 0;
   printf("\n");
   for (i = 0; i < 36; i++) printf(" ");
   for (v = 0; v < G->V; v++){ 
      lbl[v] = -1;
	  printf("%i ", v);
   }
   printf("\n");
   for (i = 0; i < 36; i++) printf(" ");
   for (v = 0; v < G->V; v++) printf("-");
   for (v = 0; v < G->V-1; v++) printf("-");
   printf("\n");
   for (v = 0; v < G->V; v++)
      if (lbl[v] == -1) {
		 printf("dfsR(G,%i)", v);
		 for (i = 0; i < 36-9; i++) printf(" ");
		 for (i = 0; i < G->V; i++){
			if(i == v) printf("%i ", conta);
			else{
				if(lbl[i] != -1) printf("%i ", lbl[i]);
				else printf("- ");
			}
		 }
		 printf("\n");
         dfsR( G, v);
		 printf("\n");
         profundidade--;
	  }
}

int main (int argc, char *argv[])
{
	    int i, j, w;
	float k[7];
	Graph G;
	
	k[0] = 0.1;
	k[1] = 0.2;
	k[2] = 0.5;
	k[3] = 1;
	k[4] = 2;
	k[5] = 5;
	k[6] = 10;
	
	for (i = 10; i <= 10; i *= 10) {
		printf( "i: %i *******-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-**-\n", i);
		for (w = 0; w < 7; w++){
			printf( "  w: %.1f -------------------------------------------------------------------------------------------\n", k[w]);
			for (j = 0; j < 10; j++) {
				printf( "    j: %i --- - - - - - \n", j);
				DIGRAPHdfs(GRAPHrand1 ( i, k[w]*i));

			}
		}
	}

   /*int v, e;

   v = atoi(argv[1]);
   e = atoi(argv[2]);

   DIGRAPHdfs(GRAPHrand1 ( v, e));
   DIGRAPHdfs(GRAPHrand1 ( v, e));
   DIGRAPHdfs(GRAPHrand1 ( v, e));
   return 0;*/
}
