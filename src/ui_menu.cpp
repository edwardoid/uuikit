#include "ui_menu.h"

Menu::Menu()
{
    type = element_type_t::menu;
}

void Menu::add(Element* e)
{
    m_children[m_childrenCount] = e;
    ++m_childrenCount;
}