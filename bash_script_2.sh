#!/usr/bin/env bash


# Xiana Carrera Alonso, Ana Carsi González
# Arquitectura de Computadores - Curso 2021/2022
# Práctica 1 - Jerarquía de memoria y comportamiento de memoria cache: Estudio del efecto de la localidad de los
# accesos a memoria en las prestaciones de programas en microprocesadores
#
# Este programa de bash automatiza las mediciones ejecutando las 350 pruebas requeridas y un programa de cálculo
# de la mediana sobre los 35 experimentos, obteniendo finalmente un archivo de resultados con los 35 valores 
# de ciclos de reloj finales.
# La ejecución de las pruebas se desordena (es decir, se alterna constantemente entre valores de D y L diferentes
# en lugar de realizar las 10 mediciones de forma consecutiva) con el objetivo de minimizar el posible impacto
# de procesos en segundo plano e incrementar la independencia. Se ha observado que esto tiene un impacto
# importante sobre los resultados.


factor_L=(0.5 1.5 0.5 0.75 2.0 4.0 8.0)         # Factores a multiplicar sobre S1 o S2  
cache=(1 1 2 2 2 2 2)               # Índice de S correspondiente a cada factor
lista_D="10 30 50 70 90"            # Valores seleccionados de D


# Borramos res_temporales y res_totales para eliminar cualquier valor anterior
rm res_temporales.txt       
rm res_totales.txt

# No hace falta volver a crear ni res_temporales ni res_totales, ya que reduccion y mediana 
# los crean si no existen (los abren en el modo "a" y "w", respectivamente).

# Escribimos una cabecera para poder interpretar los resultados
echo "L D R N CK" >> res_totales.txt
   
for k in {0..9}         # Cada par (D, L) tiene 10 pruebas asociadas
do
    for D in $lista_D       # Iteramos sobre los 5 valores de D
    do
        for i in {0..6}     # Iteramos sobre los 7 valores de L 
        do
            ./accesos_cache $D ${factor_L[i]} ${cache[i]}       # Programa principal
        done
    done        
done
 
# Una vez obtenidos todos los resultados, el programa de la mediana procesa el archivo correspondiente 
# (res_temporales.txt) y calcula los valores finales, que se guardan en res_totales.txt 
./mediana2   
