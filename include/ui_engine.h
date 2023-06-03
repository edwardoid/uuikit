#pragma once


#include <u8g2.h>
#include <U8g2lib.h>

#include "ui_style.h"
#include "ui_screen.h"
#include "ui_dialog.h"
#include "ui_button_base.h"
#include "ui_toggle.h"
#include "ui_label.h"
#include "ui_menu.h"
#include "ui_progress_bar.h"
#include "ui_slider.h"

#include <array>

class Engine
{
public:
    using ScreensArray = std::array<Screen*, 10>;
    Engine(U8G2* device);
    void begin();
    void addScreen(Screen* screen);
    inline void setCurrentScreen(uint8_t index) { m_current = index; }
    bool render();
    inline const UIStyle& style() const { return m_st; }
    void input(user_input_t key);
private:
    bool renderScreen(const Screen& screen);
    bool renderDialog(const Dialog& screen);
    bool renderButton(const ButtonBase& button);
    bool renderToggle(const Toggle& toggle);
    bool renderProgress(const ProgressBar& progress);
    bool renderSlider(const Slider& slider);
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