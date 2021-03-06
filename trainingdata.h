/**************************************************************************************************
* trainingdata.h: Inneh?ller funktionalitet f?r inl?sning och lagring av tr?ningsdata till
*                 neurala n?tverk. Tr?ningsdatan kan b?de l?sas in fr?n en fil eller via
*                 tilldelning fr?n tv?dimensionella f?lt inneh?llande flyttal. Ordningen p?
*                 tr?ningsdatan kan ocks? randomiseras, vilket b?r g?ras vid tr?ning.
**************************************************************************************************/

#ifndef TRAININGDATA_H_
#define TRAININGDATA_H_

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"

/**************************************************************************************************
* trainingdata: Lagrar tr?ningsdata i dynamiska f?lt.
**************************************************************************************************/
struct trainingdata
{
   struct double2Dvector x; /* Indata. */
   struct double2Dvector y; /* Utdata (referensv?rden). */
   struct uintvector order; /* Lagrar tr?ningsupps?ttningarnas ordningsf?ljd via index. */
   size_t sets; /* Antalet tr?ningsupps?ttningar. */
   size_t num_inputs; /* Antalet insignaler i n?tverket. */
   size_t num_outputs; /* Antalet utsignaler i n?tverket. */
};

/* Externa funktioner: */
void trainingdata_new(struct trainingdata* self, const size_t num_inputs, const size_t num_outputs);
void trainingdata_delete(struct trainingdata* self);
struct trainingdata* trainingdata_ptr_new(const size_t inputs, const size_t outputs);
void trainingdata_ptr_delete(struct trainingdata** self);
void trainingdata_clear(struct trainingdata* self);
void trainingdata_load(struct trainingdata* self, const char* filepath);
void trainingdata_set(struct trainingdata* self, const struct double2Dvector* x, const struct double2Dvector* y);
void trainingdata_shuffle(struct trainingdata* self);
void trainingdata_print(const struct trainingdata* self, FILE* stream);

#endif /* TRAININGDATA_H_ */