#include "ui_toggle.h"

Toggle::Toggle()
{
    type = element_type_t::toggle;
}

void Toggle::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    PushButton::calculateBounds(within, style, device);
    bounds.w += style.minimalOffset + style.symbolsHeight;
}