#include "ui_toggle.h"

#include "ui_types.h"

Toggle::Toggle()
    : value(false)
{
    type = element_type_t::toggle;
}

bool Toggle::handle(const user_input_t key)
{
    switch(key)
    {
        case user_input_t::key_pressed:
        {
            value = !value;
            break;
        }
        default:
            return false;
    }
    return true;
}

void Toggle::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    PushButton::calculateBounds(within, style, device);
    bounds.w += style.minimalOffset + style.symbolsHeight;
}