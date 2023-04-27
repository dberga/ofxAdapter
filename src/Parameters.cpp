
#include "Parameters.h"


int locate_parameter(std::string inname, std::vector<Parameter> inparamvector){
	for(int i=0; i<inparamvector.size(); i++){
		if(inparamvector[i].name.compare(inname) == 0){
			return i;
		}
	}
	return 0; //not found

}

int locate_mparameter(std::string inname, std::vector<MParameter> inmparamvector){
	for(int i=0; i<inmparamvector.size(); i++){
		if(inmparamvector[i].name.compare(inname) == 0){
			return i;
		}
	}
	return 0; //not found

}

int locate_cparameter(std::string inname, std::vector<CParameter> incparamvector){
	for(int i=0; i<incparamvector.size(); i++){
		if(incparamvector[i].name.compare(inname) == 0){
			return i;
		}
	}
	return 0; //not found

}

int locate_gcparameter(std::string inname, std::vector<GCParameter> ingcparamvector){
	for(int i=0; i<ingcparamvector.size(); i++){
		if(ingcparamvector[i].name.compare(inname) == 0){
			return i;
		}
	}
	return 0; //not found

}

int locate_correlation_parameter(std::string inname1, std::string inname2, Relation incorrelation, std::vector<MParameter> inparams1, std::vector<CParameter> inparams2, int inchoose){
	
	int found1 = 99;
	int found2 = 99;

	if(incorrelation.parameter1_name.compare(inname1) ==0 && incorrelation.parameter2_name.compare(inname2) == 0){
		found1= locate_mparameter(inname1,inparams1);
		found2= locate_cparameter(inname2,inparams2);
	}
	
	
	if(found1 == 99 && found2 == 99){
		std::cout << "ERROR[locate_correlation_parameter] couldnt find the parameter id for " << inname1 << " and "<< inname2 << std::endl;
		//bad
	}
	

	if(inchoose==1){ //parameter 1
		return found1;
	}else if(inchoose==2){ //parameter 2
		return found2;
	}else{
		std::cout<<"ERROR[locate_correlation_parameter]: please, choose to return 1 or 2"<<std::endl;
		//bad
	}

}



