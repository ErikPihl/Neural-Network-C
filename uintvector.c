/**************************************************************************************************
* uintvector.c: Innehåller funktionsdefinitioner som används för implementering av 
*               endimensionella fält innehållande osignerade heltal.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "uintvector.h"

/**************************************************************************************************
* uintvector_new: Initierar ett tomt dynamiskt fält.
**************************************************************************************************/
void uintvector_new(struct uintvector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* uintvector_delete: Frigör minne för ett dynamiskt fält.
**************************************************************************************************/
void uintvector_delete(struct uintvector* self)
{
   free(self->data);
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* uintvector_ptr_new: Returnerar pekare till ett nytt dynamiskt fält allokerat på heapen.
**************************************************************************************************/
struct uintvector* uintvector_ptr_new(void)
{
   struct uintvector* self = (struct uintvector*)malloc(sizeof(struct uintvector));
   if (!self) return 0;
   self->data = 0;
   self->size = 0;
   return self;
}

/**************************************************************************************************
* uintvector_ptr_delete: Frigör minne för ett dynamiskt fält allokerat på heapen.
**************************************************************************************************/
void uintvector_ptr_delete(struct uintvector** self)
{
   uintvector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* uintvector_resize: Ändrar storleken / kapaciteten på ett dynamiskt fält.
**************************************************************************************************/
int uintvector_resize(struct uintvector* self, const size_t new_size)
{
   size_t* copy = (size_t*)realloc(self->data, sizeof(size_t) * new_size);
   if (!copy) return 1;
   self->data = copy;
   self->size = new_size;
   return 0;
}

/**************************************************************************************************
* uintvector_push: Lägger till ett nytt element längst bak i ett dynamiskt fält.
**************************************************************************************************/
int uintvector_push(struct uintvector* self, const size_t new_element)
{
   size_t* copy = (size_t*)realloc(self->data, sizeof(size_t) * (self->size + 1));
   if (!copy) return 1;
   copy[self->size++] = new_element;
   self->data = copy;
   return 0;
}

/**************************************************************************************************
* uintvector_pop: Tar bort ett element längst bak i ett dynamiskt fält, om sådant finns.
**************************************************************************************************/
int uintvector_pop(struct uintvector* self)
{
   if (self->size <= 1)
   {
      uintvector_delete(self);
      return 1;
   }
   else
   {
      size_t* copy = (size_t*)realloc(self->data, sizeof(size_t) * (self->size - 1));
      if (!copy) return 1;
      self->data = copy;
      self->size--;
      return 0;
   }
}

/**************************************************************************************************
* uintvector_print: Skriver ut innehåll lagrat i ett dynamiskt fält via angiven utström.
**************************************************************************************************/
void uintvector_print(const struct uintvector* self, FILE* stream)
{
   if (!self->size) return;
   if (!stream) stream = stdout;
   fprintf(stream, "----------------------------------------------------------------------------\n");

   for (const size_t* i = self->data; i < self->data + self->size; ++i)
   {
      fprintf(stream, "%zu\n", *i);
   }

   fprintf(stream, "----------------------------------------------------------------------------\n\n");
   return;
}

/**************************************************************************************************
* uintvector_begin: Returnerar adressen till det första elementet i givet fält.
**************************************************************************************************/
size_t* uintvector_begin(const struct uintvector* self)
{
   return self->data;
}

/**************************************************************************************************
* uintvector_end: Returnerar adressen direkt efter det sista elementet i givet fält.
**************************************************************************************************/
size_t* uintvector_end(const struct uintvector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* uintvector_clear: Frigör minne för ett dynamiskt fält.
**************************************************************************************************/
void (*uintvector_clear)(struct uintvector* self) = &uintvector_delete;
