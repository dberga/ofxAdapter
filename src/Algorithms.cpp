
#include "Algorithms.h"


Value EMA_predict(MParameter inmparam, CParameter incparam, std::vector<Parameter> inalgorithm_params){
	
	
	Value n = inalgorithm_params[locate_parameter("EMA_Smoothing",inalgorithm_params)].current_value;
	Value alpha = 2/(n+1);

	if(inmparam.values->size() >1 ){

		return inmparam.average + alpha *( inmparam.values->back()- inmparam.average);
	
	}else{

		return inmparam.current_value;
	}


}

std::vector<Parameter> getEMA_defaults(){
	std::vector<Parameter> EMA_defaults = std::vector<Parameter>();

		int n = (int)STARTING_EMA;
		Parameter smoothing;
		smoothing.name = "EMA_Smoothing";
			Value alpha = 2/(n+1);
		smoothing.current_value = alpha;
		smoothing.limits.min = SMOOTHING_MIN;
		smoothing.limits.max = SMOOTHING_MAX;

	EMA_defaults.push_back(smoothing);

	return EMA_defaults;
}

Value AMA_predict(MParameter inmparam, CParameter incparam, std::vector<Parameter> inalgorithm_params){

	Value n0 = inalgorithm_params[locate_parameter("AMA_Period",inalgorithm_params)].current_value;
	Value n1 = inalgorithm_params[locate_parameter("AMA_Smoothing_Fast",inalgorithm_params)].current_value;
	Value n2 = inalgorithm_params[locate_parameter("AMA_Smoothing_Slow",inalgorithm_params)].current_value;
	Value alpha_fast  = 2/(n1+1);
	Value alpha_slow  = 2/(n2+1);

	
	if(inmparam.values->size() >1){


				Value alpha; //SSC
				Value ER;
				Value direction; //signal
				Value volatibility = 0; //noise

				if(n0 >= inmparam.values->size()-1){
					alpha = 2/(n0+1);

					if(inmparam.values->size() >1 )
						return inmparam.average + alpha *( inmparam.values->back()- inmparam.average);
					else
						return inmparam.current_value;
					
				}else{
				
					direction = fabs(inmparam.values->back() - inmparam.values->at((inmparam.values->size()-1)-n0));
					for(int j=inmparam.values->size()-n0; j<inmparam.values->size(); j++)
						volatibility += fabs(inmparam.values->at(j) - inmparam.values->at(j-1));

					ER = direction/volatibility;

					if(volatibility <= 0 && direction == 0)
						ER=0;
					else if(volatibility <= 0 && direction > 0)
						ER=1;

					if(ER > 1) ER = 1;
					else if(ER < 0) ER = 0;

					alpha = ER * (alpha_fast-alpha_slow)+alpha_slow;
					return (inmparam.average + alpha*alpha*(inmparam.current_value-inmparam.average));
				
				}
						
				
			

		
	
	}else{
		return inmparam.current_value;
	}

}


std::vector<Parameter> getAMA_defaults(){
	std::vector<Parameter> AMA_defaults = std::vector<Parameter>();

		int n0=(int)STARTING_AMA_N;
		int n1=(int)STARTING_AMA_FAST;
		int n2=(int)STARTING_AMA_SLOW;
		
		Parameter smoothing_period;
		smoothing_period.name = "AMA_Period";
		smoothing_period.current_value = n0;
		smoothing_period.limits.min = SMOOTHING_WINDOW_MIN;
		smoothing_period.limits.max = SMOOTHING_WINDOW_MAX;

		AMA_defaults.push_back(smoothing_period);

		Parameter smoothing_fast;
		smoothing_fast.name = "AMA_Smoothing_Fast";
			//Value alpha_fast = 2/(n1+1);
		smoothing_fast.current_value = n1;
		smoothing_fast.limits.min = SMOOTHING_MIN;
		smoothing_fast.limits.max = SMOOTHING_MAX-2;

		AMA_defaults.push_back(smoothing_fast);

		Parameter smoothing_slow;
		smoothing_slow.name = "AMA_Smoothing_Slow";
			//Value alpha_slow = 2/(n2+1);
		smoothing_slow.current_value = n2;
		smoothing_slow.limits.min = SMOOTHING_MIN;
		smoothing_slow.limits.max = SMOOTHING_MAX;

		AMA_defaults.push_back(smoothing_slow);

	return AMA_defaults;
}
