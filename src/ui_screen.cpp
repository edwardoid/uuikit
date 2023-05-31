#include "ui_screen.h"

Screen::Screen()
{
    m_children.fill(nullptr);
}

bool Screen::handle(const user_input_t key)
{
    for(uint8_t i = 0; i < m_childrenCount; ++i)
    {
        return m_children[i]->handle(key);
    }

    return true;
}