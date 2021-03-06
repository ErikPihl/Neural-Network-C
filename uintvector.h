/**************************************************************************************************
* uintvector.h: Implementering av endimensionella f?lt inneh?llande osignerade heltal via
*               strukten uintvector samt motsvarande externa funktioner. F?lten ?r dynamiska,
*               vilket inneb?r att element kan l?ggas till eller tas bort vid behov.
**************************************************************************************************/

#ifndef UINTVECTOR_H_
#define UINTVECTOR_H_

/* Inkluderingsdirektiv: */
#include "def.h"

/**************************************************************************************************
* uintvector: Dynamiskt f?lt f?r lagring av osignerade heltal.
**************************************************************************************************/
struct uintvector
{
   size_t* data; /* Pekare till dynamiskt f?lt inneh?llande osignerade heltal. */
   size_t size; /* Antalet element i f?ltet. */
};

/* Externa funktioner: */
void uintvector_new(struct uintvector* self);
void uintvector_delete(struct uintvector* self);
struct uintvector* uintvector_ptr_new(void);
void uintvector_ptr_delete(struct uintvector** self);
int uintvector_resize(struct uintvector* self, const size_t new_size);
int  uintvector_push(struct uintvector* self, const size_t new_element);
int uintvector_pop(struct uintvector* self);
void uintvector_print(const struct uintvector* self, FILE* stream);
size_t* uintvector_begin(const struct uintvector* self);
size_t* uintvector_end(const struct uintvector* self);


/* Funktionspekare: */
extern void (*uintvector_clear)(struct uintvector* self);

#endif /* UINTVECTOR_H_ */