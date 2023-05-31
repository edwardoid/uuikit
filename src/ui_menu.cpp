#include "ui_menu.h"

#include <u8g2.h>
#include <U8g2lib.h>

Menu::Menu()
{
    type = element_type_t::menu;
    m_children.fill(nullptr);
}

void Menu::add(Element* e)
{
    e->align_h = h_alignment_t::left;
    e->align_v = v_alignment_t::top;
    m_children[m_childrenCount] = e;
    ++m_childrenCount;
}

bool Menu::handle(const user_input_t key)
{
    uint8_t current = m_selectedEntry;
    switch(key)
    {
        case user_input_t::key_increment:
        {
            ++m_selectedEntry;
            break;
        }

        case user_input_t::key_decrement:
        {
            if (m_selectedEntry == 0)
                m_selectedEntry = m_childrenCount - 1;
            else
                --m_selectedEntry;
            break;
        }

        default:
        {
            return m_children[m_selectedEntry]->handle(key);
        }
    }

    m_selectedEntry %= m_childrenCount;
    if (m_children[current] != nullptr)
        m_children[current]->select(false);
    
    if (m_children[m_selectedEntry] != nullptr)
        m_children[m_selectedEntry]->select(true);
    return false;
}

void Menu::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    bounds = within;
    Box leftOver = within;
    for (uint8_t i = 0; i < m_childrenCount; ++i)
    {
        Element* e = m_children[i];
        if (e == nullptr) {
            break;
        }
        e->calculateBounds(leftOver, style, device);
        e->bounds.x = leftOver.x;
        e->bounds.w = leftOver.w;
        uint8_t skip = e->bounds.h + style.minimalOffset;
        leftOver.y += skip;
        leftOver.h -= skip;
    }
}