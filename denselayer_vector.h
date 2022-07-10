#ifndef DENSELAYER_VECTOR_H_
#define DENSELAYER_VECTOR_H_

/**************************************************************************************************
* denselayer_vector.h: Innehåller funktionalitet för implementering av multipla dense-lager i
*                      ett dynamiskt fält, primärt för implementering av multipla dolda lager i 
*                      ett neuralt nätverk, via strukten denselayer_vector samt motsvarande 
*                      externa funktioner.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"
#include "layer.h"

/**************************************************************************************************
* denselayer_vector: Dynamiskt fält innehållande dense-lager.
**************************************************************************************************/
struct denselayer_vector
{
   struct denselayer* data; /* Pekare till fält innehållande dense-lager. */
   size_t size; /* Antalet dense-lager i fältet. */
};

/* Externa funktioner: */
void denselayer_vector_new(struct denselayer_vector* self);
void denselayer_vector_delete(struct denselayer_vector* self);
struct denselayer_vector* denselayer_vector_ptr_new(void);
void denselayer_vector_ptr_delete(struct denselayer_vector** self);
int denselayer_vector_resize(struct denselayer_vector* self, const size_t new_size);
int denselayer_vector_push(struct denselayer_vector* self, const struct denselayer* new_layer);
int denselayer_vector_pop(struct denselayer_vector* self);
int denselayer_vector_add_layer(struct denselayer_vector* self, const size_t num_nodes, 
   const size_t num_weights);
int denselayer_vector_add_layers(struct denselayer_vector* self, const size_t num_layers,
   const size_t num_nodes, const size_t num_weights);
void denselayer_vector_print(const struct denselayer_vector* self, FILE* stream);
void denselayer_vector_feedforward(struct denselayer_vector* self, const struct doublevector* input);
void denselayer_vector_backpropagate(struct denselayer_vector* self, const struct denselayer* output_layer);
void denselayer_vector_optimize(struct denselayer_vector* self, const struct doublevector* input, 
   const double learning_rate);
struct denselayer* denselayer_vector_begin(const struct denselayer_vector* self);
struct denselayer* denselayer_vector_end(const struct denselayer_vector* self);
struct denselayer* denselayer_vector_last(const struct denselayer_vector* self);

/* Funktionspekare: */
extern void (*denselayer_vector_clear)(struct denselayer_vector* self);

#endif /* DENSELAYER_VECTOR_H_ */