
#include "BaselineCalibrator.h"


BaselineCalibrator::BaselineCalibrator() {
}


void BaselineCalibrator::Setup(std::vector<std::vector<MParameter>> inmodel_params,std::vector<std::vector<CParameter>> incontrol_params, std::vector<GCParameter> inglobal_control_params, std::vector<std::vector<std::vector<Value>>> inmodel_base,std::vector<std::vector<std::vector<Value>>> incontrol_base, int intiming_type, int intiming){
	
	model_params = inmodel_params;
	control_params = incontrol_params;
	global_control_params = inglobal_control_params;
	
	model_base = inmodel_base;
	control_base = incontrol_base;

	int iSize = 0;
	flag=1;
	running = false;

	timing_type = intiming_type;
	basetime = std::clock();
	startingtime = std::clock();

	agents = std::vector<Profile>(inmodel_params.size());
	
	switch(timing_type){
		case 0:
			followbase = false;
			break;
		case 1:
			followbase = true;
			timing = intiming; //step time
			break;
		case 2:
			followbase = true;
			timing = (int)floor((intiming*60000)/incontrol_base[0][0].size());
			//baseline();
			sessiontime = intiming;
			basetime = std::clock();
		case 3:
			followbase = true;
			timing = (int)floor((intiming*60000)/incontrol_base[0][0].size());
			//baseline();
			sessiontime = intiming;
			basetime = std::clock();
			
			//dummy
			//for(int i=0; i<incontrol_base.size(); i++){
			//	for(int j=0; j<incontrol_base[i].size(); j++){
			//		if(!incontrol_base[i][j].empty())
			//			cout << "divise" << endl;
			//			timing /= incontrol_base[i][j].size();
			//			//iSize = incontrol_base[i][j].size();
			//	}
			//}
			break;
	}
	//std::cout << "calibrator timing = " << timing << " size " << iSize << " time " << intiming*60*1000 << std::endl;

	
}

void BaselineCalibrator::Setup(std::vector<std::vector<MParameter>> inmodel_params,std::vector<std::vector<CParameter>> incontrol_params, std::vector<GCParameter> inglobal_control_params){
	model_params = inmodel_params;
	control_params = incontrol_params;
	global_control_params = inglobal_control_params;
	
	//model_base = inmodel_base;
	//control_base = incontrol_base;

	int iSize = 0;
	flag=1;
	running = false;

	timing_type = 3;
	timing = 60000;
	basetime = std::clock();
	//baseline();

	agents = std::vector<Profile>(inmodel_params.size());
	
}
void BaselineCalibrator::setcontrolbase(std::vector<std::vector<std::vector<Value>>> cbase){
	control_base = cbase;
}

void BaselineCalibrator::setSession(int stime){ //needs to be retrieved the control base before
	
			followbase = true;
			sessiontime = stime;
			timing = (int)floor((stime*60000)/control_base[0][0].size());
			startingtime = std::clock();
			basetime = std::clock();

}

int BaselineCalibrator::getSession(){
	return sessiontime;
}

void BaselineCalibrator::update(){
	if(running){
		
		if(flag==1){
			for(int i=0;i<getAgentsNum(); i++){
				int jIT = locate_cparameter("IT",control_params[i]);
				update_control_parameter(control_params[i][jIT].name,i,control_params[i][jIT].current_value);			
				flag=4;
			}
		}
		

		if((int)std::clock() >= sessiontime*60000+startingtime){
			running=false;
			followbase = false;
			saved = true;
		}
		int duration = (int)std::clock()-(int)basetime;
		
			if(timing_type == 3)
				for(int i=0;i<getAgentsNum(); i++){
					int jIT = locate_cparameter("IT",control_params[i]);
					timing = control_params[i][jIT].current_value*1000;
					
				}
			
			
			if(duration>=timing && followbase == true){
				baseline();
				basetime = std::clock();
				
			}else{
				//do nothing
			}
		
	}
}

void BaselineCalibrator::baseline(){

	for(int i=0; i<getAgentsNum(); i++){
		if(model_base.size() > 0){
			for(int j=0; j<model_params[i].size(); j++){
				if(!model_base[i][j].empty()){
					set_model_parameter(i,j,model_base[i][j].front());
					model_base[i][j].erase(model_base[i][j].begin());
				}
			}
		}
		if(control_base.size() > 0){
			for(int j=0; j<control_params[i].size(); j++){
				if(!control_base[i][j].empty()){
					set_control_parameter(i,j,control_base[i][j].front());
					control_base[i][j].erase(control_base[i][j].begin());
				}else{
					
				}
			}
		}
		
	}
	flag = 2;
}

void BaselineCalibrator::calibrate(){
	//TODO: output final csv with their parameters as one-to-one, and also getting target model params

		//optional: erase base
		/*
		for(int i=0; i<getAgentsNum(); i++){
			for(int j=0; i<model_params[i].size(); i++){
				model_base[i][j].clear();
			}
			for(int j=0; i<control_params[i].size(); i++){
				control_base[i][j].clear();
			}
		}
		*/
}


void BaselineCalibrator::calibrate_mparam(int agent, int index, std::vector<Value> invalues){

	model_params[agent][index].values = &invalues;
	followbase=false;
}

void BaselineCalibrator::calibrate_cparam(int agent, int index, std::vector<Value> invalues){

	model_params[agent][index].values = &invalues;
	followbase=false;
}


std::vector<std::vector<std::vector<Value>>> generate_base(std::vector<std::vector<MParameter>> inparams, std::vector<Value> invalues){

	std::vector<std::vector<std::vector<Value>>> values;
		
	for(int i=0; i<inparams.size(); i++){
		values.push_back(std::vector<std::vector<Value>>());
		for(int j=0; j<inparams[i].size(); j++){
			values[i].push_back(std::vector<Value>());
		}
	}

	for(int i=0; i<values.size(); i++){
		for(int j=0; j<values[i].size(); j++){
			values[i][j] = invalues;
		}
	}
	

	return values;

}

std::vector<std::vector<std::vector<Value>>> generate_base(std::vector<std::vector<CParameter>> inparams, std::vector<Value> invalues){
	std::vector<std::vector<std::vector<Value>>> values;
		
	for(int i=0; i<inparams.size(); i++){
		values.push_back(std::vector<std::vector<Value>>());
		for(int j=0; j<inparams[i].size(); j++){
			values[i].push_back(std::vector<Value>());
		}
	}

	for(int i=0; i<values.size(); i++){
		for(int j=0; j<values[i].size(); j++){
			values[i][j] = invalues;
		}
	}
	

	return values;

}

//TODO: create generate_base pero ndicando nombre de parametro (lo de arriba considera para todos)


/*
std::vector<std::vector<std::vector<Value>>> generate_IT_base(std::vector<std::vector<CParameter>> inparams, std::vector<std::vector<std::string>> base){

	std::vector<std::vector<std::vector<Value>>> values;
		
	for(int i=0; i<inparams.size(); i++){
		values.push_back(std::vector<std::vector<Value>>());
		for(int j=0; j<inparams[i].size(); j++){
			values[i].push_back(std::vector<Value>());
		}
	}

	std::vector<std::vector<Value>> basefloat;
	for(int i=0; i<base.size(); i++){
		basefloat.push_back(std::vector<Value>());
		for(int j=0; j<base[i].size(); j++){
				basefloat[i][j] = (float)atoi(base[i][j].c_str());
			
		}
	}

	

	for(int i=0; i<values.size(); i++){
		int ITindex = locate_cparameter("IT",inparams.at(i));
		values[i][ITindex] = basefloat[i];
	}
	return values;
}

*/
