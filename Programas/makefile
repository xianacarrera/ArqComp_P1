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
SRCS = reduccion.c counter.c
SRCS_MEDIANA = mediana.c

# Cabecera
LIB_HEADERS = $(HEADER_FILES_DIR)/counter.h 

# Nombre del ejecutable
OUTPUT = reduccion
OUTPUT_MEDIANA = mediana

# Archivos objeto respectivos (.o con un .c análogo como fichero fuente)
OBJS = $(SRCS:.c=.o)
OBJS_MEDIANA = $(SRCS_MEDIANA:.c=.o)

# Regla 1
# Creamos el ejecutable de cada programa
all: $(OUTPUT) $(OUTPUT_MEDIANA)

# Regla 1
# Creamos el ejecutable del programa
# $@ es el nombre del archivo que se está generando, $< es el primer prerrequisito
$(OUTPUT): $(OBJS) 
	$(CC) -o $(OUTPUT) $(OBJS) $(OPCIONES)


#fjdksl
$(OUTPUT_MEDIANA): $(OBJS_MEDIANA)
	$(CC) -o $(OUTPUT_MEDIANA) $(OBJS_MEDIANA) $(OPCIONES_MEDIANA)
	

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
