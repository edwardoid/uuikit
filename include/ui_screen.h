#pragma once

#include "ui_element.h"

#include <array>

class Screen: public Element
{
public:
    using ElementsContainer = std::array<Element*, 5>;
    Screen();
    ~Screen() = default;

    void add(Element* e) { m_children[m_childrenCount++] = e; }
    uint8_t childrenCount() const { return m_childrenCount; }
    const ElementsContainer& children() const { return m_children; }
    virtual bool handle(const user_input_t key);

private:
    ElementsContainer m_children;
    uint8_t m_childrenCount = 0;
};