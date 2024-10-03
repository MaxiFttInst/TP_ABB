#include "abb.h"
#include "abb_estructura_privada.h"
#include "abb_internas.h"
/**
 * Crea un ABB con un comparador.
 */
abb_t *abb_crear(int (*comparador)(void *, void *)){
	if (comparador == NULL)
		return NULL;

	abb_t *nuevo_abb = calloc(1, sizeof(abb_t));
	nuevo_abb->comparador = comparador;
	nuevo_abb->raiz = NULL;
	nuevo_abb->nodos = 0;
	return nuevo_abb;
}

/**
 * Destruye el abb.
 */
void abb_destruir(abb_t *abb){
	abb_destruir_todo(abb, NULL);
}

/**
 * Destruye el abb aplicando el destructor a los elementos del usuario.
 */
void abb_destruir_todo(abb_t *abb, void (*destructor)(void *)){
	if(abb->raiz != NULL)
		interna_destruir_todo(abb->raiz, destructor);
	free(abb);
}

/**
 * Inserta el elemento. Devuelve true si pudo o false si no pudo.
 */
bool abb_insertar(abb_t *abb, void *elemento){
	return false;
}

/**
 * Quita el elemento buscado del arbol. Si lo encuentra y encontrado no es NULL,
 * almacena el puntero.
 *
 * Devuelve true si pudo quitar el elemento.
 */
bool abb_quitar(abb_t *abb, void *buscado, void **encontrado){
	return false;
}

/**
 * Busca un elemento en el abb. Si lo encuentra lo devuelve. Caso contrario
 * devuelve NULL.
 */
void *abb_obtener(abb_t *abb, void *elemento){
	return NULL;
}

/**
 * Devuelve la cantidad de elementos en el abb.
 */
size_t abb_cantidad(abb_t *abb){
	if(abb == NULL)
		return 0;

	return abb->nodos;
}

/**
 * Recorre los elementos del abb en el orden específico y aplica la función f a
 * cada uno.
 *
 * Si la función f devuelve false, se deja de iterar.
 *
 * Devuelve la cantidad de veces que fue invocada la función f.
 */
size_t abb_iterar_inorden(abb_t *abb, bool (*f)(void *, void *), void *ctx){
	struct params_internas p = {
		.nodo = abb->raiz,
		.f = f,
		.ctx = ctx,
		.seguir_iterando = true
	};
	return interna_inorden_recursivo(p);
}
size_t abb_iterar_preorden(abb_t *abb, bool (*f)(void *, void *), void *ctx){
	struct params_internas p = {
		.nodo = abb->raiz,
		.f = f,
		.ctx = ctx,
		.seguir_iterando = true
	};
	return interna_preorden_recursivo(p);
}
size_t abb_iterar_postorden(abb_t *abb, bool (*f)(void *, void *), void *ctx){
	struct params_internas p = {
		.nodo = abb->raiz,
		.f = f,
		.ctx = ctx,
		.seguir_iterando = true
	};
	return interna_postorden_recursivo(p);
}

/**
 * Rellena el vector de punteros con los elementos del abb siguiendo un orden
 * dado. Tamaño indica la capacidad del vector.
 *
 * Devuelve la cantidad de elementos guardados en el vector.
 */
struct ctx_vectorizacion {
	void **vector;
	size_t posicion;
	size_t tamanio;
};
bool vectorizadora_por_iteracion(void *dato, void* _ctx){
	struct ctx_vectorizacion *ctx = (struct ctx_vectorizacion*)_ctx;
	bool resultado = false;
	if(ctx->posicion < ctx->tamanio){
		ctx->vector[ctx->posicion] = dato;
		ctx->posicion++;
		resultado = true;
	}
	return resultado;
}
size_t abb_vectorizar_inorden(abb_t *abb, void **vector, size_t tamaño){
	struct ctx_vectorizacion ctx = { .vector = vector,
		.tamanio = tamaño,
		.posicion = 0};
	return abb_iterar_inorden(abb, vectorizadora_por_iteracion, &ctx);
}
size_t abb_vectorizar_preorden(abb_t *abb, void **vector, size_t tamaño){
	struct ctx_vectorizacion ctx = { .vector = vector,
		.tamanio = tamaño,
		.posicion = 0};
	return abb_iterar_preorden(abb, vectorizadora_por_iteracion, &ctx);
}
size_t abb_vectorizar_postorden(abb_t *abb, void **vector, size_t tamaño){
	struct ctx_vectorizacion ctx = { .vector = vector,
		.tamanio = tamaño,
		.posicion = 0};
	return abb_iterar_postorden(abb, vectorizadora_por_iteracion, &ctx);
}

