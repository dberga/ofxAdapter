#pragma once

#include "Parameters.h"
#include <string>
#include <sstream>
#include <iostream>
#include <exception>

Value RandomFloat(Value a, Value b);
bool atob(std::string str);
bool is_number(const std::string& s);
std::vector<std::string> split(const std::string &s, char delim);
float round(float number);
std::vector<std::vector<std::string>> transpose(std::vector<std::vector<std::string>> data);
std::string FtoStr(float val, int decimals);

std::vector<std::vector<std::string>> loadCSV(std::string separator, std::string path);
std::vector<std::vector<std::string>> loadManualCSV( std::string separator, std::string message);

bool writeCSV(std::string separator, std::vector<std::vector<std::string>> data,std::string path);
bool writeCSV2(std::string separator, std::vector<std::vector<std::string>> data,std::string path);


std::vector<std::string> string_separate(std::string instring, std::string separator);

std::vector<Value> transform_values(std::vector<std::string> values);


std::vector<CParameter> parseCParams(std::vector<std::vector<std::string>> csv);
std::vector<MParameter> parseMParams(std::vector<std::vector<std::string>> csv);
std::vector<GCParameter> parseGCParams(std::vector<std::vector<std::string>> csv);
std::vector<Relation> parseCorrelations(std::vector<std::vector<std::string>> csv);
std::vector<std::vector<CParameter>> parsegen_CParams(std::vector<std::vector<std::string>> csv);
std::vector<std::vector<MParameter>> parsegen_MParams(std::vector<std::vector<std::string>> csv);

int parsegen_NPlayers(std::vector<std::vector<std::string>> csv);

std::vector<std::vector<std::string>> paramsToData(std::vector<MParameter> mparams, std::vector<CParameter> cparams );
std::vector<std::vector<std::string>> paramsToData2(std::vector<MParameter> mparams, std::vector<CParameter> cparams, float split );
std::vector<std::vector<std::string>> mixData(std::vector<std::vector<std::string>> data1, std::vector<std::vector<std::string>> data2);


std::vector<MParameter> parseMData(std::vector<std::vector<std::string>> csv, std::vector<MParameter> inmparams);
std::vector<CParameter> parseCData(std::vector<std::vector<std::string>> csv, std::vector<CParameter> incparams);



//specific

std::vector<Profile> parseProfiles(std::vector<std::vector<std::string>> csv);

///exer


struct EXERProfile {
	std::string name;
	int AthleteID;
	int age;
	int weight;
	int height;
	Value targetHR;
	Value targetAPA;
};

MParameter EXERgenAPA(Value targetAPA);
MParameter EXERgenHR(Value target);
CParameter EXERgenIT();
GCParameter EXERgenGlobalIT();
Relation EXERgenCorrelation(std::string inmparam, std::string incparam);

std::vector<MParameter> EXERgenProfileMParams(EXERProfile player);
std::vector<CParameter> EXERgenProfileCParams(EXERProfile player);

std::vector<GCParameter> EXERgenGlobalGCParams();
std::vector<Relation> EXERgenCorrelations();
///////////////////////

std::vector<std::vector<CParameter>> EXERgetCParams(std::vector<CParameter> cp, std::vector<EXERProfile> profiles);
std::vector<std::vector<MParameter>> EXERgetMParams(std::vector<MParameter> mp, std::vector<EXERProfile> profiles);

std::vector<EXERProfile> EXERparseUsers(std::vector<std::vector<std::string>> csv);

////////////////



struct SuuntoLogDataInTime{
	
	std::string Time;
		int hour;
		int min;
		int sec;
	int HR;
	float EPOC;
	int Vent;
	int Resp;
	int	VO2;	
	float kCal;
};

struct SuuntoLogDataInFrames{
	int frame;
	int HR;
	float EPOC;
	int Vent;
	int Resp;
	int	VO2;	
	float kCal;
};

struct SuuntoLogData{
	int LogID;
	int AthleteID;
	std::vector<SuuntoLogDataInTime> Tvalues;
	//std::vector<SuuntoLogDataInFrames> Fvalues;//not parsed

};



struct VSLogDataInTime{
	std::string Time;
		int hour;
		int min;
		int sec;

	int APA;

};

struct VSLogDataInFrames{
	int frame;
	int APA;
};

struct VSLogData{
	int AthleteID;
	std::vector<VSLogDataInFrames> Fvalues;
	//std::vector<VSLogDataInTime> Tvalues; //not parsed
};


struct PlayerExertionInTimeAndFrames{
	int frame;
	std::string Time;
		int hour;
		int min;
		int sec;

	int APA;
	int HR;
		float EPOC;
		int Vent;
		int Resp;
		int	VO2;	
		float kCal;
		
};

struct PlayerExertion{
	int AthleteID;

	std::vector<PlayerExertionInTimeAndFrames> exertion;

};

MParameter transform_HR(std::vector<SuuntoLogDataInTime> suuntologdata, MParameter hrparam);

SuuntoLogData	parseSuuntoLogData(std::vector<std::vector<std::string>> csv);
VSLogData		parseVSLogData(std::vector<std::vector<std::string>> csv);

	PlayerExertionInTimeAndFrames combineEXERparameters(SuuntoLogDataInTime sdata, VSLogDataInFrames vdata);
	std::vector<PlayerExertionInTimeAndFrames> convertEXERvectors(std::vector<SuuntoLogDataInTime> svector, std::vector<VSLogDataInFrames> vvector);

PlayerExertion readCalibVSandSuunto(int AthleteID, std::vector<SuuntoLogDataInTime> svector, std::vector<VSLogDataInFrames> vvector);

std::vector<std::vector<std::string>> parseExertion(PlayerExertion exer);

//std::vector<SuuntoLogDataInFrames> convertSuuntoTimeFrames(std::vector<SuuntoLogDataInTime> values);

Value findCorrTarget(std::vector<Value> a, std::vector<Value> b, Value tb, Value range);
Value findCorrTarget2(std::vector<Value> a, std::vector<Value> b, Value tb, Value range);
Value findCorrAbove(std::vector<Value> a, std::vector<Value> b, Value tb);
Value findCorrBelow(std::vector<Value> a, std::vector<Value> b, Value tb);


std::vector<std::vector<std::string>> parseAgents(std::vector<Profile> agents);

std::vector<Value> cutVVector(std::vector<Value> vect,int size );
std::vector<Time> cutTVector(std::vector<Time> vect,int size );




std::vector<Value> elongateVVector(std::vector<Value> vect,int size );
std::vector<Time> elongateTVector(std::vector<Time> vect,int size );

std::vector<Value>* elongateVVector(std::vector<Value> *vect,int size );
std::vector<Time>* elongateTVector(std::vector<Time> *vect,int size );



std::pair<std::vector<Time>*,std::vector<Value>*> splitValues(std::vector<Time> *vect1 ,std::vector<Value> *vect2 ,int mints, int maxts );


Threshold get_minmax_timestamp(std::vector<Time> *vector);
Value get_timestamp(int hour, int min, int sec);


std::vector<Value>* applyITFilter(std::vector<Value> *vect,int player );
std::vector<std::vector<CParameter>> adjustSlideIT(std::vector<std::vector<CParameter>> incparams);

std::vector<std::vector<CParameter>> adjustClampCParams(std::vector<std::vector<CParameter>> incparams);
std::vector<Value>* clampValues(std::vector<Value> *vect,Value minclamp, Value maxclamp );

void printvector(std::vector<Value> vector);

Value vector_avg(std::vector<Value> vector);