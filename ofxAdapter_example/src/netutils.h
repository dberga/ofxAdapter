

#include "ofxOsc.h"


#define SENDHOST "localhost"
#define SENDPORT 12543
#define LISTENPORT 12345
//#define LISTENHOST localhost


void OscSetup(std::string sendhost, int sendport, int listenport, ofxOscSender* sender, ofxOscReceiver* receiver);
void OscSend(ofxOscSender *insender, std::string inaddress,std::vector<std::string> instrings, bool print = false);
std::vector<std::string> OscReceive(ofxOscReceiver *inreceiver , std::string inaddress, int indexlimit);
std::vector<float> OscReceive2(ofxOscReceiver *inreceiver , std::string inaddress, int indexlimit);
std::vector<std::string> OscReceive3(ofxOscReceiver *inreceiver , std::string inaddress);
void receiveOSC(ofxOscReceiver* receiver);
