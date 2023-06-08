#pragma once

#include "ui_screen.h"
#include <functional>

enum class dialog_buttons_t
{
    none = 0,
    ok = 1 << 0,
    cancel = 1 << 1,
    yes = 1 << 2,
    no = 1 << 3,
    next = 1 << 4,
    previous = 1 << 5,
    abort = 1 << 6,
    accept = 1 << 7,
    dismiss = 1 << 8
};

class Dialog: public Screen
{
public:
    Dialog();
    virtual ~Dialog() = default;
    inline void setText(const char* text) { m_text = text; }
    inline const char* text() const { return m_text; }
    inline uint8_t buttons() const { return m_buttons; }
    inline void setButtons(uint8_t buttons) { m_buttons = buttons; }
    inline dialog_buttons_t selectedButton() const { return m_selected; }
    virtual bool handle(const user_input_t key, const UIStyle& style);
    std::function<void(dialog_buttons_t)> on_result = nullptr;
private:
    void setNextButton(uint8_t direction);
private:
    uint8_t m_buttons = 0;
    dialog_buttons_t m_selected = (dialog_buttons_t)0;
    const char* m_text = "";
};

