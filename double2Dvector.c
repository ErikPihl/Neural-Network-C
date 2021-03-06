/**************************************************************************************************
* double2Dvector.c: Inneh?ller funktionsdefinitioner som anv?nds f?r implementering av 
*                   tv?dimensionella f?lt inneh?llande flyttal.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "double2Dvector.h"

/**************************************************************************************************
* double2Dvector_new: Initierar ett tomt tv?dimensionellt dynamiskt f?lt.
**************************************************************************************************/
void double2Dvector_new(struct double2Dvector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* double2Dvector_delete: Frig?r minne f?r ett tv?dimensionellt dynamiskt f?lt.
**************************************************************************************************/
void double2Dvector_delete(struct double2Dvector* self)
{
   for (struct doublevector* i = self->data; i < self->data + self->size; ++i)
   {
      doublevector_delete(i);
   }

   free(self->data);
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* double2Dvector_ptr_new: Returnerar pekare till ett nytt tv?dimensionellt dynamiskt f?lt 
*                         allokerat p? heapen.
**************************************************************************************************/
struct double2Dvector* double2Dvector_ptr_new(void)
{
   struct double2Dvector* self = (struct double2Dvector*)malloc(sizeof(struct double2Dvector));
   if (!self) return 0;
   self->data = 0;
   self->size = 0;
   return self;
}

/**************************************************************************************************
* double2Dvector_ptr_delete: Frig?r minne f?r ett tv?dimensionellt dynamiskt f?lt allokerat 
*                            p? heapen.
**************************************************************************************************/
void double2Dvector_ptr_delete(struct double2Dvector** self)
{
   double2Dvector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* double2Dvector_resize: ?ndrar storleken / kapaciteten p? ett tv?dimensionellt dynamiskt f?lt.
**************************************************************************************************/
int double2Dvector_resize(struct double2Dvector* self, const size_t new_size)
{
   struct doublevector* copy = (struct doublevector*)realloc(self->data,
      sizeof(struct doublevector) * new_size);
   if (!copy) return 1;
   self->data = copy;
   self->size = new_size;
   return 0;
}

/**************************************************************************************************
* double2Dvector_push: L?gger till en ny vektor l?ngst bak i ett tv?dimensionellt dynamiskt f?lt.
**************************************************************************************************/
int double2Dvector_push(struct double2Dvector* self, const struct doublevector* new_vector)
{
   struct doublevector* copy = (struct doublevector*)realloc(self->data, 
      sizeof(struct doublevector) * (self->size + 1));
   if (!copy) return 1;
   copy[self->size++] = *new_vector;
   self->data = copy;
   return 0;
}

/**************************************************************************************************
* double2Dvector_pop: Tar bort vektorn l?ngst bak i ett tv?dimensionellt dynamiskt f?lt, 
*                     om en s?dan finns.
**************************************************************************************************/
int double2Dvector_pop(struct double2Dvector* self)
{
   if (self->size <= 1)
   {
      double2Dvector_delete(self);
      return 1;
   }
   else
   {
      struct doublevector* copy = (struct doublevector*)realloc(self->data,
         sizeof(struct doublevector) * (self->size - 1));
      if (!copy) return 1;
      self->data = copy;
      self->size--;
      return 0;
   }
}

/**************************************************************************************************
* double2Dvector_print: Skriver ut inneh?ll lagrat i ett tv?dimensionellt dynamiskt f?lt 
*                       via angiven utstr?m.
**************************************************************************************************/
void double2Dvector_print(const struct double2Dvector* self, FILE* stream)
{
   if (!self->size) return;
   if (!stream) stream = stdout;
   size_t num = 1;

   for (const struct doublevector* i = self->data; i < self->data + self->size; ++i)
   {
      fprintf(stream, "Vector %zu:\n", num++);
      doublevector_print(i, stream);
   }
   return;
}

/**************************************************************************************************
* double2Dvector_begin: Returnerar adressen till det f?rsta elementet i givet f?lt.
**************************************************************************************************/
struct doublevector* double2Dvector_begin(const struct double2Dvector* self)
{
   return self->data;
}

/**************************************************************************************************
* double2Dvector_end: Returnerar adressen direkt efter det sista elementet i givet f?lt.
**************************************************************************************************/
struct doublevector* double2Dvector_end(const struct double2Dvector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* double2Dvector_clear: Frig?r minne f?r ett tv?dimensionellt dynamiskt f?lt.
**************************************************************************************************/
void (*double2Dvector_clear)(struct double2Dvector* self) = &double2Dvector_delete;
