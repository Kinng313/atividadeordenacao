#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static inline void trocar(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

static void insertion_sort(double *arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        double chave = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > chave) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = chave;
    }
}

void quicksort_otimizado(double *arr, size_t n) {
    while (n > 16) {
        size_t mid = n / 2;
        if (arr[0] > arr[mid]) trocar(&arr[0], &arr[mid]);
        if (arr[0] > arr[n - 1]) trocar(&arr[0], &arr[n - 1]);
        if (arr[mid] > arr[n - 1]) trocar(&arr[mid], &arr[n - 1]);

        double pivo = arr[mid];
        size_t i = 0;
        size_t j = n - 1;

        while (1) {
            while (arr[i] < pivo) i++;
            while (arr[j] > pivo) j--;

            if (i >= j) break;

            trocar(&arr[i], &arr[j]);
            i++;
            j--;
        }

        if (j < n - i) {
            quicksort_otimizado(arr, j + 1);
            arr += i;
            n -= i;
        } else {
            quicksort_otimizado(arr + i, n - i);
            n = j + 1;
        }
    }

    if (n > 1) {
        insertion_sort(arr, n);
    }
}

int main() {
    FILE *arq = fopen("listaOdenada.txt", "r");
    if (arq == NULL) arq = fopen("ListahAhSerOrdenada.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo! Confira o nome e a pasta.\n");
        return 1;
    }

    size_t capacidade = 1000000, total = 0;
    double *vetor = malloc(capacidade * sizeof(double));
    if (vetor == NULL) {
        printf("Sem memoria!\n");
        fclose(arq);
        return 1;
    }

    while (1) {
        double n;
        int r = fscanf(arq, "%lf", &n);
        if (r == 1) {
            if (total == capacidade) {
                capacidade *= 2;
                double *novo_vetor = realloc(vetor, capacidade * sizeof(double));
                if (novo_vetor == NULL) {
                    printf("Sem memoria durante o realloc!\n");
                    free(vetor);
                    fclose(arq);
                    return 1;
                }
                vetor = novo_vetor;
            }
            vetor[total++] = n;
        } else if (r == EOF) {
            break;
        } else {
            fgetc(arq);
        }
    }
    fclose(arq);

    if (total == 0) {
        printf("Nenhum numero encontrado no arquivo.\n");
        free(vetor);
        return 1;
    }

    struct timespec t_inicio, t_fim;
    clock_gettime(CLOCK_MONOTONIC, &t_inicio);

    quicksort_otimizado(vetor, total);

    clock_gettime(CLOCK_MONOTONIC, &t_fim);

    double tempo_gasto = (t_fim.tv_sec - t_inicio.tv_sec) + 
                         (t_fim.tv_nsec - t_inicio.tv_nsec) / 1e9;

    int ok = 1;
    for (size_t i = 1; i < total; i++) {
        if (vetor[i - 1] > vetor[i]) {
            ok = 0;
            break;
        }
    }
    printf("Elementos ordenados : %zu\n", total);
    printf("Algoritmo utilizado : QuickSort Hibrido (Hoare + Mediana de 3 + Insertion Sort)\n");
    printf("Tempo de ordenacao  : %.6f segundos (%.3f ms)\n", tempo_gasto, tempo_gasto * 1000.0);
    printf("Ordenado com sucesso: %s\n", ok ? "SIM" : "NAO");
    free(vetor);
    return 0;
}
