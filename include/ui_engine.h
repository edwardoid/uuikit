#pragma once


#include <u8g2.h>
#include <U8g2lib.h>

#include "ui_style.h"
#include "ui_screen.h"
#include "ui_push_button.h"
#include "ui_toggle.h"
#include "ui_label.h"
#include "ui_menu.h"

class Engine
{
public:
    using ScreensArray = FixedSizeArray<Screen*, 10>;
    Engine(U8G2* device);
    void begin();
    uint8_t addScreen(Screen* screen) { m_screens[m_screensCount] = screen; return m_screensCount++;};
    void setCurrentScreen(Screen* screen) { m_current = m_screens.indexOf(screen); }
    bool render();
    inline const UIStyle& style() const { return m_st; }
private:
    bool renderScreen(const Screen& screen);
    bool renderButton(const PushButton& button);
    bool renderToggle(const Toggle& toggle);
    bool renderLabel(const Label& toggle);
    bool renderMenu(const Menu& toggle);
    bool renderUnknown(const Element* elem);
private:
    pos_t m_x = 0;
    pos_t m_y = 0;
    struct U8G2* m_dev = nullptr;
    UIStyle m_st;
    int8_t m_current = -1;
    uint8_t m_screensCount = 0;
    ScreensArray m_screens;
};