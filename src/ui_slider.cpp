#include "ui_slider.h"

#include <u8g2.h>
#include <U8g2lib.h>

Slider::Slider()
{
    type = element_type_t::slider;
}

void Slider::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    bounds.x = within.x;
    bounds.y = within.y;
    bounds.w = within.w;
    bounds.h = style.smallHeight;
}