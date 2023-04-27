
#include "Controller.h"


Controller::Controller() {
}



void Controller::Setup(std::vector<std::vector<MParameter>> inmodel_params,std::vector<std::vector<CParameter>> incontrol_params, std::vector<GCParameter> inglobal_control_params){

	model_params = inmodel_params;
	control_params = incontrol_params;
	global_control_params = inglobal_control_params;
	flag=0;
	running = false;
	saved = false;
	agents = std::vector<Profile>();
		for(int i=0; i<inmodel_params.size();i++)
			agents.push_back(Profile()); //set in setAgents
	curr_time = time(0);
}
void Controller::update(){
	if(running)
		;
	else
		; //do nothing
}

void Controller::start(){
	running = true;
}

void Controller::stop(){
	running = false;
}


Value Controller::get_control_parameter(std::string inname, int inagent){
	int id_cparam = locate_cparameter(inname, control_params[inagent]);
	return control_params[inagent][id_cparam].current_value;

}

Value Controller::get_model_parameter(std::string inname, int inagent){

	int id_mparam = locate_mparameter(inname, model_params[inagent]);
	return model_params[inagent][id_mparam].current_value;

}

Value Controller::get_global_control_parameter(std::string inname){

	int id_gcparam = locate_gcparameter(inname, global_control_params);
	return global_control_params[id_gcparam].current_value;

}

CParameter* Controller::getref_control_parameter(int inagent, int inparam){
	return &this->control_params[inagent][inparam];
}
MParameter* Controller::getref_model_parameter(int inagent, int inparam){
	return &this->model_params[inagent][inparam];
}
GCParameter* Controller::getref_global_control_parameter(int inparam){
	return &this->global_control_params[inparam];
}


void Controller::set_global_control_parameter(std::string inname, Value invalue){

	int id_gcparam = locate_gcparameter(inname, global_control_params);
	
	global_control_params[id_gcparam].current_value = invalue;
	//global_control_params[id_gcparam].values[global_control_params[id_gcparam].values.size()-1] = invalue;

}
void Controller::set_global_control_parameter( int paramit,Value invalue){
	global_control_params[paramit].current_value = invalue;
	//global_control_params[paramit].values[global_control_params[paramit].values.size()-1] = invalue; //last element
}
void Controller::set_control_parameter(std::string inname, int inagent, Value invalue){

	int id_cparam = locate_cparameter(inname, control_params[inagent]);
	
	control_params[inagent][id_cparam].current_value = invalue;
	//control_params[inagent][id_cparam].values[control_params[inagent][id_cparam].values.size()-1] = invalue; //last element
}
void Controller::set_control_parameter(int agent, int paramit,Value invalue){
	control_params[agent][paramit].current_value = invalue;
	//control_params[agent][paramit].values[control_params[agent][paramit].values.size()-1] = invalue; //last element
}

void Controller::set_model_parameter(std::string inname, int inagent, Value invalue){

	int id_mparam = locate_mparameter(inname, model_params[inagent]);
	
	model_params[inagent][id_mparam].current_value = invalue;
	//model_params[inagent][id_mparam].values[model_params[inagent][id_mparam].values.size()-1] = invalue; //last element
}
void Controller::set_model_parameter(int agent, int paramit,Value invalue){
	model_params[agent][paramit].current_value = invalue;
	//model_params[agent][paramit].values[model_params[agent][paramit].values.size()-1] = invalue; //last element
}

void Controller::set_model_parameter_target(int agent, int paramit,Value invalue){
	model_params[agent][paramit].target_value = invalue;
}

void Controller::setModelParameters(std::vector<std::vector<MParameter>> inmodel_params){
	model_params = inmodel_params;
}
void Controller::setControlParameters(std::vector<std::vector<CParameter>> incontrol_params){
	control_params = incontrol_params;
}
void Controller::setGlobalControlParameters(std::vector<GCParameter> inglobal_control_params){
	global_control_params = inglobal_control_params;
}



std::vector<std::vector<MParameter>> Controller::getModelParameters(){
	return model_params;
}
std::vector<std::vector<CParameter>> Controller::getControlParameters(){
	return control_params;
}
std::vector<GCParameter> Controller::getGlobalControlParameters(){
	return global_control_params;
}

void Controller::setAgents(std::vector<Profile> inagents){
	agents = inagents;
}

void Controller::update_control_parameter(std::string inname, int inagent, Value invalue){
	if(running){
		int id_cparam = locate_cparameter(inname, control_params[inagent]);
	
		//if(control_params[inagent][id_cparam].values->size() > control_params[inagent][id_cparam].values->capacity()-1 )
		//	control_params[inagent][id_cparam].values->reserve(control_params[inagent][id_cparam].values->size()+1000);

		control_params[inagent][id_cparam].values->push_back(invalue);
		control_params[inagent][id_cparam].current_value = invalue;

		curr_time= time(0);
		std::tm now = *localtime(&curr_time);
		control_params[inagent][id_cparam].values_time->push_back(now);
		

	}
}

void Controller::update_control_parameter(int inagent, int paramit, Value invalue){
	if(running){

		//if(control_params[inagent][paramit].values->size() > control_params[inagent][paramit].values->capacity()-1 )
		//	control_params[inagent][paramit].values->reserve(control_params[inagent][paramit].values->size()+1000);

	control_params[inagent][paramit].values->push_back(invalue);
	control_params[inagent][paramit].current_value = invalue;

	curr_time = time(0);
	std::tm now = *localtime(&curr_time);
	control_params[inagent][paramit].values_time->push_back(now);
	

	}
}
void Controller::update_global_control_parameter(int paramit, Value invalue){
	if(running){
		//if(global_control_params[paramit].values->size() > global_control_params[paramit].values->capacity()-1 )
		//	global_control_params[paramit].values->reserve(global_control_params[paramit].values->size()+1000);

	global_control_params[paramit].values->push_back(invalue);
	global_control_params[paramit].current_value = invalue;

	curr_time = time(0);
	std::tm now = *localtime(&curr_time);
	global_control_params[paramit].values_time->push_back(now);

	}
}


void Controller::update_global_control_parameter(std::string inname, Value invalue){
	if(running){
	int id_gcparam = locate_gcparameter(inname, global_control_params);
	
	//if(global_control_params[id_gcparam].values->size() > global_control_params[id_gcparam].values->capacity()-1 )
	//		global_control_params[id_gcparam].values->reserve(global_control_params[id_gcparam].values->size()+1000);

	global_control_params[id_gcparam].values->push_back(invalue);
	global_control_params[id_gcparam].current_value = invalue;

	curr_time = time(0);
	std::tm now = *localtime(&curr_time);
	global_control_params[id_gcparam].values_time->push_back(now);

	
	}

}


void Controller::update_model_parameter(std::string inname, int inagent, Value invalue){
	if(running){
	int id_mparam = locate_mparameter(inname, model_params[inagent]);
	
	//if(model_params[inagent][id_mparam].values->size() > model_params[inagent][id_mparam].values->capacity()-1 )
	//	model_params[inagent][id_mparam].values->reserve(model_params[inagent][id_mparam].values->size()+1000);

	if(invalue > model_params[inagent][id_mparam].limits.max)
		invalue = model_params[inagent][id_mparam].limits.max;

	if(invalue < model_params[inagent][id_mparam].limits.min)
		invalue = model_params[inagent][id_mparam].limits.min;


	model_params[inagent][id_mparam].values->push_back(invalue);
	model_params[inagent][id_mparam].current_value = invalue;

	curr_time = time(0);
	std::tm now = *localtime(&curr_time);
	model_params[inagent][id_mparam].values_time->push_back(now);
	

	flag=1;
	}
}

void Controller::update_model_parameter(int inagent, int paramit,Value invalue){
	if(running){

	//if(model_params[inagent][paramit].values->size() > model_params[inagent][paramit].values->capacity()-1 )
	//	model_params[inagent][paramit].values->reserve(model_params[inagent][paramit].values->size()+1000);

	if(invalue > model_params[inagent][paramit].limits.max)
		invalue = model_params[inagent][paramit].limits.max;

	if(invalue < model_params[inagent][paramit].limits.min)
		invalue = model_params[inagent][paramit].limits.min;

	model_params[inagent][paramit].values->push_back(invalue);
	model_params[inagent][paramit].current_value = invalue;

	curr_time = time(0);
	std::tm now = *localtime(&curr_time);
	model_params[inagent][paramit].values_time->push_back(now);
	

	flag=1;
	}
}

void Controller::set_mparam(std::string inname, int inagent, MParameter param){
	
		int id_mparam = locate_mparameter(inname, model_params[inagent]);
		model_params[inagent][id_mparam]  = param;

	
}

void Controller::set_cparam(std::string inname, int inagent, CParameter param){
	
		int id_cparam = locate_cparameter(inname, control_params[inagent]);
		control_params[inagent][id_cparam]  = param;

	
}

void Controller::set_tmodel_parameter(std::string inname, int inagent, Value invalue){
	
		int id_mparam = locate_mparameter(inname, model_params[inagent]);
		model_params[inagent][id_mparam].target_value = invalue;

	
}

void Controller::set_tcontrol_parameter(std::string inname, int inagent, Value invalue){
	
		int id_cparam = locate_cparameter(inname, control_params[inagent]);
		control_params[inagent][id_cparam].target_value = invalue;

	
}

void Controller::set_tmodel_parameter(int inagent, int paramit,Value invalue){

	model_params[inagent][paramit].target_value = invalue;


}
void Controller::set_tcontrol_parameter(int inagent, int paramit, Value invalue){
	control_params[inagent][paramit].target_value = invalue;
}





void Controller::add_cparam(CParameter incparam){
	for(int agent=0; agent<getAgentsNum(); agent++){
		control_params[agent].push_back(incparam);
	}
}
void Controller::add_mparam(MParameter inmparam){
	for(int agent=0; agent<getAgentsNum(); agent++){
		model_params[agent].push_back(inmparam);
	}
}
void Controller::add_gcparam(GCParameter ingcparam){
	global_control_params.push_back(ingcparam);
}



void Controller::add_agent(){
	
	if(getAgentsNum()!=0){
		//add all the model parameters to the new agent
		std::vector<MParameter> newagentmparams;
		for(int mparam=0; mparam<model_params[0].size(); mparam++){
			MParameter newmparam = model_params[0][mparam];
			newagentmparams.push_back(newmparam);
		}
		model_params.push_back(newagentmparams);

		//add all the control parameters to the new agent
		std::vector<CParameter> newagentcparams;
		for(int cparam=0; cparam<control_params[0].size(); cparam++){
			CParameter newcparam = control_params[0][cparam];
			newagentcparams.push_back(newcparam);
		}
		control_params.push_back(newagentcparams);
	}else{
		std::cout << "Cannot add new agent because there are no agents to copy, please add it externally specifying the parameters"<< std::endl;
	}


}

void Controller::add_agent(std::vector<MParameter> inmparams, std::vector<CParameter> incparams){

	model_params.push_back(inmparams);
	control_params.push_back(incparams);


}

void Controller::remove_agent(int inagent){

	control_params.erase(control_params.begin()+inagent);
	model_params.erase(model_params.begin()+inagent);

}

int Controller::getAgentsNum(){
	return model_params.size();
}


std::vector<Profile> Controller::getAgents(){
	return agents;

}

void Controller::adjustParameters(){
	for(int i=0; i< getAgentsNum(); i++)
		adjustParameters(i);

}


void Controller::adjustParameters(int i){ //on specific player

	//find the maximum vector of values
	int maxvector=0;
		for(int j=0; j< model_params[i].size(); j++){
			if(model_params[i][j].values->size()> maxvector && model_params[i][j].values->size()>0){
				maxvector = model_params[i][j].values->size();		
			}
			if(model_params[i][j].values_time->size()> maxvector && model_params[i][j].values_time->size()>0){
				maxvector = model_params[i][j].values_time->size();		
			}
		}
		for(int j=0; j< control_params[i].size(); j++){
			if(control_params[i][j].values->size()> maxvector && control_params[i][j].values->size()>0){
				maxvector = control_params[i][j].values->size();		
			}
			if(control_params[i][j].values_time->size()> maxvector && control_params[i][j].values_time->size()>0){
				maxvector = control_params[i][j].values_time->size();		
			}
		}
	
	//adjust the parameters to the maximum vector of values
	
		for(int j=0; j< model_params[i].size(); j++){
			if(model_params[i][j].values->size()< maxvector && model_params[i][j].values->size()>0){
				std::cout << "adjusted "<< model_params[i][j].name  << " from "<< model_params[i][j].values->size() <<" to "<< maxvector<< std::endl;

				model_params[i][j].values = elongateVVector(model_params[i][j].values,maxvector);
			}
			if(model_params[i][j].values_time->size()< maxvector && model_params[i][j].values_time->size()>0){
				model_params[i][j].values_time = elongateTVector(model_params[i][j].values_time,maxvector);
			}
		}
		for(int j=0; j< control_params[i].size(); j++){
			if(control_params[i][j].values->size()< maxvector && control_params[i][j].values->size()>0){
				std::cout << "adjusted "<< control_params[i][j].name  << " from "<< control_params[i][j].values->size() <<" to "<< maxvector<< std::endl;
				control_params[i][j].values = elongateVVector(control_params[i][j].values,maxvector);
				
			}
			if(control_params[i][j].values_time->size()< maxvector && control_params[i][j].values_time->size()>0){
				control_params[i][j].values_time = elongateTVector(control_params[i][j].values_time,maxvector);
			}
		}
	
}

void Controller::adjustTime(int mints, int maxts, int agent){

	//adjustParameters(agent);
	for(int j=0; j< model_params[agent].size(); j++){
		std::pair<std::vector<Time>*,std::vector<Value>*> pair = splitValues(model_params[agent][j].values_time,model_params[agent][j].values,mints,maxts);
		model_params[agent][j].values_time = pair.first;
		model_params[agent][j].values = pair.second;
	}
	adjustParameters(agent);


}



void Controller::set_prevcontrol_parameter(std::string inname, int inagent, Value invalue){

	int id_cparam = locate_cparameter(inname, control_params[inagent]);
	
	control_params[inagent][id_cparam].prevvalue = invalue;
	//control_params[inagent][id_cparam].values[control_params[inagent][id_cparam].values.size()-1] = invalue; //last element
}

Value Controller::get_prevcontrol_parameter(std::string inname, int inagent){
	int id_cparam = locate_cparameter(inname, control_params[inagent]);
	return control_params[inagent][id_cparam].prevvalue;

}


void Controller::cleanValues(){
	for(int i=0; i< model_params.size(); i++){
		for(int j=0; j < model_params[i].size(); j++){
			model_params[i][j].values->clear();
			model_params[i][j].values_time->clear();
		}
	}

	for(int i=0; i< control_params.size(); i++){
		for(int j=0; j < control_params[i].size(); j++){
			control_params[i][j].values->clear();
			control_params[i][j].values_time->clear();
		}
	}
}
