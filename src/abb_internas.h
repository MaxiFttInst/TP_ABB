#ifndef ABB_INTERNAS_H_
#define ABB_INTERNAS_H_
#include "abb_estructura_privada.h"
#include <stdbool.h>

struct params_internas {
	nodo_t *nodo;
	bool (*f)(void *, void *);
	void *ctx;
	bool *seguir_iterando;
};

bool interna_insertar(nodo_t **nodo, int (*comparador)(void *, void *),
		      void *elemento);

size_t interna_inorden_recursivo(struct params_internas);

size_t interna_preorden_recursivo(struct params_internas);

size_t interna_postorden_recursivo(struct params_internas);

nodo_t *interna_obtener_nodo(nodo_t *, void *, int (*)(void *, void *),
			     nodo_t **);
nodo_t *interna_obtener_mayor_y_anterior(nodo_t *nodo, nodo_t **anterior);
void interna_destruir_todo(nodo_t *nodo, void (*destructor)(void *));
#endif // ABB_H_
