// Copyright (c) NANANA (@nananauno). All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <Arduino.h>
#include <M5Unified.h>

//#define M5EASYUI_DEBUG
#define M5EASYUI_MAX_LABELS 5           // Maximum number of labels that can be displayed in UI class
#define M5EASYUI_MAX_BUTTON_LABELS 3    // Maximum number of buttons that can be displayed in ButtonGuide class
//#define M5EASYUI_MAX_SCROLL_LINE 25   // Maximum number of lines that can be displayed in ScrollLabel class
#define M5EASYUI_ESP_CONSOLE            // Enable ESP console
//#define M5EASYUI_NT_SHELL             // Enable NT-Shell
#define SQRT2 1.4142135623730950488016887242097

// ESP console is used to adjust UI through Serial console
// If need to use Serial, you can disable esp console by calling UI::begin with false at 2nd argument
#ifdef M5EASYUI_ESP_CONSOLE
#include <esp_console.h>
#endif
// NT-shell is an alternative to ESP console, but not yet implemented
#ifdef M5EASYUI_NT_SHELL
#include <ntshell.h>
#endif

namespace NNN::M5EasyUI{
    enum class DebugLevel{
        NONE,                   // No outline shown
        OUTLINE,                // Show outline of drawing area
        OUTLINE_W_DIMENSIONS    // Show outline of drawing area and dimensions (Not yet implemented)
    };
    enum class Layout{
        VerticalBox,            // Up to M5EASYUI_MAX_LABELS
        HorizontalBox,          // Up to M5EASYUI_MAX_LABELS   
        Grid_2x2,               // Four labels
        Header,                 // Two labels
        Footer,                 // Two labels
        Header_Footer           // Three labels
    };
    enum class TextLayout{
        Center_W_AutoScale,     // Text is centered and scaled automatically to be matched label area
        Center,                 // Center aligned text
        Right,                  // Right aligned text
        Left                    // Left aligned text
    };
    enum class TextValignment{
        Top,                    // Vertical top aligned text
        Center,                 // Vertical center aligned text
        Bottom                  // Vertical bottom aligned text
    };
    // LimitedArea
    // This class is used to limit the drawing area of displays used on M5.
    class LimitedArea{
        public:
            LimitedArea(int32_t,int32_t,int32_t,int32_t,bool);
            int32_t getX();
            int32_t getY();
            int32_t getWidth();
            int32_t getHeight();
            void updateArea(int32_t, int32_t, int32_t, int32_t, bool);
            bool Rounded();
        private:
            bool isRounded;
            int32_t x,y,w,h;
    };
    // SimpleLabel
    class SimpleLabel{
        public:
            SimpleLabel(const char* text=NULL, TextLayout layout=TextLayout::Left, TextValignment valignment=TextValignment::Top);
            void setText(const char*);
            void setLayout(TextLayout);
            void setTextValignment(TextValignment);
            void setTextScale(float);
            void setTextColor(int32_t, int32_t);
            virtual void setCoordinate(int32_t, int32_t, int32_t, int32_t);
            int32_t getX();
            int32_t getY();
            int32_t getWidth();
            int32_t getHeight();
            const char* getText();
            TextLayout getTextLayout();
            float getTextScale();
            virtual void draw(M5Canvas*);
        protected:
            const char* text;           // Text of this label
            TextLayout layout;          // Text horizontal layout of this label
            TextValignment valignment;  // Text vertical layout of this label
            float scale;                // Text scale (Default: 1.0)
            int32_t x,y,w,h;            // Drawing are of this label
            int32_t fgcolor, bgcolor;   // Text color and background color of this label
        private:
            // None
    };
    // ScrollLabel
    class ScrollLabel: public SimpleLabel{
        public:
            ScrollLabel(uint8_t max=255) : SimpleLabel("",TextLayout::Left, TextValignment::Top){
                maxLines = max;
                windowSize = 0;
                scrollPosition = 0;
                maxScrollPosition=0;
            }
            void append(const char*);
            const char* getLine(uint8_t);
            uint8_t getNumLines();
            uint8_t getMaxLines();
            void scrollUp();
            void scrollDown();
            void scrollTo(uint8_t);
            //void scrollEnd();
            //void setCoordinate(int32_t, int32_t, int32_t, int32_t) override;
            void draw(M5Canvas*) override;
        private:
            std::vector<String> buffer;
            uint8_t maxLines;           // Maximum number of lines that can be stored in this label.
            uint8_t windowSize;         // Maximum number of lines that can be displayed at once.
            uint8_t scrollPosition;     // Current scroll position.
            uint8_t maxScrollPosition;
    };
    // ButtonGuide (BtnA, BtnB, BtnC)
    class ButtonGuide{
        public:
            ButtonGuide(const char*, const char *, const char *);
            int32_t getHeight();
            SimpleLabel* getLabel(uint8_t);
        private:
            SimpleLabel labels[M5EASYUI_MAX_BUTTON_LABELS];
    };
    // UI class
    class UI{
        public:
            UI();
            void begin(M5GFX*, bool);
            void setDebugLevel(DebugLevel);
            void setLayout(Layout);
            void setRotation(uint_fast8_t);
            void setButtonGuide(ButtonGuide*, bool);
            void add(SimpleLabel*);
            void removeAll();
            void showButtonGuide(bool);
            void update();
            #ifdef M5EASYUI_ESP_CONSOLE
            // ESP console commands
            void esp_console_cmd_label(int, char **);
            void esp_console_cmd_labels(int, char **);
            void esp_console_cmd_debug(int, char **);
            void esp_console_cmd_screenshot(int, char **);
            #endif
            #ifdef M5EASYUI_DEBUG
            void dumpParams(m5::Log_Class*);
            #endif
        private:
            M5GFX* display;                 // M5 display
            lgfx::boards::board_t board;    // M5 board info
            M5Canvas canvas;                // M5 canvas
            LimitedArea area;               // Drawing area of UI
            LimitedArea btnArea;            // Drawing area of button guide
            Layout layout;                  // Label layout
            uint_fast8_t rotation;          // Rotation of UI
            SimpleLabel* labels[M5EASYUI_MAX_LABELS];   // Labels
            ButtonGuide* btnGuide;          // Button guide
            bool isButtonGuideVisible;      // Visibility of button guide
            uint8_t numLabels;              // Number of lable
            DebugLevel debugLevel;          // UI debug lebel. Switch UI outline for debugging

            #ifdef M5EASYUI_ESP_CONSOLE
            esp_console_repl_t *s_repl;     // ESP console
            #endif
            #ifdef M5EASYUI_NT_SHELL
            ntshell_t *s_ntshell;
            #endif

            bool isButtonGuideOn();
            uint8_t maxNumDrawing();
            void drawLabel(SimpleLabel*);
            //void drawScrollLabel(ScrollLabel*);
            void drawButtonIcon(SimpleLabel*);
            void drawButtonGuide();
            void drawOutline();
            void recalculateCoordinate();
            void recalculateLimitedArea();

    };
    extern UI ui;
}