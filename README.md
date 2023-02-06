# Efecto de la localidad de los accesos a memoria caché en las prestaciones de programas en microprocesadores

El propósito de este trabajo es realizar un estudio sobre la influencia de la localidad de las referencias a datos sobre los tiempos de acceso a memoria caché. 

Dicho estudio tiene en cuenta la organización de la jerarquía de memoria, los fallos caché, los fundamentos de la localidad en memoria, las operaciones de precarga y los efectos provocados por distintas opciones de compilación de código, entre otros.

El análisis gira en torno a una operación de reducción de suma de punto flotante sobre un vector de ``doubles``. Se han tomado medidas del número de ciclos de reloj necesarios para dicha operación en función de los valores de diferentes parámetros presentes en el programa. La ejecución está automatizada a través de un script de bash.

Los resultados se han analizado de forma cualitativa y cuantitativa a través de diferentes representaciones gráficas, algunas de ellas en formato 3D interactivo (véase la carpeta ``Graficas_3D``, donde hay también variantes no empleadas en el informe final en la subcarpeta ``Extra``).

La descripción y el análisis completo del problema están documentados en ``Informe.pdf`` y en ``Apendice.pdf``.

Este trabajo corresponde a la práctica 1 de la asignatura Arquitectura de Computadores, cursada en el 1º cuatrimestre del 3º año del Doble Grado en Ingeniería Informática y Matemáticas de la Universidad de Santiago de Compostela.

## Nomenclatura de las gráficas

En la carpeta ``Graficas_3D`` se incluyen los recursos .html correspondientes a gráficas 3D interactivas.

En primer lugar aparecen 2 gráficas asociadas al conjunto de datos analizado en el informe, para el que se han empleado los valores de D "1, 6, 8, 15, 71". 

Aquella terminada en D corresponde a una ejecución con el array de índices ind[]  desordenado y empleando la opción -O0 en la compilación. 

La terminada en O corresponde a una ejecución con el array de índices ind[] ordenado y  empleando la opción -O3 en la compilación. 

En la subcarpeta ``Extra`` aparecen a mayores otras 6 gráficas asociadas a datos probados con valores de D que se han estimado interesantes. Cada conjunto de valores de D se usa en 2 gráficas, una con ind[] desordenado y la opción -O0 (D), y otra con ind[] ordenado y la opción -O3 (O). Los conjuntos de valores de D aquí incluidos son "1, 3, 7, 8, 16", "10, 30, 50, 70, 90" y "14, 15, 16, 17, 18".

En el apéndice anexo hay disponibles imágenes de otros tipos de gráficas para estos valores.

___

# Consequences of locality in cache memory accesses on the capabilities of programs in microprocessors

The purpose of this assignment is to study the influence of locality in data references on access times to cache memory.

The study takes into account the memory hierarchy, cache misses, memory locality foundamental principles, preload operations to the cache, the consequences of different compilation options, etc.

The analysis is centered around a floating-point sum operation, which is then reduced, over a vector of ``doubles``. The time needed for said operation has been measured in clock cycles changing different parameters present in the program, in order to examine the influence of each of them. The execution and results generation are automatically performed using a bash script.

The results have been qualitatively and quantitavively studied through several graphical representations, some of them in a 3D interactive format (see the ``Graficas_3D`` folder, where there are also some variants that ended up not being included in the final report, in the subfolder called ``Extra``).

The full description and analysis have been documented in ``Informe.pdf`` and ``Apendice.pdf``, which comprise the report and, ultimately, the hand-in for the assignment.

This assignment was the first project in the Computer Architecture course, taken in the 1st period of the 3rd year of the Double Degree in Informatics Engineering and Mathematics, at the University of Santiago de Compostela.
