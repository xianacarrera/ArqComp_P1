#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>          // Requiere la opción -msse3 al compilar
#include <time.h>
#include <unistd.h>
#include <math.h>               // Función ceil()
#include <string.h>             // Función memcpy()
#include "counter.h"


#define N_RED 10                // Número de veces que se repite la operación de reducción


/*
 * Xiana Carrera Alonso, Ana Carsi González
 * Arquitectura de Computadores - Curso 2021/2022
 * Práctica 1 - Jerarquía de memoria y comportamiento de memoria cache: Estudio del efecto de la localidad de los
 * accesos a memoria en las prestaciones de programas en microprocesadores
 *
 * Este programa mide el número de ciclos medio por acceso a memoria durante una operación de reducción de suma
 * de punto flotante sobre R elementos de un vector de doubles. Dichos elementos están separados en D posiciones y,
 * en total, su uso supone el acceso a L líneas cache diferentes.
 * En última instancia, el objetivo es comprobar cómo diferentes factores que actúan sobre la localidad espacial en
 * el acceso a los datos puede alterar la eficiencia de los programas software.
 */


// Función que fija valores de algunos de los parámetros a emplear durante el programa (L, B, R, N y S1 ó S2)
// en base a los datos pasados como argumento al programa (factor, cache y D).
void fijar_param(long * L, int * B, long * R, long * N, long * S1, long * S2, float factor, int cache, int D);

// Función que calcula el valor medio de los resultados de 10 operaciones de reducción
void inicializar_A(double A[], long N);

// Función auxiliar que simplifica la finalización del programa con error
void salir(char * msg);

// Función que determina los índices de los sumandos de la reducción en el vector A[] y los almacena como elementos
// del vector ind[]
void calcular_indices(int ind[], int D, long R);

// Función que calcula la media de los elementos del vector de resultados
double calcular_media(double S[]);

// Función que almacena los resultados del programa como una línea de formato "L D R N ck" sobre un archivo temporal,
// el cual será posteriormente procesado por el programa medicana.c
void escribir_resultados(long L, int D, long R, long N, double ck);

// Función que mide el grado de similitud entre dos vectores (con el objetivo de verificar la aleatoriedad)
double comprobar_random(int ind[], int copia[], long R);

// Función que randomiza los elementos del vector de índices para empeorar la localidad espacial de los datos
void desordenar(int ind[], long R);


int main(int argc, char * argv[]) {
    double * A;        // Array de doubles
    int * ind;         // Array de referencias a los elementos a sumar de A
    double * S;        // Array que contendrá los 10 resultados de la reducción
    double ck;         // Tiempo de 10 operaciones de reducción en ciclos de reloj
    double ck_acceso;  // Tiempo por acceso en ciclos de reloj (= ck/(10*R))

    long N;            // Tamaño del vector A
    long R;            // Número de elementos de A a sumar
    int D;             // Parámetro de espaciado entre los elementos a sumar: A[0], A[D], A[2*D], ..., A[(R-1)*D]

    long S1;           // Número de líneas cache que caben en la caché L1 de datos
    long S2;           // Número de líneas cache que caben en la caché L2
    int B;             // Tamaño de línea cache en bytes
    long L;            // Número de líneas cache diferentes que se deben leer en el acceso a A

    int i, j;          // Variables de iteración


    /****************************************** PREPARATIVOS ****************************************************/

    // Comprobamos que la entrada del programa sea correcta
    if (argc != 4)
        salir("Error: el programa debe recibir tres argumentos:\n"
              "\t1) El valor de D a emplear"
	          "\t2) Un factor sobre S1 o S2\n"
              "\t3) 1 o 2, para indicar L1 de datos (S1) o L2 (S2)\n"
              "Ejemplo: ./programa 0.5 1 50\n"
              "(Equivale a L=0.5*S1; D=50)\n\n");

    // Fijamos la semilla para generar números aleatorios como el número de segundos desde Epoch (tiempo actual)
    srand((unsigned int) time(NULL));

    printf("\n\n\n******************** EXPERIMENTO DE LOCALIDAD ********************\n");

    // Imprimimos el valor de los parámetros determinados por los argumentos introducidos por línea de comandos
    printf("Parámetros fijados: D = %d, L=%f*S%d\n", atoi(argv[1]), atof(argv[2]), atoi(argv[3]));

    D = atoi(argv[1]);          // D se puede guardar directamente en base a los argumentos del programa
    // fijar_param escanea los niveles de cache para determinar sus características físicas (B, S1, S2)
    // Además, calcula L, R y N en función del resto de parámetros y de los argumentos introducidos al programa
    fijar_param(&L, &B, &R, &N, &S1, &S2, atof(argv[2]), atoi(argv[3]), D);

    // En la reserva de memoria de A se alinea el inicio del vector con el inicio de la línea cache, para un mayor
    // control de los resultados
    // Pasamos el número de bytes a reservar (N*sizeof(double)) y el tamaño de la línea cache (B)
    if ((A = (double *) _mm_malloc(N * sizeof(double), B)) == NULL)
        // Si tiene lugar algún error, cortamos la ejecución e imprimimos un error con salir()
        salir("Error: no se ha podido reservar memoria para A");

    // El tamaño máximo que puede tener ind es N (si apunta a todas las posiciones de A)
    if ((ind = (int *) malloc(N * sizeof(int))) == NULL)
        salir("Error: no se ha podido reservar memoria para ind");

    // S tendrá 10 posiciones
    if ((S = (double *) malloc(N_RED * sizeof(double))) == NULL)
        salir("Error: no se ha podido reservar memoria para S");


    // Guardamos N valores aleatorios con signo aleatorio y valor absoluto en el rango [1,2) para evitar posibles
    // efectos de inicialización de los niveles de la jerarquía de memoria
    inicializar_A(A, N);
    calcular_indices(ind, D, R);      // Almacenamos los índices de los elementos de A a sumar según el D elegido
    // Desordenamos ind[] de forma aleatoria para minimizar la localidad espacial
    // Se verifica que el vector quede realmente desordenado y, si no es así, se repite el proceso hasta que lo esté
    desordenar(ind, R);


    /********************************************** REDUCCIÓN ****************************************************/

    start_counter();            // Iniciamos la medición del tiempo de acceso total

    // Realizamos 10 reducciones. Cada resultado se guarda como un elemento distinto de S
    for (i = 0; i < N_RED; i++) {
        for (j = 0; j < R; j++){            // Sumamos los R elementos determinados
            S[i] += A[ind[j]];              // Accedemos a A[] a través de ind[], de forma aleatoria
        }
    }

    ck = get_counter();         // Paramos el contador


    /**************************************** ANÁLISIS DE RESULTADOS **********************************************/


    printf("\nTiempo de 10 reducciones en ciclos de reloj = %1.10lf\n", ck);

    // Imprimimos la frecuencia de reloj estimada
    mhz(1,1);

    // Imprimimos los 10 resultados (que deberían ser iguales) y su media para evitar optimizaciones del compilador,
    // que en caso contrario podría optar por no ejecutar determinadas operaciones
    for (i = 0; i < N_RED; i++) printf("S[%d] = %f\n", i, S[i]);
    printf("Media: %f\n", calcular_media(S));

    // Cada reducción realiza R accesos al vector A[]
    // Por cada llamada a reducir() se realizan 10 reducciones (que resultan en los 10 elementos de S[])
    // Por tanto, el tiempo por acceso en ciclos es ck/(10*R), esto es, el tiempo total entre el número de accesos
    ck_acceso = ck/(10*R);

    // Escribimos los resultados junto a los parámetros utilizados en un archivo temporal, que después será procesado
    // por un programa de cálculo de la mediana
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
 * @param cache Cache a utilizar: 1 -> L1 de datos; 2 -> L2 (entrada del programa).
 * @param D Numero de posiciones que separan los elementos a sumar del vector A[] (entrada del programa).
 */
void fijar_param(long * L, int * B, long * R, long * N, long * S1, long * S2, float factor, int cache, int D){
    if (cache == 1) {          // Determinamos el valor actual de los parámetros de la cache L1 de datos
        // Guardamos en B el tamaño de línea de la cache L1 de datos en bytes
        *B = (int) sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
        // S1 almacenará el número de líneas (tamaño total de la cache en bytes entre tamaño de cada línea en bytes)
        *S1 = sysconf(_SC_LEVEL1_DCACHE_SIZE) / *B;
        *L = *S1 * factor;      // L es igual al número de líneas escalado por un determinado factor
    } else {                    // Determinar el valor actual de los parámetros de la caché L2
        // Análogo al caso de la L1 de datos
        *B = (int) sysconf(_SC_LEVEL2_CACHE_LINESIZE);
        *S2 = sysconf(_SC_LEVEL2_CACHE_SIZE) / *B;
        *L = *S2 * factor;
    }

    /*
     * Obtenemos el número de sumandos de A[] que debemos emplear para que se acceda a L líneas cache y los elementos
     * estén separados por D posiciones.
     * Si D es menor que *B/sizeof(double) (presumiblemente 8), el valor de R dependerá no solo de L, sino también de
     * D, pues se está cogiendo más de un elemento por cada línea cache. Es necesario emplear la función ceiling, pues
     * puede que un bloque de D posiciones esté partido entre varias líneas cache, y es necesario tomarlo en su
     * posición iniciales. Nótese además que de la última línea solamente es necesario tomar un elemento para cumplir
     * con la restricción, de ahí el 1 añadido a mayores.
     * Si D es igual a 8, se toma exactamente un elemento por cada línea: R = L.
     * Si D es mayor que 8, eventualmente podrá haber líneas cache en las que esté almacenado A[] y a las que no se
     * acceda. Es decir, R será exactamente igual al número de líneas que se pretende usar, L.
     */
    *R = D >= *B/sizeof(double)? *L : (int) (ceil(*B * (*L - 1) / (double) (D * sizeof(double)))) + 1;
    // Calculamos el tamaño total que deberá tener el array A[]. Puesto que cada elemento "ocupará" D posiciones,
    // necesitaremos D * (R - 1) posiciones para cubrir todos los sumandos menos el último. Este se puede sumar
    // en solitario, ya que a partir de él no se empleará ningún índice más.
    *N = D * (*R-1) + 1;
}


/*
 * Función que escribe los resultados del experimento en el archivo res_temporales.txt con el formato
 * "L D R N ciclos_por_acceso".
 * @param L Número de líneas caché distintas de las que se deben leer datos.
 * @param D Numero de posiciones que separan los elementos a sumar del vector A[].
 * @param R Numero de elementos del vector A[] a sumar.
 * @param N Tamaño del vector A[].
 * @param ck_acceso Tiempo por acceso obtenido en la medición.
 */
void escribir_resultados(long L, int D, long R, long N, double ck_acceso){
    FILE *fp;       // Puntero al archivo

    // Abrimos el archivo en modo append, para continuar escribiendo a continuación de ejecuciones previas de este
    // mismo programa. Si res_temporales.txt no existía, se crea.
    if ((fp = fopen("res_temporales.txt", "a")) == NULL)
        salir("Error: no se ha podido abrir el archivo de resultados");

    fprintf(fp, "%ld %d %ld %ld %f\n", L, D, R, N, ck_acceso);

    if (fclose(fp)) salir ("Error: no se ha podido cerrar el archivo de resultados");
}

/*
 * Código de referencia: https://benpfaff.org/writings/clc/shuffle.html
 * Esta función randomiza los índices de los sumandos de A[]. Su corrección se basa en que N será siempre bastante
 * inferior a RAND_MAX
 * Si los resultados no son lo suficientemente aleatorios, se repite el proceso hasta que lo sean
 * @param ind Vector a randomizar
 * @param R Número de elementos almacenados en ind
 */
void desordenar(int ind[], long R){
    size_t i, j;
    long temp;
    int copia[R];

    // Copiamos todo el vector de índices (que llega ya ordenado)
    memcpy((void *) copia, (void *) ind, (size_t) R * sizeof(int));

    do{
        for (i = 0; i < R - 1; i++) {
            j = i + rand() / (RAND_MAX / (R - i) + 1);      // Buscamos otro índice, aleatorio
            // Intercambiamos los elementos
            temp = ind[j];
            ind[j] = ind[i];
            ind[i] = temp;
        }
    } while (comprobar_random(ind, copia, R) > 0.15);    // Si el grado de coincidencia con el vector ordenado es
    // superior al 15%, repetimos el randomizado
}

/*
 * Función que comprueba el grado de coincidencia entre dos vectores y devuelve la proporción de la misma con respecto
 * al tamaño total, que debe coincidir entre ambos.
 * @param ind Vector 1 a comparar
 * @param copia Vector 2 a comparar
 * @param R Tamaño de ambos vectores
 */
double comprobar_random(int ind[], int copia[], long R){
    int contador_iguales = 0;
    int i;

    for (i = 0; i < R; i++){
        // Si los dos vectores guardan el mismo valor en la posición i, aumentamos el contador
        if (ind[i] == copia[i]) contador_iguales++;
    }

    // Devolvemos la proporción de coincidencias
    return contador_iguales / (double) R;
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
 * Función que inicializa A con valores aleatorios con valor absoluto en el intervalo [1,2).
 * @param A Vector a inicializar
 * @param N Tamaño de A
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

/*
 * Función que almacena en ind[] los índices de los elementos de A[] que se deben sumar: 0, D, 2*D, ..., (R-1)*D.
 * @param ind Vector en el que se guardarán los índices de los sumandos (de forma ordenada).
 * @param R Número de sumandos.
 */
void calcular_indices(int ind[], int D, long R){
    int i;

    for (i = 0; i < R; i++) ind[i] = i * D;
}

/*
 * Función auxiliar que cierra el programa e imprime un mensaje de error.
 * @param msg Mensaje de error a imprimir.
 */
void salir(char * msg){
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}
