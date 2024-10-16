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
size_t interna_inorden_recursivo(struct params_internas p)
{
	// if (!p.seguir_iterando)
	// 	return 0;
	size_t invocaciones = 0;
	struct params_internas siguiente = { .nodo = NULL,
					     .f = p.f,
					     .seguir_iterando =
						     p.seguir_iterando,
					     .ctx = p.ctx };

	if (p.nodo->izq != NULL) {
		siguiente.nodo = p.nodo->izq;
		invocaciones += interna_inorden_recursivo(siguiente);
		p.seguir_iterando = siguiente.seguir_iterando;
	}
	// if (!p.seguir_iterando)
	// 	return invocaciones;

	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);
	if (p.seguir_iterando)
		invocaciones += 1;

	// if (!p.seguir_iterando)
	// 	return invocaciones;

	if (p.nodo->der != NULL) {
		siguiente.nodo = p.nodo->der;
		invocaciones += interna_inorden_recursivo(siguiente);
		p.seguir_iterando = siguiente.seguir_iterando;
	}

	return invocaciones;
}

size_t interna_preorden_recursivo(struct params_internas p)
{
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);
	if (!p.seguir_iterando)
		return 0;
	size_t invocaciones = 1;
	struct params_internas siguiente = { .nodo = NULL,
					     .f = p.f,
					     .seguir_iterando =
						     p.seguir_iterando,
					     .ctx = p.ctx };

	if (p.nodo->izq != NULL) {
		siguiente.nodo = p.nodo->izq;
		invocaciones += interna_preorden_recursivo(siguiente);
	}

	if (p.nodo->der != NULL) {
		siguiente.nodo = p.nodo->der;
		invocaciones += interna_preorden_recursivo(siguiente);
	}

	return invocaciones;
}

size_t interna_postorden_recursivo(struct params_internas p)
{
	if (!p.seguir_iterando)
		return 0;
	struct params_internas siguiente = { .nodo = NULL,
					     .f = p.f,
					     .seguir_iterando =
						     p.seguir_iterando,
					     .ctx = p.ctx };

	size_t invocaciones = 0;

	if (p.nodo->izq != NULL) {
		siguiente.nodo = p.nodo->izq;
		invocaciones += interna_postorden_recursivo(siguiente);
	}

	if (p.nodo->der != NULL) {
		siguiente.nodo = p.nodo->der;
		invocaciones += interna_postorden_recursivo(siguiente);
	}
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);
	if (p.seguir_iterando)
		invocaciones++;
	return invocaciones;
}

nodo_t *interna_obtener_nodo(nodo_t *nodo, void *elemento,
			     int (*comparador)(void *, void *),
			     nodo_t **anterior)
{
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
	if (nodo->der == NULL)
		return nodo;
	if (nodo->der->der != NULL) {
		nodo = nodo->der;
		interna_obtener_mayor_y_anterior(nodo->der, anterior);
	}
	*anterior = nodo;
	return nodo->der;
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
