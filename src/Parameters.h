


#ifndef __Parameters__
#define __Parameters__

#include <iostream>
#include <string>
#include <ctime>
#include "ofMain.h"

//
typedef float Value; //single value
typedef std::vector<Value> TValue; //value in discrete time
typedef std::tm Time; //single real time
typedef std::vector<Time> TTime; //real time duration
	//functions that output tm = asctime(tm), tm strptime("string")


struct Threshold {
    Value max;
    Value min;
};
struct Relation {
	std::string parameter1_name;
	std::string parameter2_name;
};

//

struct Parameter{
	std::string name; //name
	Value current_value;
	Threshold limits; //min,max
};


struct MParameter : Parameter{
	Value target_value; //value
	TTime* values_time;
	//Value approximationRange; //>5,<5 of target_value
	TValue* values;  //values[instance or time]
	Value average;
};

struct CParameter : Parameter{
	TValue* values;  //values[instance]
	TTime* values_time;
	bool clamp; //yes,no
	Threshold clamp_limits; //min,max
	int balance; //0=no, 1=decrease, 2=increase
	Value balance_step;
	Value prevvalue;

	//should not be considered
	Value target_value;
};

struct GCParameter : Parameter{
	TValue* values;  //values[instance or time]
	TTime* values_time;
	bool balance; //yes,no
	std::vector<Value> bsets;
	std::vector<Value> bmodulations;

	int distr_type; //1=average,2=max
	std::string cname;
};



//



int locate_parameter(std::string inname, std::vector<Parameter> inparamvector);
int locate_mparameter(std::string inname, std::vector<MParameter> inmparamvector);
int locate_cparameter(std::string inname, std::vector<CParameter> incparamvector);
int locate_gcparameter(std::string inname, std::vector<GCParameter> ingcparamvector);
int locate_correlation_parameter(std::string inname1, std::string inname2, Relation incorrelation, std::vector<MParameter> inparams1, std::vector<CParameter> inparams2, int inchoose);
//

struct Profile{
	int id;
	std::string name;
	int age;
	Value tHR;
		Value lHR;
		Value uHR;
	Value tAPA;
		Value lAPA;
		Value uAPA;
	Value tIT;
		Value lIT;
		Value uIT;

};



//parameter constructors



#endif
