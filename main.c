#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>          // Opción -msse3 al compilar
#include <time.h>
#include <unistd.h>
#include <math.h>               // Función log

#define N 20          // Tamaño del vector
#define D             // Parámetro de espaciado entre los elementos a sumar
#define N_RED 10      // Número de veces que se repite la operación de reducción


/*
 * Función que realiza una suma de R elementos de A.
 */
double sumar(double A[], int ind[], int R){
    double total = 0.0;
    int i;

    for (i = 0; i < R; i++) total += A[ind[i]];
    return total;
}

/*
 * Función que repite N_RED = 10 veces una operación de reducción de suma sobre el vector A.
 * Devuelve el valor medio.
 * Nombre temporal.
 */
double repetir_reduccion(double A[], int ind[], double S[], int R){
    double media = 0.0;
    int i;

    for (i = 0; i < N_RED; i++) {
        S[i] = sumar(A, ind, R);
        media += S[i];
    }
    return media / R;
}


/*
 * Función que inicializa A con valores aleatorios con valor absoluto en el intervalo [1,2)
 */
void inicializar_A(double A[]){
    int i;

    for (i = 0; i < N; i++) {
        // rand genera un entero aleatorio en [0, RAND_MAX)
        // Pasamos a double y dividimos entre RAND_MAX para obtener un número real en [0, 1)
        // Sumamos 1.0 para trasladar a [1, 2)
        A[i] = 1.0 + (double) rand() / RAND_MAX;
        // Damos signo aleatorio. rand % 2 limita las posibilidades a {0, 1} -- *2 --> {0, 2} -- -1 --> {-1, 1}
        A[i] *= (rand() % 2) * 2 - 1;
    }
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


void salir(char * msg){
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}


int main() {
    double * A;    // Array de doubles
    int * ind;     // Array de referencias a posiciones de A
    double * S;    // Resultados de la reducción

    srand((unsigned int) time(NULL));        // Fijamos la semilla para la generación de números aleatorios

    //if ((A = (double *) _mm_malloc(TC, CLS)) == NULL) salir("Error: no se ha podido reservar memoria para A");
    if ((A = (double *) malloc(N * sizeof(double))) == NULL)
        salir("Error: no se ha podido reservar memoria para A");
    inicializar_A(A);

    // El tamaño máximo que puede tener ind es N (si apunta a todas las posiciones de A)
    if ((ind = (int *) malloc(N * sizeof(int))) == NULL)
        salir("Error: no se ha podido reservar memoria para ind");

    if ((S = (double *) malloc(N_RED * sizeof(double))) == NULL)
        salir("Error: no se ha podido reservar memoria para S");


    print_cache_info();




    //_mm_free(A);
    free(ind);
    free(S);

    return 0;
}
