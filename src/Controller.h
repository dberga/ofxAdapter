

#ifndef __Controller__
#define __Controller__

#include "Parameters.h"
#include "utils.h"

class Controller {

public:
    Controller(); //constructor
	int flag;
	bool running;
	bool saved;

	void Setup(
		std::vector<std::vector<MParameter>> inmodel_params,
		std::vector<std::vector<CParameter>> incontrol_params, 
		std::vector<GCParameter> inglobal_control_params
		);
	void update();

	//control over controller
	void start();
	void stop();

	//feed the system
	void update_model_parameter(std::string inname, int inagent, Value invalue);
	void update_control_parameter(std::string inname, int inagent, Value invalue);
	void update_global_control_parameter(std::string inname, Value invalue);
		void update_model_parameter(int inagent, int paramit,Value invalue);
		
		void update_control_parameter(int inagent, int paramit, Value invalue);
		void update_global_control_parameter(int paramit, Value invalue);
	
	//core parameter getters and setters
	std::vector<std::vector<MParameter>> getModelParameters();
	std::vector<std::vector<CParameter>> getControlParameters();
	std::vector<GCParameter> getGlobalControlParameters();
	void setModelParameters(std::vector<std::vector<MParameter>> inmodel_params);
	void setControlParameters(std::vector<std::vector<CParameter>> incontrol_params);
	void setGlobalControlParameters(std::vector<GCParameter> inglobal_control_params);
	void setAgents(std::vector<Profile> inagents);

	//individual parameter getters and setters
	Value get_control_parameter(std::string inname, int inagent);
	Value get_model_parameter(std::string inname, int inagent);
	Value get_global_control_parameter(std::string inname);
	void set_global_control_parameter(std::string inname, Value invalue);
	void set_control_parameter(std::string inname, int inagent, Value invalue);
	void set_model_parameter(std::string inname, int inagent, Value invalue);
	void set_tmodel_parameter(std::string inname, int inagent, Value invalue);
	void set_tcontrol_parameter(std::string inname, int inagent, Value invalue);
	void set_tmodel_parameter(int inagent, int paramit, Value invalue);
	void set_tcontrol_parameter(int inagent, int paramit, Value invalue);
	void set_mparam(std::string inname, int inagent, MParameter param);
	void set_cparam(std::string inname, int inagent, CParameter param);
		void set_global_control_parameter(int paramit,Value invalue);
		void set_control_parameter(int agent, int paramit,Value invalue);
		void set_model_parameter(int agent, int paramit,Value invalue);
		void set_model_parameter_target(int agent, int paramit,Value invalue);
	void set_prevcontrol_parameter(std::string inname, int inagent, Value invalue);
	Value get_prevcontrol_parameter(std::string inname, int inagent);

	std::vector<Profile> getAgents();
	
	CParameter* getref_control_parameter(int inagent, int inparam);
	MParameter* getref_model_parameter(int inagent, int inparam);
	GCParameter* getref_global_control_parameter(int inparam);
	
	void adjustParameters();
	void adjustParameters(int i);
	void adjustTime(int mints, int maxts, int agent);
	std::vector<std::vector<std::string>> matParams();
	void cleanValues();

	//add parameters
	void add_cparam(CParameter incparam);
	void add_mparam(MParameter inmparam);
	void add_gcparam(GCParameter ingcparam);

	//add agents
	int getAgentsNum();
	void add_agent();
	void add_agent(std::vector<MParameter> inmparams, std::vector<CParameter> incparams);
	void remove_agent(int inagent);
	
protected:
	std::vector<std::vector<MParameter>> model_params; //params[agent][parameter].values[instance or time]
	std::vector<std::vector<CParameter>> control_params; //params[agent][parameter].values[instance or time]
	std::vector<GCParameter> global_control_params; //params[parameter].values[instance or time]
	std::vector<Profile> agents;
	time_t curr_time;
	
};



#endif
