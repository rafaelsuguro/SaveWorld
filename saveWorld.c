#include <stdio.h>
#include <stdlib.h>


typedef struct Estacao 
{
    char nomeEstacao[10];
    float custoEstacao;
    int *vPontosEstacao;
    int nPontosEstacao;
    int flag;
}Estacao;

typedef struct Resultado
{
    int *vetorPontosCobertos;
    Estacao *vetorEstacoesResultado;
    int nEstacoesResultado;
    float custo;
}Resultado;

Estacao *vetorEstacoes;
Resultado vetorResultado;
int nPontos, nEstacoes;


void swap(Estacao* a, Estacao* b) {
    
    Estacao tmp;
    
    tmp = *a;
    *a = *b;
    *b = tmp;
}

 
int partition(Estacao vec[], int left, int right) 
{
    
    int i, j;
       
    i = left;
    for (j = left + 1; j <= right; ++j) {
        if ((vec[j].custoEstacao/(float)(vec[j].nPontosEstacao)) < (vec[left].custoEstacao/(float)(vec[left].nPontosEstacao))) {
    
            ++i;
            swap(&vec[i], &vec[j]);
        }
        else if ((vec[j].custoEstacao/(float)(vec[j].nPontosEstacao)) == (vec[left].custoEstacao/(float)(vec[left].nPontosEstacao)))
        {
            if (vec[j].nPontosEstacao > vec[left].nPontosEstacao)
            {
                ++i;
                swap(&vec[i], &vec[j]);
            }
        }
    }
    swap(&vec[left], &vec[i]);
             
    return i;
}

 
void quickSort(Estacao vec[], int left, int right) {
    
    int r;
       
    if (right > left) {
    
        r = partition(vec, left, right);
        quickSort(vec, left, r - 1);
        quickSort(vec, r + 1, right);
    }
}


void leDados()
{
    char flag;
    int contEstacoes, contPontos = 0;

    scanf("%c %d\n", &flag, &nPontos);
    fflush(stdin);
    scanf("%c %d\n", &flag, &nEstacoes);
    fflush(stdin);

    vetorEstacoes = (Estacao*)malloc(sizeof(Estacao)*nEstacoes);

    for(contEstacoes = 0; contEstacoes < nEstacoes; contEstacoes++)
    {
        vetorEstacoes[contEstacoes].vPontosEstacao = (int*)malloc(sizeof(int)*nPontos);
        scanf("%s %f ", vetorEstacoes[contEstacoes].nomeEstacao, &vetorEstacoes[contEstacoes].custoEstacao);
        contPontos = 0;
        do
        {
            scanf("%c", &flag);
            fflush(stdin);
            if(flag != ' ' && flag != '\n')
            {
                vetorEstacoes[contEstacoes].vPontosEstacao[contPontos] = (int)flag;
            }
            contPontos++;
        } while(flag != '\n');
        vetorEstacoes[contEstacoes].nPontosEstacao = contPontos;
        vetorEstacoes[contEstacoes].flag = 0;
    }
}


void buscaGulosa()
{
    int contEstacoes, contPontos, contPontosEstacao, contResultado = 0;

    vetorResultado.custo = 0;
    vetorResultado.vetorEstacoesResultado = (Estacao*)malloc(sizeof(Estacao)*nEstacoes);
    vetorResultado.vetorPontosCobertos = (int*)malloc(sizeof(int)*nPontos);
    
    for(contPontos = 0; contPontos < nPontos; contPontos++)
    {
        vetorResultado.vetorPontosCobertos[contPontos] = 0;
    }
    
    for(contEstacoes = 0; contEstacoes < nEstacoes; contEstacoes++)
    {
        for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
        {
            if(vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1] == 0)
            {
                break;
            }
        }
        if(contPontosEstacao != vetorEstacoes[contEstacoes].nPontosEstacao)
        {
            vetorResultado.vetorEstacoesResultado[contResultado] = vetorEstacoes[contEstacoes];
            vetorResultado.custo += vetorEstacoes[contEstacoes].custoEstacao;
            for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
            {
                vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1]++;
            }
            contResultado++;
            vetorEstacoes[contEstacoes].flag = 1;
        }
        for(contPontos = 0; contPontos < nPontos; clsontPontos++)
        {
            if(vetorResultado.vetorPontosCobertos[contPontos] == 0)
            {
                break;
            }
        }
        if(contPontos == nPontos)
        {
            break;
        }
    }
    vetorResultado.nEstacoesResultado = contResultado;
}


void imprimeResultado()
{
    int contEstacoesResultado;
    
    printf("Custo: %.2f\n", vetorResultado.custo);
    printf("Total: %d\n", vetorResultado.nEstacoesResultado);
    for(contEstacoesResultado = 0; contEstacoesResultado < vetorResultado.nEstacoesResultado; contEstacoesResultado++)
    {
        printf("%s\n", vetorResultado.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao);
    }
}


int main(int argc, char *argv[])
{
    leDados();
    quickSort(vetorEstacoes, 0, nEstacoes - 1);
    buscaGulosa();
    imprimeResultado();

    return 0;
}
