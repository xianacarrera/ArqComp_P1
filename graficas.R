# Xiana Carrera Alonso, Ana Carsi González
# Arquitectura de Computadores - Curso 2021/2022
# Práctica 1 - Jerarquía de memoria y comportamiento de memoria cache: Estudio del 
# efecto de la localidad de los accesos a memoria en las prestaciones de programas 
# en microprocesadores.
#
# Este programa de R permite visualizar los resultados obtenidos en forma de 5
# gráficas, 3 en 2D y 2 en 3D. 

# Leemos los datos, con una cabecera inicial, separados por un espacio
# y con un punto como marca decimal
datos <- read.table("1 3 7 8 16 O0 D.txt", header=T, sep=" ", dec=".")
attach(datos)     # Convertimos los componentes de datos en variables 
head(datos)       # Lectura de una muestra de los datos
summary(datos)    # Resumen estadístico de los datos
str(datos)        # Estructura (tipo de dato) de la muestra



#**************************************************************************
# Gráfica 1 (2D): Número de ciclos (eje Y) en función de L (eje X) para 
# cada D (colores).
#**************************************************************************

lista_D <- unique(D)  # Distintos valores que toma D
lista_D

# Añadimos espacio extra de márgenes
# Con xpd, controlamos que el plot esté anclado a la región de dibujo
par(mar=c(5, 4, 4, 8), xpd=TRUE)

# Vamos a representar la relación entre L y CK, utilizando una línea distinta
# para cada valor de D (5 líneas en total)
# No indicamos eje X para configurarlo a continuación manualmente
# En este plot representamos en primer lugar el valor más bajo de D
# El tipo de representación es "overplotted" (puntos y líneas)
plot(L[D==min(D)], CK[D==min(D)], type="o", xaxt="n",
     ylim=c(floor(min(CK)), ceiling(max(CK))),
     col=1, pch=19,
     xlab = "",
     ylab = "Número de ciclos de reloj por acceso a memoria")

# Recorremos todos los elementos de lista_D menos el primero [-1]
for (i in lista_D[-1]){
  # Con which obtenemos el índice del elemento de la lista que es igual a i
  lines(L[D==i], CK[D==i], type="o", col=which(i==lista_D), pch=19)
}

par(xpd=F) 
# Quitamos los márgenes para incluir una rejilla únicamente 
# dentro del área gráfica
grid(nx = NULL, ny = NULL, lty = 2, col = "lightgray", lwd = 1)
# Volvemos a activar los márgenes
par(xpd=T)

# Añadimos una leyenda de colores para D
legend("right", legend=levels(factor(D)), lty=1, lwd=3, col = 1:5, 
       title = "D", inset=c(-0.2, 0))

# Introducimos un título y un subtítulo
mtext(side = 3, line = 2, cex=1, 
      "Coste en ciclos de cada acceso a memoria por número de líneas caché diferentes referenciadas")
mtext(side = 3, line = 1, cex=0.8, 
      "Influencia de L, con datos agrupados por D (número de posiciones entre los elementos a sumar)")

# Especificamos el texto del eje X con mtext para controlar su posición
# (side = 1 -> debajo del gráfico)
mtext(side = 1, line = 3.8, cex = 1, 
      "L (número de líneas caché diferentes del vector A[] referenciadas)")
axis(1, at=L, las=3, lwd=1)



#**************************************************************************
# Gráfica 2 (2D): Número de ciclos (eje Y) en función de D (eje X) para 
# cada L (colores).
#**************************************************************************

lista_L <- unique(L)    # Distintos valores que toma L
lista_L


# Añadimos espacio extra de márgenes
# Con xpd, controlamos que el plot esté anclado a la región de dibujo
par(mar=c(5, 4, 4, 8), xpd=TRUE)

# De forma análoga a la gráfica 1, imprimimos una línea con puntos para 
# cada valor de L, comenzando por aquella de menor valor
plot(D[L==min(L)], CK[L==min(L)], type="o", xaxt="n",
     col=1, pch=19,
     ylim=c(floor(min(CK)), ceiling(max(CK))),
     xlab = "",
     ylab = "Número de ciclos de reloj por acceso a memoria")

par(xpd=F)  
# Quitamos los márgenes para incluir una rejilla únicamente 
# dentro del área gráfica
grid(nx = NULL, ny = NULL, lty = 2, col = "lightgray", lwd = 1)
# Volvemos a activar los márgenes
par(xpd=T)

# Añadimos un título y un subtítulo
mtext(side = 3, line = 2, cex=1, 
      "Coste en ciclos de cada acceso a memoria según el espacio entre los sumandos de A[]")
mtext(side = 3, line = 1, cex=0.8, 
      "Influencia de D, con datos agrupados por L (número de líneas caché diferentes referenciadasr)")

# Recorremos todos los elementos de lista_L menos el primero [-1]
for (i in lista_L[-1]){
  # Con which obtenemos el índice del elemento de la lista que es igual a i
  lines(D[L==i], CK[L==i], type="o", col=which(i==lista_L), pch=19)
}

# Añadimos una leyenda en el margen derecho
legend("right", legend=levels(factor(L)), lty=1, lwd=3, col = 1:7,
       title = "L", inset=c(-0.25, 0))

# Configuramos el eje X y cambiamos las etiquetas para que coincidan con los 
# valores de D
mtext(side = 1, line = 3.8, cex = 1, 
      "D (número de posiciones entre los elementos a sumar de A[])")
axis(1, at=D, las=1, lwd=1)


#**************************************************************************
# Gráfica 3 (3D): Número de ciclos (eje Z) en función de L (eje X) y en 
# función de D (eje Y)
#**************************************************************************


library(plotly)   # Cargamos la librería plotly para representar en 3D

# El tipo de gráfico es scatter3d y utilizamos el modo líneas para poder 
# unir los puntos
p3 <- plot_ly(datos, x=~datos$L, y=~datos$D, z=CK, split=~datos$L, 
              type="scatter3d", mode="lines", line=list(width=7)) %>%
    layout(scene=list(
    xaxis = list(title = "L (número de líneas caché diferentes del vector A[] referenciadas)"),
    yaxis = list(title = "D (número de posiciones entre los elementos a sumar de A[])"),
    zaxis = list(title = "Número de ciclos de reloj por acceso a memoria")))
p3      # Imprimimos el gráfico

# Exportamos el gráfico en forma de un recurso html
htmlwidgets::saveWidget(as_widget(p3), "ciclos3D.html")


#**************************************************************************
# Gráfica 4 (3D): Función de obtención de R a partir de L y D, con B fijo.
#**************************************************************************

# Cargamos plot3D y rgl para representar funciones en 3D
library(plot3D)
library(rgl)

# Definimos la función que se utiliza para obtener R
funcion_R <- function(x, y){
  # x=L, y=D
  # Asumimos B=64
  ifelse(y < 8, ceiling(64*(x-1)/y*8)+1, x)
}

open3d()   # Abrimos una ventana para imprimir en 3D
# Utilizamos un gradiente de colores para representar intensidad en función
# de la magnitud de R
plot3d(funcion_R, col=colorRampPalette(c("blue", "lightblue", "orange", "red")),
       xlab="L", ylab="D", zlab="R", xlim=c(0,30000), ylim=c(1,99), 
       aspect=c(1,1,0.5))

# Guardamos una captura del gráfico
rgl.snapshot('R_plot.png', fmt = 'png')


#**************************************************************************
# Gráfica 5 (2D): Número de ciclos (eje Y) en función de R/L (eje X)
#**************************************************************************

# R/L representa un factor de "localidad espacial". Cuanto mayor es el número
# de datos que tenemos que coger por línea, mayor es la localidad espacial.

library(ggplot2)    # Para este gráfico emplearemos ggplot, que nos permite
# añadir fácilmente una recta de regresión

loc_esp = R/L   # Creamos una variable que guarda R/L

theme_set(theme_bw())    # Tema estético del gráfico

# Representamos con ggplot, poniendo loc_esp en el eje X y los ciclos de reloj
# en el eje Y.
# Añadimos una recta de regresión y una región de confianza
# A continuación, configuramos el título y las etiquetas de los ejes. 
ggplot(data = datos, mapping = aes(x = loc_esp, y = CK)) +
         geom_point(aes(color=L, group=L)) +
         geom_smooth(method="lm", formula=y~x) +
         stat_summary(fun.data=mean_cl_normal) +
         labs(subtitle="Influencia de R/L (cantidad de elementos que se suman por línea cache, con valores agrupados por D",
              y = "Número de ciclos de reloj por acceso a memoria",
              x = "R/L (medida del número elementos a sumar por línea cache diferente a emplear)",
              title = "Coste en ciclos de cada acceso a memoria según la localidad espacial")


