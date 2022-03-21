#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>          // Requiere la opción -msse3 al compilar
#include <time.h>
#include <unistd.h>
#include <math.h>               // Función log
#include "counter.h"


#define N_RED 10                // Número de veces que se repite la operación de reducción


// TODO: distintos tamaños de linea cache

void reducir(double A[], int ind[], double S[], long R);
void fijar_param(long * L, int * B, long * R, long * N, long * S1, long * S2, float factor, int cache, int D);
double calcular_media(double S[]);
void inicializar_A(double A[], long N);
void salir(char * msg);
void calcular_indices(int ind[], int D, long R);
void escribir_resultados(long L, int D, long R, long N, double ck);

// Usar esta función para probar código temporalmente (la borraremos al entregar)
void pruebas(double A[], int ind[], int D, long L, int B, long R, long N, long S1, long S2);


int main(int argc, char * argv[]) {
    double * A;        // Array de doubles
    int * ind;         // Array de referencias a posiciones de A
    double * S;        // Resultados de la reducción
    double ck;         // Tiempo de 10 operaciones de reducción en ciclos de reloj
    double ck_acceso;  // Tiempo por acceso en ciclos de reloj


    long N;            // Tamaño del vector A
    long R;            // Número de elementos de A a sumar
    int D;             // Parámetro de espaciado entre los elementos a sumar: A[0], A[D], A[2*D], ..., A[(R-1)*D]

    long S1;           // Número de líneas caché que caben en la caché L1 de datos
    long S2;           // Número de líneas caché que caben en la caché L2
    int B;             // Tamaño de línea caché en bytes
    long L;            // Número de líneas caché diferentes que se deben leer en el acceso a A


    if (argc != 4)
        salir("Error: el programa debe recibir tres argumentos:\n"
              "\t1) Un factor sobre S1 o S2\n"
              "\t2) 1 o 2, para indicar L1 de datos (S1) o L2 (S2)\n"
              "\t3) El valor de D a emplear"
              "Ejemplo: ./programa 0.5 1 50\n"
              "(Equivale a L=0.5*S1; D=50)\n\n");


    srand((unsigned int) time(NULL));      // Fijamos la semilla para la generación de números aleatorios

    // Imprimimos el valor de los parámetros determinados por los argumentos introducidos por línea de comandos
    printf("\n\n\nParámetros fijados: D = %d, L=%f*S%d\n",
           atoi(argv[1]), atof(argv[2]), atoi(argv[3]));


    D = atoi(argv[1]);          // D se puede guardar directamente en base a los argumentos del programa
    // El resto de parámetros se fijan en base a D, L, S1 y S2
    fijar_param(&L, &B, &R, &N, &S1, &S2, atof(argv[2]), atoi(argv[3]), D);


    pruebas(A, ind, D, L, B, R, N, S1, S2);

    // En la reserva de memoria de A se alinea el inicio del vector con el inicio de la línea cache
    if ((A = (double *) _mm_malloc(N * sizeof(double), B)) == NULL)
        salir("Error: no se ha podido reservar memoria para A");

    // El tamaño máximo que puede tener ind es N (si apunta a todas las posiciones de A)
    if ((ind = (int *) malloc(N * sizeof(int))) == NULL)
        salir("Error: no se ha podido reservar memoria para ind");

    if ((S = (double *) malloc(N_RED * sizeof(double))) == NULL)
        salir("Error: no se ha podido reservar memoria para S");



    inicializar_A(A, N);           // Guardamos N valores aleatorios en el rango [1, 2) en A
    calcular_indices(ind, D, R);      // Almacenamos los índices de los elementos de A a sumar según el D elegido



    start_counter();            // Iniciamos la medición del tiempo de acceso total

    reducir(A, ind, S, R);         // Realizamos las 10 sumas de R elementos de A

    ck = get_counter();         // Paramos el contador


    printf("\n Clocks=%1.10lf \n",ck);

    // Imprimimos la frecuencia de reloj estimada
    //mhz(1,1);

    // Imprimimos los 10 resultados (que deberían ser iguales) y su media para evitar optimizaciones del compilador
    for (int i = 0; i < N_RED; i++) printf("S[%d] = %f\n", i, S[i]);
    printf("Media: %f\n", calcular_media(S));

    //print_cache_info();
    // Cada reducción realiza R accesos al vector A[]
    // Por cada llamada a reducir() se realizan 10 reducciones (que resultan en los 10 elementos de S[])
    // Por tanto, el tiempo por acceso en ciclos es ck/(10*R), esto es, el tiempo total entre el número de accesos
    ck_acceso = ck/(10*R);

    escribir_resultados(L, D, R, N, ck_acceso);

    // Liberamos la memoria reservada
    _mm_free(A);
    free(ind);
    free(S);

    return 0;
}


/*
 * Función fija los valores de los parámetros que se emplearán en la ejecución del programa.
 * S1, S2 y B (el tamaño de la línea caché) se leen a través de una llamada al sistema.
 * L depende del factor y la caché indicados como argumentos del programa, y del valor de S1 o S2, según corresponda.
 * El valor de R se calcula en función de B, L y D.
 * El valor de N se calcula en función de D y R.
 *
 * @param L Número de líneas caché distintas de las que se deben leer datos.
 * @param B Tamaño de la línea caché.
 * @param R Numero de elementos del vector A[] a sumar.
 * @param N Tamaño del vector A[].
 * @param S1 Numero de lineas cache que caben en la cache L1 de datos.
 * @param S2 Numero de lineas cache que cabaen en la cache L2.
 * @param factor Factor que se multiplica por S1 o S2 para calcular L (entrada del programa).
 * @param cache Cache a utilizar: L1 de datos o L2 (entrada del programa).
 * @param D Numero de posiciones que separan los elementos a sumar del vector A[].
 */
void fijar_param(long * L, int * B, long * R, long * N, long * S1, long * S2, float factor, int cache, int D){
    if (cache == 1) {
    	// Determinar el valor actual de los parámetros de la caché L1
        *B = (int) sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
        // S1 almacenará el número de líneas
        *S1 = sysconf(_SC_LEVEL1_DCACHE_SIZE) / *B;
        *L = *S1 * factor;
    } else {
    	// Determinar el valor actual de los parámetros de la caché L2
        *B = (int) sysconf(_SC_LEVEL2_CACHE_LINESIZE);
        *S2 = sysconf(_SC_LEVEL2_CACHE_SIZE) / *B;
        *L = *S2 * factor;
    }
    // Obtención del número de accesos a A
    *R = D >= 8? *L : (int) (ceil(*B * (*L - 1) / (double) (D * sizeof(double)))) + 1;
    // Leeremos hasta A[(R-1)*D], el tamaño mínimo del array tendrá el último valor del índice (R-1)*D + 1 (ese mismo)
    *N = D * (*R-1) + 1;
}


void escribir_resultados(long L, int D, long R, long N, double ck_acceso){
    FILE *fp;


    if ((fp = fopen("res_temporales.txt", "a")) == NULL)
        salir("Error: no se ha podido abrir el archivo de resultados");

    //fprintf(fp, "L: %ld, D: %d, R: %ld, N: %ld\n%f\n", L, D, R, N, ck);
    fprintf(fp, "%ld %d %ld %ld %f\n", L, D, R, N, ck_acceso);

    if (fclose(fp)) salir ("Error: no se ha podido cerrar el archivo de resultados");
}


/*
 * Función que suma R elementos de A[], cuyos índices están almacenados en ind[].
 * La operación se repite N_RED = 10 veces. Cada resultado se guarda como un elemento de S[].
 * @param A Array de doubles con los datos de entrada
 * @param ind Array de enteros con los índices de los R datos de A a sumar
 * @param S Array donde se guardarán los resultados
 */
void reducir(double A[], int ind[], double S[], long R){
    int i, j;

    for (i = 0; i < N_RED; i++) {
        for (j = 0; j < R; j++){
            S[i] += A[ind[j]];
        }
    }
}

/*
 * Función que devuelve la media de los N_RED = 10 elementos de S[].
 * El objetivo de esta función es utilizar los elementos de S[] para evitar optimizaciones del compilador.
 * @param S Array de doubles sobre el que se realiza la media
 */
double calcular_media(double S[]){
    double media = 0.0;
    int i;

    for (i = 0; i < N_RED; i++) media += S[i];

    return media / N_RED;
}

/*
 * Función que inicializa A con valores aleatorios con valor absoluto en el intervalo [1,2)
 */
void inicializar_A(double A[], long N){
    int i;

    for (i = 0; i < N; i++) {
        // rand genera un entero aleatorio en [0, RAND_MAX)
        // Pasamos a double y dividimos entre RAND_MAX para obtener un número real en [0, 1)
        // Sumamos 1.0 para trasladar a [1, 2)
        A[i] = 1.0 + (double) rand() / RAND_MAX;

        // Multlicamos A[i] por 1 o -1 para dar signo aleatorio.
        // rand % 2 tiene dos resultados: {0, 1}. Multiplicando por 2, obtenemos {0, 2}. Sumando -1, tenemos {-1, 1}.
        A[i] *= (rand() % 2) * 2 - 1;
    }
}

void calcular_indices(int ind[], int D, long R){
    int i;

    for (i = 0; i < R; i++) ind[i] = i * D;
}


void salir(char * msg){
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void pruebas(double A[], int ind[], int D, long L, int B, long R, long N, long S1, long S2){
    /*int i;

    for (i = 0; i < N; i++){
        //printf("A[%d] = %f\n", i, A[i]);
    }

    printf("\n\n\n");
    for (i = 0; i < R; i++){
        //printf("ind[%d] = %d\n", i, ind[i]);
    }*/

    printf("D = %d\n", D);
    printf("L = %ld\n", L);
    printf("B = %d\n", B);
    printf("R = %ld\n", R);
    printf("N = %ld\n", N);
    printf("S1 = %ld\n", S1);
    printf("S2 = %ld\n", S2);
}

/*

int main(){
    double ck;


    start_counter();

    /* Poñer aquí o código a medir */
/*
    ck = get_counter();

    printf("\n Clocks=%1.10lf \n",ck);

    /* Esta rutina imprime a frecuencia de reloxo estimada coas rutinas start_counter/get_counter */
  /*  mhz(1,1);



    return 0;
}
*/

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
