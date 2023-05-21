#include "ui_label.h"

#include <u8g2.h>
#include <U8g2lib.h>

Label::Label()
{
    type = element_type_t::label;
}

void Label::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    bounds.x = within.x;
    bounds.y = within.y;
    bounds.w = style.minimalOffset;
    bounds.h = style.minimalOffset;

    const char* p = text;
    device->setFont(style.small);

    dim_t maxWidth = 0;
    while(*p != '\0')
    {
        bounds.h += style.smallHeight;

        uint16_t len = device->getUTF8Width(p);

        maxWidth = ui_max(len, maxWidth);
        
        while(*p != '\n' && *p != '\r' && *p != '\0') ++p;
        if (*p != '\0')
            ++p;
    }

    bounds.w = maxWidth + 2 * style.minimalOffset;
}