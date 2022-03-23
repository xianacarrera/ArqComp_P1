#include <stdio.h>
#include <stdlib.h>
#include <math.h>               // Función log
#include <unistd.h>

/*
 * Xiana Carrera Alonso, Ana Carsi González
 * Arquitectura de Computadores - Curso 2021/2022
 * Práctica 1 - Jerarquía de memoria y comportamiento de memoria cache: Estudio del efecto de la localidad de los
 * accesos a memoria en las prestaciones de programas en microprocesadores
 *
 * Este programa permite comprobar las características de los niveles L1 (datos e instrucciones), L2 y L3 de la
 * jerarquía cache del equipo. En concreto, se determinan el tamaño total, el tamaño de línea y el número de vías
 * de cada nivel.
 * Aunque realmente en el estudio no son necesarios todos estos valores, se ha querido comprobarlos igualmente para
 * asegurar la correcta comprensión de la estructura particular de memoria en el equipo de ejecución.
 */


int main(int argc, char * argv[]) {
    long total;                 // Tamaño total de cada nivel, en bytes
    long vias;                  // Número de vías de cada nivel
    long tam_linea;             // Tamaño de línea de cada nivel
    long n_lineas_L1I;          // Número de líneas de la caché L1 de instrucciones
    long n_lineas_L1D;          // Número de líneas de la caché L1 de datos
    long n_lineas_L2;           // Número de líneas de la caché L2

    /***** Caché L1 de instrucciones *****/

    // Tamaño total
    total = sysconf(_SC_LEVEL1_ICACHE_SIZE);
    // Nº de vías
    vias = sysconf(_SC_LEVEL1_ICACHE_ASSOC);
    // Tamaño de línea
    tam_linea = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);

    printf("\n**** Datos cache L1 de instrucciones: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    // El número de líneas será igual al tamaño total dividido por el tamaño de cada línea cache
    n_lineas_L1I = total / tam_linea;

    /***** Caché L1 de datos *****/

    total = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    vias = sysconf(_SC_LEVEL1_DCACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

    printf("**** Datos cache L1 de datos: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L1D = total / tam_linea;

    /***** Caché L2 *****/

    total = sysconf(_SC_LEVEL2_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL2_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL2_CACHE_LINESIZE);

    printf("**** Datos cache L2: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L2 = total / tam_linea;

    /***** Caché L3 *****/

    total = sysconf(_SC_LEVEL3_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL3_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL3_CACHE_LINESIZE);

    printf("**** Datos cache L3: ****\n");
    printf("\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    printf("\tNº de vías: %ld\n", vias);
    printf("\tTamanho de linea: %ld\n\n", tam_linea);

    // Finalmente, se imprime el número de líneas de cada cache
    printf("S1I = nº líneas L1I = %ld\n", n_lineas_L1I);
    printf("S1D = nº líneas L1D = %ld\n", n_lineas_L1D);
    printf("S2 = nº líneas L2 = %ld\n", n_lineas_L2);
    printf("S3 = nº líneas L3 = %ld\n\n", total / tam_linea);       // total y tam_linea hacen referencia a la L3
}
