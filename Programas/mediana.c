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


#define NL 7       // Número de valores de L a probar
#define NE 10      // Número de experimentos para cada par (D, L)
#define ND 5       // Número de valores que toma D



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
    long L[NL*ND];                  // 35 valores de L, el número de líneas caché diferentes a acceder
    int D[NL*ND];                   // 35 valores de D, el número de posiciones entre sumandos
    long R[NL*ND];                  // 35 valores de R, el número de sumandos
    long N[NL*ND];                  // 35 valores de N, el tamaño del array
    double CK[NL*ND][NE];           // 350 valores de CK, el tiempo en ciclos de reloj para cada experimento
    double mediana;                 // Mediana de 10 experimentos con los mismos valores de L, D, R y N
    int i, j;                       // Variables de iteración


    // El archivo de resultados temporales sobre el que reduccion.c ha escrito los 350 resultados se abre en modo
    // lectura
    if ((res_temporales = fopen("res_temporales.txt", "r")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados temporales\n");
        exit(EXIT_FAILURE);
    }


    // El archivo de resultados finales sobre el que se escribirán las 35 medianas a obtener se abre en modo
    // escritura. Si no existía previamente, se crea.
    if ((res_finales = fopen("res_totales.txt", "w")) == NULL){
        fprintf(stderr, "Error al abrir el archivo de resultados finales\n");
        exit(EXIT_FAILURE);
    }


    // Los resultados están "desordenados": los 10 resultados de cada experimento no están juntos, sino que el archivo
    // se divide en 10 bloques de 35 entradas cada uno. Por tanto, el bucle exterior para la lectura es de los 10
    // bloques, y el interior es sobre los 35 experimentos.
    // (El motivo de esta organización es incrementar la independencia de las pruebas).
    for (j = 0; j < NE; j++){
    	// Para cada experimento j, se almacenan los 35 tiempos correspondientes a los 35 pares (D,L) distintos por acceso calculados en CK[][j]
        for (i = 0; i < NL * ND; i++){
            // Almacenamos los parámetros
            // L, D, R y N no varían para distintos j
            fscanf(res_temporales, "%ld %d %ld %ld %lf", &L[i], &D[i], &R[i], &N[i], &CK[i][j]);
        }
    }

    // Para cada experimento, obtenemos la mediana
    for (i = 0; i < NL * ND; i++){
        sort(CK[i]);        // Ordenamos CK con un algoritmo de inserción
        // Como hay un número par de valores, realizamos la media de los 2 centrales
        mediana = (CK[i][4] + CK[i][5]) / ((double) 2);
        // Escribimos los resultados sobre el archivo res_totales.txt
        fprintf(res_finales, "%ld %d %ld %ld %f\n", L[i], D[i], R[i], N[i], mediana);
    }

    // Reabrimos el archivo de resultados temporales, esta vez en modo escritura, para borrarlo por completo
    freopen("res_temporales.txt", "w", res_temporales);

    // Cerramos ambos archivos
    fclose(res_temporales);
    fclose(res_finales);

    exit(EXIT_SUCCESS);
}
