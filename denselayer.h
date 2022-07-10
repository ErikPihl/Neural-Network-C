/**************************************************************************************************
* denselayer.h: Innehåller funktionalitet för implementering av dense-lager i neurala nätverk
*               via strukten denselayer samt motsvarande externa funktioner.
**************************************************************************************************/

#ifndef DENSELAYER_H_
#define DENSELAYER_H_

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"

/**************************************************************************************************
* denselayer: Implementering av ett dense-lager i ett neuralt nätverk, kan använda för dolda
*             lager samt det yttre lagret i ett reguljärt neuralt nätverk.
**************************************************************************************************/
struct denselayer
{
   struct doublevector output; /* Utsignaler från respektive nod.. */
   struct doublevector bias; /* Biasvärden / vilovärden för respektive nod. */
   struct doublevector error; /* Aktuell fel för respektive nod. */
   struct double2Dvector weights; /* Vikter för respektive nod. */
   size_t num_nodes; /* Antalet noder i lagret. */
   size_t num_weights; /* Antalet vikter per nod. */
};

/* Externa funktioner: */
void denselayer_new(struct denselayer* self, const size_t num_nodes, const size_t num_weights);
void denselayer_delete(struct denselayer* self);
struct denselayer* denselayer_ptr_new(const size_t num_nodes, const size_t num_weights);
void denselayer_ptr_delete(struct denselayer** self);
void denselayer_clear(struct denselayer* self);
void denselayer_reset(struct denselayer* self);
void denselayer_resize(struct denselayer* self, const size_t num_nodes, const size_t num_weights);
void denselayer_feedforward(struct denselayer* self, const struct doublevector* input);
void denselayer_compare_with_reference(struct denselayer* self, const struct doublevector* reference);
void denselayer_backpropagate(struct denselayer* self, const struct denselayer* next_layer);
void denselayer_optimize(struct denselayer* self, const struct doublevector* input,
   const double learning_rate);
void denselayer_print(const struct denselayer* self, FILE* stream);

#endif /* DENSELAYER_H_ */