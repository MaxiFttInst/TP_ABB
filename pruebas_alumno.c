#include "pa2m.h"
#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>

int comparador_int(void *a, void *b)
{
	return *(int *)a - *(int *)b;
}
struct ctx_iterador {
	int *vector;
	size_t pos;
};
bool funcion_iterador(void *_elemento, void *_ctx)
{
	struct ctx_iterador *ctx = _ctx;
	pa2m_afirmar(ctx->vector[ctx->pos] == *(int *)_elemento,
		     "Esperado %d <-> Obtenido %d", ctx->vector[ctx->pos],
		     *(int *)_elemento);
	ctx->pos++;
	return true;
}
void prueba_creacion_destruccion()
{
	printf(CYAN "CREACION DESTRUCCION \n");
	abb_t *arbol = abb_crear(comparador_int);
	pa2m_afirmar(arbol != NULL, "El arbol es distinto de NULL");
	abb_destruir(arbol);
}
void prueba_agregar_cosas()
{
	printf(CYAN "AGREGAR COSAS\n");
	abb_t *arbol = abb_crear(comparador_int);
	int valores[3] = { 81, 33, 46 };
	bool fue_insertado = false;
	for (int i = 0; i < 3; i++) {
		fue_insertado = false;
		fue_insertado = abb_insertar(arbol, &valores[i]);
		pa2m_afirmar(fue_insertado, "Fue insertado");
		abb_insertar(arbol, &valores[i]);
	}
	abb_destruir(arbol);
}
void prueba_obtener_nodo()
{
	printf(CYAN "OBTENER NODO \n");
	abb_t *arbol = abb_crear(comparador_int);
	int valores[3] = { 81, 33, 46 };
	for (int i = 0; i < 3; i++) {
		abb_insertar(arbol, &valores[i]);
	}

	void *obtenido = abb_obtener(arbol, &valores[1]);
	pa2m_afirmar(*(int *)obtenido == valores[1], "Encontramos al elemento");
	abb_destruir(arbol);
}
void prueba_iterar()
{
	printf(CYAN "OBTENER NODO \n");
	abb_t *arbol = abb_crear(comparador_int);
	int valores[3] = { 81, 33, 46 };
	for (int i = 0; i < 3; i++) {
		abb_insertar(arbol, &valores[i]);
	}
	int esperado[3] = { 33, 46, 81 };
	struct ctx_iterador ctx = { .vector = esperado, .pos = 0 };
	abb_iterar_inorden(arbol, funcion_iterador, &ctx);
	abb_destruir(arbol);
}
/*
 * Aañade 10 elementos: 81 33 46 9 44 49 39 63 15 63
 * se espera: img/pruebas_esperado.jpg (iguales como hijos izquierdos)
 *
 * quita 5 elementos: 46 15 81 63 39
 * se espera: img/pruebas_esperado_sin_5_elem.jpg
 *
 * recorre usando 3 estrategias diferentes
 * se espera:
 * inorden: 9 33 44 49 63
 * preorden: 33 9 49 44 63
 * postorden: 9 44 63 49 33
 *
 * agrega 3 elementos: 67 23 6
 * se espera: img/pruebas_esperado_mas_3_elem.jpg
 *
 * recorre usando 3 estrategias diferentes
 * se espera:
 * inorden: 6 9 23 33 44 49 63 67
 * preorden: 33 9 6 23 49 44 63 67
 * postorden: 6 23 9 44 67 63 49 33
 *
 * */
void prueba_abb_integral()
{
	printf(CYAN "PRUEBA ABB INTEGRAL \n");

	abb_t *arbol = abb_crear(comparador_int);
	int diez_valores[10] = { 81, 33, 46, 9, 44, 49, 39, 63, 15, 63 };
	bool fue_insertado = false;
	for (int i = 0; i < 10; i++) {
		fue_insertado = false;
		fue_insertado = abb_insertar(arbol, &diez_valores[i]);
		pa2m_afirmar(fue_insertado, "Fue insertado");
	}
	int quitar_cinco_elem[5] = { 46, 15, 81, 63, 39 };
	void *elem_quitado = NULL;
	bool fue_quitado = false;
	for (int i = 0; i < 5; i++) {
		fue_quitado = false;
		fue_quitado =
			abb_quitar(arbol, &quitar_cinco_elem[i], &elem_quitado);
		pa2m_afirmar(fue_quitado, "Fue quitado %d",
			     *(int *)elem_quitado);
	}

	int cinco_inorden_esperados[5] = { 9, 33, 44, 49, 63 };
	int cinco_preorden_esperados[5] = { 33, 9, 49, 44, 63 };
	int cinco_postorden_esperados[5] = { 9, 44, 63, 49, 33 };
	void *cinco_obtenidos[5];

	printf(NORMAL "INORDEN: \n");
	abb_vectorizar_inorden(arbol, cinco_obtenidos, 5);
	for (int i = 0; i < 5; i++)
		pa2m_afirmar(cinco_inorden_esperados[i] ==
				     *(int *)cinco_obtenidos[i],
			     "Esperado %d <-> Obtenido %d",
			     cinco_inorden_esperados[i],
			     *(int *)cinco_obtenidos[i]);
	printf(NORMAL "PREORDEN: \n");
	abb_vectorizar_preorden(arbol, cinco_obtenidos, 5);
	for (int i = 0; i < 5; i++)
		pa2m_afirmar(cinco_preorden_esperados[i] ==
				     *(int *)cinco_obtenidos[i],
			     "Esperado %d <-> Obtenido %d",
			     cinco_preorden_esperados[i],
			     *(int *)cinco_obtenidos[i]);
	printf(NORMAL "POSTORDEN: \n");
	abb_vectorizar_postorden(arbol, cinco_obtenidos, 5);
	for (int i = 0; i < 5; i++)
		pa2m_afirmar(cinco_postorden_esperados[i] ==
				     *(int *)cinco_obtenidos[i],
			     "Esperado %d <-> Obtenido %d",
			     cinco_postorden_esperados[i],
			     *(int *)cinco_obtenidos[i]);

	int tres_elem_insercion[3] = { 67, 23, 6 };

	for (int i = 0; i < 3; i++)
		abb_insertar(arbol, &tres_elem_insercion[i]);

	int ocho_inorden_esperados[8] = { 6, 9, 23, 33, 44, 49, 63, 67 };
	int ocho_preorden_esperados[8] = { 33, 9, 6, 23, 49, 44, 63, 67 };
	int ocho_postorden_esperados[8] = { 6, 23, 9, 44, 67, 63, 49, 33 };
	void *ocho_obtenidos[8];

	printf(NORMAL "INORDEN: \n");
	abb_vectorizar_inorden(arbol, ocho_obtenidos, 5);
	for (int i = 0; i < 8; i++)
		pa2m_afirmar(
			ocho_inorden_esperados[i] == *(int *)ocho_obtenidos[i],
			"Esperado %d <-> Obtenido %d",
			ocho_inorden_esperados[i], *(int *)ocho_obtenidos[i]);
	printf(NORMAL "PREORDEN: \n");
	abb_vectorizar_preorden(arbol, ocho_obtenidos, 5);
	for (int i = 0; i < 8; i++)
		pa2m_afirmar(
			ocho_preorden_esperados[i] == *(int *)ocho_obtenidos[i],
			"Esperado %d <-> Obtenido %d",
			ocho_preorden_esperados[i], *(int *)ocho_obtenidos[i]);
	printf(NORMAL "POSTORDEN: \n");
	abb_vectorizar_postorden(arbol, ocho_obtenidos, 5);
	for (int i = 0; i < 8; i++)
		pa2m_afirmar(ocho_postorden_esperados[i] ==
				     *(int *)ocho_obtenidos[i],
			     "Esperado %d <-> Obtenido %d",
			     ocho_postorden_esperados[i],
			     *(int *)ocho_obtenidos[i]);

	abb_destruir(arbol);
}

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DE ARBOL ===============");
	prueba_creacion_destruccion();
	prueba_agregar_cosas();
	prueba_obtener_nodo();
	prueba_iterar();
	prueba_abb_integral();

	return pa2m_mostrar_reporte();
}
