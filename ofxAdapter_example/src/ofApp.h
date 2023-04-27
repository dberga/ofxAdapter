#pragma once

#ifndef _OF_APP
#define _OF_APP

#include "ofMain.h"

#include "Generic_GUI.h"

#include "BaselineCalibrator.h"
#include "BaselineCalibrator_GUI.h"

#include "Adapter.h"
#include "Adapter_GUI.h"


#include "utils.h"

	#include "netutils.h"
	#include "exerutils.h"

const int BASELINE=1;
const int ADAPTER =2;
const int FPS =60;
const int MAXMILLIS = 1000;

class ofApp : public ofBaseApp{

	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:

		Generic_GUI *ggui;
		Adapter *adaptive_system;
		Adapter_GUI *agui;
		BaselineCalibrator *baseline_system;
		BaselineCalibrator_GUI *bgui;

		ofxOscReceiver *receiver;
		ofxOscSender *sender;
		bool setupmod;
		int controllertype; //0 = controller, 1 = baselinecalibrator, 2=adapter
		int receivetype; //0=random, 1=from osc

		std::clock_t start;
		std::vector<float> player_flags_sendedduration;
		std::vector<float> player_flags_duration;

			//Parameters and Profiles

			std::vector<EXERProfile> users;

			std::vector<GCParameter> global_control_parameters;
			std::vector<Relation> correlations;
			std::vector<std::vector<CParameter>> control_parameters;
			std::vector<std::vector<MParameter>> model_parameters;

			int randtype;

			std::vector<std::vector<Value>> sample_mvt;

			std::vector<int> previousmillis;
			std::vector<int> currentmillis;
};

#endif
