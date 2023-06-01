#include "ui_push_button.h"

#include <u8g2.h>
#include <U8g2lib.h>

PushButton::PushButton()
{}

bool PushButton::handle(const user_input_t key, const UIStyle& style)
{
    if (key == user_input_t::key_pressed || key == user_input_t::key_long_pressed)
    {
        if (on_click != nullptr)
        {
            on_click();
            return true;
        }
    }
    return false;
}