#!/usr/bin/env bash

factor_L=(0.5 1.5 0.5 0.75 2.0 4.0 8.0)
cache=(1 1 2 2 2 2 2)
lista_D="5 7 8 9 13"


# Borramos res_totales si ya existía
rm res_temporales.txt       
rm res_totales.txt

# No hace falta crear res_temporales porque main lo abre en "a" -> se crea si no existía

touch res_totales.txt
echo "L D R N CK" >> res_totales.txt
   
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
