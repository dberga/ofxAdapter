
#include "BaselineCalibrator_GUI.h"


BaselineCalibrator_GUI::BaselineCalibrator_GUI(bool visible, bool plots){
	show = visible;
	showplots = plots;
}

BaselineCalibrator_GUI::~BaselineCalibrator_GUI(){

}



void BaselineCalibrator_GUI::Setup( BaselineCalibrator *incalibrator){
	calibrator = incalibrator;
	setupguielements();
	setupguipanels();

	sendstarted = false;
	sendstopped = false;

	gui0->loadSettings("BaselineGUI.xml");

	s_start->setValue(false);
	s_stop->setValue(false);
	a_load->setValue(false);
	s_output->setValue(false);
	
}

void BaselineCalibrator_GUI::setupguielements(){


	///panels
	gui0 = new ofxUISuperCanvas("PANEL: BASE");

	gui1 = std::vector<ofxUISuperCanvas*>(calibrator->getAgentsNum());
	for(int i=0; i < calibrator->getAgentsNum(); i++){
		gui1[i] = new ofxUISuperCanvas("PANEL: PLAYER "+std::to_string(i));
	}

}


void BaselineCalibrator_GUI::setupguipanels(){
		setupguipanels_base();
		setupguipanels_individual();
		setupguipanels_global();
		
}

void BaselineCalibrator_GUI::setupguipanels_base(){
	
	
	//objects/widgets
	framerate = gui0->addTextArea("fps","fps");
	advice = gui0->addTextArea("advice"," ");
	gui0->addTextArea(" "," ");
	gui0->addSpacer();
	label_amanager = gui0->addLabel("label_amanager","Player Manager");
	gui0->addSpacer();

	a_name = std::vector<ofxUITextInput*>(calibrator->getAgentsNum());
	a_age = std::vector<ofxUITextInput*>(calibrator->getAgentsNum());


	gui0->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	for(int i=0; i< calibrator->getAgentsNum(); i++){
		gui0->addLabel("a_panel"+std::to_string(i),"Player "+std::to_string(i));
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		a_name[i] =gui0->addTextInput("a_name"+std::to_string(i),"Name",100.0f);
		a_age[i] =gui0->addTextInput("a_age"+std::to_string(i),"Age",40.0f);
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	}
	gui0->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

	gui0->addSpacer();
	a_load =gui0->addLabelToggle("LOAD Users File",false);
	gui0->addSpacer();
	s_output =gui0->addLabelToggle("GENERATE BL_Users File",false);
	gui0->addSpacer();
	s_pa_percentage = gui0->addSlider("MVPA %",0.40,0.90,0.40);

	gui0->addSpacer();
	label_bmanager = gui0->addLabel("label_bmanager","Baseline Manager");
	gui0->addSpacer();

	gui0->addLabel("Session Time (in min)");
	b_sessiontime =gui0->addTextInput("b_sessiontime","");

	gui0->addLabel("List of IT (ex. 5,6,7,8)");
	blist = gui0->addTextInput("blist","");
	b_filter_list = gui0->addToggle("Toggle IT filtering",false);

	 gui0->addSpacer();
	label_smanager = gui0->addLabel("label_smanager","System Manager");
	gui0->addSpacer();

	std::vector<string> players;
	for(int i=0; i< calibrator->getAgentsNum(); i++)
		players.push_back("P"+std::to_string(i)+" ");

	s_player = gui0->addRadio("s_player",players,OFX_UI_ORIENTATION_HORIZONTAL);

	s_start =gui0->addLabelToggle("START",false);

	s_stop =gui0->addLabelToggle("STOP",false);

	

	gui0->setPosition(0, 0);
	gui0->autoSizeToFitWidgets();


	
}


void BaselineCalibrator_GUI::setupguipanels_individual(){
	

	//p_manualedit = std::vector<ofxUIToggle*>();
		//for(int i=0;i<calibrator->getAgentsNum(); i++)
			//p_manualedit.push_back(new ofxUIToggle());

	label_pmpmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			label_pmpmanager.push_back(new ofxUILabel());

	pmp_params = std::vector<std::vector<ofxUISlider*>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pmp_params.push_back(std::vector<ofxUISlider*>());
			for(int j=0; j<calibrator->getModelParameters()[i].size(); j++)
				pmp_params[i].push_back(new ofxUISlider());	
		}
	//pmp_tparams = std::vector<std::vector<ofxUISlider*>>();
	//	for(int i=0;i<calibrator->getAgentsNum(); i++){
	//		pmp_tparams.push_back(std::vector<ofxUISlider*>());
	//		for(int j=0; j<calibrator->getModelParameters()[i].size(); j++)
	//			pmp_tparams[i].push_back(new ofxUISlider());	
	//	}

	pmp_plots = std::vector<std::vector<ofxHistoryPlot>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pmp_plots.push_back(std::vector<ofxHistoryPlot>());
			for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){
				pmp_plots[i].push_back(ofxHistoryPlot(&calibrator->getref_model_parameter(i,j)->current_value,calibrator->getModelParameters()[i][j].name,PLOTSMEMORY,true));

			}
		}
	pmp_plots_spacers = std::vector<std::vector<ofxUISpacer*>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pmp_plots_spacers.push_back(std::vector<ofxUISpacer*>());
			for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){
				pmp_plots_spacers[i].push_back(new ofxUISpacer(PLOTSWIDTH,PLOTSHEIGHT));
				
			}
		}
		
	label_pcpmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			label_pcpmanager.push_back(new ofxUILabel());

	pcp_params = std::vector<std::vector<ofxUISlider*>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pcp_params.push_back(std::vector<ofxUISlider*>());
			for(int j=0; j<calibrator->getControlParameters()[i].size(); j++)
				pcp_params[i].push_back(new ofxUISlider());	
		}

	pcp_plots = std::vector<std::vector<ofxHistoryPlot>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pcp_plots.push_back(std::vector<ofxHistoryPlot>());
			for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
				pcp_plots[i].push_back(ofxHistoryPlot(&calibrator->getref_control_parameter(i,j)->current_value,calibrator->getControlParameters()[i][j].name,PLOTSMEMORY,true));
				
			}
		}
	pcp_plots_spacers = std::vector<std::vector<ofxUISpacer*>>();
		for(int i=0;i<calibrator->getAgentsNum(); i++){
			pcp_plots_spacers.push_back(std::vector<ofxUISpacer*>());
			for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
				pcp_plots_spacers[i].push_back(new ofxUISpacer(PLOTSWIDTH,PLOTSHEIGHT));
				
			}
		}
	label_pcmanager = std::vector<ofxUILabel*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			label_pcmanager.push_back(new ofxUILabel());
	p_record = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			p_record.push_back(new ofxUILabelToggle("p_record",false));
	p_inrecord = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			p_inrecord.push_back(new ofxUILabelToggle("p_inrecord",false));
	p_calibrate = std::vector<ofxUILabelToggle*>();
		for(int i=0;i<calibrator->getAgentsNum(); i++)
			p_calibrate.push_back(new ofxUILabelToggle("p_calibrate",false));

	for(int i=0; i<calibrator->getAgentsNum(); i++){
		
		gui1[i]->addLabel("desc"+std::to_string(i),"Description for Player"+std::to_string(i));
		gui1[i]->addSpacer();

		
		gui1[i]->addLabel("p_name"+std::to_string(i),"Name: ");
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		gui1[i]->addLabel("p_name_val"+std::to_string(i),a_name[i]->getTextString());
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		gui1[i]->addLabel("p_age"+std::to_string(i),"Age: ");
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		gui1[i]->addLabel("p_age_val"+std::to_string(i),a_age[i]->getTextString());
		gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		

		//p_manualedit[i] = gui1[i]->addToggle("p_manualedit"+std::to_string(i),false);
		gui1[i]->addSpacer();
		label_pmpmanager[i] = gui1[i]->addLabel("label_pmpmanager"+std::to_string(i),"Model Parameters for Player "+std::to_string(i));
		gui1[i]->addSpacer();

		for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){
			//pmp_tparams[i][j] = gui1[i]->addSlider("target_"+calibrator->getModelParameters()[i][j].name,calibrator->getModelParameters()[i][j].limits.min,calibrator->getModelParameters()[i][j].limits.max, calibrator->getModelParameters()[i][j].target_value);
			pmp_params[i][j] = gui1[i]->addSlider(calibrator->getModelParameters()[i][j].name,calibrator->getModelParameters()[i][j].limits.min,calibrator->getModelParameters()[i][j].limits.max, calibrator->getModelParameters()[i][j].current_value);
			pmp_plots_spacers[i][j] =  gui1[i]->addSpacer(PLOTSWIDTH,PLOTSHEIGHT,"pmp_plots_spacers"+std::to_string(i)+std::to_string(j));
		}
		
		label_pcpmanager[i] = gui1[i]->addLabel("label_pcpmanager"+std::to_string(i),"Control Parameters for Player "+std::to_string(i));
		gui1[i]->addSpacer();

		for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
			pcp_params[i][j] = gui1[i]->addSlider(calibrator->getControlParameters()[i][j].name,calibrator->getControlParameters()[i][j].limits.min,calibrator->getControlParameters()[i][j].limits.max, calibrator->getControlParameters()[i][j].current_value);
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

void BaselineCalibrator_GUI::setupguipanels_global(){
	

}


	



void BaselineCalibrator_GUI::draw(){

	framerate->setTextString("fps="+std::to_string(ofGetFrameRate()));
	//ofBackgroundGradient(ofColor::white, ofColor::gray);

	//drawar sliders segun toggle

	if(show==true){
		gui0->setVisible(true);

		//if(calibrator->running==true){
			for(int i=0; i< calibrator->getAgentsNum(); i++){
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
				}
			}
		//}else{
		//	for(int i=0; i< calibrator->getAgentsNum(); i++){
		//		gui1[i]->setVisible(false);
		//	}
		//}

	}else{
		gui0->setVisible(false);
		for(int i=0; i< calibrator->getAgentsNum(); i++){
			gui1[i]->setVisible(false);
		}


	}
		
		//std::cout << "frameRate=" << ofGetFrameRate() << std::endl;
	
	
}

void BaselineCalibrator_GUI::update(){

	
		updateModelParameters();
		updateControlParameters();
		updateGlobalControlParameters();
		updateOrders();

}



void BaselineCalibrator_GUI::updateOrders(){

	if(s_start->getValue()==true) //press start
	{
		
		start();

	}

	if(s_stop->getValue()==true || calibrator->saved == true){ //press stop or saved automatically after session time
		

		stop();

		
	}

	if(a_load->getValue()==true){ //press load players
		
		dataloadusers();
		
	}

	//press export data
	for(int i=0; i<calibrator->getAgentsNum(); i++){
		if(p_record[i]->getValue()==true){

			

			dataexport(i);

			
		}

		
	}

	//press import data
	for(int i=0; i<calibrator->getAgentsNum(); i++){
		if(p_inrecord[i]->getValue()==true){
			

			dataimport(i);

			
		}
	}


	for(int i=0; i<calibrator->getAgentsNum(); i++){ //todo too specific
		if(p_calibrate[i]->getValue()==true){

			dataloadhr(i);

			
		}
	}

	if(s_output->getValue() == true){

		datagenerate();
		
	}
}


void BaselineCalibrator_GUI::updateModelParameters(){
	if(calibrator->running){

		//parameter manual change (from gui to system) or automatic (from system to gui)
		for(int i=0; i<calibrator->getAgentsNum(); i++){
			/*if(p_manualedit[i]->getValue()==true){
				for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){
					//calibrator->update_model_parameter(i,j,pmp_params[i][j]->getValue()); //solo si el valor es diferente al back
					calibrator->set_tmodel_parameter(i,j,pmp_tparams[i][j]->getValue());

					pmp_params[i][j]->setValue(calibrator->getModelParameters()[i][j].current_value);
				}
			}else{
				for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){
					pmp_params[i][j]->setValue(calibrator->getModelParameters()[i][j].current_value);
					pmp_tparams[i][j]->setValue(calibrator->getModelParameters()[i][j].target_value);
				}
			}*/

			for(int j=0; j<calibrator->getModelParameters()[i].size(); j++){

				//std::cout << calibrator->getModelParameters()[i][j].current_value <<std::endl;
				pmp_params[i][j]->setValue(calibrator->getModelParameters()[i][j].current_value);
				//pmp_tparams[i][j]->setValue(calibrator->getModelParameters()[i][j].target_value);
			}
		}
	}
}

void BaselineCalibrator_GUI::updateControlParameters(){
	if(calibrator->running){



		//parameter manual change (from gui to system) or automatic (from system to gui)
		for(int i=0; i<calibrator->getAgentsNum(); i++){
			/*if(p_manualedit[i]->getValue()==true){
				for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
					//calibrator->update_control_parameter(i,j,pcp_params[i][j]->getValue()); //solo si el valor es diferente al back 

					pcp_params[i][j]->setValue(calibrator->getControlParameters()[i][j].current_value);
				}
			}else{
				for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
					pcp_params[i][j]->setValue(calibrator->getControlParameters()[i][j].current_value);
				}
			}*/
			for(int j=0; j<calibrator->getControlParameters()[i].size(); j++){
					pcp_params[i][j]->setValue(calibrator->getControlParameters()[i][j].current_value);
			}
		}
	}

}
void BaselineCalibrator_GUI::updateGlobalControlParameters(){
	//global control
	if(calibrator->running){
	}
}

void BaselineCalibrator_GUI::alert(std::string message, ofxUIColor incolor = ofxUIColor(255,255,255)){
	
	advice->setTextString(message);
	advice->setColorFill(incolor);
}


void BaselineCalibrator_GUI::dataimport(int player){

	b_filter_list->setValue(false);

	///////////////load profiles
			std::vector<Profile> users = std::vector<Profile>();
			for(int i=0; i<calibrator->getAgentsNum(); i++) //todo: follow a schema instead of concrete profiles
			{
				//set agents to the calibrator
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
			calibrator->setAgents(users);

			///////////////LOAD DATA
			std::vector<std::vector<std::string>> loadedcsv = loadManualCSV(";","Choose csv data file");
			std::vector<MParameter> mvector = parseMData(loadedcsv,calibrator->getModelParameters()[player]);
			if(mvector.size()>0){
				for(int j=0; j<calibrator->getModelParameters()[player].size(); j++){
					for(int v=0; v<mvector.size(); v++){
						if(calibrator->getModelParameters()[player][j].name == mvector[v].name){
							MParameter newmparam = mvector[v];
							calibrator->set_mparam(mvector[v].name,player,newmparam);
						}
					}
				}

				std::vector<CParameter> cvector = parseCData(loadedcsv,calibrator->getControlParameters()[player]);
				for(int j=0; j<calibrator->getControlParameters()[player].size(); j++){
					for(int v=0; v<cvector.size(); v++){
						if(calibrator->getControlParameters()[player][j].name == cvector[v].name){
							CParameter newcparam = cvector[v];
							calibrator->set_cparam(cvector[v].name,player,newcparam);
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
void BaselineCalibrator_GUI::dataexport(int player){
			//all vectors the same size
			calibrator->adjustParameters(player);

			
			std::vector<std::vector<std::string>> usercsv = paramsToData(calibrator->getModelParameters()[player],calibrator->getControlParameters()[player]);
			//std::vector<std::vector<std::string>> usercsvcut = paramsToData2(calibrator->getModelParameters()[player],calibrator->getControlParameters()[player], 0.3334f);
			//usercsv = mixData(usercsv,usercsvcut);

			ofFileDialogResult savedialog = ofSystemSaveDialog("data_BL_"+calibrator->getAgents()[player].name+".csv", "Export csv file");
			writeCSV(";",usercsv,savedialog.getPath());

			alert("exported "+savedialog.getName(),ofxUIColor(0,255,0));

			p_record[player]->setValue(false);


}
void BaselineCalibrator_GUI::dataloadhr(int player){
			std::vector<std::vector<std::string>> csv_suunto = loadManualCSV(";","Select Suunto csv file");
			SuuntoLogData sdata = parseSuuntoLogData(csv_suunto);
			if(sdata.LogID != -1){

				int hrit= locate_mparameter("HR",calibrator->getModelParameters()[player]);
				calibrator->set_mparam("HR",player,transform_HR(sdata.Tvalues,calibrator->getModelParameters()[player][hrit]));
			
				

				//all vectors the same time
				int apait= locate_mparameter("APA",calibrator->getModelParameters()[player]);
				Threshold minmaxts = get_minmax_timestamp(calibrator->getModelParameters()[player][apait].values_time); 


				calibrator->adjustTime(minmaxts.min,minmaxts.max,player);
				

				//backup the new data
				std::vector<std::vector<std::string>> usercsv = paramsToData(calibrator->getModelParameters()[player],calibrator->getControlParameters()[player]);
				writeCSV(";",usercsv,"data_BL_"+calibrator->getAgents()[player].name+".csv");



				alert("loaded the HR file ",ofxUIColor(0,255,0));
				//std::cout << " " << std::endl;

			}else{
				alert("wrong HR file",ofxUIColor(255,0,0));
			}

			p_calibrate[player]->setValue(false);
}
void BaselineCalibrator_GUI::stop(){

	//stop the system
		sendstopped = true;
		calibrator->stop();
		s_start->setValue(false);

		 

		if(b_filter_list->getValue()){
				calibrator->setControlParameters(adjustSlideIT(calibrator->getControlParameters()));
				//calibrator->setControlParameters(adjustClampCParams(calibrator->getControlParameters()));
		}

		//for each player, write the csv
		for(int i=0; i<calibrator->getAgentsNum(); i++){

				//all vectors the same size
			calibrator->adjustParameters(i);
			
				std::vector<std::vector<std::string>> usercsv = paramsToData(calibrator->getModelParameters()[i],calibrator->getControlParameters()[i]);
				writeCSV(";",usercsv,"data_BL_"+std::to_string(calibrator->getAgents()[i].id)+"_"+calibrator->getAgents()[i].name+".csv");
				std::cout << "written a " <<"data_BL_"+std::to_string(calibrator->getAgents()[i].id)+"_"+calibrator->getAgents()[i].name+".csv" << " file" << std::endl;
			
		}
		calibrator->saved = false;
		alert("stopped",ofxUIColor(0,0,255));

		s_stop->setValue(false);

}
void BaselineCalibrator_GUI::start(){
	calibrator->cleanValues();

		//set session on calibrator
		std::string ssessiontime = b_sessiontime->getTextString();
		std::vector<std::string> scontrol_base = string_separate(blist->getTextString(),",");
		
		std::vector<Value> vcontrol_base = transform_values(scontrol_base);
			//add 80 two times//vcontrol_base.insert(vcontrol_base.begin(),vcontrol_base.front()); //copy the first value

		std::vector<std::vector<std::vector<Value>>> control_base = generate_base(calibrator->getControlParameters(),vcontrol_base);
		calibrator->setcontrolbase(control_base);
		calibrator->setSession(stoi(ssessiontime));

		
		//load profiles
		std::vector<Profile> users = std::vector<Profile>();
		for(int i=0; i<calibrator->getAgentsNum(); i++) //todo: follow a schema instead of concrete profiles
		{
			//set agents to the calibrator
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

			s_player->activateToggle("P"+std::to_string(i)+" ");
		}
		calibrator->setAgents(users);


		

		//std::vector<Value> base_model_values;
		//std::vector<Value> base_control_values = transform_values(dialog_separated); 
				
		//std::vector<std::vector<std::vector<Value>>> model_base = generate_base(model_parameters,base_model_values);
		//std::vector<std::vector<std::vector<Value>>> control_base = generate_base(control_parameters,base_control_values);

		//run the system
		calibrator->start();

		sendstarted = true;
		
		calibrator->baseline(); //start using first base value

		alert("started",ofxUIColor(0,0,255));
		s_start->setValue(false);

		gui0->saveSettings("BaselineGUI.xml");
}

void BaselineCalibrator_GUI::dataloadusers(){
		std::vector<std::vector<std::string>> csvusers = loadManualCSV(";","Specify the user profiles .csv file");
		std::vector<Profile> users = parseProfiles(csvusers);

		if(users.size()==a_name.size()){
			calibrator->setAgents(users);

			for(int i=0; i<users.size(); i++){
				a_name[i]->setTextString(users[i].name);
				a_age[i]->setTextString(std::to_string(users[i].age));
			}
			alert("loaded users file",ofxUIColor(0,255,0));
		}else{
			alert("wrong users file",ofxUIColor(255,0,0));
		}
		a_load->setValue(false);
}

void BaselineCalibrator_GUI::datagenerate(){
	std::vector<Profile> newagents = calibrator->getAgents();

			for(int i=0; i<calibrator->getAgentsNum();i++){
				int jAPA = locate_mparameter("APA",calibrator->getModelParameters()[i]);
				int jHR = locate_mparameter("HR",calibrator->getModelParameters()[i]);
				int jIT = locate_cparameter("IT",calibrator->getControlParameters()[i]);

				std::vector<Value> APAvalues = *calibrator->getModelParameters()[i][jAPA].values;
				std::vector<Value> HRvalues = *calibrator->getModelParameters()[i][jHR].values;
				std::vector<Value> ITvalues = *calibrator->getControlParameters()[i][jIT].values;

				Value maxHR = 208-0.7*calibrator->getAgents()[i].age;
				Value tHR = s_pa_percentage->getValue()*(maxHR);
					Value rangeHR = 0.05*(maxHR);
					Value uHR = tHR+rangeHR;
					Value lHR = tHR-rangeHR;
				Value tAPA = findCorrTarget2(APAvalues,HRvalues,tHR,rangeHR);
					//Value uAPA = findCorrTarget2(APAvalues,HRvalues,uHR,rangeHR);
					//Value lAPA = findCorrTarget2(APAvalues,HRvalues,lHR,rangeHR);
					Value uAPA = findCorrAbove(APAvalues,HRvalues,uHR);
					Value lAPA = findCorrBelow(APAvalues,HRvalues,lHR);
				Value tIT = findCorrTarget2(ITvalues,HRvalues,tHR,rangeHR);
					//Value uIT = findCorrTarget2(ITvalues,HRvalues,uHR,rangeHR);
					//Value lIT = findCorrTarget2(ITvalues,HRvalues,lHR,rangeHR);
					Value uIT = findCorrAbove(ITvalues,HRvalues,uHR);
					Value lIT = findCorrBelow(ITvalues,HRvalues,lHR);

				//std::cout << tAPA << ","<< APAvalues.size() << std::endl;
				//std::cout << tHR << ","<<HRvalues.size() << std::endl;
				//std::cout << tIT << ","<<ITvalues.size() << std::endl;

				newagents[i].tAPA = tAPA;
					newagents[i].uAPA = fabs(uAPA);
					newagents[i].lAPA = fabs(lAPA);
				newagents[i].tHR = tHR;
					newagents[i].uHR = fabs(uHR);
					newagents[i].lHR = fabs(lHR);
				newagents[i].tIT = tIT;
					newagents[i].uIT = fabs(uIT);
					newagents[i].lIT = fabs(lIT);

				
			}

			//calculate the average for all active players

			Value APAsum=0;
			Value APAavgallplayers=0;
				Value uAPAsum=0;
				Value uAPAavgallplayers=0;
				Value lAPAsum=0;
				Value lAPAavgallplayers=0;
			Value ITsum=0;
			Value ITavgallplayers=0;
				Value uITsum=0;
				Value uITavgallplayers=0;
				Value lITsum=0;
				Value lITavgallplayers=0;
			
			int goodplayers = 0;
			int ugoodplayers = 0;
			int lgoodplayers = 0;

			for(int i=0; i<calibrator->getAgentsNum();i++){
				if(newagents[i].tAPA < 0 || newagents[i].tIT < 0){
					
				}else{
					APAsum += newagents[i].tAPA;
					ITsum += newagents[i].tIT;
					goodplayers++;
				}
				if(newagents[i].uAPA < 0 || newagents[i].uIT < 0){
					
				}else{
					uAPAsum += newagents[i].uAPA;
					uITsum += newagents[i].uIT;
					ugoodplayers++;
				}
				if(newagents[i].lAPA < 0 || newagents[i].lIT < 0){
					
				}else{
					lAPAsum += newagents[i].lAPA;
					lITsum += newagents[i].lIT;
					lgoodplayers++;
				}
			}
			if(goodplayers !=0){
				APAavgallplayers = APAsum/goodplayers;
				ITavgallplayers = ITsum/goodplayers;
			}else{
				
			}
			if(ugoodplayers !=0){
				uAPAavgallplayers = uAPAsum/ugoodplayers;
				uITavgallplayers = uITsum/ugoodplayers;
			}else{
				
			}
			if(lgoodplayers !=0){
				lAPAavgallplayers = lAPAsum/lgoodplayers;
				lITavgallplayers = lITsum/lgoodplayers;
			}else{
				
			}


			//put the bad players with the average
			for(int i=0; i<calibrator->getAgentsNum();i++){
				if(goodplayers !=0){ //someone reached target performance
					if( newagents[i].tAPA <= 0 ||newagents[i].tIT <= 0 ){
						newagents[i].tAPA = APAavgallplayers;
						newagents[i].tIT = ITavgallplayers;
					}
				}else{ //nobody reached target performance
					
						newagents[i].tAPA = fabs(newagents[i].tAPA);
						newagents[i].tIT = fabs(newagents[i].tIT);
				}
				if(ugoodplayers !=0){ //someone reached target performance
					if( newagents[i].uAPA <= 0 ||newagents[i].uIT <= 0 ){
						newagents[i].uAPA = uAPAavgallplayers;
						newagents[i].uIT = uITavgallplayers;
					}
				}else{ //nobody reached target performance
					
						newagents[i].uAPA = fabs(newagents[i].uAPA);
						newagents[i].uIT = fabs(newagents[i].uIT);
				}
				if(lgoodplayers !=0){ //someone reached target performance
					if( newagents[i].lAPA <= 0 ||newagents[i].lIT <= 0 ){
						newagents[i].lAPA = lAPAavgallplayers;
						newagents[i].lIT = lITavgallplayers;
					}
				}else{ //nobody reached target performance
					
						newagents[i].lAPA = fabs(newagents[i].lAPA);
						newagents[i].lIT = fabs(newagents[i].lIT);
				}
			}

			calibrator->setAgents(newagents);
			
			writeCSV(";",parseAgents(newagents),"BL_users"+std::to_string(calibrator->getAgentsNum())+".csv");
			//std::cout << "generated a "<< "BL_users.csv" << " file" << std::endl;
			alert("generated a BL_users"+std::to_string(calibrator->getAgentsNum())+ "file.csv",ofxUIColor(0,255,0));
			
			s_output->setValue(false);
		
}