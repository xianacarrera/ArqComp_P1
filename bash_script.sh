#!/usr/bin/env bash

factor_L=(0.5 1.5 0.5 0.75 2.0 4.0 8.0)
cache=(1 1 2 2 2 2 2)
lista_D="1 5 10 50 95"

for i in {0..6}
do
    for D in $lista_D
    do
        for k in {0..9}
        do
            ./accesos_cache ${factor_L[i]} ${cache[i]} $D
        done
        ./mediana
    done        
done
