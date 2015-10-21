    /*********************************************************/
    /*                                                       */
    /*                           Tarefa 4                    */
    /*      Arquivo: Tarefa4.c                               */
    /*      Marcos Kazuya Yamazaki                           */
    /*      MKY                                              */
    /*      Numero USP: 7577622                              */
    /*      Data: 14/03/2013                                 */
    /*                                                       */
    /*********************************************************/

#include <stdio.h>
#include <stdlib.h>

#define Vertex int
#define maxV 10000

/* Vamos supor que nossos digrafos têm no máximo maxV vértices. */

static int conta, lbl[maxV];

/* A estrutura digraph representa um digrafo. O campo adj é um ponteiro para a matriz de adjacência do digrafo. 
O campo V contém o número de vértices e o campo A contém o número de arcos do digrafo. */

struct digraph {
    int V; 
    int A; 
    int **adj; 
};

/* Um objeto do tipo Digraph contém o endereço de um digraph. */

typedef struct digraph *Digraph;

/* Esta função insere um arco v-w no digrafo G. Se v == w ou o digrafo já tem arco v-w, a função não faz nada. */
/* É claro que v e w não podem ser negativos e devem ser menores que G->V. */

void DIGRAPHinsertA( Digraph G, Vertex v, Vertex w) { 
    if (v != w && G->adj[v][w] == 0) {
        G->adj[v][w] = 1; 
        G->A++;
    }
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

Digraph DIGRAPHinit( int V) { 
    Digraph G = malloc( sizeof *G);
    G->V = V; 
    G->A = 0;
    G->adj = MATRIXint( V, V, 0);
    return G;
}

/* A função dfsR supõe que o digrafo G é representado por uma matriz de adjacência.
  (Inspirado no programas 18.1, p.82, de Sedgewick.) */

void dfsR( Digraph G, Vertex v) { 
    Vertex w;
    lbl[v] = conta++; 
    for (w = 0; w < G->V; w++)
       if (G->adj[v][w] != 0) 
            if (lbl[w] == -1)
                dfsR( G, w); 
}

/* A função DIGRAPHdfs visita todos os vértices que se pode chegar a partir da raiz r. */ 
/* A função registra a ordem em que os vértices são visitados atribuindo um número de ordem lbl[x] a cada vértice x:  o k-ésimo vértice visitado recebe número de ordem k-1. */
/* (Código inspirado no programa 18.3, p.87, de Sedgewick.) */

int DIGRAPHdfs( Digraph G, Vertex r) { 
    Vertex v;
    conta = 0;
    for (v = 0; v < G->V; v++) lbl[v] = -1;
    dfsR( G, r);
    if (conta < G->V) return -1;
    return 0;
}

/*  Imprime o cabeçãlho contento algumas informações do aluno e a data que foi criado o programa */ 

void imprimecabecalho( )
{
    printf( "/*********************************************************/\n");
    printf( "/*                                                       */\n");
    printf( "/*                           TAREFA 4                    */\n");
    printf( "/*      Marcos Kazuya Yamazaki                           */\n");
    printf( "/*      MKY                                              */\n");
    printf( "/*      Numero USP: 7577622                              */\n");
    printf( "/*      Data: 14/03/2013                                 */\n");
    printf( "/*                                                       */\n");
    printf( "/*********************************************************/\n\n");
}

/*  Caso as três propriedades da arborecencia seja verificada para este digrafo, foi criada um vetor parent */
/*  que será impressa nessa função e a raiz desse digrafo */

void imprime( int v, int r, int *parent)
{
    int i, e;
    printf( "Raiz da arborescencia: %d\n", r);
    printf( "Vetor de pais (parent):\n\n");
    printf( "    v    |  parent[v]\n");
    printf( "----------------------\n");
    for (i = 0; i < v; i++){
        printf( "  %d", i);
        for (e = 6; e > i/10; e--) printf( " ");
        printf( "|  %d\n", parent[i]);
    }
}

/* A função verifica as duas primeiras propriedades de uma arborescência: */
/*   - não existem vértices com grau de entrada maior que 1               */
/*   - existe exatamente um vértice com grau de entrada 0                 */

int DIGRAPHarborescencia( Digraph G) {
    Vertex v, w;
    int entrada, raiz = -1, *parent;
    parent = malloc( G->V * sizeof (int));
    imprimecabecalho();
    if (G->V - G->A != 1) return -1;
    for (w = 0; w < G->V; w++) {
        entrada = 0;
        for (v = 0; v < G->V; v++) {
            if (G->adj[v][w] == 1) {
                parent[w] = v;
                entrada++;
            }
            if (entrada > 1) return -1;
        }
        if (entrada == 0 && raiz == -1) {
            if (raiz == -1) raiz = parent[w] = w;
            else return -1;
        }
    }
    if (DIGRAPHdfs( G, raiz) == -1) return -1;
    imprime( G->V, raiz, parent);
    return 0;
}

/*  Primeiramente le e controi o digrafo de acordo com o inserido, essa função supõem que o usuario digitou corretamente as informações */
/*  Após ler, criará o digrafo e chamará a função para verificar as três propriedades de uma arborecência */

int recebeDigrafoArbo( )
{
    int V, v, w, linha;
    Digraph G;
    fscanf( stdin, "%d", &V);
        G = DIGRAPHinit( V);
        for (linha = 0; linha < V; linha++) {
            fscanf( stdin, "%d", &v);
            while (getc(stdin) != '\n') {
                fscanf( stdin, "%d", &w);
                DIGRAPHinsertA( G, v, w);
            }
        }
    if (DIGRAPHarborescencia( G) == -1) printf("-1\n");
    return 0;
}

/*  Função main, chamara a função que lerá, criará o digrafo e verificar se é ou nao uma arborecência */

int main( int argc, char *argv[])
{
    recebeDigrafoArbo( );
    return 0;
}
