/**************************************************************************************************
* ann.c: Inneh?ller funktionsdefinitioner som anv?nds f?r implementering av neurala n?tverk.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "ann.h"

/* Statiska funktioner: */
static void ann_feedforward(struct ann* self, const struct doublevector* input);
static void ann_backpropagate(struct ann* self, const struct doublevector* reference);
static void ann_optimize(struct ann* self);
static void print_line(const struct doublevector* self, FILE* stream, const double threshold);

/**************************************************************************************************
* ann_new: Initierar objekt f?r implementering av  neuralt n?tverk och allokerar minne f?r ett 
*          dolt samt ett yttre lager. Vid behov kan fler dolda lager l?ggas till senare.
**************************************************************************************************/
void ann_new(struct ann* self, const size_t num_inputs, const size_t num_hidden,
   const size_t num_outputs, const size_t num_epochs, const double learning_rate)
{
   self->num_inputs = num_inputs;
   self->num_outputs = num_outputs;
   self->num_epochs = num_epochs;
   self->learning_rate = learning_rate;
   self->input_layer = 0;

   denselayer_new(&self->output_layer, self->num_outputs, num_hidden);
   trainingdata_new(&self->trainingdata, self->num_inputs, self->num_outputs);
   denselayer_vector_new(&self->hidden_layers);
   denselayer_vector_add_layer(&self->hidden_layers, num_hidden, num_inputs);
   return;
}

/**************************************************************************************************
* ann_delete: Frig?r minne allokerat f?r neuralt n?tverk.
**************************************************************************************************/
void ann_delete(struct ann* self)
{
   denselayer_delete(&self->output_layer);
   denselayer_vector_delete(&self->hidden_layers);
   trainingdata_delete(&self->trainingdata);
   self->input_layer = 0;
   self->num_inputs = 0;
   self->num_outputs = 0;
   self->num_epochs = 0;
   self->learning_rate = 0;
   return;
}

/**************************************************************************************************
* ann_ptr_new: Returnerar pekare till ett nytt heapallokerat objekt f?r implementering av ett 
*              neuralt n?tverk, som vid start best?r av ett ing?ngslager, ett dolt lager samt 
*              ett utg?ngslager.
**************************************************************************************************/
struct ann* ann_ptr_new(const size_t num_inputs, const size_t num_hidden,
   const size_t num_outputs, const size_t num_epochs, const double learning_rate)
{
   struct ann* self = (struct ann*)malloc(sizeof(struct ann));
   if (!self) return 0;
   ann_new(self, num_inputs, num_outputs, num_hidden, num_epochs, learning_rate);
   return self;
}

/**************************************************************************************************
* ann_ptr_delete: Frig?r minne f?r heapallokerat objekt f?r implementering av ett neuralt n?tverk.
**************************************************************************************************/
void ann_ptr_delete(struct ann** self)
{
   ann_delete(*self);
   free(*self);
   *self = 0;
   return;
}

/**************************************************************************************************
* ann_add_hidden_layer: L?gger till ett dolt lager bakom det yttre lagret i ett neuralt n?tverk
*                       och justuerar antalet vikter per nod i det yttre lagret efter detta.
**************************************************************************************************/
int ann_add_hidden_layer(struct ann* self, const size_t num_nodes)
{
   const size_t num_weights = denselayer_vector_last(&self->hidden_layers)->num_nodes;
   if (denselayer_vector_add_layer(&self->hidden_layers, num_nodes, num_weights))
   {
      return 1;
   }
   else
   {
      denselayer_resize(&self->output_layer, self->num_outputs, num_nodes);
      return 0;
   } 
}

/**************************************************************************************************
* ann_add_hidden_layers: L?gger till ett angivet antal dola lager bakom det yttre lagret i ett 
*                        neuralt n?tverk och justerar antalet vikter per nod i det yttre lagret.
**************************************************************************************************/
int ann_add_hidden_layers(struct ann* self, const size_t num_layers, const size_t num_nodes)
{
   const size_t num_weights = denselayer_vector_last(&self->hidden_layers)->num_nodes;
   if (denselayer_vector_add_layers(&self->hidden_layers, num_layers, num_nodes, num_weights))
   {
      return 1;
   }
   else
   {
      denselayer_resize(&self->output_layer, self->num_outputs, num_nodes);
      return 0;
   }
}

/**************************************************************************************************
* ann_load_training_data: L?ser in tr?ningsdata fr?n en fil.
**************************************************************************************************/
void ann_load_training_data(struct ann* self, const char* filepath)
{
   trainingdata_load(&self->trainingdata, filepath);
   return;
}

/**************************************************************************************************
* ann_set_training_data: L?gger till tr?ningsdata fr?n tv?dimensionella f?lt.
**************************************************************************************************/
void ann_set_training_data(struct ann* self, const struct double2Dvector* x,
   const struct double2Dvector* y)
{
   trainingdata_set(&self->trainingdata, x, y);
   return;
}

/**************************************************************************************************
* ann_train: Tr?nar neuralt n?tverk angivet antal epoker. Inf?r varje epok randomiseras ordningen
*            p? tr?ningsupps?ttningarna. D?refter genomf?rs en feedforward f?r att uppdatera
*            utsignalerna i varje lager. D?refter genomf?rs en backprop f?r att ber?kna aktuella
*            fel i hela n?tverket. Slutligen sker optimering i syfte att minska uppkommet fel.
*            D?rmed justeras bias samt vikter i n?tverket f?r att minimera felet.
**************************************************************************************************/
void ann_train(struct ann* self)
{
   for (size_t i = 0; i < self->num_epochs; ++i)
   {
      trainingdata_shuffle(&self->trainingdata);
      for (size_t j = 0; j < self->trainingdata.sets; ++j)
      {
         const size_t k = self->trainingdata.order.data[j];
         const struct doublevector* input = &self->trainingdata.x.data[k];
         const struct doublevector* reference = &self->trainingdata.y.data[k];

         ann_feedforward(self, input);
         ann_backpropagate(self, reference);
         ann_optimize(self);
      }
   }
   return;
}

/**************************************************************************************************
* ann_predict: Genomf?r prediktion utifr?n givna insignaler och returnerar adressen till ett f?lt 
*              inneh?llande predikterade utsignaler.
**************************************************************************************************/
double* ann_predict(struct ann* self, const struct doublevector* input)
{
   ann_feedforward(self, input);
   return self->output_layer.output.data;
}

/**************************************************************************************************
* ann_predict_range: Genomf?r prediktion f?r multipla kombinationer av insignaler och genomf?r
*                     utskrift av predikterade utsignaler via given utstr?m.
**************************************************************************************************/
void ann_predict_range(struct ann* self, const struct double2Dvector* inputs, FILE* stream)
{
   const double threshold = 1.0 / 10000;
   if (!stream) stream = stdout;
   fprintf(stream, "----------------------------------------------------------------------------\n");

   for (const struct doublevector* i = inputs->data; i < inputs->data + inputs->size; ++i)
   {
      ann_feedforward(self, i);
      fprintf(stream, "Input: ");
      print_line(i, stream, threshold);

      fprintf(stream, "Predicted output: ");
      print_line(&self->output_layer.output, stream, threshold);
   }

   fprintf(stream, "----------------------------------------------------------------------------\n\n");
   return;
}

/**************************************************************************************************
* ann_feedforward: Ber?knar nya utsignaler f?r samtliga noder i givet neuralt n?tverk.
**************************************************************************************************/
static void ann_feedforward(struct ann* self, const struct doublevector* input)
{
   const struct doublevector* hidden_output = &denselayer_vector_last(&self->hidden_layers)->output;
   if (input->size < self->num_inputs) return;
   self->input_layer = input;
   denselayer_vector_feedforward(&self->hidden_layers, self->input_layer);
   denselayer_feedforward(&self->output_layer, hidden_output);
   return;
}

/**************************************************************************************************
* ann_backpropagate: Ber?knar aktuella fel f?r samtliga noder i aktuellt neuralt n?tverk
*                    utefter referensv?rde fr?n given tr?ningsdata.
**************************************************************************************************/
static void ann_backpropagate(struct ann* self, const struct doublevector* reference)
{
   denselayer_compare_with_reference(&self->output_layer, reference);
   denselayer_vector_backpropagate(&self->hidden_layers, &self->output_layer);
   return;
}

/**************************************************************************************************
* ann_optimize: Minimerar fel i givet neuralt n?tverk genom att justera bias samt vikter.
**************************************************************************************************/
static void ann_optimize(struct ann* self)
{
   const struct doublevector* hidden_output = &denselayer_vector_last(&self->hidden_layers)->output;
   denselayer_optimize(&self->output_layer, hidden_output, self->learning_rate);
   denselayer_vector_optimize(&self->hidden_layers, self->input_layer, self->learning_rate);
   return;
}

/**************************************************************************************************
* print_line: Skriver ut flyttal lagrat i ett f?lt p? en enda rad via given utstr?m.
**************************************************************************************************/
static void print_line(const struct doublevector* self, FILE* stream, const double threshold)
{
   for (const double* i = self->data; i < self->data + self->size; ++i)
   {
      if (*i > -threshold && *i < threshold)
      {
         fprintf(stream, "0 ");
      }
      else
      {
         fprintf(stream, "%g ", *i);
      }
   }
   fprintf(stream, "\n");
   return;
}
