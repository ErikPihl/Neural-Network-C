/**************************************************************************************************
* uintvector.c: Inneh?ller funktionsdefinitioner som anv?nds f?r implementering av 
*               endimensionella f?lt inneh?llande osignerade heltal.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "uintvector.h"

/**************************************************************************************************
* uintvector_new: Initierar ett tomt dynamiskt f?lt.
**************************************************************************************************/
void uintvector_new(struct uintvector* self)
{
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* uintvector_delete: Frig?r minne f?r ett dynamiskt f?lt.
**************************************************************************************************/
void uintvector_delete(struct uintvector* self)
{
   free(self->data);
   self->data = 0;
   self->size = 0;
   return;
}

/**************************************************************************************************
* uintvector_ptr_new: Returnerar pekare till ett nytt dynamiskt f?lt allokerat p? heapen.
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
* uintvector_ptr_delete: Frig?r minne f?r ett dynamiskt f?lt allokerat p? heapen.
**************************************************************************************************/
void uintvector_ptr_delete(struct uintvector** self)
{
   uintvector_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* uintvector_resize: ?ndrar storleken / kapaciteten p? ett dynamiskt f?lt.
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
* uintvector_push: L?gger till ett nytt element l?ngst bak i ett dynamiskt f?lt.
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
* uintvector_pop: Tar bort ett element l?ngst bak i ett dynamiskt f?lt, om s?dant finns.
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
* uintvector_print: Skriver ut inneh?ll lagrat i ett dynamiskt f?lt via angiven utstr?m.
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
* uintvector_begin: Returnerar adressen till det f?rsta elementet i givet f?lt.
**************************************************************************************************/
size_t* uintvector_begin(const struct uintvector* self)
{
   return self->data;
}

/**************************************************************************************************
* uintvector_end: Returnerar adressen direkt efter det sista elementet i givet f?lt.
**************************************************************************************************/
size_t* uintvector_end(const struct uintvector* self)
{
   return self->data + self->size;
}

/**************************************************************************************************
* uintvector_clear: Frig?r minne f?r ett dynamiskt f?lt.
**************************************************************************************************/
void (*uintvector_clear)(struct uintvector* self) = &uintvector_delete;
