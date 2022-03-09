datos <- read.table("resultados.txt", header=T, sep=" ", dec=".")
attach(datos)
head(datos)
summary(datos)
str(datos)

plot(D, CK, type="p", pwd=16)
