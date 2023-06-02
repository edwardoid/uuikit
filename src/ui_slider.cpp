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

bool Slider::handle(const user_input_t key, const UIStyle& style)
{
    switch(key)
    {
        case user_input_t::key_pressed:
        case user_input_t::key_long_pressed:
        {
            flags = (flags & flags_t::editing) ? (flags & ~flags_t::editing)
                                               : (flags | flags_t::editing);
            break;
        }

        case user_input_t::key_increment:
        {
            m_value += m_step;
            break;
        }
        case user_input_t::key_decrement:
        {
            m_value -= m_step;
            break;
        }
        default:
        {
            return false;
        }
    }

    if (m_value > m_maxValue)
        m_value = m_maxValue;
    else if (m_value < m_minValue)
        m_value = m_minValue;
    return true;
}