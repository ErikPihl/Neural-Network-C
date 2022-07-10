#ifndef ANN_H_
#define ANN_H_

/**************************************************************************************************
* ann.h: Innehåller funktionalitet för implementering av neurala nätverk innehållande multipla
*        dense-lager via strukten ann samt motsvarande externa funktioner.
**************************************************************************************************/

/* Inkluderingsdirektiv: */
#include "def.h"
#include "vector.h"
#include "layer.h"
#include "trainingdata.h"

/**************************************************************************************************
* ann: Implementering av ett neuralt nätverk innehållande ett ingångslager, valfritt antal
*      dolda lager samt ett yttre lager. Antalet noder i respektive lager är valbart.
**************************************************************************************************/
struct ann
{ 
   struct denselayer output_layer; /* Yttre lager. */
   struct denselayer_vector hidden_layers; /* Fält innehållande dolda lager. */
   struct trainingdata trainingdata; /* Behålladre för träningsdata. */
   const struct doublevector* input_layer; /* Pekare till insignaler i ingångslagret. */
   size_t num_inputs; /* Antalet insignaler. */
   size_t num_outputs; /* Antalet utsignaler. */
   size_t num_epochs; /* Antalet epoker / omgångar som träning genomförs. */
   double learning_rate; /* Lärhastighet, avgör hur mycket parametrarna förändras vid fel. */
};

/* Externa funktioner: */
void ann_new(struct ann* self, const size_t num_inputs, const size_t num_hidden, 
   const size_t num_outputs, const size_t num_epochs, const double learning_rate);
void ann_delete(struct ann* self);
struct ann* ann_ptr_new(const size_t num_inputs, const size_t num_hidden,
   const size_t num_outputs, const size_t num_epochs, const double learning_rate);
void ann_ptr_delete(struct ann** self);
int ann_add_hidden_layer(struct ann* self, const size_t num_nodes);
int ann_add_hidden_layers(struct ann* self, const size_t num_layers, const size_t num_nodes);
void ann_load_training_data(struct ann* self, const char* filepath);
void ann_set_training_data(struct ann* self, const struct double2Dvector* x, 
   const struct double2Dvector* y);
void ann_train(struct ann* self);
double* ann_predict(struct ann* self, const struct doublevector* input);
void ann_predict_range(struct ann* self, const struct double2Dvector* inputs, FILE* stream);

#endif /* ANN_H_ */