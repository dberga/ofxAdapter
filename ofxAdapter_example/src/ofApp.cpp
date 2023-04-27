
#include "ofApp.h"

//-------------

void ofApp::setup(){


	start = std::clock();
	player_flags_sendedduration = std::vector<float>();
	player_flags_duration = std::vector<float>();
	randtype = 1;

	//define parameters

		// parameters csv parser
		std::vector<std::vector<std::string>> csvparams= loadCSV(";",ofToDataPath("parameters.csv"));

		global_control_parameters = parseGCParams(csvparams);
		correlations = parseCorrelations(csvparams);

		control_parameters = parsegen_CParams(csvparams);
		model_parameters = parsegen_MParams(csvparams);

		
		for(int i=0; i<model_parameters.size(); i++){ //model_parameters.size() is the number of players
			player_flags_sendedduration.push_back(0.0f);
			player_flags_duration.push_back(0.0f);
		}



	///////////////

	sender = new ofxOscSender();
	receiver = new ofxOscReceiver();
	OscSetup(SENDHOST,SENDPORT,LISTENPORT, sender, receiver);
	
		std::cout <<"MAIN: sender on "<< SENDHOST<< ":"<<SENDPORT << std::endl;
		std::cout <<"MAIN: receiver on "<< "localhost"<< ":"<<LISTENPORT << std::endl;
		

		//std::string controltype = ofSystemTextBoxDialog("1=calibrator, 2=adapter","");
		//controllertype = std::stoi(controltype); //todo: gui

		//std::string receivtype = ofSystemTextBoxDialog("1=random, 2=osc","");
		//receivetype = std::stoi(receivtype); //todo: gui

		

	
		
		 
	////////////////////////////
	
		baseline_system = new BaselineCalibrator();
		baseline_system->Setup(
			model_parameters,
			control_parameters,
			global_control_parameters
		);

		bgui = new BaselineCalibrator_GUI(false,true);
		bgui->Setup(baseline_system);

		adaptive_system = new Adapter();
		adaptive_system->Setup(
			model_parameters,
			control_parameters,
			global_control_parameters,
			correlations,
			"EMA"
			);

		agui = new Adapter_GUI(false,true);
		agui->Setup(adaptive_system);


		ggui = new Generic_GUI();
		ggui->Setup(bgui,agui);

		setupmod=false;
	/////////////////
	
		ofSetFrameRate(FPS);

		//for each player we have the sample elapsed millis for mvt limit
		for(int i=0; i<model_parameters.size(); i++)
			previousmillis.push_back(0);
		for(int i=0; i<model_parameters.size(); i++)
			currentmillis.push_back(0);

		//for each player we have the samples received from mvt
		for(int i=0; i<model_parameters.size(); i++)
			sample_mvt.push_back(std::vector<Value>());

}

//--------------------------------------------------------------
void ofApp::update(){


	//update timer
	for(int i=0; i<adaptive_system->getAgentsNum(); i++){

		float currentduration = (( std::clock() - start ) / (float) CLOCKS_PER_SEC);
		float elapsedduration = currentduration - player_flags_sendedduration.at(i);

		//std::cout<< "sendedduration=" << player_flags_sendedduration.at(i) <<std::endl;

		if(elapsedduration > 0){
			player_flags_duration.at(i) += elapsedduration;
			player_flags_sendedduration[i] = currentduration;

		}
		
	}


	



	ggui->update();
	
	

	if(ggui->init == true){

		if(setupmod==false){

			receivetype = ggui->getRectype();

			if(ggui->showBL)
				controllertype = BASELINE;
			if(ggui->showAS)
				controllertype = ADAPTER;

			std::vector<std::string> setupmode;
			if(controllertype==BASELINE)
				setupmode.push_back("baselinecalibrator");
			else if(controllertype==ADAPTER)
				setupmode.push_back("adapter");	
	
			OscSend(sender,"/setting/mode",setupmode,true);

			

			setupmod = true;
		}

		std::vector<std::string> ITvalues; //to osc
		std::vector<std::string> startstop; //to osc
		//receiveOSC(receiver);
		Value rand1;

		switch(controllertype){
			case ADAPTER:
			 
				if(receivetype==1){
					//1.1  (for each player) adapter update_model_parameter
					for(int i=0; i<adaptive_system->getAgentsNum(); i++){

							int jAPA = locate_mparameter("APA",adaptive_system->getModelParameters()[i]);
							
							Value minlimit = adaptive_system->getModelParameters()[i][jAPA].limits.min;
							Value maxlimit = adaptive_system->getModelParameters()[i][jAPA].limits.max;

							switch(randtype){
								case 0:
									minlimit =0;
									maxlimit =1;
									break;
								case 2:
									if(minlimit ==0) minlimit = 0.5;
									maxlimit = 0.7*maxlimit;
									minlimit = 1.5*minlimit;
									break;
								case 3:
									if(minlimit ==0) minlimit = 0.5;
									minlimit = 0.7*maxlimit;
									break;
								case 1:
									if(minlimit ==0) minlimit = 3;
									maxlimit = 5;
									break;
							}

							rand1 = RandomFloat(minlimit,maxlimit);
							Value mvt = rand1;

							sample_mvt[i].push_back(rand1);
							
							currentmillis[i] = ofGetElapsedTimeMillis();

							if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
								mvt=vector_avg(sample_mvt[i]);
								adaptive_system->update_model_parameter("APA",i,mvt); //name,agent,values
								sample_mvt[i].clear();
								previousmillis[i] = currentmillis[i];
							}

					}
				}
				if(receivetype==2){
					std::vector<std::string> movement = OscReceive3(receiver,"/game/mvt");
					if(movement.size() > 0){
						for(int i=0; i<adaptive_system->getAgentsNum() ; i++){
							float mvt = std::stof(movement[i*2+1]);
							int j = locate_mparameter("APA",adaptive_system->getModelParameters()[i]);

							if(mvt <= 0 && ggui->decay==true){
								
								mvt = adaptive_system->getModelParameters()[i][j].current_value*ggui->getDecay();


								sample_mvt[i].push_back(mvt);
							
								currentmillis[i] = ofGetElapsedTimeMillis();

								if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
									mvt=vector_avg(sample_mvt[i]);
									adaptive_system->update_model_parameter("APA",i,mvt); //name,agent,value
									sample_mvt[i].clear();
									previousmillis[i] = currentmillis[i];
								}


							}else{
								
								
								mvt = ofMap(mvt,0,ggui->mvtscale,adaptive_system->getModelParameters()[i][j].limits.min,adaptive_system->getModelParameters()[i][j].limits.max);

								sample_mvt[i].push_back(mvt);
							
								currentmillis[i] = ofGetElapsedTimeMillis();

								if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
									mvt=vector_avg(sample_mvt[i]);
									adaptive_system->update_model_parameter("APA",i,mvt); //name,agent,value
									sample_mvt[i].clear();
									previousmillis[i] = currentmillis[i];
								}

							}
							//std::cout << "received movement " << mvt << std::endl;	
						}
					}
				}
			
				adaptive_system->update();
				agui->update();
				

					

					if(agui->sendstarted==true){
						startstop.push_back("true");
						OscSend(sender,"/session/start",startstop,true);
						agui->sendstarted=false;

						for(int i=0; i<adaptive_system->getAgentsNum(); i++){
							ITvalues.push_back(std::to_string(adaptive_system->get_control_parameter("IT",i)));
						}
						for(int i=0; i<adaptive_system->getAgentsNum(); i++){
							std::vector<std::string> sending;
							sending.push_back(ITvalues[i]);
							sending.push_back(std::to_string(i));
							OscSend(sender,"/setting/tempo",sending,true);
							adaptive_system->set_prevcontrol_parameter("IT",i,adaptive_system->get_control_parameter("IT",i));
							player_flags_duration[i] = 0.0f;
						}
						ITvalues.clear();

					}else if(agui->sendstopped==true){
						startstop.push_back("true");
						OscSend(sender,"/session/stop",startstop,true);
						agui->sendstopped=false;
					}
					
				

				
				//send the new values [when changed, it has flag 1, so send new vals]
				if(adaptive_system->running && adaptive_system->flag == 2){ //if flag==1
					
					for(int i=0; i<adaptive_system->getAgentsNum(); i++){
						
						ITvalues.push_back(std::to_string(adaptive_system->get_control_parameter("IT",i)));

					}
			
					for(int i=0; i<ITvalues.size(); i++){
						
						//std::cout << "elapsed duration " << i << ":"<<player_flags_duration.at(i) << ",IT=" << adaptive_system->get_prevcontrol_parameter("IT",i) <<std::endl;

						if(player_flags_duration.at(i) >= adaptive_system->get_prevcontrol_parameter("IT",i)){ //send after elapsed IT
							std::vector<std::string> sending;
							sending.push_back(ITvalues[i]);
							sending.push_back(std::to_string(i));
							OscSend(sender,"/setting/tempo",sending,true);

							adaptive_system->set_prevcontrol_parameter("IT",i,adaptive_system->get_control_parameter("IT",i));
							player_flags_duration[i] = 0.0f;
						}else{
							//don't send
						}
					}
					adaptive_system->flag = 0;
				}

			

				break;
			case BASELINE:

			
				if(receivetype==1){
					//1.1  (for each player) adapter update_model_parameter
					for(int i=0; i<baseline_system->getAgentsNum(); i++){
						
							int jAPA = locate_mparameter("APA",baseline_system->getModelParameters()[i]);
							
							Value minlimit = baseline_system->getModelParameters()[i][jAPA].limits.min;
							Value maxlimit = baseline_system->getModelParameters()[i][jAPA].limits.max;

							switch(randtype){
								case 0:
									minlimit =0;
									maxlimit =1;
									break;
								case 2:
									if(minlimit ==0) minlimit = 0.5;
									maxlimit = 0.7*maxlimit;
									minlimit = 1.5*minlimit;
									break;
								case 3:
									if(minlimit ==0) minlimit = 0.5;
									minlimit = 0.7*maxlimit;
									break;
								case 1:
									if(minlimit ==0) minlimit = 3;
									maxlimit = 5;
									break;
							}

							rand1 = RandomFloat(minlimit,maxlimit);
							Value mvt = rand1;

							sample_mvt[i].push_back(rand1);
							
							currentmillis[i] = ofGetElapsedTimeMillis();

							if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
								mvt=vector_avg(sample_mvt[i]);
								baseline_system->update_model_parameter("APA",i,mvt); //name,agent,vlue
								sample_mvt[i].clear();
								previousmillis[i] = currentmillis[i];
							}

						
					}
				}
				if(receivetype==2){
					std::vector<std::string> movement = OscReceive3(receiver,"/game/mvt");
					if(movement.size() > 0){
						for(int i=0; i<baseline_system->getAgentsNum() ; i++){
							float mvt = std::stof(movement[i*2+1]);
							int j = locate_mparameter("APA",baseline_system->getModelParameters()[i]);




							if(mvt <= 0 && ggui->decay==true){
								
								mvt = baseline_system->getModelParameters()[i][j].current_value*ggui->getDecay();

								sample_mvt[i].push_back(mvt);
							
								currentmillis[i] = ofGetElapsedTimeMillis();

								if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
									mvt=vector_avg(sample_mvt[i]);
									baseline_system->update_model_parameter("APA",i,mvt); //name,agent,value
									sample_mvt[i].clear();
									previousmillis[i] = currentmillis[i];
								}

							}else{
								mvt = ofMap(mvt,0,ggui->mvtscale,baseline_system->getModelParameters()[i][j].limits.min,baseline_system->getModelParameters()[i][j].limits.max);
								
								sample_mvt[i].push_back(mvt);
							
								currentmillis[i] = ofGetElapsedTimeMillis();

								

								if(currentmillis[i] - previousmillis[i] > MAXMILLIS){
									mvt=vector_avg(sample_mvt[i]);
									baseline_system->update_model_parameter("APA",i,mvt); //name,agent,value
									sample_mvt[i].clear();

									previousmillis[i] = currentmillis[i];
								}
							
							}	
							
								
						}
					}
				}
			
			
				baseline_system->update();
				bgui->update();
				

					if(bgui->sendstarted==true){
						startstop.push_back("true");
						startstop.push_back(std::to_string(baseline_system->getSession()));
						OscSend(sender,"/session/start",startstop,true);
						bgui->sendstarted=false;

						for(int i=0; i<baseline_system->getAgentsNum(); i++){
							ITvalues.push_back(std::to_string(baseline_system->get_control_parameter("IT",i)));
						}
						for(int i=0; i<baseline_system->getAgentsNum(); i++){
							std::vector<std::string> sending;
							sending.push_back(ITvalues[i]);
							sending.push_back(std::to_string(i));
							OscSend(sender,"/setting/tempo",sending,true);
							baseline_system->set_prevcontrol_parameter("IT",i,baseline_system->get_control_parameter("IT",i));
							player_flags_duration[i] = 0.0f;
							baseline_system->flag = 0;

						}
						ITvalues.clear();

						

					}else if(bgui->sendstopped==true){
						startstop.push_back("true");
						OscSend(sender,"/session/stop",startstop,true);
						bgui->sendstopped=false;
					}


				
				if(baseline_system->running && baseline_system->flag == 2){ //if flag==1
					for(int i=0; i<baseline_system->getAgentsNum(); i++){

						ITvalues.push_back(std::to_string(baseline_system->get_control_parameter("IT",i)));

						//std::cout << i <<",previt="<<baseline_system->get_prevcontrol_parameter("IT",i) << ",it=" << adaptive_system->get_prevcontrol_parameter("IT",i)  << std::endl;
					}

					

					for(int i=0; i<ITvalues.size(); i++){
						
						//std::cout << "elapsed duration " << i << ":"<<player_flags_duration.at(i) << ",IT=" << atoi(ITvalues[i].c_str()) <<std::endl;
						
						//if(player_flags_duration.at(i) >= baseline_system->get_prevcontrol_parameter("IT",i)){ //send after elapsed IT
							std::vector<std::string> sending;
							sending.push_back(ITvalues[i]);
							sending.push_back(std::to_string(i));
							//cout << "IT " << ITvalues[i] << endl;
							OscSend(sender,"/setting/tempo",sending,true);

							baseline_system->set_prevcontrol_parameter("IT",i,baseline_system->get_control_parameter("IT",i));
							//player_flags_duration[i] = 0.0f;
						//}else{
							//do nothing
						//}
					}
					baseline_system->flag = 0;
				}


				break;
			default:
				break;

		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

				agui->draw();
			
				bgui->draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == '1'){
		randtype = 0;
	}
	if(key == '2'){
		randtype = 1;
	}
	if(key == '3'){
		randtype = 2;
	}
	if(key == '4'){
		randtype = 3;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
