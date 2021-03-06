/**************************************************************************************************
* doublevector.h: Implementering av endimensionella f?lt inneh?llande flyttal via strukten
*                 doublevector samt motsvarande externa funktioner. F?lten ?r dynamiska, vilket
*                 inneb?r att element kan l?ggas till eller tas bort vid behov.
**************************************************************************************************/

#ifndef DOUBLEVECTOR_H_
#define DOUBLEVECTOR_H_

/* Inkluderingsdirektiv: */
#include "def.h"

/**************************************************************************************************
* doublevector: Dynamiskt f?lt f?r lagring av flyttal.
**************************************************************************************************/
struct doublevector
{
   double* data; /* Pekare till dynamiskt f?lt inneh?llande flyttal. */
   size_t size; /* Antalet element i f?ltet. */
};

/* Externa funktioner: */
void doublevector_new(struct doublevector* self);
void doublevector_delete(struct doublevector* self);
struct doublevector* doublevector_ptr_new(void);
void doublevector_ptr_delete(struct doublevector** self);
int doublevector_resize(struct doublevector* self, const size_t new_size);
int  doublevector_push(struct doublevector* self, const double new_element);
int doublevector_pop(struct doublevector* self);
void doublevector_print(const struct doublevector* self, FILE* stream);
double* doublevector_begin(const struct doublevector* self);
double* doublevector_end(const struct doublevector* self);

/* Funktionspekare: */
extern void (*doublevector_clear)(struct doublevector* self);

#endif /* DOUBLEVECTOR_H_ */