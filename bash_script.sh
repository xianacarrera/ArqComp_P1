#!/usr/bin/env bash

factor_L=(0.5 1.5 0.5 0.75 2.0 4.0 8.0)
cache=(1 1 2 2 2 2 2)
lista_D="1 25 50 75 100"

for i in {1..6}
do
    for D in $lista_D
    do
        for j in {1..7}
        do
            ./accesos_cache $factor_L[i] $cache[i] $D
        done
    done        
done
