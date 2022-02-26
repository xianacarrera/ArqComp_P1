//
// Created by Xiana on 23/02/2022.
//

#include "param.h"

void fijar_param(int * D, long * L, int * B, long * R, long * N, long * S1, long * S2){
    *D = 5;          // Temporal
    *S1 = sysconf(_SC_LEVEL1_DCACHE_SIZE) / sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    *S2 = sysconf(_SC_LEVEL2_CACHE_SIZE) / sysconf(_SC_LEVEL2_CACHE_LINESIZE);
    *L = *S1;    // Temporal
    *B = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    *R = (int) (ceil(*B * (*L - 1) / (double) *D)) + 1;
    *N = *D * (*R-1) + 1;
}


/*
 * Función que comprueba e imprime las características de los distintos niveles de caché del ordenador
 */
void print_cache_info(){
    long total, vias, tam_linea;
    long n_lineas_L1I, n_lineas_L1D, n_lineas_L2;

    /***** Caché L1 de instrucciones *****/

    // Tamaño total
    total = sysconf(_SC_LEVEL1_ICACHE_SIZE);
    // Nº de vías
    vias = sysconf(_SC_LEVEL1_ICACHE_ASSOC);
    // Tamaño de línea
    tam_linea = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);

    printf("**** Datos caché L1 de instrucciones: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L1I = total / tam_linea;

    /***** Caché L1 de datos *****/

    total = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    vias = sysconf(_SC_LEVEL1_DCACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

    printf("**** Datos caché L1 de datos: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L1D = total / tam_linea;

    /***** Caché L2 *****/

    total = sysconf(_SC_LEVEL2_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL2_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL2_CACHE_LINESIZE);

    printf("**** Datos caché L2: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L2 = total / tam_linea;

    /**** Caché L3 ****/
    total = sysconf(_SC_LEVEL3_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL3_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL3_CACHE_LINESIZE);

    printf("**** Datos caché L3: ****\n");
    printf("\tTamanho total: %ld\n", total);
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    printf("S1I = nº líneas L1I = %ld\n", n_lineas_L1I);
    printf("S1D = nº líneas L1D = %ld\n", n_lineas_L1D);
    printf("S2 = nº líneas L2 = %ld\n\n", n_lineas_L2);
}

