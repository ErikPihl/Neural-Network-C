/**************************************************************************************************
* double2Dvector.h: Implementering av tv?dimensionella f?lt inneh?llande flyttal via strukten
*                   double2Dvector samt motsvarande externa funktioner. I praktiken inneh?ller
*                   varje tv?dimensionellt f?lt adresser till multipla endimensionella f?lt,
*                   implementerat via strukten doublevector, som i sin tur inneh?ller flyttal.
**************************************************************************************************/

#ifndef DOUBLE2DVECTOR_H_
#define DOUBLE2DVECTOR_H_

/* Inkluderingsdirektiv: */
#include "def.h"
#include "doublevector.h"

/**************************************************************************************************
* double2Dvector: Tv?dimensionellt dynamiskt f?lt f?r lagring av flyttal.
**************************************************************************************************/
struct double2Dvector
{
   struct doublevector* data; /* Pekare till multipla dynamiska f?lt. */
   size_t size; /* Antalet element i f?ltet. */
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