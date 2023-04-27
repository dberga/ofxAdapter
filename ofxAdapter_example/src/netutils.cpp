
#include "netutils.h"


void OscSetup(std::string sendhost, int sendport, int listenport, ofxOscSender* sender, ofxOscReceiver* receiver){
	
	sender->setup(sendhost,sendport);
	receiver->setup(listenport);
}



void OscSend(ofxOscSender *insender, std::string inaddress,std::vector<std::string> instrings, bool print){
		ofxOscMessage message;
        message.setAddress( inaddress );
		//cout << "send adapter " << inaddress << endl;

		if(print)std::cout << "OSC:"<< "sent to "<< inaddress << std::endl;
		for(int i=0; i<instrings.size(); i++)
		{
			message.addStringArg(instrings[i]);
		if(print)std::cout <<"["<< i <<"]=" << instrings[i] << std::endl;
		}

        //message.addFloatArg( ofGetElapsedTimef() );
        insender->sendMessage( message );

}




std::vector<std::string> OscReceive(ofxOscReceiver *inreceiver , std::string inaddress, int indexlimit){

	std::vector<std::string> outstrings;

	while( inreceiver->hasWaitingMessages() )
    {
		ofxOscMessage message;
		inreceiver->getNextMessage( &message );

		if (  message.getAddress() == inaddress )
        {
			//std::cout << "OSC:"<< "received from"<< inaddress << std::endl;
			for(int i=0; i<indexlimit; i++){
				outstrings.push_back(message.getArgAsString( i ));
				//std::cout <<"["<< i <<"]=" << outstrings[i] << std::endl;
			}
		}
	}
	return outstrings;

}

std::vector<float> OscReceive2(ofxOscReceiver *inreceiver , std::string inaddress, int indexlimit){

	std::vector<float> outstrings;

	while( inreceiver->hasWaitingMessages() )
    {
		ofxOscMessage message;
		inreceiver->getNextMessage( &message );

		if (  message.getAddress() == inaddress )
        {
			//std::cout << "OSC:"<< "received from"<< inaddress << std::endl;
			for(int i=0; i<indexlimit; i++){
				outstrings.push_back(message.getArgAsFloat( i ));
				//std::cout <<"["<< i <<"]=" << outstrings[i] << std::endl;
			}
		}
	}
	return outstrings;

}

std::vector<std::string> OscReceive3(ofxOscReceiver *inreceiver , std::string inaddress){

	std::vector<std::string> outstrings;

	while( inreceiver->hasWaitingMessages() )
    {
		ofxOscMessage message;
		inreceiver->getNextMessage( &message );

		if (  message.getAddress() == inaddress )
        {
			//std::cout << "OSC:"<< "received from"<< inaddress << std::endl;
			for(int i=0; i<message.getNumArgs(); i++){
				if(i%2 == 0){
					outstrings.push_back(std::to_string(message.getArgAsInt32( i )));
					//std::cout <<"["<< i <<"]=" << outstrings[i] << std::endl;
				}else{
					outstrings.push_back(std::to_string(message.getArgAsFloat( i )));
				//	std::cout <<"["<< i <<"]=" << outstrings[i] << std::endl;
				}
			}
		}
	}
	return outstrings;

}

void receiveOSC(ofxOscReceiver* receiver){
	//cout << "update receive" << endl;
	while( receiver->hasWaitingMessages() )
    {
		ofxOscMessage message;
		receiver->getNextMessage( &message );
		cout << "get something " << message.getAddress() << endl;

		if (message.getAddress() == "/game/mvt")
        {
			std::cout << "OSC:"<< "received player " << message.getArgAsInt32(0) << " activity " << message.getArgAsFloat(1) << " and player b " << message.getArgAsInt32(2) << " and player b " << message.getArgAsFloat(3) << std::endl;

		}
	}
}

