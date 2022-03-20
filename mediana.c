#include <stdio.h>
#include <stdlib.h>


#define NE 10

void sort(double CK[NE]){
    int i, j;
    double temp;

    for (i = 0; i < NE; i++){
        for (j = 0; j < NE -1; j++){
            if (CK[j] > CK[j+1]){
                temp = CK[j];
                CK[j] = CK[j+1];
                CK[j+1] = temp;
            }
        }
    }
}


int main(int argc, char * argv[]){
    FILE *res_temporales;
    FILE *res_finales;
    int i;
    double mediana;
    long L;
    int D;
    long R;
    long N;
    double CK[NE];

    if ((res_temporales = fopen("res_temporales.txt", "r")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados temporales\n");
        exit(EXIT_FAILURE);
    }

// Si no está creado, se crea??
    if ((res_finales = fopen("res_totales.txt", "a")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados finales\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NE; i++){
        fscanf(res_temporales, "%ld %d %ld %ld %lf", &L, &D, &R, &N, &CK[i]);
    }
    sort(CK);

    mediana = (CK[4] + CK[5]) / ((double) 2);

    fprintf(res_finales, "%ld %d %ld %ld %f\n", L, D, R, N, mediana);

    freopen("res_temporales.txt", "w", res_temporales);

    fclose(res_temporales);
    fclose(res_finales);

    exit(EXIT_SUCCESS);
}
