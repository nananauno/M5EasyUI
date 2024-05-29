// Display six layouts in sequence.
//  - Push Button A to switch layout.
//  - Push Button B to show/hide button guide.
//  - Push Button C to rotate screen.
//  - Hold Button A to show/hide layout outline

#include <Arduino.h>
#include <M5Unified.h>
#include <M5EasyUI.h>

using namespace NNN::M5EasyUI;
SimpleLabel label1("1",TextLayout::Center_W_AutoScale, TextValignment::Center);
SimpleLabel label2("2",TextLayout::Center_W_AutoScale, TextValignment::Center);
SimpleLabel label3("3",TextLayout::Center_W_AutoScale, TextValignment::Center);
SimpleLabel label4("4",TextLayout::Center_W_AutoScale, TextValignment::Center);
ButtonGuide btnGuide("ButtonA", "ButtonB", "ButtonC");

// Test
uint_fast8_t rotation = 1;
bool button = true;
bool debug = false;
uint8_t testmode=0;

void setup(){
	auto cfg = M5.config();
	cfg.serial_baudrate = 0;
	M5.begin(cfg);
	
	ui.begin(&M5.Display, true);
	ui.setLayout(Layout::Grid_2x2);
	ui.setButtonGuide(&btnGuide,true);
	// Add labels to UI
	ui.add(&label1);
	ui.add(&label2);
	ui.add(&label3);
	ui.add(&label4);
}

void loop(){
	M5.update();
	if(M5.BtnA.wasPressed()){
		testmode = (testmode + 1) % 6;
		switch(testmode){
			case 0:
				ui.setLayout(Layout::VerticalBox);
				label1.setText("Vertical_1");
				label2.setText("Vertical_2");
				label3.setText("Vertical_3");
				label4.setText("Vertical_4");
				break;
			case 1:
				ui.setLayout(Layout::HorizontalBox);
				label1.setText("Horizontal_1");
				label2.setText("Horizontal_2");
				label3.setText("Horizontal_3");
				label4.setText("Horizontal_4");
				break;
			case 2:
				ui.setLayout(Layout::Header);
				label1.setText("Header_1");
				label2.setText("Body_2");
				break;
			case 3:
				ui.setLayout(Layout::Footer);
				label1.setText("Body_1");
				label2.setText("Footer_2");
				break;
			case 4:
				ui.setLayout(Layout::Header_Footer);
				label1.setText("Header_1");
				label2.setText("Body_2");
				label3.setText("Footer_3");
				break;
			case 5:
				ui.setLayout(Layout::Grid_2x2);
				label1.setText("Grid_1");
				label2.setText("Grid_2");
				label3.setText("Grid_3");
				label4.setText("Grid_4");
				break;
		}
	}
	if(M5.BtnB.wasPressed()){
		button = !button;
		ui.showButtonGuide(button);
	}
	if(M5.BtnC.wasPressed()){
		rotation = (rotation + 1) % 4;
		ui.setRotation(rotation);
	}
	if(M5.BtnA.wasHold()){
		debug = !debug;
		ui.setDebugLevel(debug ? DebugLevel::OUTLINE : DebugLevel::NONE);
	}
	ui.update();
}