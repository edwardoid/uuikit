#pragma once

#include "ui_element.h"

class Screen: public Element
{
public:
    using ElementsContainer = FixedSizeArray<Element*, 10>;
    Screen() = default;
    ~Screen() = default;

    void add(Element* e) { m_children[m_childrenCount] = e; ++m_childrenCount;}
    uint8_t childrenCount() const { return m_childrenCount; }
    const ElementsContainer& children() const { return m_children; }

private:
    ElementsContainer m_children;
    uint8_t m_childrenCount = 0;
};