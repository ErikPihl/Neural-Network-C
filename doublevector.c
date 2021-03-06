/**************************************************************************************************
* doublevector.c: Inneh?ller funktionsdefinitioner som anv?nds f?r implementering av 
*                 endimensionella f?lt inneh?llande flyttal.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "doublevector.h"

/**************************************************************************************************
* doublevector_new: Initierar ett tomt dynamiskt f?lt.
**************************************************************************************************/
void doublevector_new(struct doublevector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* doublevector_delete: Frig?r minne f?r ett dynamiskt f?lt.
**************************************************************************************************/
void doublevector_delete(struct doublevector* self)
{
   free(self->data);
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* doublevector_ptr_new: Returnerar pekare till ett nytt dynamiskt f?lt allokerat p? heapen.
**************************************************************************************************/
struct doublevector* doublevector_ptr_new(void)
{
   struct doublevector* self = (struct doublevector*)malloc(sizeof(struct doublevector));
   if (!self) return 0;
   self->data = 0;
   self->size = 0;
   return self;
}

/**************************************************************************************************
* doublevector_ptr_delete: Frig?r minne f?r ett dynamiskt f?lt allokerat p? heapen. 
**************************************************************************************************/
void doublevector_ptr_delete(struct doublevector** self)
{
   doublevector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* doublevector_resize: ?ndrar storleken / kapaciteten p? ett dynamiskt f?lt.
**************************************************************************************************/
int doublevector_resize(struct doublevector* self, const size_t new_size)
{
   double* copy = (double*)realloc(self->data, sizeof(double) * new_size);
   if (!copy) return 1;
   self->data = copy;
   self->size = new_size;
   return 0;
}

/**************************************************************************************************
* doublevector_push: L?gger till ett nytt element l?ngst bak i ett dynamiskt f?lt.
**************************************************************************************************/
int doublevector_push(struct doublevector* self, const double new_element)
{
   double* copy = (double*)realloc(self->data, sizeof(double) * (self->size + 1));
   if (!copy) return 1;
   copy[self->size++] = new_element;
   self->data = copy;
   return 0;
}

/**************************************************************************************************
* doublevector_pop: Tar bort ett element l?ngst bak i ett dynamiskt f?lt, om s?dant finns.
**************************************************************************************************/
int doublevector_pop(struct doublevector* self)
{
   if (self->size <= 1)
   {
      doublevector_delete(self);
      return 1;
   }
   else
   {
      double* copy = (double*)realloc(self->data, sizeof(double) * (self->size - 1));
      if (!copy) return 1;
      self->data = copy;
      self->size--;
      return 0;
   }
}

/**************************************************************************************************
* doublevector_print: Skriver ut inneh?ll lagrat i ett dynamiskt f?lt via angiven utstr?m.
**************************************************************************************************/
void doublevector_print(const struct doublevector* self, FILE* stream)
{
   if (!self->size) return;
   if (!stream) stream = stdout;
   fprintf(stream, "----------------------------------------------------------------------------\n");

   for (const double* i = self->data; i < self->data + self->size; ++i)
   {
      fprintf(stream, "%g\n", *i);
   }

   fprintf(stream, "----------------------------------------------------------------------------\n\n");
   return;
}

/**************************************************************************************************
* doublevector_begin: Returnerar adressen till det f?rsta elementet i givet f?lt.
**************************************************************************************************/
double* doublevector_begin(const struct doublevector* self)
{
   return self->data;
}

/**************************************************************************************************
* doublevector_end: Returnerar adressen direkt efter det sista elementet i givet f?lt.
**************************************************************************************************/
double* doublevector_end(const struct doublevector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* doublevector_clear: Frig?r minne f?r ett dynamiskt f?lt.
**************************************************************************************************/
void (*doublevector_clear)(struct doublevector* self) = &doublevector_delete;
