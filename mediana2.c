#include <stdio.h>
#include <stdlib.h>

#define NL 7       // Número de valores de L a probar
#define NE 10      // Número de experimentos para cada par (D, L)
#define ND 5       // Número de valores que toma D

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
    int i, j;
    double mediana;
    long L[NL*ND];
    int D[NL*ND];
    long R[NL*ND];
    long N[NL*ND];
    double CK[NL*ND][NE];

    if ((res_temporales = fopen("res_temporales.txt", "r")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados temporales\n");
        exit(EXIT_FAILURE);
    }

    // Si no está creado, se crea??
    if ((res_finales = fopen("res_totales.txt", "a")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados finales\n");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < NE; j++){
        for (i = 0; i < NL * ND; i++){
            fscanf(res_temporales, "%ld %d %ld %ld %lf", &L[i], &D[i], &R[i], &N[i], &CK[i][j]);
        }
    }

    for (i = 0; i < NL * ND; i++){
        sort(CK[i]);
        mediana = (CK[i][4] + CK[i][5]) / ((double) 2);
        fprintf(res_finales, "%ld %d %ld %ld %f\n", L[i], D[i], R[i], N[i], mediana);
    }

    freopen("res_temporales.txt", "w", res_temporales);

    fclose(res_temporales);
    fclose(res_finales);

    exit(EXIT_SUCCESS);
}
