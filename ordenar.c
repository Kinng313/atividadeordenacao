#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparar(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
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
        return 1;
    }

    while (1) {
        double n;
        int r = fscanf(arq, "%lf", &n);
        if (r == 1) {
            if (total == capacidade) {
                capacidade *= 2;
                vetor = realloc(vetor, capacidade * sizeof(double));
                if (vetor == NULL) {
                    printf("Sem memoria!\n");
                    return 1;
                }
            }
            vetor[total++] = n;
        } else if (r == EOF) {
            break;
        } else {
            fgetc(arq); 
        }
    }
    fclose(arq);

    printf("Numeros lidos: %zu\n", total);
    if (total == 0) {
        printf("Nenhum numero encontrado no arquivo.\n");
        return 1;
    }

    
    clock_t inicio = clock();
    qsort(vetor, total, sizeof(double), comparar);
    clock_t fim = clock();

    double tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    
    int ok = 1;
    for (size_t i = 1; i < total; i++) {
        if (vetor[i - 1] > vetor[i]) { ok = 0; break; }
    }

    printf("Primeiros: %g %g %g\n", vetor[0], vetor[1 < total ? 1 : 0], vetor[2 < total ? 2 : 0]);
    printf("Ultimos:   %g %g %g\n", vetor[total >= 3 ? total - 3 : 0], vetor[total >= 2 ? total - 2 : 0], vetor[total - 1]);
    printf("Ordenado corretamente: %s\n", ok ? "SIM" : "NAO");
    printf("Tempo de ordenacao: %f segundos (%f ms)\n", tempo_gasto, tempo_gasto * 1000.0);

    free(vetor);
    return 0;
}