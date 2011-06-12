#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"


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

