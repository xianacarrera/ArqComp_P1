#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pmmintrin.h>          // Opción -msse3 al compilar
//#include "cacheinfo.h"

/*
 * Función que comprueba e imprime las características de los distintos niveles de caché del ordenador
 */
void print_cache_info(){
    long total, vias, tam_linea;
    long n_lineas_L1I, n_lineas_L1D, n_lineas_L2;
    FILE *fp_cache;
    
     if ((fp_cache = fopen("fp_cache.txt", "a")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados temporales\n");
        exit(EXIT_FAILURE);
    }

    /***** Caché L1 de instrucciones *****/

    // Tamaño total
    total = sysconf(_SC_LEVEL1_ICACHE_SIZE);
    // Nº de vías
    vias = sysconf(_SC_LEVEL1_ICACHE_ASSOC);
    // Tamaño de línea
    tam_linea = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);

    fprintf(fp_cache, "**** Datos caché L1 de instrucciones: ****\n");
    fprintf(fp_cache, "\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    fprintf(fp_cache,"\tNº de vías: %ld\n", vias);
    fprintf(fp_cache,"\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L1I = total / tam_linea;

    /***** Caché L1 de datos *****/

    total = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    vias = sysconf(_SC_LEVEL1_DCACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);

    fprintf(fp_cache,"**** Datos caché L1 de datos: ****\n");
    fprintf(fp_cache,"\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    fprintf(fp_cache,"\tNº de vías: %ld\n", vias);
    fprintf(fp_cache,"\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L1D = total / tam_linea;

    /***** Caché L2 *****/

    total = sysconf(_SC_LEVEL2_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL2_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL2_CACHE_LINESIZE);

    fprintf(fp_cache,"**** Datos caché L2: ****\n");
    fprintf(fp_cache,"\tTamanho total: %ld = 2^(%d)\n", total, (int) (log(total) / log(2)));
    fprintf(fp_cache,"\tNº de vías: %ld\n", vias);
    fprintf(fp_cache,"\tTamanho de linea: %ld\n\n", tam_linea);

    n_lineas_L2 = total / tam_linea;

    /**** Caché L3 ****/
    total = sysconf(_SC_LEVEL3_CACHE_SIZE);
    vias = sysconf(_SC_LEVEL3_CACHE_ASSOC);
    tam_linea = sysconf(_SC_LEVEL3_CACHE_LINESIZE);

    fprintf(fp_cache,"**** Datos caché L3: ****\n");
    fprintf(fp_cache,"\tTamanho total: %ld\n", total);
    fprintf(fp_cache,"\tNº de vías: %ld\n", vias);
    fprintf(fp_cache,"\tTamanho de linea: %ld\n\n", tam_linea);

    fprintf(fp_cache,"S1I = nº líneas L1I = %ld\n", n_lineas_L1I);
    fprintf(fp_cache,"S1D = nº líneas L1D = %ld\n", n_lineas_L1D);
    fprintf(fp_cache,"S2 = nº líneas L2 = %ld\n\n", n_lineas_L2);
    
    fclose(fp_cache);
}

int main(){
	print_cache_info();

}
