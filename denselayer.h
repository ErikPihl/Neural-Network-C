/**************************************************************************************************
* denselayer.h: Inneh?ller funktionalitet f?r implementering av dense-lager i neurala n?tverk
*               via strukten denselayer samt motsvarande externa funktioner.
**************************************************************************************************/

#ifndef DENSELAYER_H_
#define DENSELAYER_H_

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"

/**************************************************************************************************
* denselayer: Implementering av ett dense-lager i ett neuralt n?tverk, kan anv?nda f?r dolda
*             lager samt det yttre lagret i ett regulj?rt neuralt n?tverk.
**************************************************************************************************/
struct denselayer
{
   struct doublevector output; /* Utsignaler fr?n respektive nod.. */
   struct doublevector bias; /* Biasv?rden / vilov?rden f?r respektive nod. */
   struct doublevector error; /* Aktuell fel f?r respektive nod. */
   struct double2Dvector weights; /* Vikter f?r respektive nod. */
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