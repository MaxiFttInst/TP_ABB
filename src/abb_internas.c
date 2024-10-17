#include "abb_internas.h"
#include "abb_estructura_privada.h"

nodo_t *crear_nodo(void *elemento)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (nuevo_nodo != NULL) {
		nuevo_nodo->elemento = elemento;
		nuevo_nodo->izq = NULL;
		nuevo_nodo->der = NULL;
	}
	return nuevo_nodo;
}

bool interna_insertar(nodo_t **nodo, int (*comparador)(void *, void *),
		      void *elemento)
{
	if (*nodo == NULL) {
		*nodo = crear_nodo(elemento);
		return true;
	} else if (comparador(elemento, (*nodo)->elemento) > 0) {
		return interna_insertar(&(*nodo)->der, comparador, elemento);

	} else if (comparador(elemento, (*nodo)->elemento) < 0) {
		return interna_insertar(&(*nodo)->izq, comparador, elemento);

	} else if (comparador(elemento, (*nodo)->elemento) == 0) {
		return interna_insertar(&(*nodo)->izq, comparador, elemento);
	}

	return false;
}
size_t interna_inorden_recursivo(nodo_t *nodo, bool (*f)(void *, void *),
				 void *ctx, bool *seguir_iterando)
{
	if (nodo == NULL)
		return 0;
	if (!(*seguir_iterando))
		return 0;
	size_t invocaciones = 0;

	invocaciones +=
		interna_inorden_recursivo(nodo->izq, f, ctx, seguir_iterando);
	if (!(*seguir_iterando))
		return invocaciones;

	*seguir_iterando = f(nodo->elemento, ctx);
	invocaciones += 1;

	if (!(*seguir_iterando))
		return invocaciones;

	invocaciones +=
		interna_inorden_recursivo(nodo->der, f, ctx, seguir_iterando);

	return invocaciones;
}

size_t interna_preorden_recursivo(nodo_t *nodo, bool (*f)(void *, void *),
				  void *ctx, bool *seguir_iterando)
{
	if (nodo == NULL) {
		return 0;
	}

	size_t invocaciones = 1;
	*seguir_iterando = f(nodo->elemento, ctx);
	if (!(*seguir_iterando))
		return 1;

	invocaciones +=
		interna_preorden_recursivo(nodo->izq, f, ctx, seguir_iterando);
	if (!(*seguir_iterando))
		return invocaciones;

	invocaciones +=
		interna_preorden_recursivo(nodo->der, f, ctx, seguir_iterando);

	return invocaciones;
}

size_t interna_postorden_recursivo(nodo_t *nodo, bool (*f)(void *, void *),
				   void *ctx, bool *seguir_iterando)
{
	if (nodo == NULL || !(*seguir_iterando))
		return 0;

	size_t invocaciones = 0;

	invocaciones +=
		interna_postorden_recursivo(nodo->izq, f, ctx, seguir_iterando);
	if (!(*seguir_iterando))
		return invocaciones;

	invocaciones +=
		interna_postorden_recursivo(nodo->der, f, ctx, seguir_iterando);
	if (!(*seguir_iterando))
		return invocaciones;
	*seguir_iterando = f(nodo->elemento, ctx);
	invocaciones++;
	return invocaciones;
}

nodo_t *interna_obtener_nodo(nodo_t *nodo, void *elemento,
			     int (*comparador)(void *, void *),
			     nodo_t **anterior)
{
	if (nodo == NULL)
		return NULL;
	if (comparador(nodo->elemento, elemento) == 0)
		return nodo;
	if (comparador(elemento, nodo->elemento) > 0 && nodo->der != NULL) {
		*anterior = nodo;
		return interna_obtener_nodo(nodo->der, elemento, comparador,
					    anterior);
	}
	if (comparador(elemento, nodo->elemento) < 0 && nodo->izq != NULL) {
		*anterior = nodo;
		return interna_obtener_nodo(nodo->izq, elemento, comparador,
					    anterior);
	}
	return NULL;
}
nodo_t *interna_obtener_mayor_y_anterior(nodo_t *nodo, nodo_t **anterior)
{
	if (nodo->der != NULL) {
		*anterior = nodo;
		return interna_obtener_mayor_y_anterior(nodo->der, anterior);
	}
	return nodo;
}
void interna_destruir_todo(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo->izq != NULL)
		interna_destruir_todo(nodo->izq, destructor);

	if (nodo->der != NULL)
		interna_destruir_todo(nodo->der, destructor);

	if (destructor != NULL)
		destructor(nodo->elemento);
	free(nodo);
}
