
#ifndef __BaselineCalibrator_GUI__
#define __BaselineCalibrator_GUI__

#include "ofxGui.h"
#include "ofxHistoryPlot.h"
#include "ofxUI.h"

#include "Parameters.h"
#include "utils.h"
#include "BaselineCalibrator.h"

#define PLOTSHEIGHT 70
#define PLOTSWIDTH 350
#define PLOTSMEMORY 300


class BaselineCalibrator_GUI{
	
public:
	//cons,destr
	BaselineCalibrator_GUI(bool visible=true, bool plot=true);
	~BaselineCalibrator_GUI();
	bool show;
	bool showplots;

	
	//wrap functions
	void Setup( BaselineCalibrator *incalibrator);
	void update();
	void draw();

	void setupguielements();
	void setupguipanels();
		void setupguipanels_individual();
		void setupguipanels_global();
		void setupguipanels_base();
	

	void updateModelParameters();
	void updateControlParameters();
	void updateGlobalControlParameters();
	void updateOrders();
		void dataimport(int player);
		void dataexport(int player);
		void dataloadhr(int player);
		void stop();
		void start();
		void dataloadusers();
		void datagenerate();

	bool sendstarted;
	bool sendstopped;

	void alert(std::string message,ofxUIColor incolor);
	

private:
	
	//Adapter
	BaselineCalibrator *calibrator;

	std::vector<PlayerExertion> exertions; //todo: too specific

	ofxUISuperCanvas* gui0;
		//fps
		ofxUITextArea* framerate;
		ofxUITextArea* advice;
		//label agentmanager
		ofxUILabel* label_amanager;
			//vector<textbox> players name
			std::vector<ofxUITextInput*> a_name;
			//vector<textbox> players age
			std::vector<ofxUITextInput*> a_age;
			//button load profiles
			ofxUILabelToggle* a_load;

		//label baseline management
		ofxUILabel* label_bmanager;
			//session time
			ofxUITextInput* b_sessiontime;
			//list of IT
			ofxUITextInput* blist;
			//toggle for the list filter (slide case)
			ofxUIToggle* b_filter_list;

		//label system control
		ofxUILabel* label_smanager;
			//radio show player
			ofxUIRadio* s_player;
			//button start
			ofxUILabelToggle* s_start;
			//button stop
			ofxUILabelToggle* s_stop;
			//button output calibrated profiles
			ofxUILabelToggle* s_output;
			//slider for range of MTVPA
			ofxUISlider* s_pa_percentage;


	std::vector<ofxUISuperCanvas*> gui1;
		//toggle manual edit
		std::vector<ofxUIToggle*> p_manualedit;
		//label model parameters manager
		std::vector<ofxUILabel*> label_pmpmanager;
			//vector<slider> APA, HR
			std::vector<std::vector<ofxUISlider*>> pmp_params;
			//vector<slider> tAPA, tHR
			std::vector<std::vector<ofxUISlider*>> pmp_tparams;
			//vector<historyplot> APA, HR
			std::vector<std::vector<ofxHistoryPlot>> pmp_plots;
			std::vector<std::vector<ofxUISpacer*>> pmp_plots_spacers;
		//label control parameters manager
		std::vector<ofxUILabel*> label_pcpmanager;
			//vector<slider> IT
			std::vector<std::vector<ofxUISlider*>> pcp_params;
			//vector<historyplot> IT
			std::vector<std::vector<ofxHistoryPlot>> pcp_plots;
			std::vector<std::vector<ofxUISpacer*>> pcp_plots_spacers;
		//label agent control
		std::vector<ofxUILabel*> label_pcmanager;
			//button output recordings
			std::vector<ofxUILabelToggle*> p_record;
			//button input recordings
			std::vector<ofxUILabelToggle*> p_inrecord;
			//button calibrate profile (input suunto and recordings and combine)
			std::vector<ofxUILabelToggle*> p_calibrate;

};

#endif

