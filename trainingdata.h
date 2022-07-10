#ifndef TRAININGDATA_H_
#define TRAININGDATA_H_

/**************************************************************************************************
* trainingdata.h: Innehåller funktionalitet för inläsning och lagring av träningsdata till
*                 neurala nätverk. Träningsdatan kan både läsas in från en fil eller via 
*                 tilldelning från tvådimensionella fält innehållande flyttal. Ordningen på
*                 träningsdatan kan också randomiseras, vilket bör göras vid träning.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"

/**************************************************************************************************
* trainingdata: Lagrar träningsdata i dynamiska fält.
**************************************************************************************************/
struct trainingdata
{
   struct double2Dvector x; /* Indata. */
   struct double2Dvector y; /* Utdata (referensvärden). */
   struct uintvector order; /* Lagrar träningsuppsättningarnas ordningsföljd via index. */
   size_t sets; /* Antalet träningsuppsättningar. */
   size_t num_inputs; /* Antalet insignaler i nätverket. */
   size_t num_outputs; /* Antalet utsignaler i nätverket. */
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