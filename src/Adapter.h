

#ifndef __Adapter__
#define __Adapter__

#include "Parameters.h"
#include "utils.h"
#include "Controller.h"
#include "Algorithms.h"

class Adapter : public Controller{

public:
    Adapter(); //constructor

	void Setup(
		std::vector<std::vector<MParameter>> inmodel_params,
		std::vector<std::vector<CParameter>> incontrol_params, 
		std::vector<GCParameter> inglobal_control_params,
		std::vector<Relation> incorrelations,
		std::string inalgorithm
		);
	void update();

	//adaptive system 
	void adapt();
	void a1_predict();
	void a2_clamp();
	void a3_distribute();
	void a4_balanceGC();
	void a5_balanceC();
	
	//core parameter getters and setters
	std::vector<Parameter> getAlgorithmParameters();
	std::string getAlgorithm();
	void setAlgorithmParameters(std::vector<Parameter> inalgorithm_params);
	void setAlgorithm(std::string inalgorithm);

	//individual parameter getters and setters
	Value get_algorithm_parameter(std::string inname, std::string inalgorithm);
	void set_algorithm_parameter(std::string inname, std::string inalgorithm, Value invalue);
		void set_algorithm_parameter(int paramit, Value invalue);

	//add parameters
	void add_param_correlation(Relation incorrelation);

	
protected:
	std::vector<Relation> param_correlations; //relations[relation].parameter1_name or  relations[relation].parameter2_name (between control parameters and model parameters)
	std::vector<Parameter> algorithm_params; //params[agent][parameter].values[instance or time]
	std::string algorithm;
};



#endif
