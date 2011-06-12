/***************************************************************
** Biblioteca quicksort utilizada para a ordenacão crescente  **
** do vetor utilizado no  algoritmo guloso aleatorio          **
***************************************************************/


/* Estrutura que contém dados sobre uma estacão */
typedef struct Estacao 
{
    char nomeEstacao[10]; /* Nome da estacão */
    float custoEstacao;   /* Custo da estacão */
    int *vPontosEstacao;  /* Vetor de pontos cobertos pela estacão */
    int nPontosEstacao;   /* Número de pontos cobertos pela estacão */
}Estacao;

/* Assinaturas das funcões utilizadas no quicksort */ 

void swap(Estacao* a, Estacao* b);

int partition(Estacao vec[], int left, int right);

void quickSort(Estacao vec[], int left, int right);
