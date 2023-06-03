#pragma once

#include "ui_screen.h"

enum class dialog_buttons_t
{
    ok = 1 << 0,
    cancel = 1 << 1,
    yes = 1 << 2,
    no = 1 << 3,
    abort = 1 << 4
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
private:
    uint8_t m_buttons = 0;
    const char* m_text = "";
};

