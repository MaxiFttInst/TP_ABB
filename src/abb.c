#include "abb.h"
#include "abb_estructura_privada.h"
#include "abb_internas.h"
/**
 * Crea un ABB con un comparador.
 */
abb_t *abb_crear(int (*comparador)(void *, void *))
{
	if (comparador == NULL)
		return NULL;

	abb_t *nuevo_abb = calloc(1, sizeof(abb_t));
	if (nuevo_abb != NULL) {
		nuevo_abb->comparador = comparador;
		nuevo_abb->raiz = NULL;
		nuevo_abb->nodos = 0;
	}
	return nuevo_abb;
}

/**
 * Destruye el abb.
 */
void abb_destruir(abb_t *abb)
{
	abb_destruir_todo(abb, NULL);
}

/**
 * Destruye el abb aplicando el destructor a los elementos del usuario.
 */
void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (abb == NULL)
		return;
	if (abb->raiz != NULL)
		interna_destruir_todo(abb->raiz, destructor);
	free(abb);
}

/**
 * Inserta el elemento. Devuelve true si pudo o false si no pudo.
 */
bool abb_insertar(abb_t *abb, void *elemento)
{
	if (abb == NULL)
		return false;
	bool resultado = false;
	resultado = interna_insertar(&(abb->raiz), abb->comparador, elemento);
	if (resultado)
		abb->nodos++;
	return resultado;
}

/**
 * Quita el elemento buscado del arbol. Si lo encuentra y encontrado no es NULL,
 * almacena el puntero.
 *
 * Devuelve true si pudo quitar el elemento.
 */
void ajustar_punteros_hijo_der(abb_t *abb, nodo_t *nodo_encontrado,
			       nodo_t *anterior_encontrado)
{
	if (anterior_encontrado == NULL)
		abb->raiz = nodo_encontrado->der;
	else if (abb->comparador(anterior_encontrado->elemento,
				 nodo_encontrado->elemento) < 0)
		anterior_encontrado->der = nodo_encontrado->der;
	else
		anterior_encontrado->izq = nodo_encontrado->der;
}
void ajustar_punteros_hijo_izq(abb_t *abb, nodo_t *nodo_encontrado,
			       nodo_t *anterior_encontrado)
{
	if (anterior_encontrado == NULL)
		abb->raiz = nodo_encontrado->izq;
	else if (abb->comparador(anterior_encontrado->elemento,
				 nodo_encontrado->elemento) < 0)
		anterior_encontrado->der = nodo_encontrado->izq;
	else
		anterior_encontrado->izq = nodo_encontrado->izq;
}
void ajustar_punteros_dos_hijos(abb_t *abb, nodo_t *nodo_encontrado,
				nodo_t *anterior_encontrado)
{
	nodo_t *anterior_predecesor = NULL;
	nodo_t *nodo_predecesor = interna_obtener_mayor_y_anterior(
		nodo_encontrado->izq, &anterior_predecesor);
	if (anterior_encontrado == NULL)
		abb->raiz = nodo_predecesor;
	else if (abb->comparador(anterior_encontrado->elemento,
				 nodo_predecesor->elemento) < 0)
		anterior_encontrado->der = nodo_predecesor;
	else
		anterior_encontrado->izq = nodo_predecesor;
	if (anterior_predecesor != NULL) {
		anterior_predecesor->der = nodo_predecesor->izq;
		nodo_predecesor->izq = nodo_encontrado->izq;
	}
	nodo_predecesor->der = nodo_encontrado->der;
}
void ajustar_punteros_sin_hijos(abb_t *abb, nodo_t *nodo_encontrado,
				nodo_t *anterior_encontrado)
{
	if (anterior_encontrado == NULL)
		abb->raiz = NULL;
	else if (abb->comparador(anterior_encontrado->elemento,
				 nodo_encontrado->elemento) < 0)
		anterior_encontrado->der = NULL;
	else
		anterior_encontrado->izq = NULL;
}
bool solo_hijo_der(nodo_t *nodo)
{
	return nodo->izq == NULL && nodo->der != NULL;
}
bool solo_hijo_izq(nodo_t *nodo)
{
	return nodo->izq != NULL && nodo->der == NULL;
}
bool tiene_dos_hijos(nodo_t *nodo)
{
	return nodo->izq != NULL && nodo->der != NULL;
}

bool abb_quitar(abb_t *abb, void *buscado, void **encontrado)
{
	if (abb == NULL)
		return false;
	if (abb->raiz == NULL)
		return false;
	nodo_t *anterior_encontrado = NULL;
	nodo_t *nodo_encontrado = NULL;
	nodo_encontrado = interna_obtener_nodo(
		abb->raiz, buscado, abb->comparador, &anterior_encontrado);
	if (nodo_encontrado == NULL)
		return NULL;
	*encontrado = nodo_encontrado->elemento;
	if (solo_hijo_der(nodo_encontrado)) {
		ajustar_punteros_hijo_der(abb, nodo_encontrado,
					  anterior_encontrado);
	} else if (solo_hijo_izq(nodo_encontrado)) {
		ajustar_punteros_hijo_izq(abb, nodo_encontrado,
					  anterior_encontrado);
	} else if (tiene_dos_hijos(nodo_encontrado)) {
		ajustar_punteros_dos_hijos(abb, nodo_encontrado,
					   anterior_encontrado);
	} else {
		ajustar_punteros_sin_hijos(abb, nodo_encontrado,
					   anterior_encontrado);
	}
	free(nodo_encontrado);
	abb->nodos--;
	return true;
}

/**
 * Busca un elemento en el abb. Si lo encuentra lo devuelve. Caso contrario
 * devuelve NULL.
 */
void *abb_obtener(abb_t *abb, void *elemento)
{
	if (abb == NULL)
		return false;
	nodo_t *anterior = NULL;
	nodo_t *encontrado = interna_obtener_nodo(abb->raiz, elemento,
						  abb->comparador, &anterior);
	if (encontrado == NULL)
		return NULL;
	return encontrado->elemento;
}

/**
 * Devuelve la cantidad de elementos en el abb.
 */
size_t abb_cantidad(abb_t *abb)
{
	if (abb == NULL)
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
size_t abb_iterar_inorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (abb == NULL || abb->raiz == NULL || f == NULL)
		return 0;
	bool seguir_iterando = true;
	struct params_internas p = { .nodo = abb->raiz,
				     .f = f,
				     .ctx = ctx,
				     .seguir_iterando = &seguir_iterando };
	return interna_inorden_recursivo(p);
}
size_t abb_iterar_preorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (abb == NULL || abb->raiz == NULL || f == NULL)
		return 0;
	bool seguir_iterando = true;
	struct params_internas p = { .nodo = abb->raiz,
				     .f = f,
				     .ctx = ctx,
				     .seguir_iterando = &seguir_iterando };
	return interna_preorden_recursivo(p);
}
size_t abb_iterar_postorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (abb == NULL || abb->raiz == NULL || f == NULL)
		return 0;
	bool seguir_iterando = true;
	struct params_internas p = { .nodo = abb->raiz,
				     .f = f,
				     .ctx = ctx,
				     .seguir_iterando = &seguir_iterando };
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
bool vectorizadora_por_iteracion(void *dato, void *_ctx)
{
	struct ctx_vectorizacion *ctx = (struct ctx_vectorizacion *)_ctx;
	bool resultado = false;
	if (ctx->posicion < ctx->tamanio) {
		ctx->vector[ctx->posicion] = dato;
		ctx->posicion++;
		resultado = true;
	}
	return resultado;
}
size_t abb_vectorizar_inorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (vector == NULL)
		return 0;
	struct ctx_vectorizacion ctx = { .vector = vector,
					 .tamanio = tamaño,
					 .posicion = 0 };
	return abb_iterar_inorden(abb, vectorizadora_por_iteracion, &ctx);
}
size_t abb_vectorizar_preorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (vector == NULL)
		return 0;
	struct ctx_vectorizacion ctx = { .vector = vector,
					 .tamanio = tamaño,
					 .posicion = 0 };
	return abb_iterar_preorden(abb, vectorizadora_por_iteracion, &ctx);
}
size_t abb_vectorizar_postorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (vector == NULL)
		return 0;
	struct ctx_vectorizacion ctx = { .vector = vector,
					 .tamanio = tamaño,
					 .posicion = 0 };
	return abb_iterar_postorden(abb, vectorizadora_por_iteracion, &ctx);
}
