#include <stdio.h>
#include <stdlib.h>


/*
 * Xiana Carrera Alonso, Ana Carsi González
 * Arquitectura de Computadores - Curso 2021/2022
 * Práctica 1 - Jerarquía de memoria y comportamiento de memoria cache: Estudio del efecto de la localidad de los
 * accesos a memoria en las prestaciones de programas en microprocesadores
 *
 * Este programa calcula la mediana sobre los resultados de 10 ejecuciones de reduccion.c. Dichas medidas se leen
 * de un archivo temporal, res_temporales.txt, y se encuentran desordenados de forma intencionada, para aumentar la
 * independencia estadística al minimizar el efecto de procesos en segundo plano.
 * La mediana, que es una medida de centralización robusta, disminuye significativamente el efecto de valores atípicos.
 * Los resultados finales se guardan en el archivo res_totales.txt.
 */


#define NE 10           // Número de experimentos repetidos para los mismos valores de L y D


// Función que ordena un array empleando insertion sort (que, aunque tiene complejidad O(n^2), se ha comprobado que
// es uno de los algoritmos más eficientes para arrays de pequeño tamaño, como en este caso)
// @param CK Array con los resultados de tiempos en ciclos de reloj por acceso que se quiere ordenar
void sort(double CK[NE]){
    double temp;
    int i, j;


    for(i = 1; i < NE; i++){
        temp = CK[i];        // Guardamos un elemento temporal
        j = i - 1;
        while(temp < CK[j] && j >= 0){
            // Vamos retrocediendo por el array e desplazamos una posicioń hacia delante todos los elementos
            // mayores que temp, para hacerle hueco
            CK[j+1] = CK[j];
            j = j-1;
       }
       CK[j+1] = temp; // Insertamos temp justo después del primer elemento encontrado que sea mayor o igual que él
       // De esta forma, el inicio del array (hasta el índice i) queda ordenado
    }
}


int main(int argc, char * argv[]){
    FILE *res_temporales;           // Archivo de resultados sin procesar
    FILE *res_finales;              // Archivo de los resultados obtenidos con la mediana
    double CK[NE];
    long L;
    int D;
    long R;
    long N;
    double mediana;
    int i;

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
