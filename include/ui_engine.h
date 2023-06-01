#pragma once


#include <u8g2.h>
#include <U8g2lib.h>

#include "ui_style.h"
#include "ui_screen.h"
#include "ui_button_base.h"
#include "ui_toggle.h"
#include "ui_label.h"
#include "ui_menu.h"

#include <array>

class Engine
{
public:
    using ScreensArray = std::array<Screen*, 10>;
    Engine(U8G2* device);
    void begin();
    inline void addScreen(Screen* screen) { m_screens[m_screensCount++] = screen; };
    inline void setCurrentScreen(uint8_t index) { m_current = index; }
    bool render();
    inline const UIStyle& style() const { return m_st; }
    void input(user_input_t key);
private:
    bool renderScreen(const Screen& screen);
    bool renderButton(const ButtonBase& button);
    bool renderToggle(const Toggle& toggle);
    bool renderLabel(const Label& toggle);
    bool renderMenu(const Menu& toggle);
    bool renderUnknown(const Element* elem);
private:
    struct U8G2* m_dev = nullptr;
    UIStyle m_st;
    uint8_t m_current = 0;
    uint8_t m_screensCount = 0;
    ScreensArray m_screens;
};