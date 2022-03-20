#!/usr/bin/env bash

factor_L=(0.5 1.5 0.5 0.75 2.0 4.0 8.0)
cache=(1 1 2 2 2 2 2)
lista_D="1 5 25 50 64"


# Borramos res_totales si ya existía
rm res_totales.txt
# (mediana opera con el modo "a" sobre res_totales -> se crea si no existe)
    
   
for k in {0..9}    
do
    for D in $lista_D
    do
        for i in {0..6}
        do
            ./accesos_cache $D ${factor_L[i]} ${cache[i]}
        done
    done        
done
 
./mediana2
