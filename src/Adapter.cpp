
#include "Adapter.h"


Adapter::Adapter() {
}



void Adapter::Setup(std::vector<std::vector<MParameter>> inmodel_params,std::vector<std::vector<CParameter>> incontrol_params, std::vector<GCParameter> inglobal_control_params,std::vector<Relation> incorrelations, std::string inalgorithm = "EMA"){

	model_params = inmodel_params;
	control_params = incontrol_params;
	global_control_params = inglobal_control_params;
	param_correlations = incorrelations;
	algorithm = inalgorithm;

	agents = std::vector<Profile>(inmodel_params.size());
	running = false;

	//EMA
	std::vector<Parameter> EMA_params=getEMA_defaults();
	for(int i=0;i<EMA_params.size(); i++)
		algorithm_params.push_back(EMA_params[i]);
	//AMA
	std::vector<Parameter> AMA_params=getAMA_defaults();
	for(int i=0;i<AMA_params.size(); i++)
		algorithm_params.push_back(AMA_params[i]);
		
	

}
void Adapter::update(){
	if(running){
		if(flag==1)
			adapt();
	}
}

void Adapter::adapt(){
	a1_predict();
	a2_clamp();
	a3_distribute();
	//a4_balanceGC();
	//a5_balanceC();


	flag=2;
}

void Adapter::a1_predict(){


	for(int agent=0; agent<getAgentsNum(); agent++){
		for(int i=0; i<param_correlations.size(); i++){
			std::string mparamname = param_correlations[i].parameter1_name;
			std::string cparamname = param_correlations[i].parameter2_name;

			int id_mparam = locate_correlation_parameter(mparamname, cparamname, param_correlations[i], model_params[agent], control_params[agent], 1);
			int id_cparam = locate_correlation_parameter(mparamname, cparamname, param_correlations[i], model_params[agent], control_params[agent], 2);
				
			Value newMP_AVG;
			Value newCP;

			if(algorithm=="EMA"){	
						newMP_AVG = EMA_predict(
							model_params[agent][id_mparam],
							control_params[agent][id_cparam], 
							algorithm_params
							);
						model_params[agent][id_mparam].average = newMP_AVG;
						
						Value n = algorithm_params[locate_parameter("EMA_Smoothing",algorithm_params)].current_value;
						newCP = pow((model_params[agent][id_mparam].average/model_params[agent][id_mparam].target_value),n/control_params[agent][id_cparam].clamp_limits.max)*control_params[agent][id_cparam].current_value;
						
						
						//if(n >= model_params[agent][id_mparam].values->size() )
						//	newCP = control_params[agent][id_cparam].current_value;

			}else if(algorithm=="AMA"){
						newMP_AVG = AMA_predict(
							model_params[agent][id_mparam],
							control_params[agent][id_cparam], 
							algorithm_params
							);
						model_params[agent][id_mparam].average = newMP_AVG;
						Value n0 = algorithm_params[locate_parameter("AMA_Period",algorithm_params)].current_value;
						newCP = pow((model_params[agent][id_mparam].average/model_params[agent][id_mparam].target_value),n0/control_params[agent][id_cparam].clamp_limits.max)*control_params[agent][id_cparam].current_value;
						
						
						//start case (not to drop)
						
						//if(n0 >= model_params[agent][id_mparam].values->size() )
						//	newCP = control_params[agent][id_cparam].current_value;

			}else{
						newMP_AVG = EMA_predict(
							model_params[agent][id_mparam],
							control_params[agent][id_cparam], 
							algorithm_params
							);
						model_params[agent][id_mparam].average = newMP_AVG;
						
						Value n = algorithm_params[locate_parameter("EMA_Smoothing",algorithm_params)].current_value;
						newCP = pow((model_params[agent][id_mparam].average/model_params[agent][id_mparam].target_value),n/control_params[agent][id_cparam].clamp_limits.max)*control_params[agent][id_cparam].current_value;
						
			}
			
			
			
			update_control_parameter(cparamname,agent,newCP);
			//std::cout<<"[Adapter]a1_predict: "<< " predicted new "<<control_params[agent][id_cparam].name<< "="<<newvalue<<std::endl;
		}
	}

}

void Adapter::a2_clamp(){

	for(int agent=0; agent<getAgentsNum(); agent++){
		for(int cparam=0; cparam<control_params[agent].size(); cparam++){
			Value lastvalue = control_params[agent][cparam].current_value;
			Value clampmin = control_params[agent][cparam].clamp_limits.min;
			Value clampmax = control_params[agent][cparam].clamp_limits.max;
			Value clampedvalue;

			if( lastvalue >= clampmax){
				clampedvalue = clampmax;
			}else if(lastvalue <= clampmin){
				clampedvalue = clampmin;
			}else{
				//do nothing
				clampedvalue = lastvalue;
			}
			set_control_parameter(control_params[agent][cparam].name,agent,clampedvalue);
			//update_control_parameter(control_params[agent][cparam].name,agent,clampedvalue);
		}
	}

}

void Adapter::a3_distribute(){

	for(int gcparam=0; gcparam<global_control_params.size(); gcparam++){


		int id_cparam = locate_cparameter(global_control_params[gcparam].cname, control_params[0]);
		Value distributed_gc;


		Value sum = 0;
		Value max = 0;
		switch (global_control_params[gcparam].distr_type){
				default:
				case 1: //average
					for (int agent=0; agent<getAgentsNum(); agent++){
						sum += control_params[agent][id_cparam].current_value;
					}
					distributed_gc = sum/getAgentsNum();
					break;
				case 2: //max
					for (int agent=0; agent<getAgentsNum(); agent++){
						if(control_params[agent][id_cparam].current_value >= max) 
							max=control_params[agent][id_cparam].current_value;
					}
					distributed_gc = max;
					break;
			} 
		update_global_control_parameter(global_control_params[gcparam].name,distributed_gc);
	}
}

void Adapter::a4_balanceGC(){


	std::vector<int> cparam_index;
	std::vector<int> gcparam_index;

	for(int i=0; i<global_control_params.size(); i++){
		if(global_control_params[i].balance == true){
				cparam_index.push_back(locate_cparameter(global_control_params[i].cname,control_params[0]));
				gcparam_index.push_back(i);
		}
	}

	for(int i=0; i<gcparam_index.size(); i++){
		for(int a=0; a<getAgentsNum(); a++){
			for(int c1=0; c1<cparam_index.size(); c1++){
				for(int c2=0; c2<cparam_index.size(); c2++){
					for(int s=0; s<global_control_params[i].bsets.size(); s++){
						if(control_params[a][c1].current_value <= control_params[a][c2].current_value*global_control_params[i].bsets[s]){
							set_global_control_parameter(i,global_control_params[i].current_value*global_control_params[i].bmodulations[s]);
							control_params[a][c1].balance = 2;
						}
					}
				}
			}
		}
	}
	
}

void Adapter::a5_balanceC(){

	for(int a=0; a<getAgentsNum(); a++){
		for(int c=0; c<control_params[a].size(); c++){
			if(control_params[a][c].balance == 1){
				set_control_parameter(a,c,control_params[a][c].current_value-control_params[a][c].balance_step);
			}else if(control_params[a][c].balance == 2){
				set_control_parameter(a,c,control_params[a][c].current_value+control_params[a][c].balance_step);
			}else{
				//do not step
			}
		}
	}

}


void Adapter::set_algorithm_parameter(std::string inname, std::string inalgorithm, Value invalue){
	int id_aparam = locate_parameter(inname, algorithm_params);
	algorithm_params[id_aparam].current_value = invalue;
}

void Adapter::set_algorithm_parameter(int paramit, Value invalue){
	algorithm_params[paramit].current_value = invalue;
}

Value Adapter::get_algorithm_parameter(std::string inname, std::string inalgorithm){

	int id_aparam = locate_parameter(inname, algorithm_params);
	return algorithm_params[id_aparam].current_value;

}



void Adapter::setAlgorithmParameters(std::vector<Parameter> inalgorithm_params){
	algorithm_params = inalgorithm_params;
}
void Adapter::setAlgorithm(std::string inalgorithm){
	algorithm = inalgorithm;
}

std::vector<Parameter> Adapter::getAlgorithmParameters(){
	return algorithm_params;
}
std::string Adapter::getAlgorithm(){
	return algorithm;
}





