
#define STARTING_EMA 8
#define STARTING_AMA_SLOW 15
#define STARTING_AMA_FAST 8
#define STARTING_AMA_N 8
#define SMOOTHING_WINDOW_MIN 2
#define SMOOTHING_WINDOW_MAX 120
#define SMOOTHING_MIN 2
#define SMOOTHING_MAX 120

#include "Parameters.h"

Value EMA_predict(MParameter inmparam, CParameter incparam, std::vector<Parameter> inalgorithm_params);
std::vector<Parameter> getEMA_defaults();

Value AMA_predict(MParameter inmparam, CParameter incparam, std::vector<Parameter> inalgorithm_params);
std::vector<Parameter> getAMA_defaults();
