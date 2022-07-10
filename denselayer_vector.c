/**************************************************************************************************
* denselayer_vector.c: Innehåller funktionsdefinitioner som används för implementering av multipla
*                      dense-lager i neurala nätverk, primärt avsett för dolda lager.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "denselayer_vector.h"

/**************************************************************************************************
* denselayer_vector_new: Initierar objekt för lagring av multipla dense-lager i ett dynamiskt fält.
**************************************************************************************************/
void denselayer_vector_new(struct denselayer_vector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* denselayer_vector_delete: Frigör minne allokerat för multipla dense-lager i ett dynamiskt fält.
**************************************************************************************************/
void denselayer_vector_delete(struct denselayer_vector* self)
{
   free(self->data);
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* denselayer_vector_ptr_new: Returerar en pekare till ett nytt heapallokerat objekt för lagring
*                            av multipla dense-lager i ett dynamiskt fält.
**************************************************************************************************/
struct denselayer_vector* denselayer_vector_ptr_new(void)
{
   struct denselayer_vector* self = (struct denselayer_vector*)malloc(sizeof(struct denselayer_vector));
   if (!self) return 0;
   self->data = 0;
   self->size = 0;
   return self;
}

/**************************************************************************************************
* denselayer_vector_ptr_delete: Frigör minne för heapallokerat objekt för lagring av multipla
*                               dense-lager i ett dynamiskt fält.
**************************************************************************************************/
void denselayer_vector_ptr_delete(struct denselayer_vector** self)
{
   denselayer_vector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* denselayer_vector_resize: Ändrar storleken på fält för lagring av multipla dense-lager.
**************************************************************************************************/
int denselayer_vector_resize(struct denselayer_vector* self, const size_t new_size)
{
   struct denselayer* copy = (struct denselayer*)realloc(self->data, 
      sizeof(struct denselayer) * new_size);
   if (!copy) return 1;
   self->data = copy;
   self->size = new_size;
   return 0;
}

/**************************************************************************************************
* denselayer_vector_push: Lägger till ett nytt dense-lager längst bak i ett dynamiskt fält.
**************************************************************************************************/
int denselayer_vector_push(struct denselayer_vector* self, const struct denselayer* new_layer)
{
   struct denselayer* copy = (struct denselayer*)realloc(self->data, 
      sizeof(struct denselayer) * (self->size + 1));
   if (!copy) return 1;
   copy[self->size++] = *new_layer;
   self->data = copy;
   return 0;
}

/**************************************************************************************************
* denselayer_vector_pop: Tar bort ett dense-lager längst bak i ett dynamiskt fält.
**************************************************************************************************/
int denselayer_vector_pop(struct denselayer_vector* self)
{
   if (self->size <= 1)
   {
      denselayer_vector_delete(self);
   }
   else
   {
      struct denselayer* copy = (struct denselayer*)realloc(self->data,
         sizeof(struct denselayer) * (self->size - 1));
      if (!copy) return 1;
      self->data = copy;
      self->size--;
   }
   return 0;
}

/**************************************************************************************************
* denselayer_vector_add_layer: Lägger till ett nytt dense-lager längst bak i givet dynamiskt fält.
**************************************************************************************************/
int denselayer_vector_add_layer(struct denselayer_vector* self, const size_t num_nodes,
   const size_t num_weights)
{
   struct denselayer new_layer;
   denselayer_new(&new_layer, num_nodes, num_weights);
   return denselayer_vector_push(self, &new_layer);
}

/**************************************************************************************************
* denselayer_vector_add_layers: Lägger till multipla dense-lager i givet dynamiskt fält.
**************************************************************************************************/
int denselayer_vector_add_layers(struct denselayer_vector* self, const size_t num_layers,
   const size_t num_nodes, const size_t num_weights)
{
   const size_t old_size = self->size;
   const size_t new_size = old_size + num_layers;

   if (denselayer_vector_resize(self, new_size))
   {
      return 1;
   }
   else
   {
      struct denselayer* begin = self->data + old_size;
      struct denselayer* end = self->data + new_size;

      for (struct denselayer* i = begin; i < end; ++i)
      {
         struct denselayer new_layer;
         denselayer_new(&new_layer, num_nodes, num_weights);
         *i = new_layer;
      }
   }

   return 0;
}

/**************************************************************************************************
* denselayer_vector_print: Skriver ut information om dense-lager i givet dynamiskt fält.
**************************************************************************************************/
void denselayer_vector_print(const struct denselayer_vector* self, FILE* stream)
{
   for (const struct denselayer* i = self->data; i < self->data + self->size; ++i)
   {
      denselayer_print(i, stream);
   }
   return;
}

/**************************************************************************************************
* denselayer_vector_feedforward: Uppdaterar nodernas utsignaler i samtliga dense-lager.
**************************************************************************************************/
void denselayer_vector_feedforward(struct denselayer_vector* self, const struct doublevector* input)
{
   denselayer_feedforward(self->data, input);

   for (struct denselayer* i = self->data + 1; i < self->data + self->size; ++i)
   {
      const size_t num = i - self->data;
      const struct doublevector* previous_output = &((i - 1)->output);
      denselayer_feedforward(i, previous_output);
   }
   return;
}

/**************************************************************************************************
* denselayer_vector_backpropagate: Beräknar aktuellt fel i samtliga dense-lager.
**************************************************************************************************/
void denselayer_vector_backpropagate(struct denselayer_vector* self, const struct denselayer* output_layer)
{
   struct denselayer* first = self->data;
   struct denselayer* last = self->data + self->size - 1;
   denselayer_backpropagate(last, output_layer);

   for (struct denselayer* i = last - 1; i >= first; --i)
   {
      denselayer_backpropagate(i, i + 1);
   }
   return;
}

/**************************************************************************************************
* denselayer_vector_optimize: Justerar samtliga bias och vikter i syfte att minska aktuellt fel.
**************************************************************************************************/
void denselayer_vector_optimize(struct denselayer_vector* self, const struct doublevector* input,
   const double learning_rate)
{
   struct denselayer* first = self->data;
   struct denselayer* last = self->data + self->size - 1;

   for (struct denselayer* i = last; i > first; --i)
   {
      const struct doublevector* previous_output = &(i - 1)->output;
      denselayer_optimize(i, previous_output, learning_rate);
   }

   denselayer_optimize(first, input, learning_rate);
   return;
}

/**************************************************************************************************
* denselayer_vector_begin: Returnerar adressen till det första dense-lagret i givet fält.
**************************************************************************************************/
struct denselayer* denselayer_vector_begin(const struct denselayer_vector* self)
{
   return self->data;
}

/**************************************************************************************************
* denselayer_vector_end: Returnerar adressen direkt efter sista dense-lagret i givet fält.
**************************************************************************************************/
struct denselayer* denselayer_vector_end(const struct denselayer_vector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* denselayer_vector_last: Returnerar adressen till det sista dense-lagret i givet fält.
**************************************************************************************************/
struct denselayer* denselayer_vector_last(const struct denselayer_vector* self)
{
   if (self->size)
   {
      return self->data + self->size - 1;
   }
   else
   {
      return 0;
   }
}

/**************************************************************************************************
* denselayer_vector_clear: Tömmer fält innehållande dense-lager.
**************************************************************************************************/
void (*denselayer_vector_clear)(struct denselayer_vector* self) = &denselayer_vector_delete;