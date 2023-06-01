#include "ui_button_base.h"

#include <u8g2.h>
#include <U8g2lib.h>

ButtonBase::ButtonBase()
{
    type = element_type_t::button;
}

void ButtonBase::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    bounds.x = within.x;
    bounds.y = within.y;
    bounds.w = style.minimalOffset * 2;
    if (icon != nullptr)
    {
        bounds.w += icon->sz + style.minimalOffset;
        bounds.h = style.minimalOffset * 2 + ui_max(style.regularHeight, icon->sz);
    }
    else
        bounds.h = style.regularHeight + style.minimalOffset * 2;

    bounds.h -= 2;

    device->setFont(style.regular);
    if (text != nullptr)
        bounds.w += device->getUTF8Width(text);
}
