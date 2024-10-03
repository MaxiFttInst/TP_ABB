#include "abb_internas.h"


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
