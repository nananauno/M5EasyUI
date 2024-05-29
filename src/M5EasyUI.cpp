// Copyright (c) NANANA (@nananauno). All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <M5EasyUI.h>

namespace NNN::M5EasyUI {
    UI ui = UI(); // Global instance of UI
    // ----------------------------------------------------------------------------
    // Constructor for LimitedArea class
    // ----------------------------------------------------------------------------
    LimitedArea::LimitedArea(int32_t x=0, int32_t y=0, int32_t w=0, int32_t h=0, bool isRounded=false) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->isRounded = isRounded;
    }
    // ----------------------------------------------------------------------------
    // Public functions for LimitedArea class
    // ----------------------------------------------------------------------------
    int32_t LimitedArea::getX(){
        return this->x;
    }
    int32_t LimitedArea::getY(){
        return this->y;
    }
    int32_t LimitedArea::getWidth(){
        return this->w;
    }
    int32_t LimitedArea::getHeight(){
        return this->h;
    }
    void LimitedArea::updateArea(int32_t x, int32_t y, int32_t w, int32_t h, bool isRounded){
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->isRounded = isRounded;
    }
    bool LimitedArea::Rounded(){
        return this->isRounded;
    }
    // ----------------------------------------------------------------------------
    // Constructor for SimpleLabel class
    // ----------------------------------------------------------------------------
    SimpleLabel::SimpleLabel(const char* text, TextLayout layout, TextValignment valignment) {
        this->text = text;
        this->layout = layout;
        this->valignment = valignment;
        this->scale = 1.0;
        this->x = 0;
        this->y = 0;
        this->w = 0;
        this->h = 0;
        this->fgcolor = WHITE;
        this->bgcolor = BLACK;
    }
    // ----------------------------------------------------------------------------
    // Public functions for SimpleLabel class
    // ----------------------------------------------------------------------------
    void SimpleLabel::setText(const char* text){
        this->text = text;
    }
    void SimpleLabel::setLayout(TextLayout layout){
        this->layout = layout;
    }
    void SimpleLabel::setTextValignment(TextValignment valignment){
        this->valignment = valignment;
    }
    void SimpleLabel::setTextScale(float scale){
        this->scale = scale;
    }
    void SimpleLabel::setTextColor(int32_t fgcolor, int32_t bgcolor){
        this->fgcolor = fgcolor;
        this->bgcolor = bgcolor;
    }
    void SimpleLabel::setCoordinate(int32_t x, int32_t y, int32_t w, int32_t h){
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    int32_t SimpleLabel::getX(){
        return this->x;
    }
    int32_t SimpleLabel::getY(){
        return this->y;
    }
    int32_t SimpleLabel::getWidth(){
        return this->w;
    }
    int32_t SimpleLabel::getHeight(){
        return this->h;
    }
    const char* SimpleLabel::getText(){
        return this->text;
    }
    TextLayout SimpleLabel::getTextLayout(){
        return this->layout;
    }
    float SimpleLabel::getTextScale(){
        return this->scale;
    }
    void SimpleLabel::draw(M5Canvas* canvas){
        float textscale = this->scale;
        canvas->setTextColor(fgcolor, bgcolor);
        canvas->setTextWrap(true);
        canvas->setClipRect(this->x, this->y, this->w, this->h);
        // Autoscale
        if(this->layout == TextLayout::Center_W_AutoScale){
            int32_t textwidth = canvas->textWidth(this->text);
            textscale = (float)this->w/(float)textwidth;

            // If font height exceeds the limited area, scale down the font size.
            if(textscale*canvas->fontHeight() > this->h){
                textscale = (float)this->h/(float)canvas->fontHeight();
            }
        }
        // Vertical alignment
        int32_t voffset=0;
        switch(valignment){
            case TextValignment::Top:
                // voffset = 0;
                break;
            case TextValignment::Center:
                voffset = this->h/2-(textscale*canvas->fontHeight()/2);
                break;
            case TextValignment::Bottom:
                voffset = this->h-textscale*canvas->fontHeight();
                break;
        }
        canvas->setTextSize(textscale);
        if(this->layout == TextLayout::Center_W_AutoScale || this->layout == TextLayout::Center){
            canvas->drawCenterString(this->text, this->x+this->w/2, this->y+voffset);
        }else if(this->layout == TextLayout::Right){
            canvas->drawRightString(this->text, this->x+this->w, this->y+voffset);
        }else{
            canvas->setCursor(this->x, this->y+voffset);
            canvas->print(this->text);
        }
        canvas->setTextSize(1);
        canvas->clearClipRect();
        canvas->setTextColor(WHITE, BLACK);
    }
    // ----------------------------------------------------------------------------
    // Public functions for ScrollLabel class
    // ----------------------------------------------------------------------------
    void ScrollLabel::append(const char* text){
        if(buffer.size() >= this->maxLines){
            // Erase first element if buffer size exceeds max number of line.
            buffer.erase(buffer.begin());
        }
        buffer.push_back(text);
        scrollPosition = buffer.size()-1;
    }
    const char* ScrollLabel::getLine(uint8_t index){
        if(index >= buffer.size()) return nullptr;
        return buffer[index].c_str();
    }
    uint8_t ScrollLabel::getNumLines(){
        return this->buffer.size();
    }
    uint8_t ScrollLabel::getMaxLines(){
        return this->maxLines;
    }
    void ScrollLabel::scrollUp(){
        if(scrollPosition != 0){
            scrollPosition--;
        }
    }
    void ScrollLabel::scrollDown(){
        if(scrollPosition < buffer.size()-1){
            scrollPosition++;
        }
    }
    void ScrollLabel::scrollTo(uint8_t index){
        if(index < buffer.size()){
            scrollPosition = index;
        }
    }
    void ScrollLabel::draw(M5Canvas* canvas){
        float textscale = this->scale;

        // Calculate max lines that can be displayed and start and end points of buffer
        int8_t windowSize = this->h/(textscale*canvas->fontHeight());
        uint8_t start = 0;
        uint8_t end = 0;
        if(this->buffer.size() <= windowSize){
            // All lines can be displayed
            start = 0;
            end = this->buffer.size();
        }else if(this->buffer.size()-scrollPosition <= windowSize){
            // Number of displayed lines is less than or equal to windowSize
            //start = this->buffer.size()-max;
            start = (this->buffer.size()-windowSize < 0) ? 0 : this->buffer.size()-windowSize;
            end = this->buffer.size();
        }else{
            start = scrollPosition;
            end = scrollPosition+windowSize;
        }

        canvas->setTextWrap(true);
        canvas->setClipRect(this->x, this->y, this->w, this->h);
        canvas->setTextSize(textscale);
        canvas->setCursor(this->x, this->y);
        for(uint8_t i=start; i<end; i++){
            canvas->println(this->getLine(i));
        }
        canvas->setTextSize(1);
        canvas->clearClipRect();
    }
    // ----------------------------------------------------------------------------
    // Constructor for ButtonGuide class
    // ----------------------------------------------------------------------------
    // ButtonGuide is only available under the following conditions:
    //  - Screen rotation is Normal or Upside down for BASIC, Core2, Stick and Dial.
    // Otherwise, Button guide is not displayed even if visibility is true.
    ButtonGuide::ButtonGuide(const char* btnAText="", const char* btnBText="", const char* btnCText=""){
        this->labels[0] = SimpleLabel(btnAText, TextLayout::Center, TextValignment::Center);
        this->labels[0].setTextScale(2);
        this->labels[1] = SimpleLabel(btnBText, TextLayout::Center, TextValignment::Center);
        this->labels[1].setTextScale(2);
        this->labels[2] = SimpleLabel(btnCText, TextLayout::Center, TextValignment::Center);
        this->labels[2].setTextScale(2);
    }
    // ----------------------------------------------------------------------------
    // Public functions for ButtonGuide class
    // ----------------------------------------------------------------------------
    int32_t ButtonGuide::getHeight(){
        return -1;
    }
    SimpleLabel* ButtonGuide::getLabel(uint8_t index){
        if(index >= M5EASYUI_MAX_BUTTON_LABELS) return nullptr;
        return &this->labels[index];
    }
    // ----------------------------------------------------------------------------
    // Constructor for UI class
    // ----------------------------------------------------------------------------
    UI::UI() {
        this->display = nullptr;
        this->area = LimitedArea();
        this->btnArea = LimitedArea();
        this->numLabels = 0;
        this->btnGuide = nullptr;
    }
    // ----------------------------------------------------------------------------
    // Public functions for UI class
    // ----------------------------------------------------------------------------
    void UI::begin(M5GFX* m5gfx, bool enableConsole=true) {
        this->display = m5gfx;
        this->display->fillScreen(BLACK);

        this->rotation = display->getRotation();
        this->board = display->getBoard();

        // Calculate limited area
        this->recalculateLimitedArea();

        // Initialize canvas
        //  Check board and set PSRAM if board has PSRAM
        //  PSRAM is slower than SRAM. If using small screen it's better to not use PSRAM for sptite.
        //  I's necessary to use PSRAM for creating a sprite for M5Core2. 
        lgfx::boards::board_t board = this->display->getBoard();
        if(board == lgfx::boards::board_M5StackCore2) {
            this->canvas.setPsram(true);
        }else{
            // No use PSRAM
            this->canvas.setPsram(false);
        }
        this->canvas.createSprite(this->area.getWidth(), this->area.getHeight());
        //this->canvas.createSprite(this->display->width(), this->display->height());
        
        #ifdef M5EASYUI_ESP_CONSOLE
        // ESP console is used for adjusting UIs through Serial console.
        // This function is only available for HardwareSerial, since the pre-compiled esp32 SDK does not have functions for HWCDC and USBCDC.
        // It means that this function is available on the M5Stack series with USB-Serial bridge IC.
        // Others, need to connect UART pins directly with a USB-Serial device.

        // Initialze ESP console
        if(enableConsole){
            esp_console_cmd_t command = {};
            esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
            #ifdef CONFIG_ESP_CONSOLE_USB_CDC
            esp_console_dev_usb_cdc_config_t cdc_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&cdc_config, &repl_config, &s_repl));
            #elif CONFIG_ESP_CONSOLE_UART
            // If M5Unified installs the UART driver, this function call will be failed. 
            // So, if you want to use the UART driver, please disable UART by M5Unified.
            esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();

            // Change UART pins to Port.B for M5Dial. TX:1 RX:2
            if(board == lgfx::boards::board_M5Dial){
                uart_config.tx_gpio_num = (gpio_num_t)1;
                uart_config.rx_gpio_num = (gpio_num_t)2;
            }

            ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &s_repl));
            #elif CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
            esp_console_dev_usb_serial_jtag_config_t usbjtag_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&usbjtag_config, &repl_config, &s_repl));
            #endif

            // Register label command
            command = {};
            command.command = "label";
            command.help = "Adjust label parameters\nUsage: label [label index] [scale] [H alignment] [V alignment]";
            command.func = [](int argc, char **argv) -> int {
                ui.esp_console_cmd_label(argc, argv);
                return 0;
            };
            esp_console_cmd_register(&command);

            // Register labels command
            command = {};
            command.command = "labels";
            command.help = "List all labels registered\nUsage: labels";
            command.func = [](int argc, char **argv) -> int {
                ui.esp_console_cmd_labels(argc, argv);
                return 0;
            };
            esp_console_cmd_register(&command);

            // Register debug command
            command = {};
            command.command = "debug";
            command.help = "Set UI debug level\nUsage: debug [0:None, 1:Outline]";
            command.func = [](int argc, char **argv) -> int {
                //if(argc != 2) return 0;
                //int level = atoi(argv[1]);
                //ui.setDebugLevel((DebugLevel)level);
                ui.esp_console_cmd_debug(argc, argv);
                return 0;
            };
            esp_console_cmd_register(&command);
            ESP_ERROR_CHECK(esp_console_start_repl(s_repl));
        }
        #endif // M5EASYUI_ESP_CONSOLE
        #ifdef M5EASYUI_NT_SHELL
        // Initialze NT Shell
        #endif
    }
    void UI::add(SimpleLabel* label){
        if(this->numLabels >= M5EASYUI_MAX_LABELS) return;
        this->labels[this->numLabels] = label;
        this->numLabels++;
        this->recalculateCoordinate();
    }
    void UI::removeAll(){
        this->numLabels = 0;
        for(int i=0; i<M5EASYUI_MAX_LABELS; i++){
            this->labels[i] = nullptr;
        }
        this->recalculateCoordinate();
    }
    void UI::showButtonGuide(bool visibility){
        this->isButtonGuideVisible = visibility;
        this->recalculateCoordinate();
    }
    void UI::setDebugLevel(DebugLevel level){
        this->debugLevel = level;
    }
    void UI::setLayout(Layout layout){
        this->layout = layout;
        this->recalculateCoordinate();
    }
    void UI::setRotation(uint_fast8_t rotation){
        this->rotation = rotation;
        this->display->setRotation(rotation);
        this->recalculateLimitedArea();
        this->recalculateCoordinate();
        // Create sprit according to the new limited area
        this->canvas.deleteSprite();
        this->canvas.createSprite(this->area.getWidth(), this->area.getHeight());
        //this->canvas.createSprite(this->display->width(), this->display->height());
    }
    void UI::setButtonGuide(ButtonGuide* btnGuide, bool visibility){
        this->btnGuide = btnGuide;
        this->isButtonGuideVisible = visibility;
    }
    void UI::update() {
        // Skip if no labels are registered
        //if(numLabels == 0) return;

        // Setting up drawing
        display->startWrite();
        canvas.fillScreen(BLACK);
        canvas.setTextColor(WHITE, BLACK);
        canvas.setTextSize(1);

        //display->setClipRect(area.getX(), area.getY(), area.getWidth(), area.getHeight());
        //display->setTextWrap(true, false);

        // Drawing labels
        //uint8_t numLabelsDrawn = 0;
        // Max number of labels can be drawing depends on Layout
        // Limit labels if number of registered label exceeds number of labels can be drawing
        /*if(layout == Layout::Footer || layout == Layout::Header){
            // Max 2 labels
            numLabelsDrawn = (numLabels < 2) ? numLabels : 2;
        }else if(layout == Layout::Grid_2x2){
            // Max 4 labels
            numLabelsDrawn = (numLabels < 4) ? numLabels : 4;
        }else{
            numLabelsDrawn = numLabels;
        }*/
        for(int i=0; i<maxNumDrawing(); i++){
            labels[i]->draw(&canvas);
        }

        //display->clearClipRect();

        // ButtonGuide
        if(isButtonGuideOn()){
            drawButtonGuide();
        }

        // Debug
        if(debugLevel == DebugLevel::OUTLINE){
            drawOutline();
        }

        canvas.pushSprite(display, area.getX(), area.getY());
        //canvas.pushSprite(display, 0, 0);
        display->endWrite();
    }
    #ifdef M5EASYUI_ESP_CONSOLE
    // "label" command: Adjust label parameters
    void UI::esp_console_cmd_label(int argc, char **argv){
        if(argc != 5) {
            printf("Usage: label [label index] [scale] [H alignment] [V alignment]\n");
            return;
        }
        int32_t index = atoi(argv[1]);
        if(index < 0 || index >= this->numLabels) {
            printf("Invalid label index.\n");
            return;
        }
        float scale = atof(argv[2]);
        int32_t alignment = atoi(argv[3]);
        if(alignment < 0 || alignment > 4) {
            printf("Invalid horizontal alignment %d\n", alignment);
            return;
        }
        int32_t vAlignment = atoi(argv[4]);
        if(vAlignment < 0 || vAlignment > 3) {
            printf("Invalid vertical alignment %d\n", vAlignment);
            return;
        }
        this->labels[index]->setLayout((TextLayout)alignment);
        this->labels[index]->setTextValignment((TextValignment)vAlignment);
        this->labels[index]->setTextScale(scale);
        printf("Set label[%d] as %f scale and alignment (H,V) (%d,%d).\n", index, scale, alignment, vAlignment);
        return;
    }
    // List all labels registered
    void UI::esp_console_cmd_labels(int argc, char **argv){
        printf("Screen: %dx%d\n", this->display->width(), this->display->height());
        printf("Limited area: (%d,%d) - (%d,%d), isRounded: %d\n", this->area.getX(), this->area.getY(), this->area.getWidth(), this->area.getHeight(), this->area.Rounded());
        printf("Canvas: %dx%d\n", this->canvas.width(), this->canvas.height());
        printf("Layout: %d\n", this->layout);
        for(int i=0; i<this->numLabels; i++){
            printf("Label[%d] %s\n", i, this->labels[i]->getText());
            printf("  Layout: %d\n", this->labels[i]->getTextLayout());
            printf("  Scale: %f\n", this->labels[i]->getTextScale());
            printf("  Coordinate: %d, %d, %d, %d\n", this->labels[i]->getX(), this->labels[i]->getY(), this->labels[i]->getWidth(), this->labels[i]->getHeight());
        }
        // Button guide
        for(int i=0; i<M5EASYUI_MAX_BUTTON_LABELS; i++){
            SimpleLabel* label = this->btnGuide->getLabel(i);
            if(label == nullptr) continue;
            printf("ButtonGuide[%d] %s\n", i, label->getText());
            printf("  Layout: %d\n", label->getTextLayout());
            printf("  Scale: %f\n", label->getTextScale());
            printf("  Coordinate: %d, %d, %d, %d\n", label->getX(), label->getY(), label->getWidth(), label->getHeight());
        }
        return;
    }
    void UI::esp_console_cmd_debug(int argc, char **argv){
        if(argc != 2) {
            printf("Usage: debug [0:None, 1:Outline]\n");
            return;
        }
        int level = atoi(argv[1]);
        if(level < 0 || level > 1) {
            printf("Invalid debug level %d\n", level);
            return;
        }
        this->setDebugLevel((DebugLevel)level);
        printf("Set debug level to %d\n", level);
        return;
    }
    void UI::esp_console_cmd_screenshot(int argc, char **argv){
        if(argc != 2) {
            printf("Usage: screenshot [filename]\n");
            return;
        }
        // Read all pixel from canvas and save to file
        printf("Screenshot saved to %s\n", argv[1]);
        return;
    }
    #endif // M5EASYUI_ESP_CONSOLE
    #ifdef M5EASYUI_DEBUG
    // Use ESP console for debugging
    void UI::dumpParams(m5::Log_Class* log){
        log->printf("numLabels: %d\n", this->numLabels);
        log->printf("area.x: %d\n", this->area.getX());
        log->printf("area.y: %d\n", this->area.getY());
        log->printf("area.w: %d\n", this->area.getWidth());
        log->printf("area.h: %d\n", this->area.getHeight());
        log->printf("area.isRounded: %d\n", this->area.Rounded());
        log->printf("layout: %d\n", this->layout);
        log->printf("debugLevel: %d\n", this->debugLevel);
    }
    #endif
    // ----------------------------------------------------------------------------
    // Private functions for UI class
    // ----------------------------------------------------------------------------
    bool UI::isButtonGuideOn(){
        if(btnGuide == nullptr) return false;
        if(!isButtonGuideVisible) return false;

        // Get board info and rotation, check if button should be on.
        using namespace lgfx::boards;
        //board_t board = this->display->getBoard();
        switch(this->board){
            case board_t::board_M5StackCore2:
                if(rotation == 1) return true;
                break;
            //case board_t::board_M5StickCPlus2:
            //    if(rotation == 0 || rotation == 2) return true;
            //    break;
        }
        return false;
    }
    uint8_t UI::maxNumDrawing(){
        // Return max number of labels can be drawing depends on Layout
        uint8_t numLabelsDrawn = 0;
        // Max number of labels can be drawing depends on Layout
        // Limit labels if number of registered label exceeds number of labels can be drawing
        if(layout == Layout::Footer || layout == Layout::Header){
            // Max 2 labels
            numLabelsDrawn = (numLabels < 2) ? numLabels : 2;
        }else if(layout == Layout::Header_Footer){
            // Max 3 labels
            numLabelsDrawn = (numLabels < 3) ? numLabels : 3;
        }else if(layout == Layout::Grid_2x2){
            // Max 4 labels
            numLabelsDrawn = (numLabels < 4) ? numLabels : 4;
        }else{
            numLabelsDrawn = numLabels;
        }
        return numLabelsDrawn;
    }
    void UI::drawLabel(SimpleLabel* label){
        float textscale = label->getTextScale();
        canvas.setTextWrap(true);
        canvas.setClipRect(label->getX(), label->getY(), label->getWidth(), label->getHeight());
        // Autoscale
        if(label->getTextLayout() == TextLayout::Center_W_AutoScale){
            int32_t textwidth = display->textWidth(label->getText());
            textscale = (float)label->getWidth()/(float)textwidth;

            // If font height exceeds the limited area, scale down the font size.
            if(textscale*display->fontHeight() > label->getHeight()){
                textscale = (float)label->getHeight()/(float)display->fontHeight();
            }
        }
        canvas.setTextSize(textscale);
        if(label->getTextLayout() == TextLayout::Center_W_AutoScale || label->getTextLayout() == TextLayout::Center){
            canvas.drawCenterString(label->getText(), label->getX()+label->getWidth()/2, label->getY());
        }else if(label->getTextLayout() == TextLayout::Right){
            canvas.drawRightString(label->getText(), label->getWidth(), label->getY());
        }else{
            //canvas.drawString(label->getText(), label->getX(), label->getY());
            canvas.setCursor(label->getX(), label->getY());
            canvas.print(label->getText());
        }
        canvas.setTextSize(1);
        canvas.clearClipRect();
    }
    void UI::drawButtonIcon(SimpleLabel* label){
        canvas.setColor(WHITE);
        canvas.fillTriangle(label->getX()+label->getWidth()/2, label->getY()+label->getHeight()-3,
                            label->getX()+label->getWidth()/2-10, label->getY()+label->getHeight()/2+5,
                            label->getX()+label->getWidth()/2+10, label->getY()+label->getHeight()/2+5, WHITE);
    }
    void UI::drawButtonGuide(){
        using namespace lgfx::boards;
        for(int i=0; i<M5EASYUI_MAX_BUTTON_LABELS; i++){
            SimpleLabel* label = btnGuide->getLabel(i);
            if(label == nullptr) continue;
            drawLabel(label);
            drawButtonIcon(label);
        }
    }
    void UI::drawOutline() {
        canvas.setColor(RED);
        ///canvas.drawRect(area.getX()+1, area.getY()+1, area.getWidth()-2, area.getHeight()-2);
        canvas.drawRect(1, 1, area.getWidth()-2, area.getHeight()-2);
        canvas.setColor(GREEN);
        for(int i=0; i<maxNumDrawing(); i++){
            canvas.drawRect(labels[i]->getX()+2, labels[i]->getY()+2, labels[i]->getWidth()-4, labels[i]->getHeight()-4);
            //canvas.drawRect(2, 2, labels[i]->getWidth()-4, labels[i]->getHeight()-4);
        }
    }
    void UI::recalculateCoordinate(){
        if(numLabels == 0) return;
        int32_t widthPerLabel;
        int32_t heightPerLabel;
        int32_t heightForGuide=0;
        int32_t heightHeader=0;
        int32_t heightFooter=0;
        uint8_t rotation = display->getRotation();

        // Switch Button Guide on/off
        if(isButtonGuideOn()){
            // Rotation: Normal
            if(btnGuide->getHeight() <= 0){
                // If guide height is not set, use 1/5 of the screen height for Button Guide automatically.
                heightForGuide = display->height()/5;
            }else if(btnGuide->getHeight() > 0 && btnGuide->getHeight() > display->height()/5){
                // If guide height is set, use the set height. But guide height cannot be larger than 1/5 of the screen height.
                heightForGuide = display->height()/5;
            }else{
                heightForGuide = btnGuide->getHeight();
            }

            // Update limited area for button guide.
            btnArea.updateArea(area.getX(), area.getHeight()-heightForGuide, area.getWidth(), heightForGuide, false);
            // Update the coordinates for button guide.
            SimpleLabel* btnA = btnGuide->getLabel(0);
            SimpleLabel* btnB = btnGuide->getLabel(1);
            SimpleLabel* btnC = btnGuide->getLabel(2);
            btnA->setCoordinate(btnArea.getX(), btnArea.getY(), btnArea.getWidth()/3, btnArea.getHeight());
            btnB->setCoordinate(btnArea.getX()+btnArea.getWidth()/3, btnArea.getY(), btnArea.getWidth()/3, btnArea.getHeight());
            btnC->setCoordinate(btnArea.getX()+btnArea.getWidth()/3*2, btnArea.getY(), btnArea.getWidth()/3, btnArea.getHeight());
        }
        
        switch(layout){
            case Layout::VerticalBox:
                widthPerLabel = area.getWidth();
                heightPerLabel = (area.getHeight()-heightForGuide)/numLabels;
                for(int i=0; i<numLabels; i++){
                    //labels[i]->setCoordinate(area.getX(), area.getY()+heightPerLabel*i, widthPerLabel, heightPerLabel);
                    labels[i]->setCoordinate(0, heightPerLabel*i, widthPerLabel, heightPerLabel);
                }
                break;
            case Layout::HorizontalBox:
                widthPerLabel = area.getWidth()/numLabels;
                heightPerLabel = area.getHeight()-heightForGuide;
                for(int i=0; i<numLabels; i++){
                    //labels[i]->setCoordinate(area.getX()+widthPerLabel*i, area.getY(), widthPerLabel, heightPerLabel);
                    labels[i]->setCoordinate(widthPerLabel*i, 0, widthPerLabel, heightPerLabel);
                }
                break;
            case Layout::Grid_2x2:
                widthPerLabel = area.getWidth()/2;
                heightPerLabel = (area.getHeight()-heightForGuide)/2;
                if(labels[0] != nullptr) labels[0]->setCoordinate(0, 0, widthPerLabel, heightPerLabel);
                if(labels[1] != nullptr) labels[1]->setCoordinate(widthPerLabel, 0, widthPerLabel, heightPerLabel);
                if(labels[2] != nullptr) labels[2]->setCoordinate(0, heightPerLabel, widthPerLabel, heightPerLabel);
                if(labels[3] != nullptr) labels[3]->setCoordinate(widthPerLabel, heightPerLabel, widthPerLabel, heightPerLabel);
                break;
            case Layout::Header:
                widthPerLabel = area.getWidth();
                heightHeader = (area.getHeight()-heightForGuide)/5;
                if(labels[0] != nullptr) labels[0]->setCoordinate(0, 0, widthPerLabel, heightHeader);
                if(labels[1] != nullptr) labels[1]->setCoordinate(0, heightHeader, widthPerLabel, area.getHeight()-heightForGuide-heightHeader);
                break;
            case Layout::Footer:
                widthPerLabel = area.getWidth();
                heightFooter = (area.getHeight()-heightForGuide)/5;
                if(labels[0] != nullptr) labels[0]->setCoordinate(0, 0, widthPerLabel, area.getHeight()-heightForGuide-heightFooter);
                if(labels[1] != nullptr) labels[1]->setCoordinate(0, area.getHeight()-heightForGuide-heightFooter, widthPerLabel, heightFooter);
                break;
            case Layout::Header_Footer:
                widthPerLabel = area.getWidth();
                heightHeader = heightFooter = (area.getHeight()-heightForGuide)/5;
                if(labels[0] != nullptr) labels[0]->setCoordinate(0, 0, widthPerLabel, heightHeader);
                if(labels[1] != nullptr) labels[1]->setCoordinate(0, heightHeader, widthPerLabel, area.getHeight()-heightForGuide-heightHeader-heightFooter);
                if(labels[2] != nullptr) labels[2]->setCoordinate(0, area.getHeight()-heightForGuide-heightFooter, widthPerLabel, heightFooter);
                break;
        }
    }
    void UI::recalculateLimitedArea(){
        using namespace lgfx::boards;
        bool isRounded = false;
        board_t board = this->display->getBoard();
        if(board == board_t::board_M5Dial) {
            // Rounded screen
            isRounded = true;
            int32_t max_width = this->display->width()/SQRT2;
            int32_t max_height = this->display->height()/SQRT2;
            this->area.updateArea(this->display->width()/2-max_width/2, this->display->height()/2-max_height/2, max_width, max_height, isRounded);
        }else{
            // Normal screen
            this->area.updateArea(0, 0, this->display->width(), this->display->height(), isRounded);
        }
    }
}  // namespace NNN::M5EasyUI