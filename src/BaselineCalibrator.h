

#ifndef __BaselineCalibrator__
#define __BaselineCalibrator__

#include "Parameters.h"
#include "utils.h"
#include "Controller.h"

class BaselineCalibrator : public Controller{

public:
    BaselineCalibrator(); //constructor

	void Setup(
		std::vector<std::vector<MParameter>> inmodel_params,
		std::vector<std::vector<CParameter>> incontrol_params, 
		std::vector<GCParameter> inglobal_control_params,
		std::vector<std::vector<std::vector<Value>>> inmodel_base,
		std::vector<std::vector<std::vector<Value>>> incontrol_base,
		int intiming_type, //if the timing_type = 1, intiming is the step time; if its 2, intiming is the session time in min. which is transformed to the timing
		int intiming 
		);
	void Setup(
		std::vector<std::vector<MParameter>> inmodel_params,std::vector<std::vector<CParameter>> incontrol_params, std::vector<GCParameter> inglobal_control_params
		);
	void update();

	void setcontrolbase(std::vector<std::vector<std::vector<Value>>> cbase);
	void setSession(int stime);

	int getSession();

	//baseline system 
	void baseline();
	void calibrate();
		void calibrate_mparam(int agent, int index,  std::vector<Value> invalues);
		void calibrate_cparam(int agent, int index,  std::vector<Value> invalues);

	bool followbase;
protected:
	std::vector<std::vector<std::vector<Value>>> model_base;
	std::vector<std::vector<std::vector<Value>>> control_base;

	std::clock_t basetime;

	int timing_type;
		int timing; //type 1
		int sessiontime; //type 2
		std::vector<int> s_timings; //type 2
	int startingtime;
};

std::vector<std::vector<std::vector<Value>>> generate_base(std::vector<std::vector<CParameter>> inparams, std::vector<Value> invalues);
std::vector<std::vector<std::vector<Value>>> generate_base(std::vector<std::vector<MParameter>> inparams, std::vector<Value> invalues);

//std::vector<std::vector<std::vector<Value>>> generate_IT_base(std::vector<std::vector<CParameter>> inparams, std::vector<std::vector<std::string>> base);

#endif
