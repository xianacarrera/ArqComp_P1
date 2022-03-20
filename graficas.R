datos <- read.table("res_totales_1.txt", header=T, sep=" ", dec=".")
attach(datos)
head(datos)
summary(datos)
str(datos)



#**************************************************************************
# Gráfica 1 (2D): Número de ciclos (eje Y) en función de L (eje X) para 
# cada D (colores).
#**************************************************************************

lista_D <- unique(D)  # Distintos valores que toma D
lista_D

par(mar=c(5, 4, 4, 8), xpd=TRUE)

# Vamos a representar la relación entre L y CK, utilizando una línea distinta
# para cada valor de D (5 líneas en total)
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
grid(nx = NULL, ny = NULL, lty = 2, col = "lightgray", lwd = 1)
par(xpd=T)

legend("right", legend=levels(factor(D)), lty=1, lwd=3, col = 1:5, 
       title = "D", inset=c(-0.2, 0))

mtext(side = 3, line = 2, cex=1, 
      "Coste en ciclos de cada acceso a memoria por número de líneas caché diferentes referenciadas")
mtext(side = 3, line = 1, cex=0.8, 
      "Influencia de L, con datos agrupados por D (número de posiciones entre los elementos a sumar)")

# Especificamos el texto del eje X con mtext para controlar su posición
# side = 1 -> bottom
mtext(side = 1, line = 3.8, cex = 1, 
      "L (número de líneas caché diferentes del vector A[] referenciadas)")
axis(1, at=L, las=3, lwd=1)


#text(CK~L, labels=R, data=datos, cex=0.8, font=2, adj=1)



#**************************************************************************
# Gráfica 2 (2D): Número de ciclos (eje Y) en función de D (eje X) para 
# cada L (colores).
#**************************************************************************

lista_L <- unique(L)    # Distintos valores que toma 
lista_L


# Añadimos espacio extra de márgenes
# Con xpd, controlamos que el plot esté clipped a la región de dibujo
par(mar=c(5, 4, 4, 8), xpd=TRUE)

plot(D[L==min(L)], CK[L==min(L)], type="o", xaxt="n",
     col=1, pch=19,
     ylim=c(floor(min(CK)), ceiling(max(CK))),
     xlab = "",
     ylab = "Número de ciclos de reloj por acceso a memoria")

par(xpd=F)    # Para que el grid no se dibuje por fuera
grid(nx = NULL, ny = NULL, lty = 2, col = "lightgray", lwd = 1)
par(xpd=T)

mtext(side = 3, line = 2, cex=1, 
      "Coste en ciclos de cada acceso a memoria según el espacio impuesto entre los elementos de A[] a sumar")
mtext(side = 3, line = 1, cex=0.8, 
      "Influencia de D, con datos agrupados por L (número de líneas caché diferentes referenciadasr)")

# Recorremos todos los elementos de lista_L menos el primero [-1]
for (i in lista_L[-1]){
  # Con which obtenemos el índice del elemento de la lista que es igual a i
  lines(D[L==i], CK[L==i], type="o", col=which(i==lista_L), pch=19)
}
legend("right", legend=levels(factor(L)), lty=1, lwd=3, col = 1:5,
       title = "L", inset=c(-0.2, 0))


mtext(side = 1, line = 3.8, cex = 1, 
      "D (número de posiciones entre los elementos a sumar de A[])")
axis(1, at=D, las=1, lwd=1)


#**************************************************************************
# Gráfica 2 (3D): Número de ciclos (eje Z) en función de L (eje X) y en 
# función de D (eje Y)
#**************************************************************************


library(plotly)

p3 <- plot_ly(datos, x=~datos$L, y=~datos$D, z=CK, split=~datos$L, 
              type="scatter3d", mode="lines", line=list(width=7)) %>%
  layout(scene=list(
    xaxis = list(title = "L (número de líneas caché diferentes del vector A[] referenciadas)"),
    yaxis = list(title = "D (número de posiciones entre los elementos a sumar de A[])"),
    zaxis = list(title = "Número de ciclos de reloj por acceso a memoria")))
p3

htmlwidgets::saveWidget(as_widget(p3), "index.html")


#**************************************************************************
# Gráfica 3 (3D): Función de obtención de R a partir de L y D, con B fijo.
#**************************************************************************

#install.packages("plot3D")
library(plot3D)
library(rgl)

funcion_R <- function(x, y){
  # x=L, y=D
  # Asumimos B=64
  return(ceiling(64*(x-1)/y*8)+1)
}

open3d()
plot3d(funcion_R, col=colorRampPalette(c("blue", "lightblue", "orange", "red")),
       xlab="L", ylab="D", zlab="R", xlim=c(0,30000), ylim=c(1,99), 
       aspect=c(1,1,0.5))


#**************************************************************************
# Gráfica 4 (2D): Número de ciclos (eje Y) en función de R/L (eje X)
#**************************************************************************

# R/L representa un factor de "localidad espacial". Cuanto mayor es el número
# de datos que tenemos que coger por línea, mayor es la localidad espacial.


############# ANTIGUO
ggplot(R/L, CK, type="p",
     col=factor(L), pch=19,
     ylim=c(floor(min(CK)), ceiling(max(CK))),
     xlab = ,
     ylab = )

par(xpd=F) 
grid(nx = NULL, ny = NULL, lty = 2, col = "lightgray", lwd = 1)
par(xpd=T)

legend("right", legend=levels(factor(L)), lty=1, lwd=3, col = 1:5, 
       title = "D", inset=c(-0.5, 0))

mtext(side = 3, line = 2, cex=1, 
      )
mtext(side = 3, line = 1, cex=0.8, 
      )

###################

#install.packages("ggplot2")
library(ggplot2)

loc_esp = R/L

theme_set(theme_bw())    

ggplot(data = datos, mapping = aes(x = loc_esp, y = CK)) +
         geom_point(aes(color=L, group=L)) +
         geom_smooth(method="lm", formula=y~x) +
         stat_summary(fun.data=mean_cl_normal) +
         labs(subtitle="Influencia de R/L (cantidad de elementos que se suman por línea cache, con valores agrupados por D (espacio entre elementos)",
              y = "Número de ciclos de reloj por acceso a memoria",
              x = "R/L (medida del número elementos a sumar por línea cache diferente a emplear)",
              title = "Coste en ciclos de cada acceso a memoria según la localidad espacial")


