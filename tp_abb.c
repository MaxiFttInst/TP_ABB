#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/abb.h"
#include "src/csv.h"

#define BLANCO "\x1b[37;1m"
#define VERDE "\x1b[32;1m"
#define ROJO "\x1b[31;1m"
#define AMARILLO "\x1b[33;1m"
#define NORMAL "\x1b[0m"
#define CYAN "\033[0;36m"
#define AZUL "\033[34m"

struct pokemon {
	char *nombre;
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
};

void liberar_dato(void *_poke)
{
	free(((struct pokemon *)(_poke))->nombre);
	free(_poke);
}
int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	struct pokemon *p1 = _p1;
	struct pokemon *p2 = _p2;

	return strcmp(p1->nombre, p2->nombre);
}
struct buscar_ctx {
	char *nombre;
	struct pokemon **encontrado;
};
bool buscar_en_iter(void *_pokemon, void *_ctx)
{
	struct pokemon *pokemon = _pokemon;
	struct buscar_ctx *ctx = _ctx;
	bool resultado = false;
	if (strcmp(pokemon->nombre, ctx->nombre) == 0) {
		*(ctx->encontrado) = pokemon;
		resultado = true;
	}
	return resultado;
}

bool leer_int(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

bool crear_string_nuevo(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (nuevo == NULL)
		return false;
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool leer_caracter(const char *str, void *ctx)
{
	*(char *)ctx = *(char *)str;
	return true;
}

void cargar_pokemones_desde_csv(abb_t *pokemones, char *nombre_csv)
{
	bool (*funciones[5])(const char *, void *) = { crear_string_nuevo,
						       leer_caracter, leer_int,
						       leer_int, leer_int };
	struct pokemon lectura = { .nombre = NULL,
				   .tipo = 'Z',
				   .fuerza = 0,
				   .destreza = 0,
				   .resistencia = 0 };
	struct pokemon *a_abb = NULL;
	void *punteros[5] = { &lectura.nombre, &lectura.tipo, &lectura.fuerza,
			      &lectura.destreza, &lectura.resistencia };

	struct archivo_csv *csv = abrir_archivo_csv(nombre_csv, ';');

	while (leer_linea_csv(csv, 5, funciones, punteros) == 5) {
		a_abb = calloc(1, sizeof(struct pokemon));
		if (a_abb != NULL) {
			*a_abb = lectura;
			abb_insertar(pokemones, a_abb);
		}
	}
	cerrar_archivo_csv(csv);
}
void buscar_pokemon(abb_t *pokemones, char *nombre)
{
	struct pokemon *poke_actual = NULL;
	struct buscar_ctx ctx = { .nombre = nombre,
				  .encontrado = &poke_actual };
	abb_iterar_inorden(pokemones, buscar_en_iter, &ctx);
	if (poke_actual != NULL) {
		printf(VERDE "++=POKEMON ENCONTRADO=++ \n");
		printf(AMARILLO "Nombre: %s \n", poke_actual->nombre);
		printf(AZUL "Tipo: %c \n", poke_actual->tipo);
		printf(NORMAL "Fuerza: %d \n", poke_actual->fuerza);
		printf(NORMAL "Destreza: %d \n", poke_actual->destreza);
		printf(NORMAL "Resistencia: %d \n", poke_actual->resistencia);
	} else {
		printf(ROJO "XX=POKEMON NO ENCONTRADO=XX \n");
	}
}

bool mostrar_pokemon(void *_pokemon, void *_ctx)
{
	struct pokemon *poke = _pokemon;
	printf(AMARILLO "\n\nNombre: %s \n", poke->nombre);
	printf(AZUL "Tipo: %c \n", poke->tipo);
	printf(NORMAL "Fuerza: %d \n", poke->fuerza);
	printf(NORMAL "Destreza: %d \n", poke->destreza);
	printf(NORMAL "Resistencia: %d \n", poke->resistencia);
	return true;
}
void mostrar_pokemones(abb_t *pokemones)
{
	abb_iterar_inorden(pokemones, mostrar_pokemon, NULL);
}
int main(int argc, char *argv[])
{
	//recibir un archivo por linea de comandos
	//abrir el archivo csv y parsear pokemones
	//agregar los pokemones a una lista
	//
	//El usuario puede elegir una de dos opciones
	//
	//1. Ingrear por teclado un nombre y el programa busca el pokemon en la lista
	//2. Listar todos los pokemones de la pokedex
	//
	//
	if (argc < 2) {
		printf("Ingrese algún argumento válido\n");
		return -1;
	}
	printf(AMARILLO
	       "______ _____ _   __ ________  ________ _   _ \n" AMARILLO
	       "| ___ \\  _  | | / /|  ___|  \\/  |  _  | \\ | |\n" AMARILLO
	       "| |_/ / | | | |/ / | |__ | .  . | | | |  \\| |\n" AMARILLO
	       "|  __/| | | |    \\ |  __|| |\\/| | | | | . ` |\n" AMARILLO
	       "| |   \\ \\_/ / |\\  \\| |___| |  | \\ \\_/ / |\\  |\n" AMARILLO
	       "\\_|    \\___/\\_| \\_/\\____/\\_|  |_/\\___/\\_| \\_/ \n");

	abb_t *pokemones = abb_crear(comparar_nombre_pokemon);
	int opcion = 1;
	int resultado_input = 0;
	char nombre[100] = "";
	cargar_pokemones_desde_csv(pokemones, argv[1]);
	printf(AZUL "Puede elegir entre: \n");
	printf(BLANCO "(1) Buscar un pokemon\n");
	printf(BLANCO "(2) Mostrar todos los pokemon\n");
	printf("\n\nSu opción: ");
	resultado_input = scanf("%d", &opcion);
	if (resultado_input != 1)
		return -1;

	switch (opcion) {
	case 1:
		printf("Escriba el nombre: ");
		resultado_input = scanf("%s", nombre);
		if (resultado_input == 1)
			buscar_pokemon(pokemones, nombre);
		break;
	case 2:
		mostrar_pokemones(pokemones);
		break;
	default:
		printf("No especificó una opción válida");
	}
	abb_destruir_todo(pokemones, liberar_dato);
	return 0;
}
