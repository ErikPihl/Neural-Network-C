/**************************************************************************************************
* double2Dvector.c: Innehåller funktionsdefinitioner som används för implementering av 
*                   tvådimensionella fält innehållande flyttal.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "double2Dvector.h"

/**************************************************************************************************
* double2Dvector_new: Initierar ett tomt tvådimensionellt dynamiskt fält.
**************************************************************************************************/
void double2Dvector_new(struct double2Dvector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* double2Dvector_delete: Frigör minne för ett tvådimensionellt dynamiskt fält.
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
* double2Dvector_ptr_new: Returnerar pekare till ett nytt tvådimensionellt dynamiskt fält 
*                         allokerat på heapen.
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
* double2Dvector_ptr_delete: Frigör minne för ett tvådimensionellt dynamiskt fält allokerat 
*                            på heapen.
**************************************************************************************************/
void double2Dvector_ptr_delete(struct double2Dvector** self)
{
   double2Dvector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* double2Dvector_resize: Ändrar storleken / kapaciteten på ett tvådimensionellt dynamiskt fält.
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
* double2Dvector_push: Lägger till en ny vektor längst bak i ett tvådimensionellt dynamiskt fält.
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
* double2Dvector_pop: Tar bort vektorn längst bak i ett tvådimensionellt dynamiskt fält, 
*                     om en sådan finns.
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
* double2Dvector_print: Skriver ut innehåll lagrat i ett tvådimensionellt dynamiskt fält 
*                       via angiven utström.
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
* double2Dvector_begin: Returnerar adressen till det första elementet i givet fält.
**************************************************************************************************/
struct doublevector* double2Dvector_begin(const struct double2Dvector* self)
{
   return self->data;
}

/**************************************************************************************************
* double2Dvector_end: Returnerar adressen direkt efter det sista elementet i givet fält.
**************************************************************************************************/
struct doublevector* double2Dvector_end(const struct double2Dvector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* double2Dvector_clear: Frigör minne för ett tvådimensionellt dynamiskt fält.
**************************************************************************************************/
void (*double2Dvector_clear)(struct double2Dvector* self) = &double2Dvector_delete;
