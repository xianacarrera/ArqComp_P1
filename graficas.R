datos <- read.table("resultados.txt", header=T, sep=" ", dec=".")
attach(datos)
head(datos)
summary(datos)
str(datos)

plot(D, CK, type="p", pwd=16)

library(plotly)

plot_ly(x=L, y=D, z=CK, type="scatter3d", mode="markers", color=CK)

