#pragma once

#include "ui_element.h"

class Menu: public Element
{
public:
    using EntriesContainer = FixedSizeArray<Element*, 10>;
    Menu();
    ~Menu() = default;

    void add(Element* e);
    uint8_t childrenCount() const { return m_childrenCount; }
    const EntriesContainer& children() const { return m_children; }
    inline uint8_t selected() const { return m_selectedEntry; }
    inline void select(uint8_t entry) { m_selectedEntry = entry; }
private:
    EntriesContainer m_children;
    uint8_t m_childrenCount = 0;
    uint8_t m_selectedEntry = 0;
};