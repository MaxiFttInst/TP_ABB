#include "abb_internas.h"


size_t interna_iterar_rec_inorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx){
	size_t invocaciones = 0;

	if(nodo->izq != NULL)
		invocaciones += 1 + interna_iterar_rec_inorder(nodo->izq, f, ctx);
	f(nodo->elemento, ctx);
	if(nodo->der != NULL)
		invocaciones += 1 + interna_iterar_rec_inorder(nodo->der, f, ctx);
	else
		return 1;

	return invocaciones;
}

size_t interna_iterar_rec_preorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx);

size_t interna_iterar_rec_postorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx);
