#include "ui_dialog.h"

Dialog::Dialog()
{
    type = element_type_t::dialog;
}

bool Dialog::handle(const user_input_t key, const UIStyle& style)
{
    int dir = 0;
    if (key == user_input_t::key_increment) dir += 1;
    if (key == user_input_t::key_decrement) dir -= 1;

    if (dir == 0 && selectedButton() == dialog_buttons_t::none) setNextButton(1);
    else if (dir != 0) setNextButton(dir);
    
    if (key == user_input_t::key_pressed || key == user_input_t::key_long_pressed)
    {
        if (on_result != nullptr)
            on_result(m_selected);
    }

    return true;
}

void Dialog::setNextButton(uint8_t direction)
{
    uint8_t bits_count = (sizeof(dialog_buttons_t) * 8);
    uint16_t candidate = (uint16_t)m_selected;
    uint16_t bit = 0;

    for(int i = (bits_count -1); i >  -1; --i)
    {
        if (candidate & (1 << i))
        {
            bit = i;
            break;
        }
    }

    for(int i = 1; i < bits_count; ++i)
    {
        if (direction > 0)
        {
            candidate = 1 << ((bit + i) % bits_count);
        } else if (direction < 0)
        {
            candidate = 1 << ((bit - i) % bits_count);
        }
        if (candidate & m_buttons)
        {
            m_selected = (dialog_buttons_t)(candidate);
            break;
        }
    }
}