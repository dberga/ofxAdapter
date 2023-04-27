
#ifndef __Generic_GUI__
#define __Generic_GUI__

#include "ofxHistoryPlot.h"
#include "ofxUI.h"

#include "BaselineCalibrator_GUI.h"
#include "Adapter_GUI.h"

//#define SCALEMAX 200000.0f
//#define SCALEMIN 0.0f

//#define MVT_SCALE_KINECT 20000.0f
//#define MVT_SCALE_KINECT 627306
//#define MVT_SCALE_SLIDE 0.32f
#define MVT_MAX_KINECT 0.033f
//#define MVT_MAX_SLIDE 7.0f //290  (before) //7.7 (after) // ? (after)
#define MVT_MAX_SLIDE 1.5f //290  (before) //7.7 (after) // ? (after)
#define CNSTDECAY 0.9985f // or (1- 1/960)
#define DECAYMIN 0.0f
#define DECAYMAX 1.0f

class Generic_GUI{

public:
	//cons,destr
	Generic_GUI();
	~Generic_GUI();
	
	
	
	//wrap functions
	void Setup(BaselineCalibrator_GUI *bgui, Adapter_GUI *agui);
	void update();
	void draw();

	bool showBL;
	bool showAS;
	bool init;

	Value mvtscale;
	int interfacetype;
	bool decay;
	Value decay_value;

	int getRectype();
	int getIntfacetype();
	Value getDecay();

private:
	

	BaselineCalibrator_GUI *bl;
	Adapter_GUI *as;
	
	
	ofxUISuperCanvas* gui0;
		ofxUIRadio* radio_choose_sys;
		ofxUIRadio* radio_choose_receive;
		ofxUILabelToggle* choose;
		//ofxUISlider* mvtScaleSlider;
		ofxUIRadio* radio_choose_scale;
		ofxUISlider* decay_slider;
};

#endif

