VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc

all: clean formatear valgrind-tp_abb valgrind-alumno

valgrind-alumno: clean formatear pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

valgrind-tp_abb: clean formatear tp_abb
	valgrind $(VALGRIND_FLAGS) ./tp_abb "ejemplos/pokedex.csv"

tp_abb: src/*.c tp_abb.c
	$(CC) $(CFLAGS) src/*.c ejemplo.c -o ejemplo

formatear:
	clang-format --style=file -i *.c src/*.c src/*.h

clean:
	rm -f pruebas_alumno tp_abb
