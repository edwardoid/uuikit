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
#include "ui_status_bar.h"

#include <array>
#include <queue>


template <typename T, size_t capacity>
class history_t
{
public:
    history_t() {};

    inline void push(const T& value)
    {
        m_data[++m_head] = value;
        ++m_size;
    }

    inline T pop()
    {
        --m_size;
        return m_data[--m_head];
    }

    inline const T& top() const
    { return m_data[m_head]; }
    
    
    inline size_t size() const { return m_size; }
    inline size_t empty() const { return size() == 0; }

private:
    std::array<T, capacity> m_data;
    std::size_t m_head = -1;
    std::size_t m_size = 0;
};

class Engine
{
public:
    using ScreensArray = std::array<Screen*, 10>;
    Engine(U8G2* device);
    void begin();
    inline void setStatusBar(StatusBar* statusBar) { m_statusBar = statusBar; }
    void addScreen(Screen* screen);
    inline void setCurrentScreen(uint8_t index) { m_history.push(index); }
    inline void back() { m_history.pop(); }
    bool render();
    inline const UIStyle& style() const { return m_st; }
    void input(user_input_t key);
    Box contentBox() const;
private:
    void renderStatusBar();
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
    history_t<uint8_t, 10> m_history;
    StatusBar* m_statusBar = nullptr;
    struct U8G2* m_dev = nullptr;
    UIStyle m_st;
    uint8_t m_screensCount = 0;
    ScreensArray m_screens;
};