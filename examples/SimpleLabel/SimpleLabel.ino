// Simply display five labels with vertical box layout.
//  - Push Button A to rotate screen.
//  - Push Button B to show/hide button guide.

#include <Arduino.h>
#include <M5Unified.h>
#include <M5EasyUI.h>

using namespace NNN::M5EasyUI;
SimpleLabel label1("", TextLayout::Center_W_AutoScale);
SimpleLabel label2("Center with Autoscale", TextLayout::Center_W_AutoScale);
SimpleLabel label3("Center", TextLayout::Center);
SimpleLabel label4("Right", TextLayout::Right);
SimpleLabel label5("Left", TextLayout::Left);
ButtonGuide btnGuide = ButtonGuide("ButtonA", "ButtonB", "ButtonC");

// Test
uint32_t count=0;
char str[256];
uint_fast8_t rotation = 1;
bool button = true;

void setup(){
	auto cfg = M5.config();
	cfg.serial_baudrate = 0;
	M5.begin(cfg);
	ui.begin(&M5.Display, true);
	ui.setLayout(Layout::VerticalBox);
	ui.setButtonGuide(&btnGuide,true);
	// Add labels to UI
	ui.add(&label1);
	ui.add(&label2);
	ui.add(&label3);
	ui.add(&label4);
	ui.add(&label5);
}

void loop(){
	M5.update();
	if(M5.BtnA.wasPressed()){
		rotation = (rotation + 1) % 4;
		ui.setRotation(rotation);
	}
	if(M5.BtnB.wasPressed()){
		button = !button;
		ui.showButtonGuide(button);
	}
	sprintf(str, "%d", count++);
	label1.setText(str);
	ui.update();
}