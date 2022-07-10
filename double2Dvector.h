/**************************************************************************************************
* double2Dvector.h: Implementering av tvådimensionella fält innehållande flyttal via strukten
*                   double2Dvector samt motsvarande externa funktioner. I praktiken innehåller
*                   varje tvådimensionellt fält adresser till multipla endimensionella fält,
*                   implementerat via strukten doublevector, som i sin tur innehåller flyttal.
**************************************************************************************************/

#ifndef DOUBLE2DVECTOR_H_
#define DOUBLE2DVECTOR_H_

/* Inkluderingsdirektiv: */
#include "def.h"
#include "doublevector.h"

/**************************************************************************************************
* double2Dvector: Tvådimensionellt dynamiskt fält för lagring av flyttal.
**************************************************************************************************/
struct double2Dvector
{
   struct doublevector* data; /* Pekare till multipla dynamiska fält. */
   size_t size; /* Antalet element i fältet. */
};

/* Externa funktioner: */
void double2Dvector_new(struct double2Dvector* self);
void double2Dvector_delete(struct double2Dvector* self);
struct double2Dvector* double2Dvector_ptr_new(void);
void double2Dvector_ptr_delete(struct double2Dvector** self);
int double2Dvector_resize(struct double2Dvector* self, const size_t new_size);
int  double2Dvector_push(struct double2Dvector* self, const struct doublevector* new_vector);
int double2Dvector_pop(struct double2Dvector* self);
void double2Dvector_print(const struct double2Dvector* self, FILE* stream);
struct doublevector* double2Dvector_begin(const struct double2Dvector* self);
struct doublevector* double2Dvector_end(const struct double2Dvector* self);

/* Funktionspekare: */
extern void (*double2Dvector_clear)(struct double2Dvector* self);

#endif /* DOUBLE2DVECTOR_H_ */