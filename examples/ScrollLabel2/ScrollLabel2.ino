// Scroll up/down text with ScrollLabel.
//  - Push Button A to rotate screen.
//  - Push Button B or rotate the encoder to counter clock wise to scroll up text.
//  - Push Button C or rotate the encoder to clock wise to scroll down text.

#include <Arduino.h>
#include <M5Unified.h>
#include <M5EasyUI.h>

//#define M5DIAL // Comment out to enable encoder if you use M5Dial
#ifdef M5DIAL
#include <Rotary.h>
Rotary encoder(41, 40);
#endif

using namespace NNN::M5EasyUI;
SimpleLabel label1("Scroll text", TextLayout::Center_W_AutoScale);
ScrollLabel label2;
ButtonGuide btnGuide = ButtonGuide("Rotate", "Up", "Down");

// Test
uint_fast8_t rotation = 1;
bool button = true;

void setup(){
	auto cfg = M5.config();
	cfg.serial_baudrate = 0;
	M5.begin(cfg);

	// Init Encoder for Dial
	#ifdef M5DIAL
	encoder.begin();
	#endif

	// Init scroll text
	for(int i=0; i<50; i++){
		char buf[16];
		sprintf(buf, "Line %d", i);
		label2.append(buf);
	}
	label2.setTextScale(1);
	label2.scrollTo(0);

	ui.begin(&M5.Display, true);
	ui.setLayout(Layout::Header);
	ui.setButtonGuide(&btnGuide,true);
	// Add labels to UI
	ui.add(&label1);
	ui.add(&label2);
}

void loop(){
	M5.update();
	// Read Encoder for Dial
	#ifdef M5DIAL
	unsigned char encoderResult = encoder.process();
	if(encoderResult == DIR_CW){
		label2.scrollDown();
	}else if(encoderResult == DIR_CCW){
		label2.scrollUp();
	}
	#endif
	if(M5.BtnA.wasPressed()){
		rotation = (rotation + 1) % 4;
		ui.setRotation(rotation);
	}
	if(M5.BtnB.wasPressed()){
		label2.scrollUp();
	}
	if(M5.BtnC.wasPressed()){
		label2.scrollDown();
	}
	ui.update();
}