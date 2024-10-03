#include "abb_internas.h"

nodo_t *crear_nodo(void *elemento){
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->izq = NULL;
	nuevo_nodo->der = NULL;
	return nuevo_nodo;
}
bool interna_insertar(nodo_t *nodo, int (*comparador)(void *, void *), void *elemento){
	if(nodo == NULL){
		nodo = crear_nodo(elemento);
		return true;
	} else if(comparador(elemento, nodo) > 0){
		if(nodo->der == NULL){
			nodo->der = crear_nodo(elemento);
			return true;
		} else
			return interna_insertar(nodo->der, comparador, elemento);

	} else if (comparador(elemento, nodo) < 0){
		if(nodo->izq == NULL){
			nodo->izq = crear_nodo(elemento);
			return true;
		} else
			return interna_insertar(nodo->izq, comparador, elemento);

	} else if(comparador(elemento, nodo) == 0){
		if(nodo->izq == NULL){
			nodo->izq = crear_nodo(elemento);
			return true;
		} else
			return interna_insertar(nodo->izq, comparador, elemento);
	}

	return false;
}
size_t interna_inorden_recursivo(struct params_internas p){
	if(!p.seguir_iterando)
		return 0;
	size_t invocaciones = 0;

	if(p.nodo->izq != NULL){
		p.nodo = p.nodo->izq;
		invocaciones += interna_inorden_recursivo(p);
	}
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);

	if(p.seguir_iterando){
		if(p.nodo->der != NULL){
			p.nodo = p.nodo->der;
			invocaciones += interna_inorden_recursivo(p);
		}
		else
			return 1;
	}

	return invocaciones;
}

size_t interna_preorden_recursivo(struct params_internas p){
	return 0;
}

size_t interna_postorden_recursivo(struct params_internas p){
	if(!p.seguir_iterando)
		return 0;

	size_t invocaciones = 0;

	if(p.nodo->izq != NULL){
		p.nodo = p.nodo->izq;
		invocaciones += interna_postorden_recursivo(p);
	}

	if(p.nodo->der != NULL){ p.nodo = p.nodo->der;
		invocaciones += interna_postorden_recursivo(p);
	}
	p.seguir_iterando = p.f(p.nodo->elemento, p.ctx);
	invocaciones++;
	return invocaciones;
}

void interna_destruir_todo(nodo_t* nodo, void (*destructor)(void *)){
	if(nodo->izq != NULL)
		interna_destruir_todo(nodo->izq, destructor);

	if(nodo->der != NULL)
		interna_destruir_todo(nodo->der, destructor);

	if(destructor != NULL)
		destructor(nodo->elemento);
	free(nodo);
}
