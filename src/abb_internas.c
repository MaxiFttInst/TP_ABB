#include "abb_internas.h"
#include "abb_estructura_privada.h"

nodo_t *crear_nodo(void *elemento)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->izq = NULL;
	nuevo_nodo->der = NULL;
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
size_t interna_inorden_recursivo(struct params_internas p)
{
	if (!p.seguir_iterando)
		return 0;
	size_t invocaciones = 0;

	if (p.nodo->izq != NULL) {
		p.nodo = p.nodo->izq;
		invocaciones += interna_inorden_recursivo(p);
	}
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);

	if (p.seguir_iterando) {
		if (p.nodo->der != NULL) {
			p.nodo = p.nodo->der;
			invocaciones += interna_inorden_recursivo(p);
		} else
			return 1;
	}

	return invocaciones;
}

size_t interna_preorden_recursivo(struct params_internas p)
{
	return 0;
}

size_t interna_postorden_recursivo(struct params_internas p)
{
	if (!p.seguir_iterando)
		return 0;

	size_t invocaciones = 0;

	if (p.nodo->izq != NULL) {
		p.nodo = p.nodo->izq;
		invocaciones += interna_postorden_recursivo(p);
	}

	if (p.nodo->der != NULL) {
		p.nodo = p.nodo->der;
		invocaciones += interna_postorden_recursivo(p);
	}
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);
	invocaciones++;
	return invocaciones;
}

nodo_t *interna_obtener_nodo(nodo_t *nodo, void *elemento,
			     int (*comparador)(void *, void *),
			     nodo_t **anterior)
{
	if (comparador(nodo->elemento, elemento) == 0)
		return nodo;
	if (comparador(elemento, nodo->elemento) > 0 && nodo->der != NULL)
		return interna_obtener_nodo(nodo->der, elemento, comparador,
					    &nodo);
	if (comparador(elemento, nodo->elemento) < 0 && nodo->izq != NULL)
		return interna_obtener_nodo(nodo->izq, elemento, comparador,
					    &nodo);
	return NULL;
}
nodo_t *interna_obtener_menor_y_anterior(nodo_t *nodo, nodo_t **anterior)
{
	if (nodo->izq == NULL)
		return nodo;
	if (nodo->izq->izq != NULL) {
		nodo = nodo->izq;
		interna_obtener_menor_y_anterior(nodo->izq, anterior);
	}
	*anterior = nodo;
	return nodo->izq;
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
