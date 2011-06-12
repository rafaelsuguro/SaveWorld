/******************************************************
** Implementacão das funcões da biblioteca quicksort **
** utilizada para a ordenacão crescente do vetor     **
** utilizado no algoritmo guloso aleatorio           **
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"

/* Executa a troca dos elementos do vetor */
void swap(Estacao* a, Estacao* b) {
    
    Estacao tmp;
    
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Ordena as particões geradas pelo quicksort */
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

/* Executa o quicksort */
void quickSort(Estacao vec[], int left, int right) {
    
    int r;
    
    if (right > left) {
        
        r = partition(vec, left, right);
        quickSort(vec, left, r - 1);
        quickSort(vec, r + 1, right);
    }
}

