# Compilación
CC = gcc
# Opciones de compilación (incluir la librería de _mm_malloc, la librería matemática, no
# optimizar y mostrar todos los warnings)
OPCIONES = -msse3 -lm -O0 -Wall
OPCIONES_MEDIANA = -O0 -Wall

# Carpeta donde se encuentran las cabeceras
HEADER_FILES_DIR = .
# Opción de compilación para incluir el .h
INCLUDES = -I $(HEADER_FILES_DIR)

# Ficheros fuente
SRCS = main.c counter.c
SRCS_MEDIANA = mediana.c
SRCS_MEDIANA_2 = mediana2.c

# Cabecera
LIB_HEADERS = $(HEADER_FILES_DIR)/counter.h 

# Nombre del ejecutable
OUTPUT = accesos_cache
OUTPUT_MEDIANA = mediana
OUTPUT_MEDIANA_2 = mediana2

# Archivos objeto respectivos (.o con un .c análogo como fichero fuente)
OBJS = $(SRCS:.c=.o)
OBJS_MEDIANA = $(SRCS_MEDIANA:.c=.o)
OBJS_MEDIANA_2 = $(SRCS_MEDIANA_2:.c=.o)

# Regla 1
# Creamos el ejecutable de cada programa
all: $(OUTPUT) $(OUTPUT_MEDIANA) $(OUTPUT_MEDIANA_2)

# Regla 1
# Creamos el ejecutable del programa
# $@ es el nombre del archivo que se está generando, $< es el primer prerrequisito
$(OUTPUT): $(OBJS) 
	$(CC) -o $(OUTPUT) $(OBJS) $(OPCIONES)


#fjdksl
$(OUTPUT_MEDIANA): $(OBJS_MEDIANA)
	$(CC) -o $(OUTPUT_MEDIANA) $(OBJS_MEDIANA) $(OPCIONES_MEDIANA)
	
#mediana 2
$(OUTPUT_MEDIANA_2): $(OBJS_MEDIANA_2)
	$(CC) -o $(OUTPUT_MEDIANA_2) $(OBJS_MEDIANA_2) $(OPCIONES_MEDIANA)

# Regla 2
# Creamos cada archivo .o a partir de su correspondiente .c
$(*.o): %.c $(LIB_HEADERS) 
	$(CC) -c -o $@ $< $(INCLUDES)



# Regla 3
# Borra el ejecutable y ejecuta clean dentro del directorio actual
cleanall: clean 
	rm -f $(OUTPUT)

# Regla 4
# Borra todos los archivos .o utilizando el wildcard * (match con cualquier carácter)
# dentro del directorio actual
clean: 
	rm -f *.o
