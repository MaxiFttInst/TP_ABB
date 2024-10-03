#ifndef ABB_INTERNAS_H_
#define ABB_INTERNAS_H_
#include "abb_estructura_privada.h"
#include <stdbool.h>
typedef struct abb abb_t;

size_t interna_iterar_rec_inorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx);

size_t interna_iterar_rec_preorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx);

size_t interna_iterar_rec_postorder(nodo_t* nodo, bool (*f)(void *, void *), void *ctx);

#endif // ABB_H_
