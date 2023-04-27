
#include "Generic_GUI.h"

Generic_GUI::Generic_GUI(){

	
}
	
void Generic_GUI::Setup(BaselineCalibrator_GUI *bgui, Adapter_GUI *agui){
	bl = bgui;
	as = agui;
	
	gui0 = new ofxUISuperCanvas("PANEL: GENERIC SYSTEM");
	
	
	gui0->addSpacer();
		std::vector<std::string> sysnames;
		sysnames.push_back("Baseline Calibrator");
		sysnames.push_back("Adaptive System");
		radio_choose_sys = gui0->addRadio("radio_choose_sys",sysnames,OFX_UI_ORIENTATION_HORIZONTAL);
		gui0->addSpacer();

		std::vector<std::string> receivenames;
		receivenames.push_back("OSC");
		receivenames.push_back("Random");
		radio_choose_receive = gui0->addRadio("radio_choose_receive",receivenames,OFX_UI_ORIENTATION_HORIZONTAL);
		
		gui0->addSpacer();
		choose = gui0->addLabelToggle("Init",false,100.0f);
		gui0->addSpacer();
			
		std::vector<std::string> mvts;
		mvts.push_back("Kinect");
		mvts.push_back("Interactive Slide");

		radio_choose_scale = gui0->addRadio("mvtscale",mvts);
		radio_choose_scale->activateToggle("Interactive Slide");

		//mvtScaleSlider = gui0->addSlider("mvtscale",SCALEMIN,SCALEMAX,mvtscale);
		decay_slider = gui0->addSlider("decay_slider",DECAYMIN,DECAYMAX,CNSTDECAY);
		decay_slider->setLabelPrecision(3);

	gui0->autoSizeToFitWidgets();

	gui0->setPosition(
		ofGetWindowWidth()/2 - gui0->getRect()->getHalfWidth(), 
		ofGetWindowHeight()/2 - gui0->getRect()->getHalfWidth()
		);

	radio_choose_sys->activateToggle("Baseline Calibrator");
	showAS = false;
	showBL = true;
	init = false;
	mvtscale = 0;
	interfacetype = 2;
	radio_choose_receive->activateToggle("OSC");


	
	decay = false;

	//gui0->loadSettings("genericGUI.xml");
	choose->setValue(false);
}

void Generic_GUI::update(){
		
		

		if(radio_choose_sys->getActiveName()=="Baseline Calibrator"){
			showBL=true;
			showAS=false;
		}else if(radio_choose_sys->getActiveName()=="Adaptive System"){
			showBL=false;
			showAS=true;
		}else{
			showBL=false;
			showAS=true;
		}

		init = choose->getValue();
		if(init==true){

			std::string activemvt = radio_choose_scale->getActiveName();
			if(activemvt=="Interactive Slide"){
				mvtscale = MVT_MAX_SLIDE;
				interfacetype = 2;
				decay = true;
				decay_value = decay_slider->getValue();
			}else if(activemvt=="Kinect"){
				mvtscale = MVT_MAX_KINECT;
				interfacetype = 1;
				decay = false;
			}else{
				mvtscale = MVT_MAX_SLIDE;
				interfacetype = 2;
				decay = true;
			}

			//mvtscale = mvtScaleSlider->getValue();


			//gui0->saveSettings("genericGUI.xml");
			if(showBL==true){
				bl->show = true;
				as->show = false;
				gui0->setVisible(false);
			}else if(showAS==true){
				bl->show = false;
				as->show = true;
				gui0->setVisible(false);
			}	
			
		}else{
			bl->show = false;
			as->show = false;
		}

}

void Generic_GUI::draw(){
	
}


int Generic_GUI::getRectype(){
	if(radio_choose_receive->getActiveName()=="OSC"){
		return 2;
	}else if(radio_choose_receive->getActiveName()=="Random"){
		return 1;
	}

}
int Generic_GUI::getIntfacetype(){
	return interfacetype;


}


Value Generic_GUI::getDecay(){

	return decay_value;
}



