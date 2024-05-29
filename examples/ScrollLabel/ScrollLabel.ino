// Simply display scrolling text with ScrollLabel.
//  - Push Button A to rotate screen.
//  - Push Button B to show/hide button guide.

#include <Arduino.h>
#include <M5Unified.h>
#include <M5EasyUI.h>

using namespace NNN::M5EasyUI;
SimpleLabel label1("Scroll text", TextLayout::Center_W_AutoScale);
ScrollLabel label2;
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
	ui.setLayout(Layout::Header);
	ui.setButtonGuide(&btnGuide,true);
	// Add labels to UI
	ui.add(&label1);
	ui.add(&label2);
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
	sprintf(str, "Line %d", count++);
	label2.append(str);
	ui.update();
	delay(50);
}