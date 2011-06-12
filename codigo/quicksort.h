typedef struct Estacao 
{
    char nomeEstacao[10];
    float custoEstacao;
    int *vPontosEstacao;
    int nPontosEstacao;
}Estacao;

void swap(Estacao* a, Estacao* b);

int partition(Estacao vec[], int left, int right);

void quickSort(Estacao vec[], int left, int right);
