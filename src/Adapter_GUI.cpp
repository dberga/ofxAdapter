
#include "Adapter_GUI.h"


Adapter_GUI::Adapter_GUI(bool visible, bool plots){
	show = visible;
	showplots = plots;
}

Adapter_GUI::~Adapter_GUI(){

}



void Adapter_GUI::Setup( Adapter *inadapter){
	adapter = inadapter;
	setupguielements();
	setupguipanels();

	sendstarted = false;
	sendstopped = false;

	gui0->loadSettings("AdaptiveGUI.xml");


	s_start->setValue(false);
	s_stop->setValue(false);
	a_load->setValue(false);
}

void Adapter_GUI::setupguielements(){


	///panels
	gui0 = new ofxUISuperCanvas("PANEL: BASE");

	gui1 = std::vector<ofxUISuperCanvas*>(adapter->getAgentsNum());
	for(int i=0; i < adapter->getAgentsNum(); i++){
		gui1[i] = new ofxUISuperCanvas("PANEL: PLAYER "+std::to_string(i));
	}

}


void Adapter_GUI::setupguipanels(){
		setupguipanels_base();
		setupguipanels_individual();
		setupguipanels_global();
}

void Adapter_GUI::setupguipanels_base(){
	
	
	//objects/widgets
	framerate = gui0->addTextArea("fps","fps");
	advice = gui0->addTextArea("advice"," ");
	gui0->addTextArea(" "," ");
	gui0->addSpacer();
	label_amanager = gui0->addLabel("label_amanager","Player Manager");
	gui0->addSpacer();

	a_name = std::vector<ofxUITextInput*>(adapter->getAgentsNum());
	a_age = std::vector<ofxUITextInput*>(adapter->getAgentsNum());
	a_tHR = std::vector<ofxUITextInput*>(adapter->getAgentsNum());
	a_tAPA = std::vector<ofxUITextInput*>(adapter->getAgentsNum());
	a_sIT = std::vector<ofxUITextInput*>(adapter->getAgentsNum());

	gui0->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	for(int i=0; i< adapter->getAgentsNum(); i++){
		gui0->addLabel("a_panel"+std::to_string(i),"Player "+std::to_string(i));
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		a_name[i] =gui0->addTextInput("a_name"+std::to_string(i),"Name",100.0f);
		a_age[i] =gui0->addTextInput("a_age"+std::to_string(i),"Age",40.0f);
		a_tHR[i] =gui0->addTextInput("a_tHR"+std::to_string(i),"tHR",50.0f);
		a_tAPA[i] =gui0->addTextInput("a_tAPA"+std::to_string(i),"tAPA",50.0f);
		a_sIT[i] =gui0->addTextInput("a_sIT"+std::to_string(i),"sIT",50.0f);
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	}
	gui0->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

	gui0->addSpacer();
	a_load =gui0->addLabelToggle("LOAD BL_Users File",false);

	gui0->addSpacer();
	label_algmanager = gui0->addLabel("label_bmanager","Algorithm Manager");
	gui0->addSpacer();

	std::vector<std::string> ma_algorithms = std::vector<std::string>();
	ma_algorithms.push_back("EMA");
	ma_algorithms.push_back("AMA");

	 	alg_algorithm=gui0->addRadio("alg_algorithm",ma_algorithms,OFX_UI_ORIENTATION_HORIZONTAL);
		alg_algorithm->activateToggle("EMA");
	
	for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){
		alg_params.push_back(new ofxUISlider());
		alg_params[a]=gui0->addSlider(adapter->getAlgorithmParameters()[a].name,adapter->getAlgorithmParameters()[a].limits.min,adapter->getAlgorithmParameters()[a].limits.max,adapter->getAlgorithmParameters()[a].current_value);
	}

	 gui0->addSpacer();
	label_smanager = gui0->addLabel("label_smanager","System Manager");
	gui0->addSpacer();

	std::vector<string> players;
	for(int i=0; i< adapter->getAgentsNum(); i++)
		players.push_back("P"+std::to_string(i)+" ");

	s_player = gui0->addRadio("s_player",players,OFX_UI_ORIENTATION_HORIZONTAL);

	s_start =gui0->addLabelToggle("START",false);

	s_stop =gui0->addLabelToggle("STOP",false);


	gui0->setPosition(0, 0);
	gui0->autoSizeToFitWidgets();

}


void Adapter_GUI::setupguipanels_individual(){
	

	p_manualedit = std::vector<ofxUIToggle*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			p_manualedit.push_back(new ofxUIToggle());

	label_pmpmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			label_pmpmanager.push_back(new ofxUILabel());

	pmp_params = std::vector<std::vector<ofxUISlider*>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pmp_params.push_back(std::vector<ofxUISlider*>());
			for(int j=0; j<adapter->getModelParameters()[i].size(); j++)
				pmp_params[i].push_back(new ofxUISlider());	
		}
	pmp_tparams = std::vector<std::vector<ofxUISlider*>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pmp_tparams.push_back(std::vector<ofxUISlider*>());
			for(int j=0; j<adapter->getModelParameters()[i].size(); j++)
				pmp_tparams[i].push_back(new ofxUISlider());	
		}

	pmp_plots = std::vector<std::vector<ofxHistoryPlot>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pmp_plots.push_back(std::vector<ofxHistoryPlot>());
			for(int j=0; j<adapter->getModelParameters()[i].size(); j++){
				pmp_plots[i].push_back(ofxHistoryPlot(&adapter->getref_model_parameter(i,j)->current_value,adapter->getModelParameters()[i][j].name,PLOTSMEMORY,true));

			}
		}
	pmp_plots_spacers = std::vector<std::vector<ofxUISpacer*>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pmp_plots_spacers.push_back(std::vector<ofxUISpacer*>());
			for(int j=0; j<adapter->getModelParameters()[i].size(); j++){
				pmp_plots_spacers[i].push_back(new ofxUISpacer(PLOTSWIDTH,PLOTSHEIGHT));
				
			}
		}
		
	label_pcpmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			label_pcpmanager.push_back(new ofxUILabel());

	pcp_params = std::vector<std::vector<ofxUISlider*>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pcp_params.push_back(std::vector<ofxUISlider*>());
			for(int j=0; j<adapter->getControlParameters()[i].size(); j++)
				pcp_params[i].push_back(new ofxUISlider());	
		}

	pcp_plots = std::vector<std::vector<ofxHistoryPlot>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pcp_plots.push_back(std::vector<ofxHistoryPlot>());
			for(int j=0; j<adapter->getControlParameters()[i].size(); j++){
				pcp_plots[i].push_back(ofxHistoryPlot(&adapter->getref_control_parameter(i,j)->current_value,adapter->getControlParameters()[i][j].name,PLOTSMEMORY,true));
				
			}
		}
	pcp_plots_spacers = std::vector<std::vector<ofxUISpacer*>>();
		for(int i=0;i<adapter->getAgentsNum(); i++){
			pcp_plots_spacers.push_back(std::vector<ofxUISpacer*>());
			for(int j=0; j<adapter->getControlParameters()[i].size(); j++){
				pcp_plots_spacers[i].push_back(new ofxUISpacer(PLOTSWIDTH,PLOTSHEIGHT));
				
			}
		}
	label_pcmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			label_pcmanager.push_back(new ofxUILabel());
	p_record = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			p_record.push_back(new ofxUILabelToggle("p_record",false));
	p_inrecord = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			p_inrecord.push_back(new ofxUILabelToggle("p_inrecord",false));
	p_calibrate = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<adapter->getAgentsNum(); i++)
			p_calibrate.push_back(new ofxUILabelToggle("p_calibrate",false));

	for(int i=0; i<adapter->getAgentsNum(); i++){
		
		gui1[i]->addLabel("desc"+std::to_string(i),"Description for Player"+std::to_string(i));
		gui1[i]->addSpacer();

		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		gui1[i]->addLabel("p_name"+std::to_string(i),"Name: ");
		
		gui1[i]->addLabel("p_name_val"+std::to_string(i),a_name[i]->getTextString());
		gui1[i]->addLabel("p_age"+std::to_string(i),"Age: ");
		gui1[i]->addLabel("p_age_val"+std::to_string(i),a_age[i]->getTextString());

		gui1[i]->addLabel("p_tHR"+std::to_string(i),"targetHR: ");

		gui1[i]->addLabel("p_tHR_val"+std::to_string(i),a_tHR[i]->getTextString());
		gui1[i]->addLabel("p_tAPA"+std::to_string(i),"targetAPA: ");

		gui1[i]->addLabel("p_tAPA_val"+std::to_string(i),a_tAPA[i]->getTextString());
		gui1[i]->addLabel("p_sIT"+std::to_string(i),"startingIT: ");

		gui1[i]->addLabel("p_sIT_val"+std::to_string(i),a_sIT[i]->getTextString());
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		

		p_manualedit[i] = gui1[i]->addToggle("Edit tvalues manually p"+std::to_string(i),false);
		gui1[i]->addSpacer();
		label_pmpmanager[i] = gui1[i]->addLabel("label_pmpmanager"+std::to_string(i),"Model Parameters for Player "+std::to_string(i));
		gui1[i]->addSpacer();

		for(int j=0; j<adapter->getModelParameters()[i].size(); j++){
			pmp_tparams[i][j] = gui1[i]->addSlider("target_"+adapter->getModelParameters()[i][j].name,adapter->getModelParameters()[i][j].limits.min,adapter->getModelParameters()[i][j].limits.max, adapter->getModelParameters()[i][j].target_value);
			pmp_params[i][j] = gui1[i]->addSlider(adapter->getModelParameters()[i][j].name,adapter->getModelParameters()[i][j].limits.min,adapter->getModelParameters()[i][j].limits.max, adapter->getModelParameters()[i][j].current_value);
			pmp_plots_spacers[i][j] =  gui1[i]->addSpacer(PLOTSWIDTH,PLOTSHEIGHT,"pmp_plots_spacers"+std::to_string(i)+std::to_string(j));
		}
		
		label_pcpmanager[i] = gui1[i]->addLabel("label_pcpmanager"+std::to_string(i),"Control Parameters for Player "+std::to_string(i));
		gui1[i]->addSpacer();

		for(int j=0; j<adapter->getControlParameters()[i].size(); j++){
			pcp_params[i][j] = gui1[i]->addSlider(adapter->getControlParameters()[i][j].name,adapter->getControlParameters()[i][j].limits.min,adapter->getControlParameters()[i][j].limits.max, adapter->getControlParameters()[i][j].current_value);
			pcp_plots_spacers[i][j] =  gui1[i]->addSpacer(PLOTSWIDTH,PLOTSHEIGHT,"pcp_plots_spacers"+std::to_string(i)+std::to_string(j));
		}
		
		gui1[i]->addSpacer();
		label_pcmanager[i] = gui1[i]->addLabel("label_pcmanager"+std::to_string(i),"System Control for Player "+std::to_string(i));
		gui1[i]->addSpacer();

		
		p_calibrate[i]= gui1[i]->addLabelToggle("Load HR from player "+std::to_string(i),false);
		p_inrecord[i]= gui1[i]->addLabelToggle("Import data from player "+std::to_string(i),false);
		p_record[i]= gui1[i]->addLabelToggle("Export data from player "+std::to_string(i),false);

		gui1[i]->setPosition(gui0->getRect()->getWidth()+2+PLOTSWIDTH*(i%2), 0);
		gui1[i]->autoSizeToFitWidgets();

	}
}

void Adapter_GUI::setupguipanels_global(){
	

}


	



void Adapter_GUI::draw(){

	framerate->setTextString("fps="+std::to_string(ofGetFrameRate()));
	//ofBackgroundGradient(ofColor::white, ofColor::gray);

	//drawar sliders segun toggle

	if(show==true){
		gui0->setVisible(true);

		//if(adapter->running==true){
			for(int i=0; i< adapter->getAgentsNum(); i++){
				if("P"+std::to_string(i)+" " != s_player->getActiveName()){
					gui1[i]->setVisible(false);
				}else{
					gui1[i]->setVisible(true);
					for(int j=0; j<pmp_plots_spacers[i].size();j++){
						pmp_plots_spacers[i][j]->setDrawFill(false);
						if(pmp_plots[i][j].getValues().size() > PLOTSMEMORY)
							pmp_plots[i][j].reset();
						pmp_plots[i][j].draw((int)pmp_plots_spacers[i][j]->getRect()->getX(),(int)pmp_plots_spacers[i][j]->getRect()->getY(),PLOTSWIDTH,PLOTSHEIGHT);
					}
					for(int j=0; j<pcp_plots_spacers[i].size();j++){
						pcp_plots_spacers[i][j]->setDrawFill(false);
						if(pcp_plots[i][j].getValues().size() > PLOTSMEMORY)
							pcp_plots[i][j].reset();
						pcp_plots[i][j].draw((int)pcp_plots_spacers[i][j]->getRect()->getX(),(int)pcp_plots_spacers[i][j]->getRect()->getY(),PLOTSWIDTH,PLOTSHEIGHT);
					}
						
						//
				}
			}
		//}else{
		//	for(int i=0; i< adapter->getAgentsNum(); i++){
		//		gui1[i]->setVisible(false);
		//	}
		//}

	}else{

		gui0->setVisible(false);
		for(int i=0; i< adapter->getAgentsNum(); i++){
			gui1[i]->setVisible(false);
		}
	}
		
		//std::cout << "frameRate=" << ofGetFrameRate() << std::endl;
	
	
}

void Adapter_GUI::update(){

		updateOrders();

		updateModelParameters();

		updateControlParameters();

		updateGlobalControlParameters();
		
		updateAlgorithmParameters();
}





void Adapter_GUI::updateOrders(){

	if(s_start->getValue()==true) //press start
	{
		start();
	}

	if(s_stop->getValue()==true){ //press stop
		stop();
	}

	if(a_load->getValue()==true){ //press load players
		dataloadusers();
	}

	//press save recordings
	for(int i=0; i<adapter->getAgentsNum(); i++){
		if(p_record[i]->getValue()==true){
			
			dataexport(i);
		
		}
	}
	//press import data
	for(int i=0; i<adapter->getAgentsNum(); i++){
		if(p_inrecord[i]->getValue()==true){
			

			dataimport(i);

			
		}
	}

	for(int i=0; i<adapter->getAgentsNum(); i++){ //todo too specific
		if(p_calibrate[i]->getValue()==true){

			dataloadhr(i);
		}
	}

}


void Adapter_GUI::updateModelParameters(){
	if(adapter->running){

		//parameter manual change (from gui to system) or automatic (from system to gui)
		for(int i=0; i<adapter->getAgentsNum(); i++){
			if(p_manualedit[i]->getValue()==true){
				for(int j=0; j<adapter->getModelParameters()[i].size(); j++){
					//adapter->update_model_parameter(i,j,pmp_params[i][j]->getValue()); //solo si el valor es diferente al back
					adapter->set_tmodel_parameter(i,j,pmp_tparams[i][j]->getValue());

					pmp_params[i][j]->setValue(adapter->getModelParameters()[i][j].current_value);
				}
			}else{
				for(int j=0; j<adapter->getModelParameters()[i].size(); j++){
					pmp_params[i][j]->setValue(adapter->getModelParameters()[i][j].current_value);
					pmp_tparams[i][j]->setValue(adapter->getModelParameters()[i][j].target_value);
				}
			}
		}
	}
}

void Adapter_GUI::updateControlParameters(){
	if(adapter->running){



		//parameter manual change (from gui to system) or automatic (from system to gui)
		for(int i=0; i<adapter->getAgentsNum(); i++){
			if(p_manualedit[i]->getValue()==true){
				for(int j=0; j<adapter->getControlParameters()[i].size(); j++){
					//adapter->update_control_parameter(i,j,pcp_params[i][j]->getValue()); //solo si el valor es diferente al back

					pcp_params[i][j]->setValue(adapter->getControlParameters()[i][j].current_value);
				}
			}else{
				for(int j=0; j<adapter->getControlParameters()[i].size(); j++){
					pcp_params[i][j]->setValue(adapter->getControlParameters()[i][j].current_value);
				}
			}
		}
	}

}
void Adapter_GUI::updateGlobalControlParameters(){
	//global control
	if(adapter->running){
	}
}

void Adapter_GUI::updateAlgorithmParameters(){
	
	//if(adapter->running == false){

		std::string activealgorithm = alg_algorithm->getActiveName();
		adapter->setAlgorithm(activealgorithm);
		
		if(activealgorithm=="EMA"){
			for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){
				if(alg_params[a]->getName() == "EMA_Smoothing")
					alg_params[a]->setVisible(true);
				if(alg_params[a]->getName() == "AMA_Period")
					alg_params[a]->setVisible(false);
				if(alg_params[a]->getName() == "AMA_Smoothing_Fast")
					alg_params[a]->setVisible(false);
				if(alg_params[a]->getName() == "AMA_Smoothing_Slow")
					alg_params[a]->setVisible(false);
			}
		}else if(activealgorithm=="AMA"){
			for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){
				if(alg_params[a]->getName() == "EMA_Smoothing")
					alg_params[a]->setVisible(false);
				if(alg_params[a]->getName() == "AMA_Period")
					alg_params[a]->setVisible(true);
				if(alg_params[a]->getName() == "AMA_Smoothing_Fast")
					alg_params[a]->setVisible(true);
				if(alg_params[a]->getName() == "AMA_Smoothing_Slow")
					alg_params[a]->setVisible(true);
			}
		}
		Value period;
		Value period_slow;
		Value period_fast;
		for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){
			if(alg_params[a]->getName() == "AMA_Period")
				period = alg_params[a]->getValue();
			if(alg_params[a]->getName() == "AMA_Smoothing_Fast")
				period_fast = alg_params[a]->getValue();
			if(alg_params[a]->getName() == "AMA_Smoothing_Slow")
				period_slow = alg_params[a]->getValue();
		}
		if(period_fast > period_slow - 2)
			period_fast = period_slow - 2;

		for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){ //ama fast cannot be < ama slow -1
			if(alg_params[a]->getName() == "AMA_Period")
				alg_params[a]->setValue(period);
			if(alg_params[a]->getName() == "AMA_Smoothing_Fast")
				alg_params[a]->setValue(period_fast);
			if(alg_params[a]->getName() == "AMA_Smoothing_Slow")
				alg_params[a]->setValue(period_slow);
		}


		for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){ //set to int values
			alg_params[a]->setValue((int)alg_params[a]->getValue());
		}

		for(int a=0; a<adapter->getAlgorithmParameters().size(); a++)
			adapter->set_algorithm_parameter(a,alg_params[a]->getValue());
		

		
	//}else{
		//alg_algorithm->activateToggle(adapter->getAlgorithm());

		//for(int a=0; a<adapter->getAlgorithmParameters().size(); a++)
		//	alg_params[a]->setValue(adapter->getAlgorithmParameters()[a].current_value);
	//}
}


void Adapter_GUI::alert(std::string message, ofxUIColor incolor = ofxUIColor(255,255,255)){
	advice->setTextString(message);
	advice->setColorFill(incolor);

}




void Adapter_GUI::dataexport(int player){
	


			std::vector<std::vector<std::string>> usercsv = paramsToData(adapter->getModelParameters()[player],adapter->getControlParameters()[player]);
			//std::vector<std::vector<std::string>> usercsvcut = paramsToData2(adapter->getModelParameters()[player],adapter->getControlParameters()[player], 0.3334f);
			//usercsv = mixData(usercsv,usercsvcut);


			ofFileDialogResult savedialog = ofSystemSaveDialog("data_AS_"+adapter->getAgents()[player].name+".csv", "Export csv file");
			writeCSV(";",usercsv,savedialog.getPath());

			alert("exported "+savedialog.getName(),ofxUIColor(0,255,0));


			p_record[player]->setValue(false);
}
void Adapter_GUI::dataloadhr(int player){
	std::vector<std::vector<std::string>> csv_suunto = loadManualCSV(";","Select Suunto csv file");
			SuuntoLogData sdata = parseSuuntoLogData(csv_suunto);

			if(sdata.LogID != -1){
				int hrit= locate_mparameter("HR",adapter->getModelParameters()[player]);
				adapter->set_mparam("HR",player,transform_HR(sdata.Tvalues,adapter->getModelParameters()[player][hrit]));

				//all vectors the same size
				//adapter->adjustParameters(i);

				//all vectors the same time
				int apait= locate_mparameter("APA",adapter->getModelParameters()[player]);
				Threshold minmaxts = get_minmax_timestamp(adapter->getModelParameters()[player][apait].values_time); 
				adapter->adjustTime(minmaxts.min,minmaxts.max,player);
				adapter->adjustParameters(player);

				//backup the new data
				std::vector<std::vector<std::string>> usercsv = paramsToData(adapter->getModelParameters()[player],adapter->getControlParameters()[player]);
				writeCSV(";",usercsv,"data_AS_"+adapter->getAgents()[player].name+".csv");


				//std::cout << "loaded the HR file " << std::endl;
				alert("loaded the HR file",ofxUIColor(0,255,0));
			}else{
				alert("wrong HR file",ofxUIColor(0,255,0));
			}
			p_calibrate[player]->setValue(false);
}
void Adapter_GUI::stop(){
		//stop the system

		sendstopped = true;
		adapter->stop();
		s_start->setValue(false);

		//for each player, write the csv
		for(int i=0; i<adapter->getAgentsNum(); i++){
		
			
				//all vectors the same size
				adapter->adjustParameters(i);

				std::vector<std::vector<std::string>> usercsv = paramsToData(adapter->getModelParameters()[i],adapter->getControlParameters()[i]);
				writeCSV(";",usercsv,"data_AS_"+std::to_string(adapter->getAgents()[i].id)+"_"+adapter->getAgents()[i].name+".csv");
				//std::cout << "written a " <<"data_AS_"+std::to_string(adapter->getAgents()[i].id)+"_"+adapter->getAgents()[i].name+".csv" << " file" << std::endl;
				alert("written a "+std::to_string(adapter->getAgents()[i].id)+"_"+adapter->getAgents()[i].name+".csv",ofxUIColor(0,255,0));
		}
		alert("stopped",ofxUIColor(0,0,255));
		s_stop->setValue(false);
}
void Adapter_GUI::start(){
	adapter->cleanValues();
		//set algorithm values to the adapter (todo)
		

		
		//load profiles
		std::vector<Profile> users = std::vector<Profile>();
		for(int i=0; i<adapter->getAgentsNum(); i++) //todo: follow a schema instead of concrete profiles
		{


			//set agents to the adapter
			Profile user;
			user.id = i;
			user.name = a_name[i]->getTextString();
			if(is_number(a_age[i]->getTextString()))
				user.age = std::stoi( a_age[i]->getTextString());
			else
				user.age = 1;
			
				user.tHR = std::stof( a_tHR[i]->getTextString());

				user.tAPA = std::stof( a_tAPA[i]->getTextString());

				user.tIT = std::stof( a_sIT[i]->getTextString());
			
			users.push_back(user);


			//set agents to their labels
			ofxUILabel* temp;
			temp = (ofxUILabel*) gui1[i]->getWidget("p_name_val"+std::to_string(i));
			temp->setLabel(user.name);
			temp = (ofxUILabel*) gui1[i]->getWidget("p_age_val"+std::to_string(i));
			temp->setLabel(std::to_string(user.age));
			temp = (ofxUILabel*) gui1[i]->getWidget("p_tHR_val"+std::to_string(i));
			temp->setLabel(std::to_string(user.tHR));
			temp = (ofxUILabel*) gui1[i]->getWidget("p_tAPA_val"+std::to_string(i));
			temp->setLabel(std::to_string(user.tAPA));
			temp = (ofxUILabel*) gui1[i]->getWidget("p_sIT_val"+std::to_string(i));
			temp->setLabel(std::to_string(user.tIT));

			s_player->activateToggle("P"+std::to_string(i)+" ");


			//update the model parameters with the target values
			adapter->set_tmodel_parameter("APA",i,user.tAPA);
			adapter->set_tmodel_parameter("HR",i,user.tHR);
			adapter->set_control_parameter("IT",i,user.tIT);

		}
		adapter->setAgents(users);

		sendstarted = true;


		if(alg_algorithm->getActiveName() != "EMA" && alg_algorithm->getActiveName() != "AMA")
			alg_algorithm->activateToggle("EMA");


		for(int a=0; a<adapter->getAlgorithmParameters().size(); a++){
			adapter->set_algorithm_parameter(a,alg_params[a]->getValue());
		}

		//run the system
		adapter->start();

		

		alert("started",ofxUIColor(0,0,255));
		s_start->setValue(false);

		gui0->saveSettings("AdaptiveGUI.xml");
}
void Adapter_GUI::dataloadusers(){
	std::vector<std::vector<std::string>> csvusers = loadManualCSV(";","Specify the user profiles .csv file");
		std::vector<Profile> users = parseProfiles(csvusers);

		if(users.size()==a_name.size()){
			adapter->setAgents(users);

			for(int i=0; i<users.size(); i++){
				a_name[i]->setTextString(users[i].name);
				a_age[i]->setTextString(std::to_string(users[i].age));
				a_tHR[i]->setTextString(std::to_string(users[i].tHR));
				a_tAPA[i]->setTextString(std::to_string(users[i].tAPA));
				a_sIT[i]->setTextString(std::to_string(users[i].tIT));
			}
			alert("loaded users file",ofxUIColor(0,255,0));
		}else{
			alert("wrong users file",ofxUIColor(255,0,0));
		}
		a_load->setValue(false);
}


void Adapter_GUI::dataimport(int player){


	///////////////load profiles
			std::vector<Profile> users = std::vector<Profile>();
			for(int i=0; i<adapter->getAgentsNum(); i++) //todo: follow a schema instead of concrete profiles
			{
				//set agents to the AS
				Profile user;
				user.id = i;
				user.name = a_name[i]->getTextString();
				if(is_number(a_age[i]->getTextString()))
					user.age = std::stoi( a_age[i]->getTextString());
				else
					user.age = 1;
				users.push_back(user);

				//set agents to their labels
				ofxUILabel* temp;
				temp = (ofxUILabel*) gui1[i]->getWidget("p_name_val"+std::to_string(i));
				temp->setLabel(user.name);
				temp = temp = (ofxUILabel*) gui1[i]->getWidget("p_age_val"+std::to_string(i));
				temp->setLabel(std::to_string(user.age));

				//s_player->activateToggle("P"+std::to_string(i)+" ");
			}
			adapter->setAgents(users);

			///////////////LOAD DATA
			std::vector<std::vector<std::string>> loadedcsv = loadManualCSV(";","Choose csv data file");
			std::vector<MParameter> mvector = parseMData(loadedcsv,adapter->getModelParameters()[player]);
			if(mvector.size()>0){
				for(int j=0; j<adapter->getModelParameters()[player].size(); j++){
					for(int v=0; v<mvector.size(); v++){
						if(adapter->getModelParameters()[player][j].name == mvector[v].name){
							MParameter newmparam = mvector[v];
							adapter->set_mparam(mvector[v].name,player,newmparam);
						}
					}
				}

				std::vector<CParameter> cvector = parseCData(loadedcsv,adapter->getControlParameters()[player]);
				for(int j=0; j<adapter->getControlParameters()[player].size(); j++){
					for(int v=0; v<cvector.size(); v++){
						if(adapter->getControlParameters()[player][j].name == cvector[v].name){
							CParameter newcparam = cvector[v];
							adapter->set_cparam(cvector[v].name,player,newcparam);
						}
					}
				}
				//std::cout << "imported player " << i << std::endl;
				alert("imported player ",ofxUIColor(0,255,0));
			}else{
				alert("wrong data file",ofxUIColor(255,0,0));
			}

			p_inrecord[player]->setValue(false);

}
