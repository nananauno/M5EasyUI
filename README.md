# M5EasyUI
This library is for drawing UI on M5 devices with a single code.
Draw labels and button guides without being aware of coordinates. Simply select the layout.

# Quick start
This is an example for simply creating one UI label with "Hello!" string and located at center of the display.
```cpp
#include <M5EasyUI.h>

using namespace NNN::M5EasyUI;
SimpleLabel label("Hello!", TextLayout::Center, TextValignment::Center);

void setup(){
  // If you want to adjust UIs through Serial console, you need to disable Serial by setting serial_baurate as 0.
  auto cfg = M5.config();
  cfg.serial_baudrate = 0;
  M5.begin(cfg);

  ui.begin(&M5.Display, true);
  ui.setLayout(Layout::HorizontalBox);
  ui.add(&label);
}

void loop(){
  M5.update();
  ui.update();
}

```
# Features
- Draw UI labels on the various displays used by M5 devices without being aware of the coordinates.
- Physical button guide (only for M5core2)
- Adjusting UI parameters through Serial console.

# Support device
This library has been verfied to be working on the following M5 devices:

M5Stack core2, AtomS3, M5Dial and StickC Plus2

# UI components
This section describes the UI components availbale in this library.

## UI
UI class is main class of this library to manage UI labels and layout.
A global instance "NNN::M5EasyUI::ui" is initialized when starting program up.
You can use this "ui" instance to add labels and set layout.

### void begin(M5GFX* m5gfx, bool enable)
This method should be called once at the setup function.
- m5gfx: Pointer to M5.Display
- enable: ture: Console On, false: Console Off
Console function allows you to adjust UI parameters through Serial console. Refer to "Adjusting UI parameters through Serial console" section.
> [!NOTE]
> If you set enable as true, Serial should not be used by M5Unified.

### void setDebugLevel(DebugLevel level)
Debug level controls show/hide outline of each label for debugging.
- 0: None
- 1: Show outline

### void setLayout(Layout layout)
Layout controls how labels placed on the screen.
- layout: Set value of Layout class:
  - Layout::VerticalBox: Place labels from Left to Right of the screen.
  - Layout::HorizontalBox: Place labels from Top to Bottom of the screen.
  - Layout::Grid_2x2: Place labels in 2x2 grid.
  - Layout::Header: One header and One body.
  - Layout::Footer: One body and One footer.
  - Layout::Header_Footer: One header, One body and One footer.

Maximum number of labels can be displayed are depending on Layout.
If number of labels registered exceeds max, labels cannot be displayed will be ignored.
Refer to the following:
- Five labels: VerticalBox, HorizontalBox (Considering to expand max more)
- Four labels: Grid_2x2
- Three labels: Header_Footer
- Two labels: Header, Footer

### void setRotation(uint_fast8_t rotation)
Rotate screen. Refer to [M5.Lcd.setRotation](https://github.com/m5stack/m5-docs/blob/master/docs/ja/api/lcd.md#setrotation).
- rotation: angle x90 degree

### void setButtonGuide(ButtonGuide* btnGuide, bool visibility)
Button guide is the guide of the physical buttons on M5 devices.
By setting a pointer to ButtonGuide instance, UI draws the button guide on the display.
- btnGuide: Pointer to ButtonGuide instance
- visibility: true: On, false: Off

### void add(SimpleLabel* label)
Add a label to UI. Maximum number of label is defined by M5EASYUI_MAX_LABELS. If number of label exceeds M5EASYUI_MAX_LABELS, it will be ignored.
- label: Pointer to a SimpleLabel instance.

### void removeAll()
Remove all labels registered from the ui instance.
Current version of this library does not support selecting and removing any labels.
For replacing exist labels by new, remove all and then add new one.

### void showButtonGuide(bool visibility)
Show/Hide the button guide.
- visibility: true: Show, false: Hide

## SimpleLabel
SimpleLabel is a label which displayed on the display and it has label text, text color and layout settings. Because coordinates of labels are automatically calculated by UI class and placed in appropriate positions of the display, therefore this library does not provide way to set coordinates of UI components.

### SimpleLabel(const char* text, TextLayout layout, TextValignment valignment)
Constructor of SimpleLabel.
- text: Pointer to characters.
- layout: Horizontal alignment of text. See setLayout.
- valignment: Vertical alignment of text. See setTextValignment.

### setText(const char* text)
Set text to the label.
- text: Pointer to characters.

### setLayout(TextLayout layout)
Set text horizontal alignment.
- layout: Set value of TextLayout.
  - TextLayout::Center_W_AutoScale: Center aligned text with Auto scaling to be fit to label area. 
  - TextLayout::Center: Center aligned text.
  - TextLayout::Right: Right aligned text.
  - TextLayout::Left: Left aligned text.

Except Left aligned, return code is ignored.

### setTextValignment(TextValignment valignment)
Set text vertical alignment.
- valignment: Set value of TextValignement
  - TextValignment::Top: Vertical top aligned text.
  - TextValignment::Center: Vertical center aligned text.
  - TextValignment::Bottom: Vertical bottom aligned text.

### setTextScale(float scale)
Set text scale.
- scale: Set text scale value. (scale x font height)

### setTextColor(int32_t fgcolor, int32_t bgcolor)
Set text foregound color and background color.
- fgcolor: Foreground color of text.
- bgcolor: Background color of text.

## ScrollLabel
ScrollLabel is a label with scrolling function. It has multiple line and allows you to scroll up/down.

### ScrollLabel(uint8_t max=255)
Constructor of ScrollLabel.
- max: Maximum number of lines.

Maximum number of lines can be displayed on the display is depending on font height and text scale.

### void append(const char* text)
Append text to a buffer in ScrollLabel. If number of lines exceeds M5EASYUI_MAX_SCROLL_LINE, oldest line will be removed and append new line to last of the buffer.
- text: Pointer to characters.

### void scrollUp()
Scroll up text.

### void scrollDown()
Scroll down text.

## ButtonGuide
ButtonGuide is a class which have text for button guide. Button guide function displays text labels near physical buttons.
Current version only supports M5core2 and setRotation(1).

### ButtonGuide(const char* btnAText, const char* btnBText, const char* btnCText)
Constructor of ButtonGuide class.
- btnAText: Text label for Button A.
- btnBText: Text label for Button B.
- btnCText: Text label for Button C.

# UI debugging
This library provides function to display outlines of labels registerd in ui instance. Refer to setDebugLevel of UI class.
- RED line: Limited drawing area of labels. All labels will be drawn within this area.
- GREEN line: Outline of each label.

# Adjusting UI parameters through Serial console
This library supports to adjust UI parameters thourgh Serial console.
You can adjust label layout and text scale without re-flashing firmware.
**Current version only supports HardwareSerial and this function does not work with USB-SERIAL console and USB-OTG console.** This means that you can use this function in M5 devices with a USB-SERIAL converter IC like M5core2 without additional equipment. But if you use M5 devices with ESP32-S3/C3, you need to connect a USB-SERIAL converter to UART0.

## Command list
- label: Adjust label parameters.
- labels: List all labels registered.
- debug: Set UI debug level. (0:None, 1:Outline)
- help: Display help.
