/**************************************************************************************************
* trainingdata.c: Innehåller funktionsdefinitioner som används för inläsning samt lagring av
*                 träningsdata för neurala nätverk.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "trainingdata.h"

/* Statiska funktioner: */
static void trainingdata_extract(struct trainingdata* self, const char* s);
static bool is_digit(const char c);
static void print_line(const double* data, const size_t size, FILE* stream);

/**************************************************************************************************
* trainingdata_new: Initierar tomt objekt för lagring av träningsdata.
**************************************************************************************************/
void trainingdata_new(struct trainingdata* self, const size_t num_inputs, const size_t num_outputs)
{
   double2Dvector_new(&self->x);
   double2Dvector_new(&self->y);
   uintvector_new(&self->order);
   self->sets = 0;
   self->num_inputs = num_inputs;
   self->num_outputs = num_outputs;
   return;
}

/**************************************************************************************************
* trainingdata_delete: Frigör minne allokerat för given träningsuppsättning.
**************************************************************************************************/
void trainingdata_delete(struct trainingdata* self)
{
   double2Dvector_delete(&self->x);
   double2Dvector_delete(&self->y);
   uintvector_delete(&self->order);
   self->sets = 0;
   self->num_inputs = 0;
   self->num_outputs = 0;
   return;
}

/**************************************************************************************************
* trainingdata_ptr_new: Returnerar pekare till heapallokerat objekt för en träningsuppsättning.
**************************************************************************************************/
struct trainingdata* trainingdata_ptr_new(const size_t inputs, const size_t outputs)
{
   struct trainingdata* self = (struct trainingdata*)malloc(sizeof(struct trainingdata));
   if (!self) return 0;
   trainingdata_new(self, inputs, outputs);
   return self;
}

/**************************************************************************************************
* trainingdata_ptr_delete: Frigör minne för heapallokerat objekt innehållande träningsdata.
**************************************************************************************************/
void trainingdata_ptr_delete(struct trainingdata** self)
{
   trainingdata_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* trainingdata_clear: Nollställer aktuell träningsdata inför inläsning av ny träningsdata.
**************************************************************************************************/
void trainingdata_clear(struct trainingdata* self)
{
   double2Dvector_delete(&self->x);
   double2Dvector_delete(&self->y);
   uintvector_delete(&self->order);
   self->sets = 0;
   return;
}

/**************************************************************************************************
* trainingdata_load: Läser in träningsdata från en fil via angiven filsökväg.
**************************************************************************************************/
void trainingdata_load(struct trainingdata* self, const char* filepath)
{
   FILE* fstream = fopen(filepath, "r");

   if (!fstream)
   {
      fprintf(stderr, "Could not open file at path %s!\n\n", filepath);
   }
   else
   {
      char s[100] = { '\0 ' };
      while (fgets(s, sizeof(s), fstream))
      {
         trainingdata_extract(self, s);
      }
      fclose(fstream);
   }
   return;

}

/**************************************************************************************************
* trainingdata_set: Lägger till träningsdata via tvådimensionella vektorer.
**************************************************************************************************/
void trainingdata_set(struct trainingdata* self, const struct double2Dvector* x, 
   const struct double2Dvector* y)
{
   trainingdata_clear(self);
   self->sets = x->size;
   double2Dvector_resize(&self->x, self->sets);
   double2Dvector_resize(&self->y, self->sets);
   uintvector_resize(&self->order, self->sets);

   self->x = *x;
   self->y = *y;

   for (size_t i = 0; i < self->sets; ++i)
   {
      self->order.data[i] = i;
   }
   return;
}

/**************************************************************************************************
* trainingdata_shuffle: Randomiserar ordningen på träningsdatan via uppsättningarnas index.
**************************************************************************************************/
void trainingdata_shuffle(struct trainingdata* self)
{
   for (size_t i = 0; i < self->sets; ++i)
   {
      const size_t r = (size_t)(rand() % self->sets);
      const size_t temp = self->order.data[i];
      self->order.data[i] = self->order.data[r];
      self->order.data[r] = temp;
   }

   return;
}

/**************************************************************************************************
* trainingdata_print: Skriver ut aktuell träningsdata till given utström.
**************************************************************************************************/
void trainingdata_print(const struct trainingdata* self, FILE* stream)
{
   if (!stream) stream = stdout;

   if (!self->sets)
   {
      fprintf(stream, "No training data!\n\n");
   }
   else
   {
      fprintf(stream, "Number of training sets: %zu\n", self->sets);
      fprintf(stream, "Inputs: %zu\n", self->num_inputs);
      fprintf(stream, "Outputs: % zu\n", self->num_outputs);
      fprintf(stream, "----------------------------------------------------------------------------\n");

      for (size_t i = 0; i < self->sets; ++i)
      {
         fprintf(stream, "Set %zu\n", i + 1);
         fprintf(stream, "Inputs: "); 
         print_line(self->x.data[i].data, self->num_inputs, stream);
         fprintf(stream, "Outputs: ");
         print_line(self->y.data[i].data, self->num_outputs, stream);
         if (i < self->sets - 1) fprintf(stream, "\n");
      }
   }

   fprintf(stream, "----------------------------------------------------------------------------\n\n");
   return;
}

/**************************************************************************************************
* trainingdata_extract: Extraherar träningsdata ur en rad med text. OBS! Se till att textfilen
*                       avslutas med en blank rad, annars sker inte inläsning av sista raden.
**************************************************************************************************/
static void trainingdata_extract(struct trainingdata* self, const char* s)
{
   char num_str[20] = { '\0 ' };
   struct doublevector v = { .data = 0, .size = 0 };
   size_t index = 0;
   const size_t datapoints = self->num_inputs + self->num_outputs;

   for (const char* i = s; *i; ++i)
   {
      if (is_digit(*i) || (i == s && *i == '-'))
      {
         num_str[index++] = *i;
      }
      else
      {
         num_str[index] = '\0';
         const double number = atof(num_str);
         doublevector_push(&v, number);
         index = 0;
      }
   }

   if (v.data && v.size == datapoints)
   {
      struct doublevector x = { .data = 0, .size = 0 };
      struct doublevector y = { .data = 0, .size = 0 };
      doublevector_resize(&x, self->num_inputs);
      doublevector_resize(&y, self->num_outputs);

      for (size_t i = 0; i < self->num_inputs; ++i)
      {
         x.data[i] = v.data[i];
      }
      for (size_t i = 0; i < self->num_outputs; ++i)
      {
         y.data[i] = v.data[i + self->num_inputs];
      }

      double2Dvector_push(&self->x, &x);
      double2Dvector_push(&self->y, &y);
      uintvector_push(&self->order, self->sets++);
   }
   else
   {
      fprintf(stderr, "Could not extract %zu datapoints out of current line!\n\n", datapoints);
   }

   doublevector_clear(&v);
   return;
}

/**************************************************************************************************
* is_digit: Indikerar ifall givet tecken utgör en siffra eller ett decimaltecken (punkt).
**************************************************************************************************/
static bool is_digit(const char c)
{
   const char* s = "0123456789.";
   for (const char* i = s; *i; ++i)
   {
      if (c == *i) return true;
   }
   return false;
}

/**************************************************************************************************
* print_line: Skriver ut flyttal lagrat i ett fält på en enda rad via given utström.
**************************************************************************************************/
static void print_line(const double* data, const size_t size, FILE* stream)
{
   for (const double* i = data; i < data + size; ++i)
   {
      fprintf(stream, "%g ", *i);
   }
   fprintf(stream, "\n");
   return;
}
