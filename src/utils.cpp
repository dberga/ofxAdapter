
#include "utils.h"


Value RandomFloat(Value a, Value b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

bool atob(std::string str){
	if(str == "true" || str == "TRUE")
		return true;
	else
		return false;
}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
float round(float number)
{
  return floor(number + 0.5);
}

std::string FtoStr(float val, int decimals= 2){
	std::stringstream sval;
	sval << val;
	sval.precision(decimals);
	return sval.str();
}

std::vector<std::vector<std::string>> loadCSV(std::string separator, std::string path){
		ofFile file (path); 
		if (file.exists()){
			string fileExtension = ofToUpper(file.getExtension());
			
			
			if (fileExtension == "CSV") {
				ofBuffer buffer(file);
				std::vector<std::vector<std::string>> csv = std::vector<std::vector<std::string>>();
				
				while (!buffer.isLastLine()) {
					string line = buffer.getNextLine();
					std::vector<std::string> row = ofSplitString(line, separator);
					csv.push_back(row);
				}
				return csv;
				
			}else{
				std::cout << "ERROR: "<< "Selected file is not a CSV file"<< std::endl;
				return std::vector<std::vector<std::string>>();
			}
		}else{ 
			std::cout << "ERROR: "<< "Selected file does not exist"<< std::endl;
			return std::vector<std::vector<std::string>>();
		}
}

std::vector<std::vector<std::string>> loadManualCSV( std::string separator, std::string message = "Select a csv file"){
	
		std::string path;
		ofFileDialogResult openFileResult= ofSystemLoadDialog(message); 
		if (openFileResult.bSuccess){
			 path = openFileResult.getPath();
			//std::string selectedname = openFileResult.getName();
			
		}else{
			std::cout << "ERROR: "<< "Error on file selection"<< std::endl;
		}
		return loadCSV(separator,path);
	
}


std::vector<CParameter> parseCParams(std::vector<std::vector<std::string>> csv){
	std::vector<CParameter> cparams; 
	for(int i=1; i<csv.size(); i++){
	
		if(csv[i][0] == "CP" || csv[i][0] == "cp"){
			CParameter cp; //name,min,max,clamp,clamp_min,clamp_max,balance,balance_step
			cp.name = csv[i][1];
			cp.limits.min = atof(csv[i][2].c_str());
			cp.limits.max = atof(csv[i][3].c_str());
			cp.clamp = atob(csv[i][4].c_str());
			cp.clamp_limits.min = atof(csv[i][5].c_str());
			cp.clamp_limits.max = atof(csv[i][6].c_str());
			cp.balance = atoi(csv[i][7].c_str());
			cp.balance_step = atof(csv[i][8].c_str());
			cp.current_value = cp.limits.max *0.5;
			cp.prevvalue = 0.0f;
			cp.values = new std::vector<Value>();
			cp.values_time = new std::vector<Time>();
			cparams.push_back(cp);
		}
	}

	return cparams;
}

std::vector<MParameter> parseMParams(std::vector<std::vector<std::string>> csv){
	std::vector<MParameter> mparams; 
	for(int i=1; i<csv.size(); i++){
	
		if(csv[i][0] == "MP" || csv[i][0] == "mp"){
			MParameter mp; //name,min,max,target_value,approx_range
			mp.name = csv[i][1];
			mp.limits.min = atof(csv[i][2].c_str());
			mp.limits.max = atof(csv[i][3].c_str());
			if(is_number(csv[i][4]))
				mp.target_value = atof(csv[i][4].c_str());
			else
				mp.target_value = NULL;
			//mp.approximationRange = atof(csv[i][5].c_str());
			mp.current_value = mp.limits.max * 0.5;
			mp.values = new std::vector<Value>();
			mp.values_time = new std::vector<Time>();
			mparams.push_back(mp);
		}
	}

	return mparams;
}



std::vector<GCParameter> parseGCParams(std::vector<std::vector<std::string>> csv){
	std::vector<GCParameter> gcparams; 
	for(int i=1; i<csv.size(); i++){
	
		if(csv[i][0] == "GCP" || csv[i][0] == "gcp"){
			GCParameter gcp; //name,min,max,distr_type,balance,bsets<>,bmodulations<>
			gcp.name = csv[i][1];
			gcp.limits.min = atof(csv[i][2].c_str());
			gcp.limits.max = atof(csv[i][3].c_str());
			gcp.distr_type = atoi(csv[i][4].c_str());

				std::vector<Value> bsets;
					std::vector<std::string> bpsets = split(csv[i][5],',');
					for(int j=0; j<bpsets.size(); j++)
						bsets.push_back(atof(bpsets[j].c_str()));
			gcp.bsets = bsets;

				std::vector<Value> bmodulations;
					std::vector<std::string> bpmodulations = split(csv[i][6],',');
					for(int j=0; j<bpmodulations.size(); j++)
						bmodulations.push_back(atof(bpmodulations[j].c_str()));
			gcp.bmodulations = bmodulations;
			gcp.current_value = gcp.limits.max * 0.5;
			gcp.values = new std::vector<Value>();
			gcp.values_time = new std::vector<Time>();
			gcparams.push_back(gcp);
		}
	}

	return gcparams;
}
std::vector<Relation> parseCorrelations(std::vector<std::vector<std::string>> csv){
	std::vector<Relation> correlations; 
	for(int i=1; i<csv.size(); i++){
		if(csv[i][0] == "RL" || csv[i][0] == "rl"){
				Relation rel; //mp,cp
				rel.parameter1_name = csv[i][1];
				rel.parameter2_name = csv[i][2];

				correlations.push_back(rel);
		}
	}
	return correlations;
}

std::vector<std::vector<MParameter>> parsegen_MParams(std::vector<std::vector<std::string>> csv){
	std::vector<std::vector<MParameter>> mparams = std::vector<std::vector<MParameter>>();
	for(int i=1; i<csv.size(); i++){
		if(csv[i][0] == "MA" || csv[i][0] == "ma"){
			for(int j=0; j<std::stoi(csv[i][1]); j++)
				mparams.push_back(parseMParams(csv));
		}
	}
	return mparams;

}

std::vector<std::vector<CParameter>> parsegen_CParams(std::vector<std::vector<std::string>> csv){
	std::vector<std::vector<CParameter>> cparams = std::vector<std::vector<CParameter>>();
	for(int i=1; i<csv.size(); i++){
		if(csv[i][0] == "MA" || csv[i][0] == "ma"){
			for(int j=0; j<std::stoi(csv[i][1]); j++)
				cparams.push_back(parseCParams(csv));
		}
	}
	return cparams;
}

int parsegen_NPlayers(std::vector<std::vector<std::string>> csv){
	int count=0;
	for(int i=1; i<csv.size(); i++){
		if(csv[i][0] == "MA" || csv[i][0] == "ma"){
			count++;
		}
	}
	return count;
}


bool writeCSV(std::string separator, std::vector<std::vector<std::string>> data, std::string path){
	ofFile output(path,ofFile::WriteOnly);
		//relative path = ofToDataPath(file)

	for(int i=0; i<data.size(); i++){
		output << std::endl;
		for(int j=0; j<data[i].size(); j++){
			output << data[i][j];
			output << separator;
		}
	}
	output.close();

	return true;	
}

bool writeCSV2(std::string separator, std::vector<std::vector<std::string>> data, std::string path){
	//ofFile output(path,ofFile::WriteOnly);
		//relative path = ofToDataPath(file)

	
	writeCSV(separator, transpose(data), path);
	
	//output.close();

	return true;	
}

std::vector<std::vector<std::string>> transpose(std::vector<std::vector<std::string>> data){
	std::vector<std::vector<std::string>> data2;
	int max = 0;
	for(int i=0; i<data.size(); i++){
		for(int j=0; j<data[i].size(); j++){
			if(max <= data[i].size())
				max = data[i].size();
		}
	}
	data2 = std::vector<std::vector<std::string>>(max);
	for(int j=0; j< data2.size(); j++){
		for(int i=0; i<data.size(); i++){
			if(j< data[i].size())
				 data2[j].push_back(data[i][j]);
		}
	}
	return data2;
}

std::vector<std::string> string_separate(std::string instring, std::string separator){
	
	std::istringstream ss(instring);
	std::vector<std::string> outstring_vector;

	std::string token;

	while(std::getline(ss, token, separator.front())) {
		outstring_vector.push_back( token );
	}

	return outstring_vector;
}


std::vector<Value> transform_values(std::vector<std::string> values){
	std::vector<Value> f_values;
	for(int i=0; i<values.size(); i++){
		f_values.push_back(std::stof(values[i]));
	}

	return f_values;
}

std::vector<Profile> parseUsers(std::vector<std::vector<std::string>> csv){
	std::vector<Profile> users; 
	for(int i=1; i<csv.size(); i++){
		Profile user; //name;AthleteID;age
		user.name = csv[i][0];
		user.id = atoi(csv[i][1].c_str());
		user.age = atoi(csv[i][2].c_str());
		if(csv[i][3] != "null" && csv[i][3] != "")
			user.tHR = atof(csv[i][3].c_str());
		else
			user.tHR = 208-0.7*user.age;

		if(csv[i][4] != "null" && csv[i][4] != "")
			user.tAPA = atof(csv[i][4].c_str());
		else
			user.tAPA = -1;

		users.push_back(user);
	}

	return users;
}





std::vector<Profile> parseProfiles(std::vector<std::vector<std::string>> csv){ //todo: do not specify the type of parameter targets but as a vector of stuff
	std::vector<Profile> users; 

	try{
		for(int i=1; i<csv.size(); i++){
			Profile user; //ID;name;age;targetHR;targetAPA
			user.id = atoi(csv[i][0].c_str());
			user.name = csv[i][1];
			user.age = atoi(csv[i][2].c_str());
			if(csv[i].size()>5){ //specific target values
				if(csv[i][3] != "unknown" && csv[i][3] != "")
					user.tHR = atof(csv[i][3].c_str());
				else
					user.tHR = 100.0f;

				if(csv[i][4] != "unknown" && csv[i][4] != "")
					user.tAPA = atof(csv[i][4].c_str());
				else
					user.tAPA = 15;
				if(csv[i][5] != "unknown" && csv[i][5] != "")
						user.tIT = atof(csv[i][5].c_str());
				else
					user.tIT = 15;
				}
			users.push_back(user);
		}
	}catch(...){

		ofSystemAlertDialog("wrong csv file ");
	}
	return users;
}
	
std::vector<std::vector<std::string>> paramsToData(std::vector<MParameter> mparams, std::vector<CParameter> cparams ){
	std::vector<std::vector<std::string>> data;
	for(int j=0; j<mparams.size(); j++){
		std::vector<std::string> vals;
		vals.push_back(mparams[j].name);
		for(int v=0; v<mparams[j].values->size(); v++){
			vals.push_back(FtoStr(mparams[j].values->at(v),2));
		}
		data.push_back(vals);
	}
	for(int j=0; j<cparams.size(); j++){
		std::vector<std::string> vals;
		vals.push_back(cparams[j].name);
		for(int v=0; v<cparams[j].values->size(); v++){
			vals.push_back(FtoStr(cparams[j].values->at(v),2));
		}
		data.push_back(vals);
	}
	for(int j=0; j<mparams.size(); j++){ //only once IF there is any param
		std::vector<std::string> vals;
		vals.push_back("Time"+mparams[j].name);
		for(int v=0; v<mparams[j].values_time->size(); v++){
			vals.push_back(std::to_string(mparams[j].values_time->at(v).tm_hour)+":"+std::to_string(mparams[j].values_time->at(v).tm_min)+":"+std::to_string(mparams[j].values_time->at(v).tm_sec));
		}
		data.push_back(vals);
	}
	/*
	for(int j=0; j<cparams.size(); j++){ //only once IF there is any param
		std::vector<std::string> vals;
		vals.push_back("Time"+cparams[j].name);
		for(int v=0; v<cparams[j].values_time->size(); v++){
			vals.push_back(std::to_string(cparams[j].values_time->at(v).tm_hour)+":"+std::to_string(cparams[j].values_time->at(v).tm_min)+":"+std::to_string(cparams[j].values_time->at(v).tm_sec));
		}
		data.push_back(vals);
	}
	*/
	return data;
}


std::vector<std::vector<std::string>> paramsToData2(std::vector<MParameter> mparams, std::vector<CParameter> cparams, float split ){
	std::vector<std::vector<std::string>> data;

	//std::cout << "split="<< split << std::endl;

	for(int j=0; j<mparams.size(); j++){
		std::vector<std::string> vals;
		vals.push_back("(CUT)"+mparams[j].name);
		//std::cout << vals.back() << " from " << (int)(mparams[j].values->size()*(split)) << " to " << mparams[j].values->size() << std::endl;
		for(int v=(int)(mparams[j].values->size()*(split)); v<mparams[j].values->size(); v++){
			vals.push_back(FtoStr(mparams[j].values->at(v),2));
		}
		data.push_back(vals);
	}
	for(int j=0; j<cparams.size(); j++){
		std::vector<std::string> vals;
		vals.push_back("(CUT)"+cparams[j].name);
		//std::cout << vals.back() << " from " << (int)(cparams[j].values->size()*(split)) << " to " << cparams[j].values->size() << std::endl;
		for(int v=(int)(cparams[j].values->size()*(split)); v<cparams[j].values->size(); v++){
			vals.push_back(FtoStr(cparams[j].values->at(v),2));
		}
		data.push_back(vals);
	}
	for(int j=0; j<mparams.size(); j++){ //only once IF there is any param
		std::vector<std::string> vals;
		vals.push_back("(CUT)Time"+mparams[j].name);
		//std::cout << vals.back() << " from " << (int)(mparams[j].values->size()*(split)) << " to " << mparams[j].values->size() << std::endl;
		for(int v=(int)(mparams[j].values->size()*(split)); v<mparams[j].values_time->size(); v++){
			vals.push_back(std::to_string(mparams[j].values_time->at(v).tm_hour)+":"+std::to_string(mparams[j].values_time->at(v).tm_min)+":"+std::to_string(mparams[j].values_time->at(v).tm_sec));
		}
		data.push_back(vals);
	}

	return data;
}


std::vector<std::vector<std::string>> mixData(std::vector<std::vector<std::string>> data1, std::vector<std::vector<std::string>> data2){
	std::vector<std::vector<std::string>> data;

	for(int i=0; i<data1.size(); i++){
		data.push_back(data1[i]);
	}

	for(int i=0; i<data2.size(); i++){
		data.push_back(data2[i]);
	}
	return data;
}

////exer




MParameter EXERgenAPA(Value target){
	MParameter APA;
	APA.name = "APA";
	APA.current_value = 15;
	APA.limits.min = 0;
	APA.limits.max = 30;
	APA.target_value = target;
	APA.values = new std::vector<Value>();

	//APA.approximationRange = 5;

	return APA;
}

MParameter EXERgenHR(Value target){
	MParameter HR;
	HR.name = "HR";
	HR.current_value = 100;
	HR.limits.min = 0;
	HR.limits.max = 30;
	HR.target_value = target;
	HR.values = new std::vector<Value>();

	//HR.approximationRange=15;

	return HR;
}

CParameter EXERgenIT(){
	CParameter IT;
	IT.name = "IT";
	IT.current_value = 0;
	IT.limits.min = 0;
	IT.limits.max = 20;			
	IT.clamp = true;
	IT.clamp_limits.min = 4;
	IT.clamp_limits.max = 16;	
	IT.values = new std::vector<Value>();

	IT.balance=0; //0=no, 1=decrease, 2=increase
	IT.balance_step = 5;

	return IT;
}

GCParameter EXERgenGlobalIT(){
	GCParameter global_IT;
	global_IT.name = "global_IT";
	global_IT.cname = "IT";
	global_IT.current_value = 0;
	global_IT.limits.min = 0;
	global_IT.limits.max = 20;
	global_IT.distr_type = 1; //1=avg, 2=max
	global_IT.values = new std::vector<Value>();


	global_IT.balance = true;
	global_IT.bsets.push_back(1.8);
	global_IT.bmodulations.push_back(0.85);

	return global_IT;
}

Relation EXERgenCorrelation(std::string inmparam, std::string incparam){
	Relation correlation;
	correlation.parameter1_name = inmparam;
	correlation.parameter2_name = incparam;
	return correlation;
}



std::vector<MParameter> EXERgenProfileMParams(EXERProfile player){
	std::vector<MParameter> mparams = std::vector<MParameter>();
	MParameter APA = EXERgenAPA(player.targetAPA);
	MParameter HR = EXERgenHR(player.targetHR);
	mparams.push_back(APA);
	mparams.push_back(HR);
	return mparams;

}
std::vector<CParameter> EXERgenProfileCParams(EXERProfile player){
	std::vector<CParameter> cparams = std::vector<CParameter>();
	CParameter IT = EXERgenIT();
	cparams.push_back(IT);
	return cparams;
}

std::vector<GCParameter> EXERgenGlobalGCParams(){
	std::vector<GCParameter> gcparams = std::vector<GCParameter>();
	GCParameter global_IT = EXERgenGlobalIT();
	gcparams.push_back(global_IT);
	return gcparams;
}
std::vector<Relation> EXERgenCorrelations(){
	std::vector<Relation> correlations = std::vector<Relation>();
	Relation correlation1;
	correlation1 = EXERgenCorrelation("APA","IT");
	correlations.push_back(correlation1);
	return correlations;
}
/////////////////////////////////////////////

std::vector<EXERProfile> EXERparseUsers(std::vector<std::vector<std::string>> csv){
	std::vector<EXERProfile> users; 
	for(int i=1; i<csv.size(); i++){
		EXERProfile user; //name;AthleteID;age;weight;height;targetHR;targetAPA
		user.name = csv[i][0];
		user.AthleteID = atoi(csv[i][1].c_str());
		user.age = atoi(csv[i][2].c_str());
		user.weight = atoi(csv[i][3].c_str());
		user.height = atoi(csv[i][4].c_str());
		
		if(csv[i][5] != "unknown" && csv[i][5] != "")
			user.targetHR = atof(csv[i][5].c_str());
		else
			user.targetHR = 120.0f;

		if(csv[i][6] != "unknown" && csv[i][6] != "")
			user.targetAPA = atof(csv[i][6].c_str());
		else
			user.targetAPA = 15;
		
		users.push_back(user);
	}

	return users;
}



std::vector<std::vector<CParameter>> EXERgetCParams(std::vector<CParameter> cp, std::vector<EXERProfile> profiles){
	std::vector<std::vector<CParameter>> cparams;
	for(int i=0; i<profiles.size(); i++){
		cparams.push_back(cp);
	}
	return cparams;
}

std::vector<std::vector<MParameter>> EXERgetMParams(std::vector<MParameter> mp, std::vector<EXERProfile> profiles){
	std::vector<std::vector<MParameter>> mparams;
	for(int i=0; i<profiles.size(); i++){
		mparams.push_back(mp);
	}
	return mparams;
}

/////////////////////////


VSLogData parseVSLogData(std::vector<std::vector<std::string>> csv){
	VSLogData vdata;


		//TO DO!!!


	return vdata;
}

SuuntoLogData parseSuuntoLogData(std::vector<std::vector<std::string>> csv){

	SuuntoLogData sdata;
	try{
		sdata.LogID = atoi(csv[1][1].c_str());
		sdata.AthleteID = atoi(csv[5][1].c_str());

			std::string starttime = csv[2][1].c_str();
			std::vector<std::string> starttimevect = string_separate(starttime," ");
				starttimevect = string_separate(starttimevect[1],":");
				if(starttimevect.size() < 3)
					starttimevect.push_back("00");
				int starthour = atoi(starttimevect[0].c_str());
				int startmin = atoi(starttimevect[1].c_str());
				int startsec = atoi(starttimevect[2].c_str());

			std::string time = "00:00:00";
			std::string lasttime = csv[55][1]; //SecondInsid
			std::vector<SuuntoLogDataInTime> sdata_intime = std::vector<SuuntoLogDataInTime>();
				int i = 2;
			
				while(time != lasttime){
					SuuntoLogDataInTime scurrent;
						scurrent.HR = atoi(csv[i][15].c_str());
						scurrent.EPOC = atof(csv[i][16].c_str());
						scurrent.Vent = atoi(csv[i][17].c_str());
						scurrent.Resp = atoi(csv[i][18].c_str());
						scurrent.VO2 = atoi(csv[i][19].c_str());
						scurrent.kCal = atof(csv[i][20].c_str());
						scurrent.Time = csv[i][14]; 
						std::vector<std::string> timevect = string_separate(scurrent.Time,":");
							scurrent.hour = atoi(timevect[0].c_str());
							scurrent.min = atoi(timevect[1].c_str());
							scurrent.sec = atoi(timevect[2].c_str());

								scurrent.hour = scurrent.hour+starthour;
								scurrent.min = scurrent.min+startmin;
								scurrent.sec = scurrent.sec+startsec;


								if(scurrent.sec >= 60){
									scurrent.min +=1;
									scurrent.sec = scurrent.sec - 60;
								}

								if(scurrent.min >= 60){
									scurrent.hour +=1;
									scurrent.min =scurrent.min - 60;
								}
							
							
								//std::cout << "parsed"<< scurrent.hour <<":" << scurrent.min << ":"<< scurrent.sec <<std::endl;
						sdata_intime.push_back(scurrent);

						time = scurrent.Time;
						i++;
				}

			sdata.Tvalues = sdata_intime;
		}catch(...){
			sdata.LogID = -1;
			ofSystemAlertDialog("wrong file ");
		}
		return sdata;

		
	//LOGID=[1][1]
	//ATHLETEID=[5][1]

	//DATA = from [2][14] to [2][27]
	//	[2][14]=time
	//	[2][15]=HR
	//	[2][16]=EPOC
	//	[2][17]=Vent
	//	[2][18]=Resp
	//	[2][19]=VO2
	//	[2][20]=kCal
	
}
/*
std::vector<SuuntoLogDataInFrames> convertSuuntoTimeFrames(std::vector<SuuntoLogDataInTime> values){

	std::vector<SuuntoLogDataInFrames> Fvalues;
	for(int i=0;i<values.size();i++){
		SuuntoLogDataInFrames Fvalue;
			Fvalue.frame = i;
			Fvalue.HR = values[i].HR;
			Fvalue.EPOC = values[i].EPOC;
			Fvalue.kCal = values[i].kCal;
			Fvalue.Resp = values[i].Resp;
			Fvalue.Vent = values[i].Vent;
			Fvalue.VO2 = values[i].VO2;
		Fvalues.push_back(Fvalue);
	
	}
	return Fvalues;
}
*/

PlayerExertionInTimeAndFrames combineEXERparameters(SuuntoLogDataInTime sdata, VSLogDataInFrames vdata){
	PlayerExertionInTimeAndFrames data;
	data.Time = sdata.Time;
	data.hour = sdata.hour;
	data.sec = sdata.sec;
	data.frame = vdata.frame;
	data.APA = vdata.APA;
	data.HR = sdata.HR;
	data.EPOC = sdata.EPOC;
	data.kCal = sdata.kCal;
	data.Resp = sdata.Resp;
	data.Vent = sdata.Vent;
	data.VO2 = sdata.VO2;
	return data;

}



std::vector<PlayerExertionInTimeAndFrames> convertEXERvectors(std::vector<SuuntoLogDataInTime> svector, std::vector<VSLogDataInFrames> vvector){
	std::vector<PlayerExertionInTimeAndFrames> exertion;

	if(svector.size() != vvector.size()){
		if(svector.size() < vvector.size()){
			float prop = vvector.size()/svector.size();

			for(int i=0; i<svector.size(); i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>svector.size()-1) j=svector.size()-1;

					PlayerExertionInTimeAndFrames currentdata;
					currentdata = combineEXERparameters(svector[i],vvector[j]);
					exertion.push_back(currentdata);

			}
		}else{ //less probable
			float prop = svector.size()/vvector.size();

			for(int i=0; i<vvector.size(); i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vvector.size()-1) j=vvector.size()-1;

					PlayerExertionInTimeAndFrames currentdata;
					currentdata = combineEXERparameters(svector[j],vvector[i]);
					exertion.push_back(currentdata);
				
			}
		}
	}else{
		for(int i=0; i<svector.size(); i++){
			PlayerExertionInTimeAndFrames currentdata;
			currentdata = combineEXERparameters(svector[i],vvector[i]);
			exertion.push_back(currentdata);
		}
	}

	return exertion;
}



PlayerExertion readCalibVSandSuunto(int AthleteID, std::vector<SuuntoLogDataInTime> svector, std::vector<VSLogDataInFrames> vvector){
	PlayerExertion userExertion;
	userExertion.AthleteID = AthleteID;
	userExertion.exertion = convertEXERvectors(svector, vvector);
	return userExertion;
}

std::vector<std::vector<std::string>> parseExertion(PlayerExertion exer){
	std::vector<std::vector<std::string>> data;
	data.push_back(std::vector<std::string>());
	data[0].push_back( "APA");
	for(int z=0; z <exer.exertion.size(); z++ )
	{
		data[0].push_back(std::to_string(exer.exertion[z].APA));
	}
	data.push_back(std::vector<std::string>());
	data[1].push_back( "HR");
	for(int z=0; z <exer.exertion.size(); z++ )
	{
		data[1].push_back(std::to_string(exer.exertion[z].HR));
	}
	return data;
}


Value findCorrTarget(std::vector<Value> a, std::vector<Value> b, Value tb, Value range){
	
	if(a.size() == b.size()){
		std::vector<Value> tavalues = std::vector<Value>();
		tavalues.clear();

		Value tbminrange = tb-range;
		Value tbmaxrange = tb+range;

		for(int z=0; z<b.size(); z++){
			if(b[z] > tbminrange && b[z] < tbmaxrange){
				
				tavalues.push_back(a[z]);
			}
		}

		
		Value mean=0;
		Value sum=0;
		int flag=0;

		if(tavalues.size() > 0){
			mean=0;
			sum=0;
			for(int ta=0; ta<tavalues.size(); ta++){
				sum += tavalues[ta];
			}
			
			mean = sum/tavalues.size();
		}else{

			//range+= (range*0.1);
			//return findCorrTarget(a,b,tb,range);

			return -1;

		}

		//mean = (mean*tb)/(tbminrange);
		return mean;
		
	}else{
		return 0;
	}
}

Value findCorrTarget2(std::vector<Value> a, std::vector<Value> b, Value tb, Value range){
	
	if(a.size() == b.size()){
		std::vector<Value> tavalues = std::vector<Value>();
		tavalues.clear();

		Value tbminrange = tb-range;
		Value tbmaxrange = tb+range;

		for(int z=0; z<b.size(); z++){
			if(b[z] >= tbminrange && b[z] <= tbmaxrange){
				
				tavalues.push_back(a[z]);
			}
		}

		
		Value mean=0;
		Value sum=0;
		int flag=0;

		if(tavalues.size() > 0){
			mean=0;
			sum=0;
			for(int ta=0; ta<tavalues.size(); ta++){
				sum += tavalues[ta];
			}
			
			mean = sum/tavalues.size();
		}else{

			range+= (range*0.1);
			
			mean= findCorrTarget2(a,b,tb,range);
			//std::cout << "changed mean="<<mean << std::endl;
			if(mean > 0) 
				mean = mean*(-1); //put to negative, to indicate modification

		}

		//mean = (mean*tb)/(tbminrange);
		return mean;
		
	}else{
		return 0;
	}
}
Value findCorrAbove(std::vector<Value> a, std::vector<Value> b, Value tb){
	if(a.size() == b.size()){
		std::vector<Value> tavalues = std::vector<Value>();
		tavalues.clear();


		for(int z=0; z<b.size(); z++){
			if(b[z] > tb){
				
				tavalues.push_back(a[z]);
			}
		}

		
		Value mean=0;
		Value sum=0;
		int flag=0;

		if(tavalues.size() > 0){
			mean=0;
			sum=0;
			for(int ta=0; ta<tavalues.size(); ta++){
				sum += tavalues[ta];
			}
			
			mean = sum/tavalues.size();
		}else{
			mean= findCorrAbove(a,b,tb-1);
			//std::cout << "changed mean="<<mean << std::endl;
			if(mean > 0) 
				mean = mean*(-1); //put to negative, to indicate modification

		}

		//mean = (mean*tb)/(tbminrange);
		return mean;
		
	}else{
		return 0;
	}
}
Value findCorrBelow(std::vector<Value> a, std::vector<Value> b, Value tb) {

	if(a.size() == b.size()){
		std::vector<Value> tavalues = std::vector<Value>();
		tavalues.clear();


		for(int z=0; z<b.size(); z++){
			if(b[z] < tb){
				
				tavalues.push_back(a[z]);
			}
		}

		
		Value mean=0;
		Value sum=0;
		int flag=0;

		if(tavalues.size() > 0){
			mean=0;
			sum=0;
			for(int ta=0; ta<tavalues.size(); ta++){
				sum += tavalues[ta];
			}
			
			mean = sum/tavalues.size();
		}else{
			mean= findCorrBelow(a,b,tb+1);
			//std::cout << "changed mean="<<mean << std::endl;
			if(mean > 0) 
				mean = mean*(-1); //put to negative, to indicate modification

		}

		//mean = (mean*tb)/(tbminrange);
		return mean;
		
	}else{
		return 0;
	}
}


std::vector<std::vector<std::string>> parseAgents(std::vector<Profile> agents){
	std::vector<std::vector<std::string>> data;


		for(int i=0; i<agents.size(); i++){
			data.push_back(std::vector<std::string>());
			data[i].push_back(std::to_string(agents[i].id));
			data[i].push_back(agents[i].name);
			data[i].push_back(std::to_string(agents[i].age));
			data[i].push_back(std::to_string(agents[i].tHR));
			data[i].push_back(std::to_string(agents[i].tAPA));
			data[i].push_back(std::to_string(agents[i].tIT));
				data[i].push_back(std::to_string(agents[i].uHR));
				data[i].push_back(std::to_string(agents[i].lHR));
				data[i].push_back(std::to_string(agents[i].uAPA));
				data[i].push_back(std::to_string(agents[i].lAPA));
				data[i].push_back(std::to_string(agents[i].uIT));
				data[i].push_back(std::to_string(agents[i].lIT));
		}

	
	return data;
	

}


MParameter transform_HR(std::vector<SuuntoLogDataInTime> suuntologdata, MParameter hrparam){

	MParameter mparam = hrparam;
	
	hrparam.values = new std::vector<Value>();	
	hrparam.values_time->clear();
	for(int z=0; z<suuntologdata.size(); z++){
		hrparam.values->push_back(suuntologdata[z].HR);

		Time logtime =  std::tm();
		logtime.tm_hour = suuntologdata[z].hour;
		logtime.tm_min = suuntologdata[z].min;
		logtime.tm_sec = suuntologdata[z].sec;
		
		hrparam.values_time->push_back(logtime);
	}
	
	return hrparam;
}



std::vector<Value> cutVVector(std::vector<Value> vect,int size ){
	std::vector<Value> fvect;
	if(size != vect.size()){
		if(size < vect.size()){
			float prop = (float)vect.size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect.size()-1) j=vect.size()-1;

					fvect.push_back(vect[j]);

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}

std::vector<Time> cutTVector(std::vector<Time> vect,int size ){
	std::vector<Time> fvect;
	if(size != vect.size()){
		if(size < vect.size()){
			float prop = (float)vect.size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect.size()-1) j=vect.size()-1;

					fvect.push_back(vect[j]);

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}


std::vector<Value> elongateVVector(std::vector<Value> vect,int size ){




	std::vector<Value> fvect;
	if(size != vect.size()){
		if(size > vect.size() && vect.size() > 0){
			float prop = (float)vect.size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect.size()-1) j=vect.size()-1;
					
					fvect.push_back(vect[j]);

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}

std::vector<Time> elongateTVector(std::vector<Time> vect,int size ){
	std::vector<Time> fvect;
	if(size != vect.size()){
		if(size > vect.size() && vect.size() > 0){
			float prop = (float)vect.size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect.size()-1) j=vect.size()-1;
					
					

					fvect.push_back(vect[j]);

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}



std::vector<Value>* elongateVVector(std::vector<Value> *vect,int size ){

	std::vector<Value> *fvect = new std::vector<Value>();
	if(size != vect->size()){
		if(size > vect->size() && vect->size() > 0){
			float prop = (float)vect->size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect->size()-1) j=vect->size()-1;
					
								

					fvect->push_back(vect->at(j));

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}

std::vector<Time>* elongateTVector(std::vector<Time> *vect,int size ){
	std::vector<Time> *fvect = new std::vector<Time>();
	if(size != vect->size()){
		if(size > vect->size() && vect->size() > 0){
			float prop = (float)vect->size()/(float)size;

			for(int i=0; i<size; i++){
				
					int j = floor((i*prop) + 0.5);//round(i*prop);
					if(j>vect->size()-1) j=vect->size()-1;
					
					

					fvect->push_back(vect->at(j));

			}
			
		}
		
	}else{
		fvect = vect;
	}
	return fvect;

}

Threshold get_minmax_timestamp(std::vector<Time> *vector){

	Threshold minmax;
	if(vector->size()>0){
	
		Value shours = vector->front().tm_hour;
		Value smin = vector->front().tm_min;
		Value ssec = vector->front().tm_sec;
		minmax.min = get_timestamp(shours,smin,ssec);

		Value fhours = vector->back().tm_hour;
		Value fmin = vector->back().tm_min;
		Value fsec = vector->back().tm_sec;
		minmax.max = get_timestamp(fhours,fmin,fsec);
	}else{
		minmax.min = get_timestamp(0,0,0);
		minmax.max = get_timestamp(23,59,59);
	}
	return minmax;
}
Value get_timestamp(int hour, int min, int sec){
	return hour*3600+min*60+sec;
}



std::vector<MParameter> parseMData(std::vector<std::vector<std::string>> csv, std::vector<MParameter> inmparams){
	std::vector<MParameter> mparams = inmparams;
	try{
		for(int c1=0; c1<csv.size(); c1++){
			for(int j=0; j<mparams.size(); j++){
				if(mparams.at(j).name == csv[c1][0]){
					mparams.at(j).values->clear();
					for(int c2=1; c2<csv[c1].size(); c2++){
						mparams.at(j).values->push_back(atof(csv[c1][c2].c_str()));
					}
				}
				else if("Time"+mparams.at(j).name == csv[c1][0]){
					mparams.at(j).values_time->clear();
					for(int c2=1; c2<csv[c1].size()-1; c2++){
						std::vector<std::string> timevect = string_separate(csv[c1][c2],":");
						Time newtime = Time();
						newtime.tm_hour = atoi(timevect[0].c_str());
						newtime.tm_min = atoi(timevect[1].c_str());
						newtime.tm_sec = atoi(timevect[2].c_str());
						mparams.at(j).values_time->push_back(newtime);
					}
				}
			}
		}
		
	}catch(...){
		ofSystemAlertDialog("wrong csv file ");
		
	}
	return mparams;

}



std::vector<CParameter> parseCData(std::vector<std::vector<std::string>> csv,std::vector<CParameter> incparams){
	std::vector<CParameter> cparams = incparams;
	try{
		for(int c1=0; c1<csv.size(); c1++){
			for(int j=0; j<cparams.size(); j++){
				if(cparams.at(j).name == csv[c1][0]){
					cparams.at(j).values->clear();
					for(int c2=1; c2<csv[c1].size(); c2++){
						cparams.at(j).values->push_back(atof(csv[c1][c2].c_str()));
					}
				}
				else if("Time"+cparams.at(j).name == csv[c1][0]){
					cparams.at(j).values_time->clear();
					for(int c2=1; c2<csv[c1].size()-1; c2++){
						std::vector<std::string> timevect = string_separate(csv[c1][c2],":");
						Time newtime = Time();
						newtime.tm_hour = atoi(timevect[0].c_str());
						newtime.tm_min = atoi(timevect[1].c_str());
						newtime.tm_sec = atoi(timevect[2].c_str());
						cparams.at(j).values_time->push_back(newtime);
					}
				}
			}
		}
	}catch(...){
		ofSystemAlertDialog("wrong csv file ");
		
	}
	return cparams;
}




std::pair<std::vector<Time>*,std::vector<Value>*> splitValues(std::vector<Time> *vect1 ,std::vector<Value> *vect2 ,int mints, int maxts ){
	std::pair<std::vector<Time>*,std::vector<Value>*> pair = std::pair<std::vector<Time>*,std::vector<Value>*>();
	pair.first = new std::vector<Time>();
	pair.second = new std::vector<Value>();

	for(int z=0; z<vect1->size() ; z++){
		if(get_timestamp(vect1->at(z).tm_hour,vect1->at(z).tm_min,vect1->at(z).tm_sec) >= mints && get_timestamp(vect1->at(z).tm_hour,vect1->at(z).tm_min,vect1->at(z).tm_sec) <= maxts){
			pair.first->push_back(vect1->at(z));		
			pair.second->push_back(vect2->at(z));	
		}
	}

	return pair;
	
}


std::vector<Value>* applyITFilter(std::vector<Value> *vect,int player ){
	std::vector<Value> *fvect = new std::vector<Value>();

	if(vect->size()>0){
		
		for(int z=0; z<vect->size(); z++){
				//std::cout << "IT before:" << std::to_string(vect->at(z)) << std::endl;

			//std::cout << "IT before base filter=" << vect->at(z) << std::endl;	
			Value filteredIT = (float)vect->at(z)-(player*2);

			fvect->push_back(filteredIT); 
				//std::cout << "IT after:" <<  std::to_string(filteredIT) << std::endl;

			//std::cout << "IT after base filter=" << filteredIT << std::endl;	
		}
	}
	return fvect;
}
std::vector<std::vector<CParameter>> adjustSlideIT(std::vector<std::vector<CParameter>> incparams){
	std::vector<std::vector<CParameter>> cparams = incparams;

	for(int i=0; i<cparams.size(); i++){
		for(int j=0; j<cparams[i].size(); j++){
			if(cparams[i][j].name == "IT"){
				cparams[i][j].values = applyITFilter(cparams[i][j].values,i);
				
			}
		}
	}
	return cparams;
}


std::vector<std::vector<CParameter>> adjustClampCParams(std::vector<std::vector<CParameter>> incparams){
	std::vector<std::vector<CParameter>> cparams = incparams;

	for(int i=0; i<cparams.size(); i++){
		for(int j=0; j<cparams[i].size(); j++){

			
			cparams[i][j].values = clampValues(cparams[i][j].values,cparams[i][j].clamp_limits.min,cparams[i][j].clamp_limits.max);
				
		}
	}
	return cparams;
}

std::vector<Value>* clampValues(std::vector<Value> *vect,Value minclamp, Value maxclamp ){
	std::vector<Value> *fvect = new std::vector<Value>();

	if(vect->size()>0){
		fvect->push_back(vect->at(0));

		for(int z=1; z<vect->size(); z++){
			//std::cout << "IT before base clamp=" << vect->at(z) << std::endl;	
			Value clampedvalue;
			if(vect->at(z) < minclamp)
				clampedvalue = minclamp;
			if( vect->at(z) > maxclamp)
				clampedvalue = maxclamp;
			
			fvect->push_back(clampedvalue); 
			//std::cout << "IT after base clamp=" << clampedvalue << std::endl;
		}
	}
	return fvect;
}




void printvector(std::vector<Value> vector){
	std::cout << "vector values: " << std::endl ;
	for(int z=0; z<vector.size(); z++)
		std::cout << vector.at(z) << ",";
	
	std::cout << std::endl;
}



Value vector_avg(std::vector<Value> vector){
	
	Value sum = 0;
	for(int i=0; i<vector.size(); i++)
		sum += vector[i];

	return sum/vector.size();

}

