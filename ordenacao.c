#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 10000000

void merge(int *v, int *aux, int esq, int meio, int dir) {
    int i = esq, j = meio + 1, k = esq;

    while (i <= meio && j <= dir) {
        if (v[i] <= v[j]) aux[k++] = v[i++];
        else aux[k++] = v[j++];
    }
    while (i <= meio) aux[k++] = v[i++];
    while (j <= dir) aux[k++] = v[j++];

    for (k = esq; k <= dir; k++) v[k] = aux[k];
}

void mergeSort(int *v, int *aux, int esq, int dir) {
    if (esq >= dir) return;
    int meio = (esq + dir) / 2;
    mergeSort(v, aux, esq, meio);
    mergeSort(v, aux, meio + 1, dir);
    merge(v, aux, esq, meio, dir);
}

int main() {
    FILE *arq = fopen("listaOdenada.txt", "r");

    int *vetor = malloc(TAM * sizeof(int));
    int *aux = malloc(TAM * sizeof(int));
    int total = 0;
    while (total < TAM && fscanf(arq, "%d", &vetor[total]) == 1) {
        total++;
    }
    fclose(arq);

    clock_t inicio = clock();
    mergeSort(vetor, aux, 0, total - 1);
    clock_t fim = clock();

    double tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo de ordenação: %f segundos (%f ms)\n", tempo_gasto, tempo_gasto * 1000.0);

    free(vetor);
    free(aux);
    return 0;
}
