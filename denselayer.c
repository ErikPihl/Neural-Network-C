/**************************************************************************************************
* denselayer.c: Innehåller funktionsdefinitioner som används för implementering av enskilda 
*               dense-lager i neurala nätverk.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "denselayer.h"

/* Statiska funktioner: */
static void denselayer_init(struct denselayer* self);
static void denselayer_set_nodes(struct denselayer* self, const size_t num_nodes);
static void denselayer_set_weights(struct denselayer* self, const size_t num_weights);
static inline double get_random_start_val(void);
static inline double relu(const double sum);
static inline double delta_relu(const double output);
static void print_line(const struct doublevector* self, FILE* stream);

/**************************************************************************************************
* denselayer_new: Initierar objekt som utgör ett dense-lager. Minne allokeras för lagrets
*                 noder och samtliga parametrar tilldelas startvärden.
**************************************************************************************************/
void denselayer_new(struct denselayer* self, const size_t num_nodes, const size_t num_weights)
{
   doublevector_new(&self->output);
   doublevector_new(&self->bias);
   doublevector_new(&self->error);
   double2Dvector_new(&self->weights);
   self->num_nodes = num_nodes;
   self->num_weights = num_weights;
   denselayer_init(self);
   return;
}

/**************************************************************************************************
* denselayer_delete: Frigör minne allokerat för givet dense-lager.
**************************************************************************************************/
void denselayer_delete(struct denselayer* self)
{
   doublevector_delete(&self->output);
   doublevector_delete(&self->bias);
   doublevector_delete(&self->error);
   double2Dvector_delete(&self->weights);
   self->num_nodes = 0;
   self->num_weights = 0;
   return;
}

/**************************************************************************************************
* denselayer_ptr_new: Returnerar heapallokerat objekt för ett nytt dense-lager.
**************************************************************************************************/
struct denselayer* denselayer_ptr_new(const size_t num_nodes, const size_t num_weights)
{
   struct denselayer* self = (struct denselayer*)malloc(sizeof(struct denselayer));
   if (!self) return 0;
   denselayer_new(self, num_nodes, num_weights);
   return self;
}

/**************************************************************************************************
* denselayer_ptr_delete: Frigör minne allokerat på heapen för givet dense-lager. 
**************************************************************************************************/
void denselayer_ptr_delete(struct denselayer** self)
{
   denselayer_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* denselayer_clear: Frigör minne allokerat för parametrar i givet dense-lager.
**************************************************************************************************/
void denselayer_clear(struct denselayer* self)
{
   doublevector_delete(&self->output);
   doublevector_delete(&self->bias);
   doublevector_delete(&self->error);
   double2Dvector_delete(&self->weights);
   return;
}

/**************************************************************************************************
* denselayer_reset: Återställer dense-lager till start.
**************************************************************************************************/
void denselayer_reset(struct denselayer* self)
{
   denselayer_clear(self);
   denselayer_init(self);
   return;
}
/**************************************************************************************************
* denselayer_resize: Ändrar antalet noder och/eller vikter i givet dense-lager.
**************************************************************************************************/
void denselayer_resize(struct denselayer* self, const size_t num_nodes, const size_t num_weights)
{
   if (num_nodes != self->num_nodes)
   {
      denselayer_set_nodes(self, num_nodes);
   }
   if (num_weights != self->num_weights)
   {
      denselayer_set_weights(self, num_weights);
   }
   return;
}

/**************************************************************************************************
* denselayer_feedforward: Beräknar nya utsignaler för givet dense-lager via nya insignaler.
**************************************************************************************************/
void denselayer_feedforward(struct denselayer* self, const struct doublevector* input)
{
   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      double sum = self->bias.data[i];
      struct doublevector* weights = &self->weights.data[i];

      for (size_t j = 0; j < self->num_weights && j < input->size; ++j)
      {
         sum += input->data[j] * weights->data[j];
      }
      
      self->output.data[i] = relu(sum);
   }
   return;
}

/**************************************************************************************************
* denselayer_compare_with_reference: Beräknar fel i givet yttre lager via jämförelse med
*                                    referensvärden från träningsdatan. OBS! Denna funktion är
*                                    avsedd för yttre lager och bör ej användas för dolda lager.
**************************************************************************************************/
void denselayer_compare_with_reference(struct denselayer* self, const struct doublevector* reference)
{
   for (size_t i = 0; i < self->num_nodes && i < reference->size; ++i)
   {
      const double error = reference->data[i] - self->output.data[i];
      self->error.data[i] = error * delta_relu(self->output.data[i]);
   }
   return;
}

/**************************************************************************************************
* denselayer_backpropagate: Beräknar aktuellt fel i givet dolt lager. OBS! Denna funktion är
*                           avsedd för dolda lager och bör inte användas för yttre lager.
**************************************************************************************************/
void denselayer_backpropagate(struct denselayer* self, const struct denselayer* next_layer)
{
   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      double deviation = 0;

      for (size_t j = 0; j < next_layer->num_nodes; ++j)
      {
         const struct doublevector* weights = &next_layer->weights.data[j];
         deviation += next_layer->error.data[j] * weights->data[i];
      }

      self->error.data[i] = deviation * delta_relu(self->output.data[i]);
   }
   return;
}

/**************************************************************************************************
* denselayer_optimize: Justerar bias samt vikter med angiven lärhastighet för att minska fel.
**************************************************************************************************/
void denselayer_optimize(struct denselayer* self, const struct doublevector* input,
   const double learning_rate)
{
   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      const double change_rate = self->error.data[i] * learning_rate;
      struct doublevector* weights = &self->weights.data[i];
      self->bias.data[i] += change_rate;

      for (size_t j = 0; j < self->num_weights && j < input->size; ++j)
      {
         weights->data[j] += change_rate * input->data[j];
      }
   }

   return;
}

/**************************************************************************************************
* denselayer_print: Skriver ut information gällande givet dense-lager.
**************************************************************************************************/
void denselayer_print(const struct denselayer* self, FILE* stream)
{
   if (!self->num_nodes) return;
   if (!stream) stream = stdout;

   fprintf(stream, "Number of nodes: %zu\n", self->num_nodes);
   fprintf(stream, "Weights per node: %zu\n", self->num_weights);
   fprintf(stream, "----------------------------------------------------------------------------\n");

   fprintf(stream, "Outputs: ");
   print_line(&self->output, stream);

   fprintf(stream, "Bias: ");
   print_line(&self->bias, stream);

   fprintf(stream, "Error: ");
   print_line(&self->error, stream);

   fprintf(stream, "\nWeights:\n");

   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      fprintf(stream, "\tNode %zu: ", i + 1);
      struct doublevector* weights = &self->weights.data[i];
      print_line(weights, stream);
   }

   fprintf(stream, "----------------------------------------------------------------------------\n\n");
   return;
}

/**************************************************************************************************
* denselayer_init: Allokerar minne och sätter startvärden på parametrar i ett dense-lager.
**************************************************************************************************/
static void denselayer_init(struct denselayer* self)
{
   doublevector_resize(&self->output, self->num_nodes);
   doublevector_resize(&self->bias, self->num_nodes);
   doublevector_resize(&self->error, self->num_nodes);
   double2Dvector_resize(&self->weights, self->num_nodes);

   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      struct doublevector weights = { .data = 0, .size = 0 };
      doublevector_resize(&weights, self->num_weights);

      for (size_t j = 0; j < self->num_weights; ++j)
      {
         weights.data[j] = get_random_start_val();
      }

      self->output.data[i] = 0;
      self->bias.data[i] = get_random_start_val();
      self->error.data[i] = 0;
      self->weights.data[i] = weights;
   }

   return;
}

/**************************************************************************************************
* denselayer_set_nodes: Justerar antalet noder i givet dense-lager. Ifall nya noder lades till 
*                       så tilldelas startvärden till samtliga parametrar.
**************************************************************************************************/
static void denselayer_set_nodes(struct denselayer* self, const size_t num_nodes)
{
   doublevector_resize(&self->output, num_nodes);
   doublevector_resize(&self->bias, num_nodes);
   doublevector_resize(&self->error, num_nodes);
   double2Dvector_resize(&self->weights, num_nodes);

   if (num_nodes > self->num_nodes)
   {
      for (size_t i = self->num_nodes; i < num_nodes; ++i)
      {
         struct doublevector weights = { .data = 0, .size = 0 };
         doublevector_resize(&weights, self->num_weights);

         for (size_t j = 0; j < self->num_weights; ++j)
         {
            weights.data[j] = get_random_start_val();
         }

         self->output.data[i] = 0;
         self->bias.data[i] = get_random_start_val(); 
         self->error.data[i] = 0;
         self->weights.data[i] = weights;
      }
   }

   self->num_nodes = num_nodes;
   return;
}

/**************************************************************************************************
* denselayer_set_weights: Justerar antalet vikter i givet dense-lager. Ifall nya vikter har
*                         lagts till så tilldelas randomiserade startvärden till dessa.
**************************************************************************************************/
static void denselayer_set_weights(struct denselayer* self, const size_t num_weights)
{
   for (size_t i = 0; i < self->num_nodes; ++i)
   {
      doublevector_resize(&self->weights.data[i], num_weights);
   }

   if (num_weights > self->num_weights)
   {
      for (size_t i = 0; i < self->num_nodes; ++i)
      {
         struct doublevector* weights = &self->weights.data[i];
         for (size_t j = self->num_weights; j < num_weights; ++j)
         {
            weights->data[j] = get_random_start_val();
         }
      }
   }

   self->num_weights = num_weights;
   return;
}

/**************************************************************************************************
* get_random_start_val: Returnerar randomiserat flyttal mellan 0 - 1.
**************************************************************************************************/
static inline double get_random_start_val(void)
{
   return rand() / (double)RAND_MAX;
}

/**************************************************************************************************
* relu: Returnerar ReLU (Rectified Linear Unit) ur given summa:
*       sum > 0  => ReLU(sum) = sum
*       sum <= 0 => ReLU(sum) = 0
**************************************************************************************************/
static inline double relu(const double sum)
{
   if (sum > 0)
   {
      return sum;
   }
   else
   {
      return 0;
   }
}

/**************************************************************************************************
* delta_relu: Returnerar derivatan av aktuell utsignal:
*       output > 0  => ReLU(output) = sum => ReLU'(output) = 1 
*       output <= 0 => ReLU(output) = 0   => ReLU'(output) = 0 
**************************************************************************************************/
static inline double delta_relu(const double output)
{
   if (output > 0)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/**************************************************************************************************
* print_line: Skriver ut flyttal lagrat i ett fält på en enda rad via given utström.
**************************************************************************************************/
static void print_line(const struct doublevector* self, FILE* stream)
{
   for (const double* i = self->data; i < self->data + self->size; ++i)
   {
      fprintf(stream, "%g ", *i);
   }
   fprintf(stream, "\n");
   return;
}
